#pragma once

class string
{
public:
    /* constructors */

    string();

    string(const char*);

    /* constructors */

    /* destructors */

    ~string();

    /* destructors */

    /* element access */

    const char& operator [] (size_t) const;

    const char& at(size_t) const;

    const char& front() const;
    const char& back() const;

    /* element access */

    /* operators */

    string& operator=(const string&);
    string& operator+=(const string&);
    friend string operator+(const string&, const string&);

    friend bool operator==(const string&, const string&);
    friend bool operator!=(const string&, const string&);
    friend bool operator>(const string&, const string&);
    friend bool operator>=(const string&, const string&);
    friend bool operator<(const string&, const string&);
    friend bool operator<=(const string&, const string&);

    /* operators */

    /* constants */

    static const size_t npos = -1;

    /* constants */

    /* operations */

    const char* c_str() { return this->_string; }
    size_t length() { return this->_length; };

    const char* substr(int) const;
    const char* substr(int, int) const;

    size_t find(const string& other, size_t pos = 0) const;
    size_t find(const char* s, size_t pos = 0) const;
    size_t find(const char* s, size_t pos, size_t n) const;
    size_t find(char c, size_t pos = 0) const;

    size_t rfind(const string& other, size_t pos = npos) const;
    size_t rfind(const char* s, size_t pos = npos) const;
    size_t rfind(const char* s, size_t pos, size_t n) const;
    size_t rfind(char c, size_t pos = npos) const;

    size_t find_first_of(const string& other, size_t pos = 0) const;
    size_t find_first_of(const char* other, size_t pos = 0) const;
    size_t find_first_of(const char* other, size_t pos, size_t n) const;
    size_t find_first_of(char c, size_t pos = 0) const;

    size_t find_last_of(const string& other, size_t pos = string::npos) const;
    size_t find_last_of(const char* other, size_t pos = string::npos) const;
    size_t find_last_of(const char* other, size_t pos, size_t n) const;
    size_t find_last_of(char c, size_t pos = string::npos) const;

    size_t find_first_not_of(const string& other, size_t pos = 0) const;
    size_t find_first_not_of(const char* other, size_t pos = 0) const;
    size_t find_first_not_of(const char* other, size_t pos, size_t n) const;
    size_t find_first_not_of(char c, size_t pos = 0) const;

    size_t find_last_not_of(const string& other, size_t pos = string::npos) const;
    size_t find_last_not_of(const char* other, size_t pos = string::npos) const;
    size_t find_last_not_of(const char* other, size_t pos, size_t n) const;
    size_t find_last_not_of(char c, size_t pos = string::npos) const;

private:
    size_t _find(const char* other, size_t len, size_t pos) const;
    size_t _rfind(const char* other, size_t len, size_t pos) const;

    size_t _find_first_of(const char* other, size_t len, size_t pos, bool isTrue) const;
    size_t _find_last_of(const char* other, size_t len, size_t pos, bool isTrue) const;

    bool _find_compare(const char* other, size_t len, size_t pos) const;
    bool _find_of_compare(const char* other, size_t len, size_t pos, bool isTrue) const;

public:

    /* operations */

    /* iterators */

    class iterator
    {
    private:
        string* str_;
        long int index_;
    public:
        iterator(string* = nullptr, long int = 0) noexcept;
        iterator(const iterator&) noexcept;
        iterator(iterator&&) noexcept;
        ~iterator() noexcept;

        iterator& operator = (const iterator&) noexcept;
        iterator& operator = (iterator&&) noexcept;
        bool operator != (const iterator&) const noexcept;
        bool operator == (const iterator&) const noexcept;
        iterator& operator ++ () noexcept;
        iterator operator ++ (int) noexcept;
        iterator& operator -- () noexcept;
        iterator operator -- (int) noexcept;
        const char& operator * () const;
    };

    iterator begin();
    iterator end();

    class const_iterator
    {
    private:
        const string* str_;
        long int index_;
    public:
        const_iterator(const string*, long int) noexcept;
        const_iterator(const const_iterator&) noexcept;
        const_iterator(const_iterator&&) noexcept;
        ~const_iterator() noexcept;

        const_iterator& operator = (const const_iterator&) noexcept;
        const_iterator& operator = (const_iterator&&) noexcept;
        bool operator != (const const_iterator&) const noexcept;
        bool operator == (const const_iterator&) const noexcept;
        const_iterator& operator ++ () noexcept;
        const_iterator operator ++ (int) noexcept;
        const_iterator& operator -- () noexcept;
        const_iterator operator -- (int) noexcept;
        const char& operator * () const;
    };

    const_iterator cbegin() const;
    const_iterator cend() const;

    class reverse_iterator
    {
    private:
        string* str_;
        long int index_;
    public:
        reverse_iterator(string* = nullptr, long int = 0) noexcept;
        reverse_iterator(const reverse_iterator&) noexcept;
        reverse_iterator(reverse_iterator&&) noexcept;
        ~reverse_iterator() noexcept;

        reverse_iterator& operator = (const reverse_iterator&) noexcept;
        reverse_iterator& operator = (reverse_iterator&&) noexcept;
        bool operator != (const reverse_iterator&) const noexcept;
        bool operator == (const reverse_iterator&) const noexcept;
        reverse_iterator& operator ++ () noexcept;
        reverse_iterator operator ++ (int) noexcept;
        reverse_iterator& operator -- () noexcept;
        reverse_iterator operator -- (int) noexcept;
        const char& operator * () const;
    };

    reverse_iterator rbegin();
    reverse_iterator rend();

    class reverse_const_iterator
    {
    private:
        const string* str_;
        long int index_;
    public:
        reverse_const_iterator(const string*, long int) noexcept;
        reverse_const_iterator(const reverse_const_iterator&) noexcept;
        reverse_const_iterator(reverse_const_iterator&&) noexcept;
        ~reverse_const_iterator() noexcept;

        reverse_const_iterator& operator = (const reverse_const_iterator&) noexcept;
        reverse_const_iterator& operator = (reverse_const_iterator&&) noexcept;
        bool operator != (const reverse_const_iterator&) const noexcept;
        bool operator == (const reverse_const_iterator&) const noexcept;
        reverse_const_iterator& operator ++ () noexcept;
        reverse_const_iterator operator ++ (int) noexcept;
        reverse_const_iterator& operator -- () noexcept;
        reverse_const_iterator operator -- (int) noexcept;
        const char& operator * () const;
    };

    reverse_const_iterator crbegin() const;
    reverse_const_iterator crend() const;

    /* iterators */

private:
    const char* _string;

    size_t _length;
};