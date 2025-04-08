/*
Austin Nguyen
anguyen10@scu.edu
CSEN79 Lab 5 mystring.cxx
Description: cxx file for implementation of all function prototypes in 'mystring.h'
*/

#include "mystring.h"

using namespace coen79_lab5;
namespace coen79_lab5{

    //string(const char str[ ] = "") -- default argument is the empty string.
    //Precondition: str is an ordinary null-terminated string.
    //Postcondition: The string contains the sequence of chars from str.
    string::string(const char str[]){
        current_length = strlen(str);
        allocated = current_length + 1;     //+1 for NULL

        characters = new char [allocated];
        strncpy(characters, str, allocated);

        characters[allocated - 1] = '\0';   //null termination

        //if STR is longer than allocated, manually
        if(current_length >= allocated-1){
            characters[allocated -1] = '\0';
        }
    }

    //string(char c)
    //Postcondition: The string contains c and then the null character.
    string::string(char c){
        current_length = 1;
        allocated = 2;
        characters = new char [allocated];
        characters[0] = c;
        characters[1] = '\0';
    }

    //string(const string& source) -- Copy Constructor.
    //Postcondition: The string becomes a copy of the source string.
    string::string(const string& source){
        current_length = source.current_length;
        allocated = source.allocated;
        characters = new char[allocated];
        strncpy(characters, source.characters,allocated);
    }

    string::~string(){
        current_length = 0;
        allocated = 0;
        delete characters;
    }

    //void insert(const string& source, unsigned int position)
    //Precondtion: position <= length()
    //Postcondition: The source string is inserted into this string before
    //the character at the given index.
    void string::insert(const string& source, unsigned int position){
        if(position > current_length){      // Check if current position is valid
            return;                         // Invalid position, do nothing and return
        }

        // Calculate new length (nlength) and allocate memory for updated string
        size_t nlength = current_length + source.current_length;
        char *nchar = new char[nlength +1]; // '+1' for Null Terminator

        //Copy new character before insertion
        strncpy(nchar, characters, position);
        //Copy Characters from source string
        strncpy(nchar + position, source.characters, source.current_length);
        //Copy characters after insertion
        strncpy(nchar + position + source.current_length, characters + position, current_length - position);

        //Null terminate new string
        nchar[nlength] = '\0';

        //Deallocate memory
        delete[] characters;

        //Update pointers and current length
        characters = nchar;
        current_length = nlength;
        allocated = nlength + 1;
    }

    //void dlt(unsigned int position, unsigned int num)
    //Precondition: (position + num) <= length( ).
    //Postcondition: num characters are deleted from the sequence, starting
    //at index position.
    void string::dlt(unsigned int position, unsigned int num){
        if (position + num > current_length){
            return;
        }
        size_t nlength = current_length - num;
        char *nchar = new char[nlength +1];

        strncpy(nchar, characters, position);
        strncpy(nchar + position, characters + position + num, current_length-position - num);

        nchar[nlength] = '\0';
        delete[] characters;

        characters = nchar;
        current_length = nlength;
        allocated = nlength + 1;
    }

    //void replace(char c, unsigned int position)
    //Precondition: position < length()
    //Postcondtion: the character at the given position in the string is
    //replaced with c.
    void string::replace(char c, unsigned int position){
        if (position >= current_length){
            return;
        }
        characters[position] = c;
    }

    //void replace(const string& source, unsigned int position)
    //Precondition: (position + source.length()) <= length()
    //Postcondtion: the characters in this string starting at position are
    //replaced with those in the source srting.
    void string::replace(const string& source, unsigned int position){
        if(position + source.current_length > current_length){
            return;
        }

        size_t nlength = current_length - source.current_length +source.current_length;
        char *nchar = new char[nlength + 1];

        strncpy(nchar, characters, position);
        strncpy(nchar + position, source.characters, source.current_length);
        strncpy(nchar + position + source.current_length, characters + position + source.current_length, current_length - position - source.current_length);

        nchar[nlength] = '\0';

        delete[] characters;
        characters = nchar;
        current_length = nlength;
        allocated = nlength + 1;
    }

    //void reserve(size_t n)
    //Postcondition: All functions will now work efficiently (without
    //allocating new memory) until n characters are in the string.
    void string::reserve(size_t n){
        if (n > allocated){
            char* nstorage = new char[n];
            std::memcpy(nstorage, characters,  current_length);
            delete[] characters;
            characters = nstorage;
            allocated += n;
        }
    }

    string& string::operator =(const string& source){
        if(this == &source){
            return *this;
        }
        if(source.current_length > allocated){
            char* nchar = new char[source.current_length +1];
            delete[] characters;
            characters = nchar;
            allocated = source.current_length + 1;
        }

        std:: memcpy(characters, source.characters, source.current_length);
        characters[source.current_length] = '\0';
        current_length = source.current_length;
        return *this;
    }

    void string::operator+=(const string& addend){
        size_t min_alloc = current_length + addend.length() + 1;
        reserve(min_alloc);
        
        strncat(characters, addend.characters, addend.length()); 

        current_length += addend.length();
        characters[current_length] = '\0';
    }

    void string:: operator+=(const char addend[]){
        size_t addend_length = std::strlen(addend);
        size_t nlength = current_length + addend_length;
        if (nlength >= allocated){
            reserve(nlength +1);
        }
        std:: memcpy(characters + current_length, addend, addend_length);
        current_length = nlength;
        characters[current_length] = '\0';
    }


    void string::operator+=(char addend){
        if(current_length +1 >= allocated){
            reserve(current_length + 2);
        }
        characters[current_length] = addend;
        current_length++;
        characters[current_length] = '\0';
    }

    char string::operator[](size_t position) const {
        if(position >= current_length){
            return '\0';
        }
        return characters[position];
    }

    int string::search(char c) const{
        for(size_t i = 0; i < current_length; ++i){
            if(characters[i] == c){
                return static_cast<int>(i);
            }
        }
        return -1;
    }

    int string::search(const string& substring) const {
        // Iterate through the characters in the string
        for (size_t i = 0; i <= current_length - substring.current_length; ++i) {
            // Check if the substring matches the characters starting at index i
            if (strncmp(characters + i, substring.characters, substring.current_length) == 0) {
                return static_cast<int>(i); // Found the substring, return its index
            }
        }
        return -1; // Substring not found
    }

    //unsigned int cout(char c) const
    //Postcondition: The count of the occurence of the character c within this
    //string is returned.
    unsigned int string::count(char c) const{
        char* location;
        size_t count = 0;
        char* substring = characters;

        for (;;) {
            location = strchr(substring, c);

            if (location) { 
                ++count; 
                substring = location + 1;
            } else { break; }
        }
        
        return count;
    }

    bool operator ==(const string& s1, const string& s2) {
        size_t len = s1.length() > s2.length() ? s1.length() : s2.length();
        return strncmp(s1.characters, s2.characters, len) == 0;
    }

    // Check if two strings are not the same
    bool operator !=(const string& s1, const string& s2) {
        return ~(s1 == s2);
    }

    // Check if string s1 has higher ASCII values
    bool operator >(const string& s1, const string& s2) {
        return std::strncmp(s1.characters, s2.characters, std::max(s1.current_length, s2.current_length)) > 0;
    }

    // Check if string s2 has higher ASCII values
    bool operator <(const string& s1, const string& s2) {
        return std::strncmp(s1.characters, s2.characters, std::max(s1.current_length, s2.current_length)) < 0;
    }

    // Check if s1 is equal to or larger than s2
    bool operator >=(const string& s1, const string& s2) {
        return (s1 > s2) || (s1 == s2);
    }

    // Check if s1 is equal to or smaller than s2
    bool operator <=(const string& s1, const string& s2) {
        return (s1 < s2) || (s1 == s2);
    }

    //ostream& operator <<(ostream& outs, const string& source)
    //Postcondition: The sequence of characters in source has been written
    //to outs. The return value is the ostream outs.
    std::ostream& operator <<(std::ostream& outs, const string& source) {
        outs << source.characters;                                      
        return outs;                                                    
    }

    //string operator +(const string& s1, const string& s2)
    //Postcondition: The string returned is the catenation of s1 and s2.
    string operator+(const string& s1, const string& s2) {
        string temp;                                                    
        temp.reserve(s1.length()+s2.length()+1);                        
        temp += s1;                                                     
        temp += s2;                                                     
        return temp;                                                    
    }

    // Returns the concatenation of string s1 and char array addend[]
    string operator +(const string& s1, const char addend[ ]) {
        string temp;                                                    
        temp.reserve(s1.length()+strlen(addend)+1);                     
        temp += s1;                                                     
        temp += addend;                                                 
        return temp;                                                    
    }
    std::istream& operator>>(std::istream& ins, string& target) {
        char temp[1000];
        ins >> std::setw(1000) >> temp;
        target = string(temp);
        return ins;    
    }

}
