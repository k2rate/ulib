#pragma once

#include <assert.h>
#include <string.h>
#include <ulib/types.h>
#include <ulib/resources/resource.h>
#include "iterators/binaryiterator.h"
#include "memorymodel.h"

#include <algorithm>
#include <vector>

namespace ulib
{
	template<class T, MemoryModel Model = MemoryModel::Default>
	class Buffer : public Resource
	{
	public:

		using Iterator = BinaryIterator<T>;
		using ConstIterator = BinaryIterator<const T>;

		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator = Iterator;
		using const_iterator = ConstIterator;

		Buffer(ULIB_RESOURCE_PARAMS)
			: Resource(ULIB_RESOURCE_ARGS)
		{
			mBeginB = nullptr;
			mLastB = nullptr;
			mEndB = nullptr;
		}

		Buffer(size_t capacity, ULIB_RESOURCE_PARAMS)
			: Resource(ULIB_RESOURCE_ARGS)
		{
			size_t capacityInBytes = capacity * sizeof(T);
			mBeginB = rawptr_t(Resource::Alloc(capacityInBytes));
			mLastB = mBeginB;

			if (mBeginB == nullptr)
			{
				mEndB = nullptr;
				return;
			}

			mEndB = mBeginB + capacityInBytes;
		}

		~Buffer()
		{
			if (mBeginB)
				Resource::Free(mBeginB);
		}

		// check allocation functions
		inline bool IsGood() const { return mBeginB != nullptr; }
		inline bool IsBad() const { return mBeginB == nullptr; }

		// check size functions
		// if allocation failed all returns 0
		inline size_t Size() const { return mLast - mBegin; }
		inline size_t Capacity() const { return mEnd - mBegin; }
		inline size_t Available() const { return mEnd - mLast; }
		inline size_t SizeInBytes() const { return mLast - mBegin; }
		inline size_t CapacityInBytes() const { return mEnd - mBegin; }
		inline size_t AvailableInBytes() const { return mEnd - mLast; }
		inline bool IsEmpty() const { return mBeginB == mLastB; }
		inline bool IsContains() const { return mBeginB != mLastB; };

		inline const T* Data() const { return mBegin; }
		inline T* Data() { return mBegin; }
		inline ConstIterator Begin() const { return mBegin; }
		inline Iterator Begin() { return mBegin; }
		inline ConstIterator End() const { return mLast; }
		inline Iterator End() { return mLast; }
		inline const T& Front() const { assert(Size()); return mBegin; }
		inline T& Front() { assert(Size()); return mBegin; }
		inline const T& Back() const { assert(Size()); return mLast - 1; }
		inline T& Back() { assert(Size()); return mLast - 1; }

		inline void Clear() { mLast = mBegin; }

		/*
		
			Reserve
			Resize

			ExpandCapacity
			ExpandSize

			AddSize
			SetSize
		
		*/

		inline bool Reserve(size_t newCapacity)
		{
			return Reserve_Internal(newCapacity * sizeof(T));
		}

		inline bool ExpandCapacity(size_t additionalCapacity)
		{
			return ExpandCapacity_Internal(additionalCapacity * sizeof(T));
		}


		inline bool Resize(size_t newSize, const T& value)
		{
			size_t capacity = Capacity();
			size_t size = Size();

			if (newSize > capacity)
			{
				if (!ReserveUnsafe_Internal(newSize))
					return false;
			}
			else if (newSize <= size)
			{
				return true;
			}

			T* it = mLast;
			T* end = mBegin + newSize;
			
			for (; it != end; ++it)
				memcpy(it, &value, sizeof(value));
		}

		inline bool ExpandSize(size_t additionalSize, const T& initializer)
		{
			
		}
		
		void Resize(size_t newSize)
		{
			assert(Capacity() >= newSize);
			mLast = mBegin + newSize;
		}

		void Resize(size_t newSize, size_t init)
		{
			assert(Capacity() >= newSize);
			mLast = mBegin + newSize;
		}


		void AddSize(size_t size)
		{
			assert(Available() >= size);
			mLast += size;
		}

		inline void InsertBack(size_t size)
		{
			assert(Available() >= size);
			mLast += size;
		}

		inline void InsertBack(const T* data, size_t size)
		{
			assert(Available() >= size);

			memcpy(mLast, data, size);
			mLast += size;
		}

		inline void EraseBegin(size_t length)
		{
			size_t lengthB = length * sizeof(T);

			assert(lengthB <= SizeInBytes());

			mLastB -= lengthB;
			memmove(mBeginB, mBeginB + lengthB, mLastB - mBeginB);
		}

	protected:

		inline bool ReserveUnsafe_Internal(size_t newCapacity)
		{
			assert(SizeInBytes() <= newCapacity);
			assert(newCapacity < 0x07ffffff); // 128mb

			rawptr_t newPtr = (uchar*)Resource::Alloc(newCapacity);
			if (newPtr == nullptr)
				return false;

			size_t size = SizeInBytes();
			memcpy(newPtr, mBegin, size);
			Resource::Free(mBegin);

			mBegin = newPtr;
			mLast = mBegin + size;
			mEnd = mBegin + newCapacity;

			return true;
		}

		inline bool Reserve_Internal(size_t currentCapacity, size_t newCapacity)
		{
			return (newCapacity > currentCapacity) ? ReserveUnsafe_Internal(newCapacity) : true;
		}

		inline bool Reserve_Internal(size_t newCapacity)
		{
			return Reserve_Internal(CapacityInBytes(), newCapacity);
		}

		inline bool ExpandCapacity_Internal(size_t additionalCapactiy)
		{
			return ReserveUnsafe_Internal(CapacityInBytes() + additionalCapactiy);
		}

	protected:
		union
		{
			struct { T* mBegin, * mLast, * mEnd; };
			struct { rawptr_t mBeginB, mLastB, mEndB; };
		};

	};

	/*
	class Buffer : public ULIB_RESOURCE
	{
	public:
		Buffer(size_t capacity, ULIB_ALLOCATOR_PARAM(pAllocator))
			: ULIB_RESOURCE_CONSTRUCT(pAllocator)
		{
			mBegin = new uchar[capacity];
			mLast = mBegin;
			mEnd = mBegin + capacity;

			assert(mBegin);
		}

		~Buffer()
		{
			delete[] mBegin;
		}

		inline size_t Size() const { return mLast - mBegin; }
		inline size_t Capacity() const { return mEnd - mBegin; }
		inline size_t Available() const { return mEnd - mLast; }

		inline const uchar* Data() const { return mBegin; }
		inline uchar* Data() { return mBegin; }
		inline const uchar* Begin() const { return mBegin; }
		inline uchar* Begin() { return mBegin; }
		inline const uchar* End() const { return mLast; }
		inline uchar* End() { return mLast; }

		inline void Clear() { mLast = mBegin; }

		inline void InsertBack(size_t size)
		{
			assert(Available() >= size);
			mLast += size;
		}

		inline void InsertBack(const void* data, size_t size)
		{
			assert(Available() >= size);

			memcpy(mLast, data, size);
			mLast += size;
		}

		void Resize(size_t newSize)
		{
			assert(Capacity() >= newSize);
			mLast = mBegin + newSize;
		}

		void AddSize(size_t size)
		{
			assert(Available() >= size);
			mLast += size;
		}

		inline bool Reserve(size_t newCapacity)
		{
			if (newCapacity > Capacity())
			{
				assert(Size() <= newCapacity);
				assert(newCapacity < 0x07ffffff); // 128mb

				uchar* newPtr = (uchar*)Alloc(newCapacity);
				if (!newPtr)
					return false;

				size_t size = Size();
				memcpy(newPtr, mBegin, size);
				Free(mBegin);

				mBegin = newPtr;
				mLast = mBegin + size;
				mEnd = mBegin + newCapacity;
			}

			return true;
		}

		bool ReserveSpace(size_t newAvailable)
		{
			return Reserve(Size() + newAvailable);
		}

		inline void Erase(void* it, size_t length)
		{
			uchar* beginErase = (uchar*)it;
			uchar* endErase = beginErase + length;
			size_t eraseSize = mLast - endErase;

			assert(endErase + eraseSize <= mLast);

			memmove(beginErase, endErase, eraseSize);
			mLast -= length;
		}

		inline void EraseBegin(size_t length)
		{
			assert(length <= Size());

			mLast -= length;
			memmove(mBegin, mBegin + length, mLast - mBegin);
		}

		inline void EraseEnd(size_t length)
		{
			assert(length <= Size());
			mLast -= length;
		}

	private:
		uchar* mBegin;
		uchar* mLast;
		uchar* mEnd;
	};
	*/

}