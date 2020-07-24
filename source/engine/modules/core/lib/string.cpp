#include "string.hpp"
#include "cstring.hpp"

string::string()
{
    this->_string = nullptr;

    this->_length = 0;
}

string::string(const char* unnamed)
{
    this->_string = unnamed;

    this->_length = cstrlen(unnamed);
}

string::~string()
{
    this->_string = nullptr;

    this->_length = 0;
}

const char& string::operator [] (size_t i) const
{
    return *(this->_string + i);
}

const char& string::at(size_t i) const
{
    if (i < 0 || this->_length <= i)
        throw;

    return operator[](i);
}

const char& string::front() const
{
    return operator[](0);
}

const char& string::back() const
{
    return operator[](this->_length - 1);
}

string& string::operator=(const string& rhs)
{
    if (this != &rhs)
    {
        this->_string = rhs._string;
        this->_length = rhs._length;
    }

    return *this;
}

string& string::operator+=(const string& rhs)
{
    char* ts = new char[cstrlen(this->_string) + cstrlen(rhs._string) + sizeof(char)];

    for (int i = 0; i < cstrlen(this->_string); i++)
        ts[i] = this->_string[i];
    for (int ii = cstrlen(this->_string), j = 0; ii <= cstrlen(this->_string) + cstrlen(rhs._string); ii++, j++)
        ts[ii] = rhs._string[j];

    delete this->_string;
    this->_string = ts;

    return *this;
}

string operator+(const string& lhs, const string& rhs)
{
    string temp_string = lhs;

    return temp_string += rhs;
}

bool operator==(const string& lhs, const string& rhs)
{
    return cstrcmp(lhs._string, rhs._string);
}

bool operator!=(const string& lhs, const string& rhs)
{
    return !(cstrcmp(lhs._string, rhs._string));
}

bool operator>(const string& lhs, const string& rhs)
{
    return (cstrlen(lhs._string) > cstrlen(rhs._string)) ? true : false;
}

bool operator>=(const string& lhs, const string& rhs)
{
    return (cstrlen(lhs._string) >= cstrlen(rhs._string)) ? true : false;
}

bool operator<(const string& lhs, const string& rhs)
{
    return (cstrlen(lhs._string) < cstrlen(rhs._string)) ? true : false;
}

bool operator<=(const string& lhs, const string& rhs)
{
    return (cstrlen(lhs._string) <= cstrlen(rhs._string)) ? true : false;
}

const char* string::substr(int start) const
{
    return subcstr(this->_string, start, this->_length);
}

const char* string::substr(int start, int end) const
{
    return subcstr(this->_string, start, end);
}

size_t string::find(const string& other, size_t pos) const
{
    return _find(other._string, other._length, pos);
}

size_t string::find(const char* s, size_t pos) const
{
    return _find(s, cstrlen(s), pos);
}

size_t string::find(const char* s, size_t pos, size_t n) const
{
    return _find(s, n, pos);
}

size_t string::find(char c, size_t pos) const
{
    return _find(&c, 1, pos);
}

size_t string::rfind(const string& other, size_t pos) const
{
    return _rfind(other._string, other._length, pos);
}

size_t string::rfind(const char* s, size_t pos) const
{
    return  _rfind(s, cstrlen(s), pos);
}

size_t string::rfind(const char* s, size_t pos, size_t n) const
{
    return _rfind(s, n, pos);
}

size_t string::rfind(char c, size_t pos) const
{
    return _rfind(&c, 1, pos);
}

size_t string::find_first_of(const string& other, size_t pos) const
{
    return _find_first_of(other._string, other._length, pos, true);
}

size_t string::find_first_of(const char* other, size_t pos) const
{
    return _find_first_of(other, cstrlen(other), pos, true);
}

size_t string::find_first_of(const char* other, size_t pos, size_t n) const
{
    return _find_first_of(other, n, pos, true);
}

size_t string::find_first_of(char c, size_t pos) const
{
    return _find_first_of(&c, 1, pos, true);
}

size_t string::find_last_of(const string& other, size_t pos) const
{
    return _find_last_of(other._string, other._length, pos, true);
}

size_t string::find_last_of(const char* other, size_t pos) const
{
    return _find_last_of(other, cstrlen(other), pos, true);
}

size_t string::find_last_of(const char* other, size_t pos, size_t n) const
{
    return _find_last_of(other, n, pos, true);
}

size_t string::find_last_of(char c, size_t pos) const
{
    return _find_last_of(&c, 1, pos, true);
}

size_t string::find_first_not_of(const string& other, size_t pos) const
{
    return _find_first_of(other._string, other._length, pos, false);
}

size_t string::find_first_not_of(const char* other, size_t pos) const
{
    return _find_first_of(other, cstrlen(other), pos, false);
}

size_t string::find_first_not_of(const char* other, size_t pos, size_t n) const
{
    return _find_first_of(other, n, pos, false);
}

size_t string::find_first_not_of(char c, size_t pos) const
{
    return _find_first_of(&c, 1, pos, false);
}

size_t string::find_last_not_of(const string& other, size_t pos) const
{
    return _find_last_of(other._string, other._length, pos, false);
}

size_t string::find_last_not_of(const char* other, size_t pos) const
{
    return _find_last_of(other, cstrlen(other), pos, false);
}

size_t string::find_last_not_of(const char* other, size_t pos, size_t n) const
{
    return _find_last_of(other, n, pos, false);
}

size_t string::find_last_not_of(char c, size_t pos) const
{
    return _find_last_of(&c, 1, pos, false);
}

size_t string::_find(const char* other, size_t len, size_t pos) const
{
    size_t toReturn = npos;

    if (pos == npos)
        pos = this->_length - 1;

    for (; pos <= this->_length - len; ++pos)
        if (operator[](pos) == *other && _find_compare(other, len, pos))
        {
            toReturn = pos;
            break;
        }

    return toReturn;
}

size_t string::_rfind(const char* other, size_t len, size_t pos) const
{
    size_t toReturn = npos;

    if (pos == npos || pos + len > this->_length + 1)
        pos = this->_length - len + 1;

    for (int i = pos; 0 <= i; --i)
        if (operator[](i) == *other && _find_compare(other, len, i))
        {
            toReturn = i;
            break;
        }

    return toReturn;
}

size_t string::_find_first_of(const char* other, size_t len, size_t pos, bool isTrue) const
{
    size_t toReturn = npos;

    if (pos == npos)
        pos = this->_length - 1;

    for (; pos < this->_length; ++pos)
        if (_find_of_compare(other, len, pos, isTrue))
        {
            toReturn = pos;
            break;
        }

    return toReturn;
}

size_t string::_find_last_of(const char* other, size_t len, size_t pos, bool isTrue) const
{
    size_t toReturn = npos;

    if (pos == npos)
        pos = this->_length - 1;

    for (int i = pos; 0 <= i; --i)
        if (_find_of_compare(other, len, i, isTrue))
        {
            toReturn = i;
            break;
        }

    return toReturn;
}

bool string::_find_compare(const char* other, size_t len, size_t pos) const
{
    for (size_t i = 1; i < len; ++i)
        if (operator[](pos + i) != *(other + i))
            return false;
        else
            return true;
}

bool string::_find_of_compare(const char* other, size_t len, size_t pos, bool isTrue) const
{
    for (size_t i = 0; i < len; ++i)
        if (operator[](pos) == *(other + i))
            return isTrue;
        else
            return !isTrue;
}

string::iterator::iterator(string* str, long int index) noexcept
    : str_{ str }, index_{ index } {}

string::iterator::iterator(const iterator& itr) noexcept
    : str_{ itr.str_ }, index_{ itr.index_ } {}

string::iterator::iterator(iterator&& rval) noexcept
    : str_{ rval.str_ }, index_{ rval.index_ } {}

string::iterator::~iterator() noexcept
{
    str_ = nullptr;
    index_ = 0;
}

string::iterator& string::iterator::operator = (const iterator& rhs) noexcept
{
    str_ = rhs.str_;
    index_ = rhs.index_;
    return *this;
}

string::iterator& string::iterator::operator = (iterator&& rhs) noexcept
{
    str_ = rhs.str_;
    index_ = rhs.index_;
    return *this;
}

bool string::iterator::operator != (const iterator& rhs) const noexcept
{
    return (str_ != rhs.str_) || (index_ != rhs.index_);
}

bool string::iterator::operator == (const iterator& rhs) const noexcept
{
    return (str_ == rhs.str_) && (index_ == rhs.index_);
}

string::iterator& string::iterator::operator ++ () noexcept
{
    ++index_;
    return *this;
}

string::iterator string::iterator::operator ++ (int) noexcept
{
    auto temp = *this;
    ++(*this);
    return temp;
}

string::iterator& string::iterator::operator -- () noexcept
{
    --index_;
    return *this;
}

string::iterator string::iterator::operator -- (int) noexcept
{
    auto temp = *this;
    --(*this);
    return temp;
}

const char& string::iterator::operator * () const
{
    return (*str_)[index_];
}

string::iterator string::begin()
{
    return iterator(this);
}

string::iterator string::end()
{
    return iterator(this, static_cast<long int>(this->_length));
}

string::const_iterator::const_iterator(const string* str, long int index) noexcept
    : str_{ str }, index_{ index } {}

string::const_iterator::const_iterator(const const_iterator& itr) noexcept
    : str_{ itr.str_ }, index_{ itr.index_ } {}

string::const_iterator::const_iterator(const_iterator&& rval) noexcept
    : str_{ rval.str_ }, index_{ rval.index_ } {}

string::const_iterator::~const_iterator() noexcept
{
    str_ = nullptr;
    index_ = 0;
}

string::const_iterator& string::const_iterator::operator = (const const_iterator& rhs) noexcept
{
    str_ = rhs.str_;
    index_ = rhs.index_;
    return *this;
}

string::const_iterator& string::const_iterator::operator = (const_iterator&& rhs) noexcept
{
    str_ = rhs.str_;
    index_ = rhs.index_;
    return *this;
}

bool string::const_iterator::operator != (const const_iterator& rhs) const noexcept
{
    return (str_ != rhs.str_) || (index_ != rhs.index_);
}

bool string::const_iterator::operator == (const const_iterator& rhs) const noexcept
{
    return (str_ == rhs.str_) && (index_ == rhs.index_);
}

string::const_iterator& string::const_iterator::operator ++ () noexcept
{
    ++index_;
    return *this;
}

string::const_iterator string::const_iterator::operator ++ (int) noexcept
{
    auto temp = *this;
    ++(*this);
    return temp;
}

string::const_iterator& string::const_iterator::operator -- () noexcept
{
    --index_;
    return *this;
}

string::const_iterator string::const_iterator::operator -- (int) noexcept
{
    auto temp = *this;
    --(*this);
    return temp;
}

const char& string::const_iterator::operator * () const
{
    return (*str_)[index_];
}

string::const_iterator string::cbegin() const
{
    return const_iterator(this, 0);
}

string::const_iterator string::cend() const
{
    return const_iterator(this, static_cast<long int>(this->_length));
}

string::reverse_iterator::reverse_iterator(string* str, long int index) noexcept
    : str_{ str }, index_{ index } {}

string::reverse_iterator::reverse_iterator(const reverse_iterator& itr) noexcept
    : str_{ itr.str_ }, index_{ itr.index_ } {}

string::reverse_iterator::reverse_iterator(reverse_iterator&& rval) noexcept
    : str_{ rval.str_ }, index_{ rval.index_ } {}

string::reverse_iterator::~reverse_iterator() noexcept
{
    str_ = nullptr;
    index_ = 0;
}

string::reverse_iterator& string::reverse_iterator::operator = (const reverse_iterator& rhs) noexcept
{
    str_ = rhs.str_;
    index_ = rhs.index_;
    return *this;
}

string::reverse_iterator& string::reverse_iterator::operator = (reverse_iterator&& rhs) noexcept
{
    str_ = rhs.str_;
    index_ = rhs.index_;
    return *this;
}

bool string::reverse_iterator::operator != (const reverse_iterator& rhs) const noexcept
{
    return (str_ != rhs.str_) || (index_ != rhs.index_);
}

bool string::reverse_iterator::operator == (const reverse_iterator& rhs) const noexcept
{
    return (str_ == rhs.str_) && (index_ == rhs.index_);
}

string::reverse_iterator& string::reverse_iterator::operator ++ () noexcept
{
    --index_;
    return *this;
}

string::reverse_iterator string::reverse_iterator::operator ++ (int) noexcept
{
    auto temp = *this;
    ++(*this);
    return temp;
}

string::reverse_iterator& string::reverse_iterator::operator -- () noexcept
{
    ++index_;
    return *this;
}

string::reverse_iterator string::reverse_iterator::operator -- (int) noexcept
{
    auto temp = *this;
    --(*this);
    return temp;
}

const char& string::reverse_iterator::operator * () const
{
    return (*str_)[index_];
}

string::reverse_iterator string::rbegin()
{
    return reverse_iterator(this, static_cast<long int>(this->_length - 1));
}

string::reverse_iterator string::rend()
{
    return reverse_iterator(this, -1);
}

string::reverse_const_iterator::reverse_const_iterator(const string* str, long int index) noexcept
    : str_{ str }, index_{ index } {}

string::reverse_const_iterator::reverse_const_iterator(const reverse_const_iterator& itr) noexcept
    : str_{ itr.str_ }, index_{ itr.index_ } {}

string::reverse_const_iterator::reverse_const_iterator(reverse_const_iterator&& rval) noexcept
    : str_{ rval.str_ }, index_{ rval.index_ } {}

string::reverse_const_iterator::~reverse_const_iterator() noexcept
{
    str_ = nullptr;
    index_ = 0;
}

string::reverse_const_iterator& string::reverse_const_iterator::operator = (const reverse_const_iterator& rhs) noexcept
{
    str_ = rhs.str_;
    index_ = rhs.index_;
    return *this;
}

string::reverse_const_iterator& string::reverse_const_iterator::operator = (reverse_const_iterator&& rhs) noexcept
{
    str_ = rhs.str_;
    index_ = rhs.index_;
    return *this;
}

bool string::reverse_const_iterator::operator != (const reverse_const_iterator& rhs) const noexcept
{
    return (str_ != rhs.str_) || (index_ != rhs.index_);
}

bool string::reverse_const_iterator::operator == (const reverse_const_iterator& rhs) const noexcept
{
    return (str_ == rhs.str_) && (index_ == rhs.index_);
}

string::reverse_const_iterator& string::reverse_const_iterator::operator ++ () noexcept
{
    --index_;
    return *this;
}

string::reverse_const_iterator string::reverse_const_iterator::operator ++ (int) noexcept
{
    auto temp = *this;
    ++(*this);
    return temp;
}

string::reverse_const_iterator& string::reverse_const_iterator::operator -- () noexcept
{
    ++index_;
    return *this;
}

string::reverse_const_iterator string::reverse_const_iterator::operator -- (int) noexcept
{
    auto temp = *this;
    --(*this);
    return temp;
}

const char& string::reverse_const_iterator::operator * () const
{
    return (*str_)[index_];
}

string::reverse_const_iterator string::crbegin() const
{
    return reverse_const_iterator(this, static_cast<long int>(this->_length - 1));
}

string::reverse_const_iterator string::crend() const
{
    return reverse_const_iterator(this, -1);
}