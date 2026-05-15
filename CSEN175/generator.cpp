/*
 * File:		generator.cpp
 *
 * Description: This file contains the public and member function
 *				definitions for the code generator for Simple C.
 *
 *				Extra functionality:
 *				- putting all the global declarations at the end
 */

# include <vector>
# include <cassert>
# include <iostream>
# include "allocator.h"
# include "generator.h"
# include "machine.h"
# include "string.h"
# include "Tree.h"
# include <map>



using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;

static string funcname, tab = "\t";
static unsigned framesize;

static vector<int> temps;
static map<string, Label> strings;

/* Parameter registers */

static Register *x0 = new Register("x0", "w0");
static Register *x1 = new Register("x1", "w1");
static Register *x2 = new Register("x2", "w2");
static Register *x3 = new Register("x3", "w3");
static Register *x4 = new Register("x4", "w4");
static Register *x5 = new Register("x5", "w5");
static Register *x6 = new Register("x6", "w6");
static Register *x7 = new Register("x7", "w7");


/* Caller-saved registers in the register pool */

static Register *x9 = new Register("x9", "w9");
static Register *x10 = new Register("x10", "w10");
static Register *x11 = new Register("x11", "w11");
static Register *x12 = new Register("x12", "w12");
static Register *x13 = new Register("x13", "w13");
static Register *x14 = new Register("x14", "w14");
static Register *x15 = new Register("x15", "w15");


/* Scratch registers for address calculations */

static Register *x16 = new Register("x16", "w16");
static Register *x17 = new Register("x17", "w17");


/* The frame pointer, link register, and stack pointer */

static Register *fp = new Register("fp", "fp");
static Register *lr = new Register("lr", "lr");
static Register *sp = new Register("sp", "sp");


/* The register sets */

typedef std::vector<Register *>Registers;

static Registers parameters = {x0, x1, x2, x3, x4, x5, x6, x7};
static Registers registers = {x9, x10, x11, x12, x13, x14, x15, x0};


/* Placeholders to be replaced with functions in the next phase. */

// # define spill(reg) (reg->node = nullptr)
// # define assign(node, reg)


// Lecture 20 Slide 4
static void assign(Expression *expr, Register *reg){
	if(expr != nullptr){
		if(expr->reg != nullptr){
			expr->reg->node = nullptr;
		}
		expr->reg = reg;
	}
	if(reg != nullptr){
		if(reg->node != nullptr){
			reg->node->reg = nullptr;
		}
		reg->node = expr;
	}
}

/*
 * Function:	loadimmediate (private)
 *
 * Description: Load an immediate value into the given register using
 *				either a MOV instruction (if the value is within range) or
 *				the LDR pseudo-instruction (if the value is not within
 *				range), which places the value in a literal pool.
 *
 *				Whether the LDR pseudo-instruction is supposed to handle
 *				values that are within range by using a MOV instruction is
 *				a bit ambiguous.  The ARM 32 documentation explicitly
 *				states the following:
 *
 *				"If the value of expr can be loaded with a valid MOV or MVN
 *				instruction, the assembler uses that instruction."
 *
 *				However the ARM 64 documentation simply states that the
 *				value is placed in a literal pool.	The clang assembler
 *				will use a MOV instruction for small, positive values but
 *				not for any negative values.  The GNU assembler always uses
 *				a literal pool.	 Hence, we use this function.
 */

static void loadimmediate(Register *reg, long val, unsigned size = 0)
{
	if (labs(val) <= MAX_IMM_MOV)
		cout << tab << "mov" << tab << reg->name(size) << ", " << val << endl;
	else
		cout << tab << "ldr" << tab << reg->name(size) << ", =" << val << endl;
}


/*
 * Function:	addoffset (private)
 *
 * Description: Adds a 64-bit offset to a source register and stores the
 *				result in a destination register.  We correctly handle any
 *				size offset, positive or negative, and omit the instruction
 *				if it's not necessary.
 */

static void addoffset(Register *dest, Register *src, int offset)
{
	string opcode;
	Register *scratch = x17;


	if (dest != src || offset != 0) {
		if (offset < 0) {
			opcode = "sub";
			offset = -offset;
		} else
			opcode = "add";

		if (offset > MAX_IMM_ADD) {
			loadimmediate(scratch, offset);
			cout << tab << opcode << tab << dest << ", " << src << ", ";
			cout << scratch << endl;
		} else {
			cout << tab << opcode << tab << dest << ", " << src << ", ";
			cout << offset << endl;
		}
	}
}


/*
 * Function:	offsetokay (private)
 *
 * Description: Return whether the given offset is okay for a load or store
 *				instruction.
 */

static bool offsetokay(int offset, unsigned size)
{
	if (offset < 0)
		return offset >= MIN_SIMM;

	if (offset <= MAX_SIMM)
		return true;

	if (size == 1 && offset <= MAX_PIMM_8)
		return true;

	if (size == 4 && offset <= MAX_PIMM_32 && offset % 4 == 0)
		return true;

	if (size == 8 && offset <= MAX_PIMM_64 && offset % 8 == 0)
		return true;

	return false;
}


/*
 * Function:	loadstore (private)
 *
 * Description: Perform a load to or store from the given register with the
 *				memory location specified by the given offset.	If the
 *				offset is out of range, it will loaded into a scratch
 *				register first.
 */

static void loadstore(const string &opcode, Register *reg, unsigned size,
		Register *base, int offset)
{
	bool okay;
	Register *scratch = x17;


	okay = offsetokay(offset, size);

	if (!okay)
		loadimmediate(scratch, offset);

	cout << tab << opcode << tab << reg->name(size) << ", [" << base;

	if (!okay)
		cout << ", " << scratch;
	else if (offset != 0)
		cout << ", " << offset;

	cout << "]" << endl;
}


/*
 * Function:	loadstore (private)
 *
 * Description: Perform a load to or store from the given register with the
 *				memory location specified by the given symbol.	A scratch
 *				register is used for global variables.
 */

static void loadstore(const string &opcode, Register *reg, const Symbol *symbol)
{
	unsigned size;
	Register *scratch = x17;


	size = symbol->type().size();

	if (symbol->offset == 0) {
		cout << tab << "adrp" << tab << scratch << ", ";
		cout << symbol->name() << endl;

		cout << tab << opcode << tab << reg->name(size) << ", [";
		cout << scratch << ", :lo12:" << symbol->name() << "]" << endl;

	} else
		loadstore(opcode, reg, size, sp, framesize + symbol->offset);
}

// Lecture 20 Slide 17
static void spill(Register *reg){
	if(reg->node != nullptr){
		assert(temps.size() > 0);

		int offset = temps.back();
		temps.pop_back();

		reg->node->offset = offset;

		// assign(reg->node, nullptr);
		
		loadstore("str", reg, reg->node->type().size(), sp, framesize + offset);
		assign(reg->node, nullptr);
	}
}

/*
 * Function:	Block::generate
 *
 * Description: Generate code for this block, which simply means we
 *				generate code for each statement within the block.
 */

void Block::generate()
{
	for (auto stmt : _stmts) {
		stmt->generate();

		for (auto reg : registers)
			assert(reg->node == nullptr);
	}
}


/*
 * Function:	Simple::generate
 *
 * Description: Generate code for a simple (expression) statement, which
 *				means simply generating code for the expression.
 */

void Simple::generate()
{
	_expr->generate();
	assign(_expr, nullptr);
}


/*
 * Function:	Call::generate
 *
 * Description: Generate code for a function call expression.
 */

void Call::generate()
{
	int offset;
	Register *reg, *scratch = x16;


	/* Generate code for all arguments first. */

	for (auto arg : _args)
		if (arg->hasCall())
			arg->generate();

	for (auto arg : _args)
		if (!arg->hasCall())
			arg->generate();


	/* Move the arguments into the correct registers or memory locations. */

	offset = 0;

	for (unsigned i = 0; i < _args.size(); i ++) {
		if (i < NUM_PARAM_REGS) {
			if (parameters[i]->node != _args[i]) {
				spill(parameters[i]);
				_args[i]->load(parameters[i]);
			}
		} else {
			reg = _args[i]->reg;

			if (reg == nullptr) {
				reg = scratch;
				_args[i]->load(scratch);
			}

			loadstore("str", reg, _args[i]->type().size(), sp, offset);
			offset += SIZEOF_REG;
		}

		assign(_args[i], nullptr);
	}


	/* Spill any caller-saved registers. */

	for (auto reg : registers)
		spill(reg);


	/* Call the function. */

	cout << tab << "bl" << tab << _id->name() << endl;
	assign(this, x0);
}


/*
 * Function:	Function::generate
 *
 * Description: Generate code for this function, which entails allocating
 *				space for local variables, then emitting our prologue, the
 *				body of the function, and the epilogue.
 */

void Function::generate()
{
	unsigned size;
	const Types *formals = _id->type().parameters();
	const Symbols &symbols = _body->declarations()->symbols();


	/* Assign offsets to the parameters and local variables. */

	_paramOffset = SIZEOF_REG * 2;
	_numCallerSaved = registers.size();
	allocate();


	/* Compute the final frame size. */

	_argSpace = align(_argSpace, SIZEOF_REG);
	_tempSpace = align(_tempSpace, SIZEOF_REG);
	_localSpace = align(_localSpace, SIZEOF_REG);

	framesize = _localSpace + _tempSpace + _argSpace;
	framesize = align(framesize, STACK_ALIGNMENT);


	/*
	 * Create the pool of temporaries.
	 *
	 * There are _numTemps temporaries, and they start at the end of
	 * _localSpace (so initialize offset to -_localSpace) and go downward.
	 */

	temps.clear();
	int offset = -_localSpace;		// might be unsigned

	for(unsigned int i = 0; i < _numTemps; i++){
		offset -= 8;
		temps.push_back(offset);
	}

	/* Generate our prologue. */

	funcname = _id->name();
	cout << tab << ".globl" << tab << funcname << endl;
	cout << funcname << ":" << endl;

	cout << tab << "stp" << tab << fp << ", " << lr << ", [sp, -16]!" << endl;
	cout << tab << "mov" << tab << fp << ", " << sp << endl;

	addoffset(sp, sp, -framesize);


	/* Spill any parameters. */

	if (formals != nullptr) {
		for (unsigned i = 0; i < NUM_PARAM_REGS; i ++)
			if (i < formals->size()) {
				size = symbols[i]->type().size();
				loadstore("str", parameters[i], size, fp, symbols[i]->offset);
			} else
				break;
	}


	/* Generate the body of this function. */

	cout << endl;
	_body->generate();
	cout << endl;


	/* Generate our epilogue. */

	cout << funcname << ".exit:" << endl;
	cout << tab << "mov" << tab << sp << ", " << fp << endl;
	cout << tab << "ldp" << tab << fp << ", " << lr << ", [sp], 16" << endl;
	cout << tab << "ret" << endl << endl;
}


/*
 * Function:	Assignment::generate
 *
 * Description: Generate code for an assignment statement.
 */

void Assignment::generate()
{
	_left->store(_right);
}


/*
 * Function:	generateGlobals
 *
 * Description: Generate code for any global variable declarations.
 */

void generateGlobals(Scope *scope)
{
	const Symbols &symbols = scope->symbols();

	for (auto symbol : symbols){
		if (!symbol->type().isFunction()) {
			cout << tab << ".comm" << tab << symbol->name();
			cout << ", " << symbol->type().size();
			cout << ", " << symbol->type().alignment() << endl;
		}
	}

	if(!strings.empty()){
		cout << tab << ".data" << endl;
		for(auto const&[str, label] : strings){
			cout << label << ":" << tab << ".asciz";
			cout << tab << "\"" << escapeString(str) << "\"" << endl;
		}
	}
}


/*
 * Function:	Number::load
 *
 * Description: Load this number into the given register.
 */

void Number::load(Register *reg)
{
	loadimmediate(reg, _value, _type.size());
	assign(this, reg);
}


/*
 * Function:	Identifier::load
 *
 * Description: Load this identifier into the given register.
 */

void Identifier::load(Register *reg)
{
	loadstore("ldr", reg, _symbol);
	assign(this, reg);
}


/*
 * Function:	Identifier::store
 *
 * Description: Store the given expression into this identifier.
 *
 *				NOT FINISHED: Only works if the right-hand side is an
 *				integer literal (or integer scalar variable).
 */



// Lecture 20 Slide 6
static Register *getreg(){
	for (auto reg : registers){
		if(reg->node == nullptr){
			return reg;
		}
	}
	abort();
}


void Identifier::store(Expression *expr)
{
	// assert(dynamic_cast<Number *>(expr));

	// expr->load(x0);
	// loadstore("str", x0, _symbol);
	// assign(expr, nullptr);
	expr->generate();
	if(expr->reg == nullptr){
		expr->load(getreg());
	}
	loadstore("str", expr->reg, _symbol);
	assign(expr, nullptr);
}

// Lecture 20 Slide 7
static std::ostream &operator <<(std::ostream &ostr, Expression *expr){
	assert(expr->reg != nullptr);

	ostr << expr->reg->name(expr->type().size());
	return ostr;
}


// Lecture 20 Slide 18
void Expression::load(Register *reg){
	if(this->reg == reg){
		return;
	}
	if(this->reg != nullptr){
		cout << tab << "mov" << tab << reg->name(_type.size()) << ", " << this << endl;		
	}else{
		loadstore("ldr", reg, _type.size(), sp, framesize + offset);
		temps.push_back(offset);
	}
	assign(this, reg);
}


void Add::generate(){

	if(_left->hasCall() || !_right->hasCall()){
		_left->generate();
		_right->generate();
	}else if(_right->hasCall()){
		_right->generate();
		_left->generate();
	}

	if(_left->reg == nullptr){
		_left->load(getreg());
	}

	if(_right->reg == nullptr){
		_right->load(getreg());
	}

	cout << tab << "add" << tab << _left << ", ";
	cout << _left << ", " << _right << endl;

	assign(_right, nullptr);
	assign(this, _left->reg);
}

void Subtract::generate(){

	if(_left->hasCall() || !_right->hasCall()){
		_left->generate();
		_right->generate();
	}else if(_right->hasCall()){
		_right->generate();
		_left->generate();
	}

	if(_left->reg == nullptr){
		_left->load(getreg());
	}

	if(_right->reg == nullptr){
		_right->load(getreg());
	}

	cout << tab << "sub" << tab << _left << ", ";
	cout << _left << ", " << _right << endl;

	assign(_right, nullptr);
	assign(this, _left->reg);
}

void Multiply::generate(){

	if(_left->hasCall() || !_right->hasCall()){
		_left->generate();
		_right->generate();
	}else if(_right->hasCall()){
		_right->generate();
		_left->generate();
	}

	if(_left->reg == nullptr){
		_left->load(getreg());
	}

	if(_right->reg == nullptr){
		_right->load(getreg());
	}

	cout << tab << "mul" << tab << _left << ", ";
	cout << _left << ", " << _right << endl;

	assign(_right, nullptr);
	assign(this, _left->reg);
}

void Divide::generate(){

	if(_left->hasCall() || !_right->hasCall()){
		_left->generate();
		_right->generate();
	}else if(_right->hasCall()){
		_right->generate();
		_left->generate();
	}

	if(_left->reg == nullptr){
		_left->load(getreg());
	}

	if(_right->reg == nullptr){
		_right->load(getreg());
	}

	cout << tab << "sdiv" << tab << _left << ", ";	
	cout << _left << ", " << _right << endl;

	assign(_right, nullptr);
	assign(this, _left->reg);
}

void Remainder::generate(){
	if(_left->hasCall() || !_right->hasCall()){
		_left->generate();
		_right->generate();
	}else if(_right->hasCall()){
		_right->generate();
		_left->generate();
	}
	if(_left->reg == nullptr){
		_left->load(getreg());
	}
	if(_right->reg == nullptr){
		_right->load(getreg());
	}
	// use "sdiv" to store left/right into scratch reg (x17?)
	// use "msub" to store the remainder in a register
	/*
	Lecture 18 Slide 15
	sdiv scratch, Left, right
	msub Left, scratch, right, left
	*/

	// cout << _left->reg->node->type().size() << endl;		// print for fun (see the size)
	// if(_left->reg->node->type().size() > 4 || _right->reg->node->type().size() > 4){
	// 	cout << tab << "sdiv" << tab << "x17, ";
	// 	cout << _left << ", " << _right << endl;

	// 	cout << tab << "msub" << tab << _left << ", x17, ";
	// 	cout << _right << ", " << _left << endl;
	// }else{
	// 	cout << tab << "sdiv" << tab << "w17, ";
	// 	cout << _left << ", " << _right << endl;

	// 	cout << tab << "msub" << tab << _left << ", w17, ";
	// 	cout << _right << ", " << _left << endl;
	// }

	cout << tab << "sdiv" << tab << x17->name(_type.size());
	cout << ", " << _left << ", " << _right << endl;

	cout << tab << "msub" << tab << _left << ", " << x17->name(_type.size());
	cout << ", " << _right << ", " << _left << endl;
	
	// unassign right register
	assign(_right, nullptr);
	// assign left reg to this expression
	assign(this, _left->reg);
}

void LessThan::generate(){
	// Generate Left and Right
	// Does order matter depending on function call?

	_left->generate();
	_right->generate();

	// Load left and right (if not already in registers)

	if(_left->reg == nullptr){
		_left->load(getreg());
	}
	if(_right->reg == nullptr){
		_right->load(getreg());
	}

	// Compare left and right
	// Lecture 18 Slide 16
	// cmp reg1, reg2
	// cset reg, cond (eq, ne, le, ge, lt, gt)

	cout << tab << "cmp" << tab << _left;
	cout << ", " << _right << endl;

	// Unassign left and right

	assign(_left, nullptr);
	assign(_right, nullptr);

	// Assign this to a register

	assign(this, getreg());		//?

	// Use "cset" to load this using the corresponding condition code

	cout << tab << "cset" << tab << this;
	cout << ", lt" << endl;
}

void GreaterThan::generate(){
	_left->generate();
	_right->generate();

	if(_left->reg == nullptr){
		_left->load(getreg());
	}
	if(_right->reg == nullptr){
		_right->load(getreg());
	}

	// Compare left and right
	// Lecture 18 Slide 16
	// cmp reg1, reg2
	// cset reg, cond (eq, ne, le, ge, lt, gt)

	cout << tab << "cmp" << tab << _left;
	cout << ", " << _right << endl;

	assign(_left, nullptr);
	assign(_right, nullptr);

	assign(this, getreg());	

	cout << tab << "cset" << tab << this;
	cout << ", gt" << endl;
}

void LessOrEqual::generate(){
	_left->generate();
	_right->generate();

	if(_left->reg == nullptr){
		_left->load(getreg());
	}
	if(_right->reg == nullptr){
		_right->load(getreg());
	}

	// Compare left and right
	// Lecture 18 Slide 16
	// cmp reg1, reg2
	// cset reg, cond (eq, ne, le, ge, lt, gt)

	cout << tab << "cmp" << tab << _left;
	cout << ", " << _right << endl;

	assign(_left, nullptr);
	assign(_right, nullptr);

	assign(this, getreg());	

	cout << tab << "cset" << tab << this;
	cout << ", le" << endl;
}

void GreaterOrEqual::generate(){
	_left->generate();
	_right->generate();

	if(_left->reg == nullptr){
		_left->load(getreg());
	}
	if(_right->reg == nullptr){
		_right->load(getreg());
	}

	// Compare left and right
	// Lecture 18 Slide 16
	// cmp reg1, reg2
	// cset reg, cond (eq, ne, le, ge, lt, gt)

	cout << tab << "cmp" << tab << _left;
	cout << ", " << _right << endl;

	assign(_left, nullptr);
	assign(_right, nullptr);

	assign(this, getreg());	

	cout << tab << "cset" << tab << this;
	cout << ", ge" << endl;
}

void Equal::generate(){
	_left->generate();
	_right->generate();

	if(_left->reg == nullptr){
		_left->load(getreg());
	}
	if(_right->reg == nullptr){
		_right->load(getreg());
	}

	// Compare left and right
	// Lecture 18 Slide 16
	// cmp reg1, reg2
	// cset reg, cond (eq, ne, le, ge, lt, gt)

	cout << tab << "cmp" << tab << _left;
	cout << ", " << _right << endl;

	assign(_left, nullptr);
	assign(_right, nullptr);

	assign(this, getreg());	

	cout << tab << "cset" << tab << this;
	cout << ", eq" << endl;
}

void NotEqual::generate(){
	_left->generate();
	_right->generate();

	if(_left->reg == nullptr){
		_left->load(getreg());
	}
	if(_right->reg == nullptr){
		_right->load(getreg());
	}

	// Compare left and right
	// Lecture 18 Slide 16
	// cmp reg1, reg2
	// cset reg, cond (eq, ne, le, ge, lt, gt)

	cout << tab << "cmp" << tab << _left;
	cout << ", " << _right << endl;

	assign(_left, nullptr);
	assign(_right, nullptr);

	assign(this, getreg());	

	cout << tab << "cset" << tab << this;
	cout << ", ne" << endl;
}

void Not::generate(){
	// start with generate and load expression
	_expr->generate();
	if(_expr->reg == nullptr){
		_expr->load(getreg());
	}
	// cmp _expr, 0
	// cset _expr, eq
	cout << tab << "cmp" << tab << _expr << ", 0" << endl;
	cout << tab << "cset" << tab << _expr << ", eq" << endl; 

	// end with assign result expression to register
	assign(this, _expr->reg);
}


void Negate::generate(){
	// start with generate and load expression
	_expr->generate();
	if(_expr->reg == nullptr){
		_expr->load(getreg());
	}
	cout << tab << "neg" << tab << _expr << ", " << _expr << endl;

	// end with assign result expression to register
	assign(this, _expr->reg);
}



void Expression::test(const Label &label, bool ifTrue){
	generate();		// ?? not sure about this one

	if(this->reg == nullptr){
		this->load(getreg());
	}
	cout << tab << "cmp" << tab << this << ", 0" << endl;
	cout << (ifTrue? "\tbne\t" : "\tbeq\t");
	cout << label << endl;

	assign(this, nullptr);
}

// Lecture 21 Slide 11
void While::generate(){
	Label loop, exit;
	cout << loop << ":" << endl;
	_expr->test(exit, false);
	_stmt->generate();

	cout << tab << "b" << tab << loop << endl;
	cout << exit << ":" << endl;
}

void Dereference::address(Register *&reg){
	// generate code for expression
	_expr->generate();
	// load the expression if not already in register
	if(_expr->reg == nullptr){
		_expr->load(getreg());
	}
	// set register given as a parameter to the expression's register
	reg = _expr->reg;
}

void Identifier::address(Register *&reg){
	// you will always need to getreg
	reg = getreg();
	/*
	adrp reg64, name
	add reg64, reg 64, :lo12:name
	*/
	if(_symbol->offset == 0){	// global var
		cout << tab << "adrp" << tab << reg->name(8);
		cout << ", " << _symbol->name() << endl;
		cout << tab << "add" << tab << reg->name(8) << ", ";
		cout << reg->name(8) << ", :lo12:" << _symbol->name() << endl;
	}else{						// local
		// destination, source, offset
		addoffset(reg, sp, framesize + _symbol->offset);
	}
}

void Dereference::generate(){
	// generate & load child expression if necessary
	_expr->generate();
	if(_expr->reg == nullptr){
		_expr->load(getreg());
	}

	// load memory location of expr into correctly sized reg (cant use loadstore)
	cout << tab << "ldr" << tab << _expr->reg->name(_expr->type().size());
	cout << ", [" << _expr->reg->name(8) << "]" << endl;

	// assign result expr to reg
	assign(this, _expr->reg);
}

void Address::generate(){
	Register *reg;
	_expr->address(reg);
	assign(this, reg);
}

// CHECK IN 1 DONE

void Dereference::store(Expression *expr){
	Expression *child;
	Expression *right;
	child = _expr;		// child of deref expr to store into!
	right = expr;		// RHS of expr
	// Generate child & right (evaluate one with function call first)
	if(child->hasCall() || !right->hasCall()){
		child->generate();
		right->generate();
	}else if (right -> hasCall()){
		right->generate();
		child->generate();
	}
	// Load child and right into registers if not alread
	if(child->reg == nullptr){
		child->load(getreg());
	}
	if(right->reg == nullptr){
		right->load(getreg());
	}
	// emit store instruction to store right into child's mem.loc.
	// need to get VALUE of right into child's mem loc
	cout << tab << "str" << tab << right;
	cout << ", [" << child << "]" << endl;
	// unassign child and right regs
	assign(child, nullptr);
	assign(right, nullptr);
}
// got up to here checked off by dr atkinson :)

void String::address(Register *&reg){
	reg = getreg();

	if(strings.find(_value) == strings.end()){
		strings[_value] = Label();
	}
	/*
	adrp reg64, name
	add reg64, reg 64, :lo12:name
	*/
	cout << tab << "adrp" << tab << reg->name(8) << ", " <<  strings[_value] << endl;
	cout << tab << "add" << tab << reg->name(8) << ", " << reg->name(8) << ", :lo12:";
	cout << strings[_value] << endl;

	assign(this, reg);
}

void Return::generate(){
	_expr->generate();
	_expr->load(x0);		// function handles x0 or w0
	cout << tab << "bl" << tab << funcname << ".exit" << endl;
	assign(_expr, nullptr);
}

// CHECK IN 2 DONE

void Cast::generate(){
	// Generate & Load Expression
	_expr->generate();

	if(_expr->reg == nullptr){
		_expr->load(getreg());
	}

	// Type cast from int -> long
	// sxtw reg64, reg32
	if(_expr->type().specifier() == INT  && _expr->type().indirection() == 0 && (_type.isPointer() || _type.specifier() == LONG )){
		cout << tab << "sxtw" << tab << _expr->reg->name(8);
		cout << ", " << _expr->reg->name(4) << endl;		
	}
	// cout << tab << "sxtw" << tab << _expr->reg->name(8);
	// cout << ", " << _expr->reg->name(4) << endl;

	// assign result expression to reg
	assign(this, _expr->reg);
}

void LogicalOr::generate(){
	Label trueL, exitL;
	Register *reg = getreg();

	_left->test(trueL, true);
	_right->test(trueL, true);
	// both false
	cout << tab << "mov" << tab << reg->name(4) << ", 0" << endl;
	cout << tab << "b" << tab << exitL << endl;
	// at least 1 true
	cout << trueL << ":" << endl;
	cout << tab << "mov" << tab << reg->name(4) << ", 1" << endl;

	cout << exitL << ":" << endl;
	assign(this, reg);
}

void LogicalAnd::generate(){
	Label falseL, exitL;
	Register *reg = getreg();

	_left->test(falseL, false);
	_right->test(falseL, false);
	// both true
	cout << tab << "mov" << tab << reg->name(4) << ", 1" << endl;
	cout << tab << "b" << tab << exitL << endl;

	//at least 1 true
	cout << falseL << ":" << endl;
	cout << tab << "mov" << tab << reg->name(4) << ", 0" << endl;

	cout << exitL << ":" << endl;
	assign(this, reg);
}

void For::generate(){
	// Labels
	Label loopL, exitL;
	// <code for init>
	_init->generate();
	// print out Loop Label
	cout << loopL << ":" << endl;
	// <code for expr>
	_expr->test(exitL, false);
	// <code for stmt>
	_stmt->generate();
	// <code for stmt>
	_incr->generate();
	cout << tab << "b" << tab << loopL << endl;
	cout << exitL << ":" << endl;
}

void If::generate(){
	Label skipL, exitL;
	_expr->test(skipL, false);
	_thenStmt->generate();

	if(_elseStmt != nullptr){	// if there is an else
		cout << tab << "b" << tab << exitL << endl;
		cout << skipL << ":" << endl;
		_elseStmt->generate();
		cout << exitL << ":" << endl;
	}else{
		cout << skipL << ":" << endl;
	}
}