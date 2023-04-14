#pragma once

#include "ulib/encodings/type.h"
#include <stdexcept>
#include <ulib/containers/encodedstring.h>
#include <ulib/containers/encodedstringview.h>
#include <ulib/encodings/literalencoding.h>

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

    template <class String1T, class String2T, class Encoding1T = typename String1T::EncodingT,
              class Encoding2T = typename String2T::EncodingT,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline bool starts_with(const String1T &str, const String2T &sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        return StartsWithImpl<CH>(str.data(), str.data() + str.size(), VC(sep.data()), VC(sep.data() + sep.size()));
    }

    template <class Char1T, class Char2T, class Encoding1T = LiteralEncodingT<Char1T>,
              class Encoding2T = LiteralEncodingT<Char2T>,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline bool starts_with(const Char1T *str, const Char2T *sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        size_t s1 = CStringLengthHack(str);
        size_t s2 = CStringLengthHack(sep);

        return StartsWithImpl<CH>(str, str + s1, VC(sep), VC(sep + s2));
    }

    template <class String1T, class Char2T, class Encoding1T = typename String1T::EncodingT,
              class Encoding2T = LiteralEncodingT<Char2T>,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline bool starts_with(const String1T &str, const Char2T *sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        size_t s2 = CStringLengthHack(sep);

        return StartsWithImpl<CH>(str.data(), str.data() + str.size(), VC(sep), VC(sep + s2));
    }

    template <class Char1T, class String2T, class Encoding1T = LiteralEncodingT<Char1T>,
              class Encoding2T = typename String2T::EncodingT,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline bool starts_with(const Char1T *str, const String2T &sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        size_t s1 = CStringLengthHack(str);

        return StartsWithImpl<CH>(str, str + s1, VC(sep.data()), VC(sep.data() + sep.size()));
    }
} // namespace ulib