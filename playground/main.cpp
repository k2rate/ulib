#include <ulib/string.h>

int main()
{
    {
        char arr[] = {'h', 'e', 'l', 'l', 'o'};
        ulib::span<char> spn = arr;

        for (auto &i : spn)
        {
            i = 0;
        }
    }

    {
        const char arr[] = {'h', 'e', 'l', 'l', 'o'};
        ulib::span_view<char> spn = arr;

        for (auto &i : spn)
        {
            // i = 0; // error
        }
    }

    {
        char arr[] = {'h', 'e', 'l', 'l', 'o'};
        ulib::span<const char> spn = arr;

        for (auto &i : spn)
        {
            // i = 0; // error
        }
    }

    {
        char arr[] = {'h', 'e', 'l', 'l', 'o'};
        ulib::span_view<char> spn = arr;

        for (auto &i : spn)
        {
            // i = 0; // error
        }
    }

    {
        ulib::list<char> arr = {'h', 'e', 'l', 'l', 'o'};
        ulib::span<char> spn = arr;

        for (auto &i : spn)
        {
            i = 0; // error
        }
    }

    {
        ulib::list<char> arr = {'h', 'e', 'l', 'l', 'o'};
        ulib::span_view<char> spn = arr;

        for (auto &i : spn)
        {
            // i = 0; // error
        }
    }

    {
        ulib::list<char> arr = {'h', 'e', 'l', 'l', 'o'};
        const ulib::list<char> &li = arr;
        ulib::span_view<char> spn = li;

        for (auto &i : spn)
        {
            // i = 0; // error
        }
    }

    {
        ulib::list<char> arr = {'h', 'e', 'l', 'l', 'o'};
        ulib::list<char> &li = arr;
        ulib::span<char> spn = li;

        for (auto &i : spn)
        {
            // i = 0; // error
        }
    }

    {
        ulib::string str;
        ulib::string_span spn = str;

        for (auto &it : str)
        {
            it = 0;
        }
    }

    {
        ulib::string str;
        ulib::string_view spn = str;

        for (auto &it : spn)
        {
        }
    }

    {
        ulib::span<int> spn;
        ulib::span<const int> spnc;

        spn.find(spnc);
        spn.find(spn);

        spn.starts_with(spnc);
        spn.starts_with(spn);

        spn.ends_with(spnc);
        spn.ends_with(spn);
    }

    {
        ulib::span<int> spn;
        ulib::span_view<int> spnc;

        spnc.find(spnc);
        spnc.find(spn);

        spnc.starts_with(spnc);
        spnc.starts_with(spn);

        spnc.ends_with(spnc);
        spnc.ends_with(spn);
    }

    {
        ulib::string_span spn;
        ulib::string_view spnc;

        spnc.find(spnc);
        spnc.find(spn);

        spnc.starts_with(spnc);
        spnc.starts_with(spn);

        spnc.ends_with(spnc);
        spnc.ends_with(spn);
    }

    {
        int buf[5] = {0, 1, 2, 3, 4};
        ulib::span<int> ints2;

        ints2 = buf;

        ulib::list<int> list;
        ints2 = list;
    }

    {
        ulib::span<int> ints1;
        ulib::span<const int> ints2;

        // ulib::span<int> ints3 = ints2;

        ints2 = ints1;
        // ints1 = ints2;

        bool r = ints1 == ints2;
        r = ints2 == ints1;

        ulib::list<int> arr = {'h', 'e', 'l', 'l', 'o'};
        int arr1[] = {'h', 'e', 'l', 'l', 'o'};

        r = ints1 == arr;
        r = ints1 == arr1;

        r = ints2 == arr;
        r = ints2 == arr1;
    }

    {
        ulib::list<char> arr = {'h', 'e', 'l', 'l', 'o'};
        ulib::span<char> spn = arr;

        spn = arr;

        ulib::span<const char> spn1 = arr;

        spn1 = arr;
    }

    {
        char arr[] = {'h', 'e', 'l', 'l', 'o'};
        ulib::span<char> spn = arr;

        spn.rfind('b');
        spn.rfind(spn);

        // spn.rfind({'h', 'e', 'l'});
    }

    return 0;
}