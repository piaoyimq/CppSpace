/*
 * OverloadOperator.cpp
 *
 *  Created on: Nov 15, 2015
 *      Author: ezhweib
 */


#include <iostream>
#include <vector>

    template <class AllocT = std::allocator<char> > class String {
    public :

        ~String();

        explicit String(const std::string& str, const AllocT& allocator = AllocT());

        void print(std::ostream& os) const;

        char& operator[](std::size_t pos);

        const char& operator[](std::size_t pos) const;

        std::size_t size() const;

        String<AllocT>* operator=(const std::string& rhs);

#if 1//zhuweibo operator
        String&
             operator+=(const String& __str)
             { }

        basic_string&
             operator=(const basic_string& __str)
             { return this->assign(__str); }

        template<typename _CharT, typename _Traits, typename _Alloc>
            basic_string<_CharT, _Traits, _Alloc>
            operator+(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
                  const basic_string<_CharT, _Traits, _Alloc>& __rhs)
            {
              basic_string<_CharT, _Traits, _Alloc> __str(__lhs);
              __str.append(__rhs);
              return __str;
            }

        template<typename _CharT, typename _Traits, typename _Alloc>
            inline bool
            operator==(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
                   const basic_string<_CharT, _Traits, _Alloc>& __rhs)
            { return __lhs.compare(__rhs) == 0; }

        template<typename _CharT, typename _Traits, typename _Alloc>
            inline bool
            operator!=(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
                   const basic_string<_CharT, _Traits, _Alloc>& __rhs)
            { return !(__lhs == __rhs); }

        template<typename _CharT, typename _Traits, typename _Alloc>
            inline bool
            operator<(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
                  const basic_string<_CharT, _Traits, _Alloc>& __rhs)
            { return __lhs.compare(__rhs) < 0; }

        template<typename _CharT, typename _Traits, typename _Alloc>
            inline bool
            operator<(const _CharT* __lhs,
                  const basic_string<_CharT, _Traits, _Alloc>& __rhs)
            { return __rhs.compare(__lhs) > 0; }

        template<typename _CharT, typename _Traits, typename _Alloc>
            inline bool
            operator<=(const basic_string<_CharT, _Traits, _Alloc>& __lhs,
                   const basic_string<_CharT, _Traits, _Alloc>& __rhs)
            { return __lhs.compare(__rhs) <= 0; }

        template<typename _CharT, typename _Traits, typename _Alloc>
           basic_istream<_CharT, _Traits>&
           operator>>(basic_istream<_CharT, _Traits>& __is,
                  basic_string<_CharT, _Traits, _Alloc>& __str);

#endif

        explicit String(const AllocT& allocator = AllocT());

        template <class AllocU> inline String<AllocT>* operator=(const String<AllocU>& rhs) {
            itsVector = rhs.itsVector;
        }

        void append(const std::string& str);

        void resize(std::size_t n);


    private :

        std::vector<char,AllocT> itsVector;
    };


    template <class AllocT> String<AllocT>::String(const std::string& str, const AllocT& allocator) : itsVector(str.c_str(), str.c_str() + str.size() + 1, allocator) {
    }

    template <class AllocT> void String<AllocT>::print(std::ostream& os) const {
        os << &itsVector[0];
    }

    template <class AllocT> char& String<AllocT>::operator[](std::size_t pos) {
        return itsVector[pos];
    }

    template <class AllocT> const char& String<AllocT>::operator[](std::size_t pos) const {
        return itsVector[pos];
    }

    template <class AllocT> std::size_t String<AllocT>::size() const {
        if (!itsVector.empty())
        {
            return strlen(&itsVector[0]);
        }
        return 0;
    }

    template <class AllocT> String<AllocT>* String<AllocT>::operator=(const std::string& rhs) {
        itsVector.resize(rhs.size() + 1);
        memcpy(&itsVector[0], rhs.c_str(), rhs.size());
        itsVector[rhs.size()] = '\0';
    }

    template <class AllocT> String<AllocT>::String(const AllocT& allocator) : itsVector(allocator) {
    }

    template <class AllocT> void String<AllocT>::append(const std::string& str) {
        const std::size_t end = size();
        itsVector.resize(size() + str.size() + 1);
        memcpy(&itsVector[end], str.c_str(), str.size() + 1);
    }

    template <class AllocT> void String<AllocT>::resize(std::size_t n) {
        const std::size_t len = size();
        itsVector.resize(n);
        itsVector[len] = '\0';
    }

    template <class AllocT> String<AllocT>::~String() {
    }


int main(){
    std::string s;

}
