/*
Austin Nguyen
anguyen10@scu.edu
CSEN Lab 5 tester.cxx
Description: File designed to test cases with implementation in 'mystring.cxx'
*/

#include "mystring.h"

using namespace coen79_lab5;

int main() {
    // Test constructors
    string s1;  // Default constructor
    assert(s1.length() == 0);   //Checking if Default Const. has length of 0
    std::cout << "Default constructor passed." << std::endl;

    string s2("Hello");
    assert(s2.length() == 5);   //Length of "Hello" is 5
    std::cout << "C-string constructor passed." << std::endl;

    string s3('A');
    assert(s3.length() == 1 && s3[0] == 'A');
    std::cout << "Single-character constructor passed." << std::endl;

    string s4(s2);  // Copy constructor
    assert(s4 == s2);
    std::cout << "Copy constructor passed." << std::endl;

    // Test assignment operator
    string s5("Hello");
    string s6 = s5;
    assert(s6 == s5);
    std::cout << "Assignment operator passed." << std::endl;

    // Test append
    string s7("Hello");
    string s8(" World");

    s7 += s8;  // Append string
    assert(s7 == "Hello World");
    std::cout << "String append passed." << std::endl;

    s7 += '!';  // Append character
    assert(s7 == "Hello World!");
    std::cout << "Char append passed." << std::endl;

    s7 += " Welcome!";
    assert(s7 == "Hello World! Welcome!");
    std::cout << "C-string append passed." << std::endl;

    // Test insert
    string s9("Hello!");
    s9.insert(string(" World"), 5);
    assert(s9 == "Hello World!");
    std::cout << "Insert function passed." << std::endl;

    // Test delete
    string s10("Hello World!");
    s10.dlt(5, 6);  // Delete " World"
    assert(s10 == "Hello!");
    std::cout << "Delete function passed." << std::endl;

    // Test replace
    string s11("Hello!");
    s11.replace('?', 5);  // Replace '!' with '?'
    assert(s11 == "Hello?");
    std::cout << "Replace character passed." << std::endl;

    s11.replace(string("ipp"), 1);
    assert(s11 == "Hippo?");
    std::cout << "Replace substring passed." << std::endl;

    // Test search
    string s12("Hello World!");
    assert(s12.search('W') == 6);
    assert(s12.search('Z') == -1);
    std::cout << "Search character passed." << std::endl;

    assert(s12.search(string("World")) == 6);
    assert(s12.search(string("Earth")) == -1);
    std::cout << "Search substring passed." << std::endl;

    // Test count
    string s13("Hello World!");
    assert(s13.count('o') == 2);
    assert(s13.count('z') == 0);
    std::cout << "Count function passed." << std::endl;

    // Test index operator
    string s14("Hello");
    assert(s14[0] == 'H');
    assert(s14[4] == 'o');
    std::cout << "Index operator passed." << std::endl;

    // Test concatenation operator
    string s15("Hello");
    string s16(" World");

    string s17 = s15 + s16;
    assert(s17 == "Hello World");
    std::cout << "Concatenation operator (string) passed." << std::endl;

    string s18 = s15 + "!";
    assert(s18 == "Hello!");
    std::cout << "Concatenation operator (C-string) passed." << std::endl;

    // Test comparison operators
    string s19("Hello");
    string s20("Hello");
    string s21("World");

    assert(s19 == s20);
    assert(s19 != s21);
    assert(s21 > s19);
    assert(s19 < s21);
    std::cout << "Comparison operators passed." << std::endl;

    // Test reserve
    string s22("Hello");
    s22.reserve(20);  // Reserve memory for 20 characters
    assert(s22.length() == 5);  // Length should remain unchanged
    std::cout << "Reserve function passed." << std::endl;

    // Test input/output operators
    string s23;
    std::cout << "Enter a string: ";
    std::cin >> s23;  // Test input operator
    std::cout << "You entered: " << s23 << std::endl;  // Test output operator

    std::cout << "All Tests Passed." << std::endl;
    return 0;
}
