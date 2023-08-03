#pragma once

#include <ulib/encodings/type.h>
#include <stdexcept>
#include <ulib/containers/encodedstring.h>
#include <ulib/containers/encodedstringview.h>
#include <ulib/encodings/literalencoding.h>
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

        size_t s1 = cstrlen(str);
        size_t s2 = cstrlen(sep);

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

        size_t s2 = cstrlen(sep);

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

        size_t s1 = cstrlen(str);

        return StartsWithImpl<CH>(str, str + s1, VC(sep.data()), VC(sep.data() + sep.size()));
    }

    template <class String1T, class String2T, class Encoding1T = typename String1T::EncodingT,
              class Encoding2T = typename String2T::EncodingT,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline bool ends_with(const String1T &str, const String2T &sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        return EndsWithImpl<CH>(str.data(), str.data() + str.size(), VC(sep.data()), VC(sep.data() + sep.size()));
    }

    template <class Char1T, class Char2T, class Encoding1T = LiteralEncodingT<Char1T>,
              class Encoding2T = LiteralEncodingT<Char2T>,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline bool ends_with(const Char1T *str, const Char2T *sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        size_t s1 = cstrlen(str);
        size_t s2 = cstrlen(sep);

        return EndsWithImpl<CH>(str, str + s1, VC(sep), VC(sep + s2));
    }

    template <class String1T, class Char2T, class Encoding1T = typename String1T::EncodingT,
              class Encoding2T = LiteralEncodingT<Char2T>,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline bool ends_with(const String1T &str, const Char2T *sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        size_t s2 = cstrlen(sep);

        return EndsWithImpl<CH>(str.data(), str.data() + str.size(), VC(sep), VC(sep + s2));
    }

    template <class Char1T, class String2T, class Encoding1T = LiteralEncodingT<Char1T>,
              class Encoding2T = typename String2T::EncodingT,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline bool ends_with(const Char1T *str, const String2T &sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        size_t s1 = cstrlen(str);

        return EndsWithImpl<CH>(str, str + s1, VC(sep.data()), VC(sep.data() + sep.size()));
    }


    template <class String1T, class String2T, class Encoding1T = typename String1T::EncodingT,
              class Encoding2T = typename String2T::EncodingT,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline typename String1T::ConstIterator find_first(const String1T &str, const String2T &sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        return FindFirst<CH>(str.data(), str.data() + str.size(), VC(sep.data()), VC(sep.data() + sep.size()));
    }

    template <class Char1T, class Char2T, class Encoding1T = LiteralEncodingT<Char1T>,
              class Encoding2T = LiteralEncodingT<Char2T>,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline const Char1T* find_first(const Char1T *str, const Char2T *sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        size_t s1 = cstrlen(str);
        size_t s2 = cstrlen(sep);

        return FindFirst<CH>(str, str + s1, VC(sep), VC(sep + s2));
    }

    template <class String1T, class Char2T, class Encoding1T = typename String1T::EncodingT,
              class Encoding2T = LiteralEncodingT<Char2T>,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline typename String1T::ConstIterator find_first(const String1T &str, const Char2T *sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        size_t s2 = cstrlen(sep);

        return FindFirst<CH>(str.data(), str.data() + str.size(), VC(sep), VC(sep + s2));
    }

    template <class Char1T, class String2T, class Encoding1T = LiteralEncodingT<Char1T>,
              class Encoding2T = typename String2T::EncodingT,
              std::enable_if_t<IsParentCompatible<Encoding1T, Encoding2T>, bool> = true>
    inline const Char1T* find_first(const Char1T *str, const String2T &sep)
    {
        using VT = EncodedStringView<Encoding1T>;
        using CH = typename VT::CharT;
        using VC = CH *;

        size_t s1 = cstrlen(str);

        return FindFirst<CH>(str, str + s1, VC(sep.data()), VC(sep.data() + sep.size()));
    }

} // namespace ulib