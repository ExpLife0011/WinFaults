#pragma once
#include <ntstatus.h>

// All fault IDs possible
//
enum FaultId : char
{
	/* --------------------------------------------------------------- */
	// -- Faults
	FAULT_PF,					// KiPageFault
	FAULT_BR,					// KiBoundFault
	FAULT_SS,					// KiStackFault
	FAULT_XF,					// KiXmmException
	FAULT_AC,					// KiAlignmentFault
	FAULT_DB,					// KiDebugTrapOrFault
	FAULT_DE,					// KiDivideErrorFault
	FAULT_UD,					// KiInvalidOpcodeFault
	FAULT_MF,					// KiFloatingErrorFault
	FAULT_NP,					// KiSegmentNotPresentFault
	FAULT_GP,					// KiGeneralProtectionFault
	
	// -- Traps
	TRAP_OF,					// KiOverflowTrap
	TRAP_BP,					// KiBreakpointTrap
								
	// -- Exceptional cases
	INT_2C,						// KiRaiseAssertion
	SYSCALL_32,					// KiSystemCall32ShadowCommon
	FAULT_XM_OR_XF,				// KiXmmException or KiFloatingErrorFault
	/* ---------------------------------------------------------------- */

	FAULT_MAX
};


// Mapping of codes passed to KiExceptionDispatch before KiPreprocessFault
//
struct FaultBinding
{
	FaultId Id;
	unsigned Nt;
	constexpr FaultBinding( unsigned Nt, FaultId Id ) : Nt( Nt ), Id( Id ) {}
};

static constexpr FaultBinding FaultMap[] =
{
	/* ---------------------------------------------------------------- */
	{		 0x10000001,							FAULT_GP			},
	{		 0x10000002,							FAULT_UD			},
	{		 0x10000003,							FAULT_DE			},
	{		 0x10000004,							FAULT_PF			},
	{		 0x10000006,							FAULT_NP			},
	{		 0x10000007,							FAULT_NP			},

	{		 STATUS_SINGLE_STEP,					FAULT_DB			},
	{		 STATUS_FLOAT_UNDERFLOW,				FAULT_XF			},
	{		 STATUS_ACCESS_VIOLATION,				FAULT_SS			},
	{		 STATUS_FLOAT_STACK_CHECK,				FAULT_MF			},
	{		 STATUS_FLOAT_MULTIPLE_TRAPS,			FAULT_XF			},
	{		 STATUS_ARRAY_BOUNDS_EXCEEDED,			FAULT_BR			},
	{		 STATUS_FLOAT_MULTIPLE_FAULTS,			FAULT_XF			},
	{		 STATUS_DATATYPE_MISALIGNMENT,			FAULT_AC			},

	{		 STATUS_BREAKPOINT,						TRAP_BP				},
	{		 STATUS_INTEGER_OVERFLOW,				TRAP_OF				},

	{		 STATUS_ASSERTION_FAILURE,				INT_2C				},
	{		 STATUS_ILLEGAL_INSTRUCTION,			SYSCALL_32			},

	{		 STATUS_FLOAT_OVERFLOW,					FAULT_XM_OR_XF		},
	{		 STATUS_FLOAT_INEXACT_RESULT,			FAULT_XM_OR_XF		},
	{		 STATUS_FLOAT_DIVIDE_BY_ZERO,			FAULT_XM_OR_XF		},
	{		 STATUS_FLOAT_INVALID_OPERATION,		FAULT_XM_OR_XF		},
	/* ---------------------------------------------------------------- */
};

// One way as it's not one to one mapped.
//
static constexpr FaultId NtToFaultId( long Status )
{
	for ( const FaultBinding& Mapping : FaultMap )
		if ( Mapping.Nt == Status )
			return Mapping.Id;
	return FAULT_MAX;
}
