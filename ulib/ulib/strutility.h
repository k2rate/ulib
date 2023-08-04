#pragma once

#include <ulib/encodings/type.h>
#include <stdexcept>
#include <ulib/containers/encodedstring.h>
#include <ulib/containers/encodedstringview.h>
#include <ulib/typetraits/literalencoding.h>
#include <ulib/cstrlen.h>

namespace ulib
{
    template <class CharT>
    inline bool StartsWithImpl(const CharT *b, const CharT *e, const CharT *bv, const CharT *ev)
    {
        while (true)
        {
            if (bv == ev)
                return true;
            if (b == e)
                return false;
            if (*b != *bv)
                return false;

            b++;
            bv++;
        }
    }

    template <class CharT>
    inline bool REndsWithImpl(const CharT *rb, const CharT *re, const CharT *rbv, const CharT *rev)
    {
        while (true)
        {
            if (rbv == rev)
                return true;
            if (rb == re)
                return false;
            if (*rb != *rbv)
                return false;

            rb--;
            rbv--;
        }
    }

    template <class CharT>
    inline bool EndsWithImpl(const CharT *b, const CharT *e, const CharT *bv, const CharT *ev)
    {
        return REndsWithImpl(e - 1, b - 1, ev - 1, bv - 1);
    }

    template <class CharT>
    inline const CharT *FindFirst(const CharT *b, const CharT *e, const CharT *bv, const CharT *ev)
    {
        size_t ss = ev - bv;
        while (b != e)
        {
            assert(e >= b);
            if (size_t(e - b) >= ss)
            {
                auto bb = b;
                auto it = bv;

                while (true)
                {
                    if (it == ev)
                    {
                        return b;
                    }

                    if(*it != *bb)
                        break;

                    it++;
                    bb++;
                }
            }
            else
            {
                return e;
            }

            b++;
        }

        return e;
    }

    template<class StringT, class String2T, class EncodingT = argument_encoding_t<StringT>, std::enable_if_t<!std::is_same_v<EncodingT, missing_type> && is_argument_encoding_v<String2T, EncodingT>>>
    inline bool starts_with(const StringT &str, const StringT &sep)
    {
        EncodedStringView<EncodingT> vstr = str;
        EncodedStringView<EncodingT> vsep = sep;

        return StartsWithImpl(vstr.data(), vstr.data() + vstr.size(), vsep.data(), vsep.data() + vsep.size());
    }

    template<class StringT, class String2T, class EncodingT = argument_encoding_t<StringT>, std::enable_if_t<!std::is_same_v<EncodingT, missing_type> && is_argument_encoding_v<String2T, EncodingT>>>
    inline bool ends_with(const StringT &str, const StringT &sep)
    {
        EncodedStringView<EncodingT> vstr = str;
        EncodedStringView<EncodingT> vsep = sep;

        return EndsWithImpl(vstr.data(), vstr.data() + vstr.size(), vsep.data(), vsep.data() + vsep.size());
    }

    template<class StringT, class String2T, class EncodingT = argument_encoding_t<StringT>, std::enable_if_t<!std::is_same_v<EncodingT, missing_type> && is_argument_encoding_v<String2T, EncodingT>>>
    inline bool find_first(const StringT &str, const StringT &sep)
    {
        EncodedStringView<EncodingT> vstr = str;
        EncodedStringView<EncodingT> vsep = sep;

        return FindFirstImpl(vstr.data(), vstr.data() + vstr.size(), vsep.data(), vsep.data() + vsep.size());
    }

} // namespace ulib