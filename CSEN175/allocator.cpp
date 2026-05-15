/*
 * File:	allocator.cpp
 *
 * Description:	This file contains the member function definitions for
 *		functions dealing with storage allocation.  The actual
 *		classes are declared elsewhere, mainly in Tree.h.
 *
 *		Extra functionality:
 *		- computing space for stack-based arguments
 *		- maintaining minimum offset in nested blocks
 *		- allocation within statements
 */

# include <cassert>
# include <algorithm>
# include "machine.h"
# include "tokens.h"
# include "Tree.h"

using std::min;
using std::max;

static bool isleaf;
static int current, minimum;
static unsigned argspace;


/*
 * Function:	align
 *
 * Description:	Align the specified offset and return the aligned offset.
 */

int align(int offset, int alignment)
{
    if (offset < 0)
	return (offset - (alignment - 1)) / alignment * alignment;

    return (offset + (alignment - 1)) / alignment * alignment;
}


/*
 * Function:	Type::size
 *
 * Description:	Return the size of a type in bytes.
 */

unsigned long Type::size() const
{
    unsigned long count;


    assert(_kind != FUNCTION && _kind != ERROR);
    count = (_kind == ARRAY ? _length : 1);

    if (_indirection > 0)
	return count * SIZEOF_PTR;

    if (_specifier == CHAR)
	return count * SIZEOF_CHAR;

    if (_specifier == INT)
	return count * SIZEOF_INT;

    if (_specifier == LONG)
	return count * SIZEOF_LONG;

    return 0;
}


/*
 * Function:	Type::alignment
 *
 * Description:	Return the alignment of a type in bytes.
 */

unsigned Type::alignment() const
{
    assert(_kind != FUNCTION && _kind != ERROR);

    if (_indirection > 0)
	return ALIGNOF_PTR;

    if (_specifier == CHAR)
	return ALIGNOF_CHAR;

    if (_specifier == INT)
	return ALIGNOF_INT;

    if (_specifier == LONG)
	return ALIGNOF_LONG;

    return 0;
}


/*
 * Function:	Binary::allocate
 *
 * Description:	Allocate storage for this binary expression, which
 *		essentially means computing any extra stack space for its
 *		subexpressions.
 */

void Binary::allocate()
{
    _left->allocate();
    _right->allocate();
}


/*
 * Function:	Unary::allocate
 *
 * Description:	Allocate storage for this unary expression, which
 *		essentially means computing any extra stack space for its
 *		subexpression.
 */

void Unary::allocate()
{
    _expr->allocate();
}


/*
 * Function:	Call::allocate
 *
 * Description:	Allocate space for this call expression, which essentially
 *		means allocating space any extra stack space for its
 *		arguments.
 */

void Call::allocate()
{
    isleaf = false;

    if (_args.size() > NUM_PARAM_REGS) {
	unsigned bytes = (_args.size() - NUM_PARAM_REGS) * SIZEOF_REG;
	argspace = max(argspace, bytes);
    }

    for (auto arg : _args)
	arg->allocate();
}


/*
 * Function:	Assignment::allocate
 *
 * Description:	Allocate storage for this assignment statement, which
 *		essentially means computing any extra stack space for its
 *		expressions.
 */

void Assignment::allocate()
{
    _left->allocate();
    _right->allocate();
}


/*
 * Function:	Return::allocate
 *
 * Description:	Allocate storage for this return statement, which
 *		essentially means computing any extra stack space for its
 *		expression.
 */

void Return::allocate()
{
    _expr->allocate();
}


/*
 * Function:	Block::allocate
 *
 * Description:	Allocate storage for this block.  We assign decreasing
 *		offsets for all symbols declared within this block, and
 *		then for all symbols declared within any nested block.
 *		Only symbols that have not already been allocated an offset
 *		will be assigned one, since the parameters are already
 *		assigned special offsets.
 */

void Block::allocate()
{
    int saved;
    const Symbols &symbols = _decls->symbols();


    for (auto symbol : symbols)
	if (symbol->offset == 0 && !symbol->type().isFunction()) {
	    current -= symbol->type().size();
	    current = align(current, symbol->type().alignment());
	    symbol->offset = current;
	}

    minimum = min(minimum, current);

    for (auto stmt : _stmts) {
	saved = current;
	stmt->allocate();
	minimum = min(minimum, current);
	current = saved;
    }
}


/*
 * Function:	While::allocate
 *
 * Description:	Allocate storage for this while statement, which
 *		essentially means allocating storage for variables declared
 *		as part of its statement and computing any extra stack
 *		space needed for its expression.
 */

void While::allocate()
{
    _expr->allocate();
    _stmt->allocate();
}


/*
 * Function:	For::allocate
 *
 * Description:	Allocate storage for this for statement, which
 *		essentially means allocating storage for variables declared
 *		as part of its statement and computing any extra stack
 *		space needed for its expressions.
 */

void For::allocate()
{
    _init->allocate();
    _expr->allocate();
    _incr->allocate();
    _stmt->allocate();
}


/*
 * Function:	If::allocate
 *
 * Description:	Allocate storage for this if-then or if-then-else
 *		statement, which essentially means allocating storage for
 *		variables declared as part of its statements and computing
 *		any extra stack space needed for its expression.
 */

void If::allocate()
{
    int saved;


    saved = current;
    _expr->allocate();
    _thenStmt->allocate();

    if (_elseStmt != nullptr) {
	current = saved;
	_elseStmt->allocate();
	minimum = min(minimum, current);
    }
}


/*
 * Function:	Simple::allocate
 *
 * Description:	Allocate storage for a simple (expression) statement, which
 *		simply means computing any extra stack space needed for its
 *		expression.
 */

void Simple::allocate()
{
    _expr->allocate();
}


/*
 * Function:	Function::allocate
 *
 * Description:	Allocate storage for this function and return the number of
 *		bytes required.  The parameters are allocated offsets as
 *		well, starting with the given offset.
 */

void Function::allocate()
{
    int offset = _paramOffset;
    const Types *formals = _id->type().parameters();
    const Symbols &symbols = _body->declarations()->symbols();


    if (formals != nullptr)
	for (unsigned i = NUM_PARAM_REGS; i < formals->size(); i ++) {
	    symbols[i]->offset = offset;
	    offset += SIZEOF_REG;
	}

    current = 0;
    minimum = 0;
    argspace = 0;
    isleaf = true;

    _body->allocate();

    _isLeaf = isleaf;
    _argSpace = argspace;
    _localSpace = -minimum;


    /* Here we assume that we will never run out of registers.  However,
       all registers may be in use at the time of a function call and need
       to be saved to the stack.  Therefore, if this function itself calls
       any other function, we need this additional space. */

    _numTemps = (isleaf ? 0 : _numCallerSaved);
    _tempSpace = _numTemps * SIZEOF_REG;
}
