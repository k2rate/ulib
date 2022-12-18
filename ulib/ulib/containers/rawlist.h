#pragma once

#include <ulib/resources/resource.h>
#include <ulib/types.h>
#include "iterators/binaryiterator.h"
#include "memorymodel.h"

#include <assert.h>
#include <string.h> // memcpy
#include <initializer_list>
#include <new.h>

namespace ulib
{
	template<class T, MemoryModel Model = MemoryModel::Default>
	class RawList : public Resource
	{
	public:
		struct icat {};

		constexpr static size_t C_STEP = 16;
		constexpr static size_t M_STEP = sizeof(T) * C_STEP;

		using Iterator = BinaryIterator<T>;
		using ConstIterator = BinaryIterator<const T>;

		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator = Iterator;
		using const_iterator = ConstIterator;

		inline RawList(ULIB_RESOURCE_PARAMS)
			: Resource(ULIB_RESOURCE_ARGS)
		{
			Init_BLE(C_STEP * sizeof(T));
		}

		inline RawList(size_t capacity, ULIB_RESOURCE_PARAMS)
			: Resource(ULIB_RESOURCE_ARGS)
		{
			Init_BLE(capacity * sizeof(T));
		}

		inline RawList(std::initializer_list<T> init, ULIB_RESOURCE_PARAMS)
			: Resource(ULIB_RESOURCE_ARGS)
		{
			const size_t size = init.size() * sizeof(T);
			Init_BLE(size, size);

			memcpy(mBeginB, init.begin(), size);
		}

		inline RawList(const RawList& obj)
			: Resource(ULIB_RESOURCE_COPY_ARGS(obj))
		{
			const size_t size = obj.SizeInBytes();
			Init_BLE(obj.CapacityInBytes(), size);

			memcpy(mBeginB, obj.mBeginB, size);
		}

		inline RawList(RawList&& obj)
			: Resource(ULIB_RESOURCE_COPY_ARGS(obj)),
			mBegin(obj.mBegin), mLast(obj.mLast), mEnd(obj.mEnd)
		{
			obj.mBegin = nullptr;
		}

		inline ~RawList()
		{
			if (mBeginB)
			{
				Free(mBeginB);
				mBeginB = nullptr;
			}
		}

		inline size_t Size() const { return mLast - mBegin; }
		inline size_t Capacity() const { return mEnd - mBegin; }
		inline size_t Available() const { return mEnd - mLast; }
		inline size_t SizeInBytes() const { return mLastB - mBeginB; }
		inline size_t CapacityInBytes() const { return mEndB - mBeginB; }
		inline size_t AvailableInBytes() const { return mEndB - mLastB; }
		inline bool IsEof() const { return mLast == mEnd; }

		inline const uchar* Data() const { return mBegin; }
		inline uchar* Data() { return mBegin; }
		inline ConstIterator Begin() const { return mBegin; }
		inline Iterator Begin() { return mBegin; }
		inline const ConstIterator End() const { return mLast; }
		inline Iterator End() { return mLast; }

		inline const T& Front() const { assert(Size()); return *mBegin; }
		inline T& Front() { assert(Size()); return *mBegin; }
		inline const T& Back() const { assert(Size()); return *(mLast - 1); }
		inline T& Back() { assert(Size()); return *(mLast - 1); }

		inline const_iterator begin() const { return mBegin; }
		inline iterator begin() { return mBegin; }
		inline const_iterator end() const { return mLast; }
		inline iterator end() { return mLast; }

		inline void Clear() { mLast = mBegin; }

		inline bool Reserve(size_t newCapacity)
		{
			size_t ncib = newCapacity * sizeof(T);
			if (ncib > CapacityInBytes())
				return Realloc_Internal(ncib);

			return true;
		}

		inline bool Add(const T& v)
		{
			if (mLast == mEnd)
			{
				size_t c = CapacityInBytes();
				if (!Realloc_Internal(c + c))
					return false;
			}

			new (mLast) T(v);
			mLast++;

			return true;
		}

		inline bool Insert(Iterator i, const T* data, size_t size)
		{
			rawptr_t iptr = rawptr_t(i.ptr);
			rawptr_t dptr = rawptr_t(data);
			size_t avl = AvailableInBytes();
			size_t ss = size * sizeof(T);

			assert(ss + iptr <= mLast);
			assert(iptr >= mBegin);

			rawptr_t page = mBeginB;
			if (avl < ss)
			{
				page = AllocateSpace_Internal(ss);
				if (!page)
					return false;

				memcpy(page, dptr + iptr - mBeginB, SizeInBytes());
				memcpy(iptr + ss, iptr, ss);
			}
			else
			{
				memcpy(iptr + ss, iptr, ss);
			}

			

			if (avl < ss)
			{
				if constexpr (Model == MemoryModel::Flex)
				{
					size_t cc = CapacityInBytes();
					if (!Realloc_Internal(cc + cc + ss))
						return false;
				}
				else
				{
					size_t currSize = SizeInBytes();
					if (!Realloc_Internal(currSize + ss))
						return false;
				}
			}
		}

		inline bool InsertBack(const T* data, size_t size)
		{
			size_t sizeInBytes = size * sizeof(T);
			size_t currAvl = AvailableInBytes();

			if (!ReserveSpace_Internal(sizeInBytes))
				return false;

			memcpy(mLast, data, sizeInBytes);
			mLastB += sizeInBytes;

			return true;
		}



	private:

		inline void Init_BE(size_t capacityInBytes)
		{
			mBeginB = rawptr_t(mAllocator->Alloc(capacityInBytes));
			assert(mBeginB);

			mEndB = mBeginB + capacityInBytes;
		}

		inline void Init_BLE(size_t capacityInBytes)
		{
			Init_BE(capacityInBytes);
			mLastB = mBeginB;
		}

		inline void Init_BLE(size_t capacityInBytes, size_t sizeInBytes)
		{
			Init_BE(capacityInBytes);
			mLastB = mBeginB + sizeInBytes;
		}

		inline bool Realloc_Internal(size_t newCapacity)
		{
			assert(newCapacity > SizeInBytes());
			assert(newCapacity < 0x07ffffff); // 128mb

			if (rawptr_t ptr = rawptr_t(Alloc(newCapacity)))
			{
				const size_t l = mLastB - mBeginB;

				memcpy(ptr, mBeginB, l);
				Free(mBeginB);

				mBeginB = ptr;
				mLastB = ptr + l;
				mEndB = ptr + newCapacity;

				return true;
			}

			return false;
		}

		inline bool ReserveSpace_Internal(size_t newAvailable)
		{
			size_t currAvl = AvailableInBytes();
			if (currAvl < newAvailable)
			{
				if constexpr (Model == MemoryModel::Flex)
				{
					size_t cc = CapacityInBytes();
					if (!Realloc_Internal(cc + cc + newAvailable))
						return false;
				}
				else
				{
					size_t currSize = SizeInBytes();
					if (!Realloc_Internal(currSize + newAvailable))
						return false;
				}
			}

			return true;
		}

		inline rawptr_t AllocateSpace_Internal(size_t requiredCapacity)
		{
			if constexpr (Model == MemoryModel::Flex)
			{
				return rawptr_t(Alloc(requiredCapacity + CapacityInBytes()));
			}
			else
			{		
				return rawptr_t(Alloc(requiredCapacity));
			}
		}

		inline bool Insert_Internal(uchar* it, const uchar* data, size_t size)
		{
			size_t avl = AvailableInBytes();

			assert(size + it <= mLast);
			assert(it >= mBegin);

			rawptr_t page = mBeginB;
			if (avl < size)
			{
				page = AllocateSpace_Internal(size);
				if (page == nullptr)
					return false;

				size_t firstFragmentSize = it - mBeginB;

				memcpy(page, mBeginB, it - mBeginB);
				memcpy(it + size, it, size);
			}
			else
			{
				memcpy(it + size, it, size);
			}
		}

	protected:
		union
		{
			struct { T *mBegin, *mLast, *mEnd; };
			struct { rawptr_t mBeginB, mLastB, mEndB; };
		};
	};
}