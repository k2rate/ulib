#pragma once

#include "ulib/encodings/type.h"
#include <stdexcept>
#include <ulib/containers/encodedstring.h>
#include <ulib/containers/encodedstringview.h>
#include <ulib/encodings/literalencoding.h>
#include <ulib/typetraits/encoding.h>

namespace ulib
{
    template <class EncodingT>
    class SplitIterator
    {
    public:
        using StringViewT = EncodedStringView<EncodingT>;
        using IteratorT = typename StringViewT::ConstIterator;

        struct SetupBegin
        {
        };
        struct SetupEnd
        {
        };

        inline SplitIterator(StringViewT str, StringViewT sep, SetupBegin)
        {
            mBegin = str.begin();
            mEnd = str.end();

            mWordBegin = str.begin();
            mWordEnd = str.begin();

            mSeparator = sep;

            PreInit();
        }

        inline SplitIterator(StringViewT str, StringViewT sep, SetupEnd)
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

        inline StringViewT operator*() const { return StringViewT(mWordBegin, mWordEnd); }

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
        StringViewT FindNextSeparator() const
        {
            auto it = mWordEnd;
            auto sepEnd = it + mSeparator.Size();
            const auto end = mEnd;

            for (; sepEnd <= end; ++it, ++sepEnd)
            {
                if (StringViewT(it, sepEnd) == mSeparator)
                {
                    return StringViewT(it, sepEnd);
                }
            }

            return StringViewT(mEnd, mEnd);
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
                StringViewT sep = FindNextSeparator();
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
        StringViewT mSeparator;
    };

    template <class EncodingT>
    class SplitView
    {
    public:
        using StringViewT = EncodedStringView<EncodingT>;
        using IteratorT = typename StringViewT::ConstIterator;
        using SplitIteratorT = SplitIterator<EncodingT>;

        inline SplitView(StringViewT str, StringViewT sep) : mString(str), mSeparator(sep)
        {
            assert(!sep.empty() && "Separator for SplitView can not be empty");
        }

        inline SplitIteratorT begin() const { return Begin(); }
        inline SplitIteratorT end() const { return End(); }
        inline size_t size() const { return Size(); }
        inline StringViewT at(size_t index) const { return At(index); }

        inline SplitIteratorT Begin() const
        {
            return SplitIteratorT(mString, mSeparator, typename SplitIteratorT::SetupBegin{});
        }

        inline SplitIteratorT End() const
        {
            return SplitIteratorT(mString, mSeparator, typename SplitIteratorT::SetupEnd{});
        }

        inline size_t Size() const
        {
            size_t counter = 0;
            for (auto obj : *this)
                ++counter;

            return counter;
        }

        inline StringViewT At(size_t index) const
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

        inline StringViewT operator[](size_t index) const { return At(index); }

    private:
        StringViewT mString;
        StringViewT mSeparator;
    };

    template <class EncodingT>
    inline SplitView<EncodingT> split_impl(EncodedStringView<EncodingT> str, EncodedStringView<EncodingT> sep)
    {
        return SplitView<EncodingT>(str, sep);
    }

    template <class StringT, class SeparatorStringT, class EncodingT = argument_encoding_t<StringT>,
              std::enable_if_t<!std::is_same_v<EncodingT, void> &&
                               std::is_same_v<EncodingT, argument_encoding_t<SeparatorStringT>>, bool> = true>
    inline SplitView<EncodingT> split(const StringT& str, const SeparatorStringT& sep)
    {
        return split_impl(ulib::EncodedStringView<EncodingT>(str), ulib::EncodedStringView<EncodingT>(sep));
    }

} // namespace ulib