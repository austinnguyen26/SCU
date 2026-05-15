/*
 * File:	Register.h
 *
 * Description:	This file contains the class definition for registers on
 *		the ARM 64-bit processor.
 */

# ifndef REGISTER_H
# define REGISTER_H
# include <string>
# include <ostream>

class Register {
    typedef std::string string;
    string _name64;
    string _name32;

public:
    class Expression *node;

    Register(const string &name64, const string &name32);
    const string &name(unsigned size) const;

};

std::ostream &operator <<(std::ostream &ostr, const Register *reg);

# endif /* REGISTER_H */
