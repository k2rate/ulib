#pragma once

#include <ulib/resources/resource.h>
#include "iterators/binaryiterator.h"
#include "memorymodel.h"

#include <ulib/types.h>
#include <assert.h>
#include <vector>

#include <iostream>

/*

*/
namespace ulib
{


	template<class T, MemoryModel Model = MemoryModel::Default>
	class BinaryContainer : public Resource
	{
	public:
		using Iterator = BinaryIterator<T>;
		using ConstIterator = BinaryIterator<const T>;

		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator = Iterator;
		using const_iterator = ConstIterator;


		BinaryContainer(ULIB_RESOURCE_PARAMS)
			: Resource(ULIB_RESOURCE_ARGS)
		{
			mBeginB = nullptr;
			mLastB = nullptr;
			mEndB = nullptr;
		}

		BinaryContainer(size_t capacity, ULIB_RESOURCE_PARAMS)
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

		~BinaryContainer()
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

		inline const T& Front() const { assert(Size()); return *mBegin; }
		inline T& Front() { assert(Size()); return *mBegin; }
		inline const T& Back() const { assert(Size()); return mLast - 1; }
		inline T& Back() { assert(Size()); return mLast - 1; }

	protected:

		inline rawptr_t CalcPageSizeViaModelB(size_t requiredSize)
		{
			if constexpr (Model == MemoryModel::Flex)
				return requiredSize + CapacityInBytes();
			else
				return requiredSize;
		}

		inline bool ReserveUnsafeB(size_t newCapacityB)
		{
			assert(SizeInBytes() <= newCapacityB);
			assert(newCapacityB < 0x07ffffff); // 128mb

			rawptr_t newPtr = (uchar*)Resource::Alloc(newCapacityB);
			if (newPtr == nullptr)
				return false;

			size_t size = SizeInBytes();
			memcpy(newPtr, mBegin, size);
			Resource::Free(mBegin);

			mBeginB = newPtr;
			mLastB = mBeginB + size;
			mEndB = mBeginB + newCapacityB;

			return true;
		}

		inline bool ReserveB(size_t currentCapacityB, size_t newCapacityB)
		{
			return (newCapacityB > currentCapacityB) ? ReserveUnsafeB(newCapacityB) : true;
		}

		inline bool ReserveB(size_t newCapacityB)
		{
			return ReserveB(CapacityInBytes(), newCapacityB);
		}

		inline bool ExpandCapacityB(size_t additionalCapactiyB)
		{
			return ReserveUnsafeB(CapacityInBytes() + additionalCapactiyB);
		}

		union
		{
			struct { T* mBegin, * mLast, * mEnd; };
			struct { rawptr_t mBeginB, mLastB, mEndB; };
		};
	};
}