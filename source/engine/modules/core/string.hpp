#pragma once

namespace ray::core::lib
{
    int strlength(const char* _str)
    {
        int size = 0;

        for (; _str[size] != 0; size++);

        return size;
    }

    void strcopy(char* in_str, const char* src_str)
    {
        for (int i = 0; i < strlength(in_str); i++)
            in_str[i] = src_str[i];
    }

    bool strcompare(char* str_f, char* str_s)
    {
        int i = 0;

        for (; str_f[i] == str_s[i] && i < strlength(str_f); i++);

        return (i == strlength(str_f)) ? true : false;
    }

    inline void swap(char* x, char* y)
    {
        char temp = *x; *x = *y; *y = temp;
    }

    char* reverse(char* buffer, int i, int j)
    {
        while (i < j)
            swap(&buffer[i++], &buffer[j--]);

        return buffer;
    }

    int abs(int i)
    {
        return i < 0 ? -i : i;
    }


    char* itoa(int value, char* buffer, int base)
    {
        if (base < 2 || base > 32)
            return buffer;

        int n = abs(value);

        int i = 0;
        while (n)
        {
            int r = n % base;

            if (r >= 10)
                buffer[i++] = 65 + (r - 10);
            else
                buffer[i++] = 48 + r;

            n = n / base;
        }

        if (i == 0)
            buffer[i++] = '0';

        if (value < 0 && base == 10)
            buffer[i++] = '-';

        buffer[i] = '\0';

        return reverse(buffer, 0, i - 1);
    }

    class String
    {
    public:
        String(const char* _str = "");
        String(int _int);
        /*String(double _double);
        String(float _float);*/
        String(const String&);

        ~String();

        String& operator=(const String&);
        String& operator+=(const String&);
        friend String operator+(const String&, const String&);

        friend bool operator==(const String&, const String&);
        friend bool operator!=(const String&, const String&);
        friend bool operator>(const String&, const String&);
        friend bool operator>=(const String&, const String&);
        friend bool operator<(const String&, const String&);
        friend bool operator<=(const String&, const String&);

        char* c_str() { return str; }

    private:
        char* str;
        int length;
    };

    String::String(const char* _str)
    {
        str = new char[strlength(_str) + 1];
        strcopy(str, _str);
    }

    String::String(int _int)
    {
        char buffer[sizeof(_int)];
        itoa(_int, buffer, 10);
        str = new char[strlength(buffer) + 1];
        strcopy(str, buffer);
    }

    /*String::String(double _double)
    {
        str = new char[strlength(_str) + 1];
        strcopy(str, _str);
    }

    String::String(float _float)
    {
        str = new char[strlength(_str) + 1];
        strcopy(str, _str);
    } */

    String::String(const String& rhs)
    {
        str = new char[strlength(rhs.str) + 1];
        strcopy(str, rhs.str);
    }

    String::~String()
    {
        delete str;
    }

    String& String::operator=(const String& rhs)
    {
        if (this != &rhs)
        {
            delete[] this->str;
            this->str = new char[strlength(rhs.str) + 1];
            strcopy(this->str, rhs.str);
        }

        return *this;
    }

    String& String::operator+=(const String& rhs)
    {
        int sz = strlength(this->str) + strlength(rhs.str);

        char* ts = new char[sz + 1];

        for (int i = 0; i < strlength(this->str); i++)
            ts[i] = this->str[i];
        for (int ii = strlength(this->str), j = 0; ii <= sz; ii++, j++)
            ts[ii] = rhs.str[j];

        delete this->str;
        this->str = ts;

        return *this;
    }

    String operator+(const String& lhs, const String& rhs)
    {
        String ts = lhs;

        return ts += rhs;
    }

    bool operator==(const String& lhs, const String& rhs)
    {
        return strcompare(lhs.str, rhs.str);
    }

    bool operator!=(const String& lhs, const String& rhs)
    {
        return !(strcompare(lhs.str, rhs.str));
    }

    bool operator>(const String& lhs, const String& rhs)
    {
        return (strlength(lhs.str) > strlength(rhs.str)) ? true : false;
    }

    bool operator>=(const String& lhs, const String& rhs)
    {
        return (strlength(lhs.str) >= strlength(rhs.str)) ? true : false;
    }

    bool operator<(const String& lhs, const String& rhs)
    {
        return (strlength(lhs.str) < strlength(rhs.str)) ? true : false;
    }

    bool operator<=(const String& lhs, const String& rhs)
    {
        return (strlength(lhs.str) <= strlength(rhs.str)) ? true : false;
    }
}