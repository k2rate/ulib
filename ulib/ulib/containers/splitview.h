#pragma once

#include <stdexcept>
#include <assert.h>

#include <ulib/types.h>
#include <ulib/allocators/defaultallocator.h>

namespace ulib
{
    template <class SpanT>
    class SplitIterator
    {
    public:
        using IteratorT = typename SpanT::ConstIterator;

        struct SetupBegin
        {
        };
        struct SetupEnd
        {
        };

        inline SplitIterator(SpanT str, SpanT sep, SetupBegin)
        {
            mBegin = str.begin();
            mEnd = str.end();

            mWordBegin = str.begin();
            mWordEnd = str.begin();

            mSeparator = sep;

            PreInit();
        }

        inline SplitIterator(SpanT str, SpanT sep, SetupEnd)
        {
            mBegin = str.begin();
            mEnd = str.end();

            mWordBegin = str.end();
            mWordEnd = str.end();

            mSeparator = sep;

            // PreInit();
        }

        inline SplitIterator(const SplitIterator &it)
        {
            mBegin = it.mBegin;
            mEnd = it.mEnd;

            mWordBegin = it.mWordBegin;
            mWordEnd = it.mWordEnd;

            mSeparator = it.mSeparator;
        }

        inline SpanT operator*() const { return SpanT(mWordBegin, mWordEnd); }

        inline SplitIterator &operator++()
        {
            StepNext();
            return *this;
        } // Prefix increment operator.

        inline SplitIterator &operator++(int)
        {
            StepNext();
            return *this;
        } // Postfix increment operator.

        inline bool operator==(const SplitIterator &right) const { return mWordBegin == right.mWordBegin; }
        inline bool operator!=(const SplitIterator &right) const { return mWordBegin != right.mWordBegin; }

    private:
        SpanT FindNextSeparator() const
        {
            auto it = mWordEnd;
            auto sepEnd = it + mSeparator.Size();
            const auto end = mEnd;

            for (; sepEnd <= end; ++it, ++sepEnd)
            {
                if (SpanT(it, sepEnd) == mSeparator)
                {
                    return SpanT(it, sepEnd);
                }
            }

            return SpanT(mEnd, mEnd);
        }

        void PreInit()
        {
            assert(mWordBegin == mWordEnd);
            SelectWord();
        }

        void SelectWord()
        {
            while (true)
            {
                SpanT sep = FindNextSeparator();
                if (mWordEnd == sep.Begin())
                {
                    if (mWordEnd == mEnd)
                        return;

                    mWordBegin += mSeparator.Size();
                    mWordEnd += mSeparator.Size();

                    continue;
                }

                mWordEnd = sep.Begin();
                break;
            }
        }

        void StepNext()
        {
            if (mWordEnd == mEnd)
            {
                mWordBegin = mWordEnd;
                return;
            }

            mWordEnd += mSeparator.Size();
            mWordBegin = mWordEnd;

            SelectWord();
        }

        IteratorT mBegin;
        IteratorT mEnd;
        IteratorT mWordBegin;
        IteratorT mWordEnd;
        SpanT mSeparator;
    };

    
    template <class SpanT>
    class SplitView
    {
    public:
        using IteratorT = typename SpanT::ConstIterator;
        using SplitIteratorT = SplitIterator<SpanT>;
        using ValueT = typename SpanT::value_type;

        inline SplitView(SpanT str, SpanT sep) : mString(str), mSeparator(sep)
        {
            assert(!sep.empty() && "Separator for SplitView can not be empty");
        }

        inline SplitIteratorT begin() const { return Begin(); }
        inline SplitIteratorT end() const { return End(); }
        inline size_t size() const { return Size(); }
        inline SpanT at(size_t index) const { return At(index); }

        inline SplitIteratorT Begin() const { return SplitIteratorT(mString, mSeparator, typename SplitIteratorT::SetupBegin{}); }
        inline SplitIteratorT End() const { return SplitIteratorT(mString, mSeparator, typename SplitIteratorT::SetupEnd{}); }

        inline size_t Size() const
        {
            size_t counter = 0;
            for (auto obj : *this)
                ++counter;

            return counter;
        }

        inline SpanT At(size_t index) const
        {
            size_t counter = 0;
            auto e = this->end();

            for (auto it = this->begin(); it != e; it++)
            {
                if (counter == index)
                    return *it;

                ++counter;
            }

            throw std::out_of_range("splitview out of range");
        }

        inline SpanT operator[](size_t index) const { return At(index); }

    private:
        SpanT mString;
        SpanT mSeparator;
    };

} // namespace ulib