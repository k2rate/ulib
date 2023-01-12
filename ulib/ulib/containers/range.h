#pragma once

#include <type_traits>
#include <ulib/types.h>

#include <stdio.h>
#include <typeinfo>

namespace ulib
{
    template <class T>
    class Range
    {
    public:

        inline Range()
        {
            mBegin = nullptr;
            mLast = nullptr;
        }

        inline Range(const T* begin, const T* end)
        {
            mBegin = begin;
            mLast = end;
        }

        template <class ContainerT>
        inline Range(ContainerT &cont)
        {
            using std::data;
            using std::size;

            using ContT = std::remove_cv_t<std::remove_reference_t<decltype(*data(cont))>>;
            static_assert(std::is_same_v<ContT, std::remove_cv_t<T>>, "Range<T> type conversion failed");

            mBegin = data(cont);
            mLast = data(cont) + size(cont);
        }

        template <class ContainerT>
        inline Range<T> &operator=(ContainerT &cont)
        {
            using std::data;
            using std::size;
            
            using ContT = std::remove_cv_t<std::remove_reference_t<decltype(*data(cont))>>;
            static_assert(std::is_same_v<ContT, std::remove_cv_t<T>>, "RangeView type conversion failed");

            mBegin = data(cont);
            mLast = data(cont) + size(cont);

            return *this;
        }

        inline T *begin() const { return mBegin; }
        inline T *end() const { return mLast; }
        inline T *data() const { return mBegin; }
        inline size_t size() const { return mLast - mBegin; }
        inline bool empty() const { return mLast == mBegin; }

        inline T *Begin() const { return mBegin; }
        inline T *End() const { return mLast; }
        inline T *Data() const { return mBegin; }
        inline size_t Size() const { return mLast - mBegin; }
        inline bool Empty() const { return mLast == mBegin; }
        inline size_t SizeInBytes() const { return (uchar *)mLast - (uchar *)mBegin; }

        inline T *ReverseBegin() const { return mLast - 1; }
        inline T *ReverseEnd() const { return mBegin - 1; }

        inline const T &Front() const
        {
            assert(!Empty());
            return *Begin();
        }

        inline const T &Back() const
        {
            assert(!Empty());
            return *ReverseBegin();
        }

    protected:
        T *mBegin;
        T *mLast;
    };

    /*
        template <class T>
    class Range : public RangeView<T>
    {
    public:
        template <class ContainerT>
        Range(ContainerT &cont) : RangeView<T>(cont)
        {
        }

        inline T *begin() { return this->mBegin; }
        inline T *end() { return this->mLast; }
        inline const T *data() { return this->mBegin; }
        inline size_t size() { return this->mLast - this->mBegin; }
        inline bool empty() { return this->mLast == this->mBegin; }

        inline T *Begin() { return this->mBegin; }
        inline T *End() { return this->mLast; }
        inline T *Data() { return this->mBegin; }
        inline size_t Size() { return this->mLast - this->mBegin; }
        inline bool Empty() { return this->mLast == this->mBegin; }
        inline size_t SizeInBytes() { return (uchar *)this->mLast - (uchar *)this->mBegin; }

        inline T *ReverseBegin() { return this->mLast - 1; }
        inline T *ReverseEnd() { return this->mBegin - 1; }

        inline T &Front()
        {
            assert(!Empty());
            return *Begin();
        }

        inline T &Back()
        {
            assert(!Empty());
            return *ReverseBegin();
        }
    };
    */

}