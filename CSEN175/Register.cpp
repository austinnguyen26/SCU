/*
 * File:	Register.cpp
 *
 * Description:	This file contains the member function definitions for
 *		registers on the ARM 64-bit processor.
 */

# include "Tree.h"
# include "Register.h"

using std::ostream;
using std::string;


/*
 * Function:	Register::Register (constructor)
 *
 * Description:	Initialize this register with its correct operand names.
 */

Register::Register(const string &name64, const string &name32)
    : _name64(name64), _name32(name32), node(nullptr)
{
}


/*
 * Function:	Register::name
 *
 * Description:	Return the correct operand name given an access size.  The
 *		default is to return the 64-bit operand name.
 */

const string &Register::name(unsigned size) const
{
    return size == 4 || size == 1 ? _name32 : _name64;
}


/*
 * Function:	operator <<
 *
 * Description:	Write a register to a stream.
 */

ostream &operator <<(ostream &ostr, const Register *reg)
{
    return ostr << reg->name(8);
}
