#pragma once

namespace ray::core::lib
{
    int StrLength(const char* _str)
    {
        int size = 0;

        for (; _str[size] != 0; size++);

        return size;
    }

    void StrCopy(char* in_str, const char* src_str)
    {
        for (int i = 0; i < StrLength(in_str); i++)
            in_str[i] = src_str[i];
    }

    bool StrCompare(char* str_f, char* str_s)
    {
        int i = 0;

        for (; str_f[i] == str_s[i] && i < StrLength(str_f); i++);

        return (i == StrLength(str_f)) ? true : false;
    }

    inline void Swap(char* x, char* y)
    {
        char temp = *x; *x = *y; *y = temp;
    }

    char* Reverse(char* buffer, int i, int j)
    {
        while (i < j)
            Swap(&buffer[i++], &buffer[j--]);

        return buffer;
    }

    int Abs(int i)
    {
        return i < 0 ? -i : i;
    }


    char* IToA(int value, char* buffer, int base)
    {
        if (base < 2 || base > 32)
            return buffer;

        int n = Abs(value);

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

        return Reverse(buffer, 0, i - 1);
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

        char* c_str() { return _str; }

    private:
        char* _str;
    };

    String::String(const char* _str)
    {
        _str = new char[StrLength(_str) + 1];
        StrCopy(this->_str, _str);
    }

    String::String(int _int)
    {
        char buffer[sizeof(_int)];
        IToA(_int, buffer, 10);
        _str = new char[StrLength(buffer) + 1];
        StrCopy(_str, buffer);
    }

    /*String::String(double _double)
    {
        _str = new char[StrLength(_str) + 1];
        StrCopy(_str, _str);
    }

    String::String(float _float)
    {
        _str = new char[StrLength(_str) + 1];
        StrCopy(_str, _str);
    } */

    String::String(const String& rhs)
    {
        _str = new char[StrLength(rhs._str) + 1];
        StrCopy(_str, rhs._str);
    }

    String::~String()
    {
        delete _str;
    }

    String& String::operator=(const String& rhs)
    {
        if (this != &rhs)
        {
            delete[] this->_str;
            this->_str = new char[StrLength(rhs._str) + 1];
            StrCopy(this->_str, rhs._str);
        }

        return *this;
    }

    String& String::operator+=(const String& rhs)
    {
        int sz = StrLength(this->_str) + StrLength(rhs._str);

        char* ts = new char[sz + 1];

        for (int i = 0; i < StrLength(this->_str); i++)
            ts[i] = this->_str[i];
        for (int ii = StrLength(this->_str), j = 0; ii <= sz; ii++, j++)
            ts[ii] = rhs._str[j];

        delete this->_str;
        this->_str = ts;

        return *this;
    }

    String operator+(const String& lhs, const String& rhs)
    {
        String ts = lhs;

        return ts += rhs;
    }

    bool operator==(const String& lhs, const String& rhs)
    {
        return StrCompare(lhs._str, rhs._str);
    }

    bool operator!=(const String& lhs, const String& rhs)
    {
        return !(StrCompare(lhs._str, rhs._str));
    }

    bool operator>(const String& lhs, const String& rhs)
    {
        return (StrLength(lhs._str) > StrLength(rhs._str)) ? true : false;
    }

    bool operator>=(const String& lhs, const String& rhs)
    {
        return (StrLength(lhs._str) >= StrLength(rhs._str)) ? true : false;
    }

    bool operator<(const String& lhs, const String& rhs)
    {
        return (StrLength(lhs._str) < StrLength(rhs._str)) ? true : false;
    }

    bool operator<=(const String& lhs, const String& rhs)
    {
        return (StrLength(lhs._str) <= StrLength(rhs._str)) ? true : false;
    }
}