#pragma once

#include "ulib/encodings/type.h"
#include <ulib/containers/encodedstringview.h>
#include <ulib/containers/encodedstring.h>
#include <ulib/encodings/literalencoding.h>

namespace ulib
{
    template<class EncodingT>
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

        inline StringViewT operator*() const
        {
            return StringViewT(mWordBegin, mWordEnd);
        }

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

        inline bool operator==(const SplitIterator &right) const
        {
            return mWordBegin == right.mWordBegin;
        }

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

    template<class EncodingT>
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

        inline SplitIteratorT begin() const
        {
            return SplitIteratorT(mString, mSeparator, typename SplitIteratorT::SetupBegin{});
        }
        inline SplitIteratorT end() const
        {
            return SplitIteratorT(mString, mSeparator, typename SplitIteratorT::SetupEnd{});
        }

    private:
        StringViewT mString;
        StringViewT mSeparator;
    };

    /*
        template<class EncodingT>
    inline SplitView<EncodingT> split(EncodedStringView<EncodingT> str, EncodedStringView<EncodingT> sep)
    {
        return SplitView<EncodingT>(str, sep);
    }
    */

    template<class String1T, class String2T, 
        class Encoding1T = typename String1T::EncodingT,
        class Encoding2T = typename String2T::EncodingT,
        std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline SplitView<Encoding1T> split(const String1T& str, const String2T& sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using VC = typename VT::CharT*;

        return SplitView<Encoding1T>(VT((VC)str.data(), VC(str.data() + str.size())), VT((VC)sep.data(), VC(sep.data() + sep.size())));
    }

    template<class Char1T, class Char2T, 
        class Encoding1T = LiteralEncodingT<Char1T>,
        class Encoding2T = LiteralEncodingT<Char2T>,
        std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline SplitView<Encoding1T> split(const Char1T* str, const Char2T* sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using VC = typename VT::CharT*;

        return SplitView<Encoding1T>(VT(VC(str)), VT(VC(sep)));
    }

    template<class String1T, class Char2T,
        class Encoding1T = typename String1T::EncodingT, 
        class Encoding2T = LiteralEncodingT<Char2T>,
        std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline SplitView<Encoding1T> split(const String1T& str, const Char2T* sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using VC = typename VT::CharT*;

        return SplitView<Encoding1T>(VT((VC)str.data(), VC(str.data() + str.size())), VT(VC(sep)));
    }

    template<class Char1T, class String2T, 
        class Encoding1T = LiteralEncodingT<Char1T>,
        class Encoding2T = typename String2T::EncodingT,
        std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline SplitView<Encoding1T> split(const Char1T* str, const String2T& sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using VC = typename VT::CharT*;

        return SplitView<Encoding1T>(VT(VC(str)), VT((VC)sep.data(), VC(sep.data() + sep.size())) );
    }

} // namespace ulib