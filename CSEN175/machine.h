/*
 * File:	machine.h
 *
 * Description:	This file contains the values of various parameters for the
 *		target machine architecture.
 */

# ifndef MACHINE_H
# define MACHINE_H

# define SIZEOF_CHAR 1
# define SIZEOF_INT  4
# define SIZEOF_LONG 8
# define SIZEOF_PTR  8

# define ALIGNOF_CHAR 1
# define ALIGNOF_INT  4
# define ALIGNOF_LONG 8
# define ALIGNOF_PTR  8

# define SIZEOF_REG 8
# define NUM_PARAM_REGS 8
# define STACK_ALIGNMENT 16

# define MAX_SIMM 255
# define MIN_SIMM -256

# define MAX_PIMM_8  0x0fff
# define MAX_PIMM_32 0x3ffc
# define MAX_PIMM_64 0x7ff8

# define MAX_IMM_ADD 0x0fff
# define MAX_IMM_MOV 0xffff

# endif
