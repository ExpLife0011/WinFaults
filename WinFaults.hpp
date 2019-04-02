#pragma once
#include <ntstatus.h>

namespace WinFaults
{
	// Encoding of enums.
	// You can change this to Id to get 0-N and leave it
	// as is to get a bit mask for each fault.
	// 
	static constexpr unsigned E( unsigned Id )
	{
		return 1 << Id;
	}

	// All fault IDs possible
	//
	enum FaultId
	{
		/* ------------------------------------------------------------------------------------ */
		// -- Faults
		//
		FAULT_PF =			E( 0 ),		// KiPageFault
		FAULT_BR =			E( 1 ),		// KiBoundFault
		FAULT_SS =			E( 2 ),		// KiStackFault
		FAULT_XF =			E( 3 ),		// KiXmmException
		FAULT_AC =			E( 4 ),		// KiAlignmentFault
		FAULT_DB =			E( 5 ),		// KiDebugTrapOrFault
		FAULT_DE =			E( 6 ),		// KiDivideErrorFault
		FAULT_UD =			E( 7 ),		// KiInvalidOpcodeFault
		FAULT_MF =			E( 8 ),		// KiFloatingErrorFault
		FAULT_NP =			E( 9 ),		// KiSegmentNotPresentFault
		FAULT_GP =			E( 10 ),	// KiGeneralProtectionFault
	
		// -- Traps
		//
		TRAP_OF =			E( 11 ),	// KiOverflowTrap
		TRAP_BP =			E( 12 ),	// KiBreakpointTrap
								
		// -- Exceptional cases
		//
		INT_2C =			E( 13 ),	// KiRaiseAssertion
		SYSCALL_32 =			E( 14 ),	// KiSystemCall32ShadowCommon
		FAULT_XM_OR_XF =		E( 15 ),	// KiXmmException or KiFloatingErrorFault
		/* ------------------------------------------------------------------------------------ */

		// -- Constants
		FAULT_UNKNOWN =			E( 16 ),
		FAULT_MAX =			E( 16 ),	// Only relevant if using IDs
		FAULT_ALL =			FAULT_MAX - 1	// Only relevant if using masks
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
		/* -------------------------------------------------------------------- */
		{	0x10000001,				FAULT_GP		},
		{	0x10000002,				FAULT_UD		},
		{	0x10000003,				FAULT_DE		},
		{	0x10000004,				FAULT_PF		},
		{	0x10000006,				FAULT_NP		},
		{	0x10000007,				FAULT_NP		},

		{	STATUS_SINGLE_STEP,			FAULT_DB		},
		{	STATUS_FLOAT_UNDERFLOW,			FAULT_XF		},
		{	STATUS_ACCESS_VIOLATION,		FAULT_SS		},
		{	STATUS_FLOAT_STACK_CHECK,		FAULT_MF		},
		{	STATUS_FLOAT_MULTIPLE_TRAPS,		FAULT_XF		},
		{	STATUS_ARRAY_BOUNDS_EXCEEDED,		FAULT_BR		},
		{	STATUS_FLOAT_MULTIPLE_FAULTS,		FAULT_XF		},
		{	STATUS_DATATYPE_MISALIGNMENT,		FAULT_AC		},

		{	STATUS_BREAKPOINT,			TRAP_BP			},
		{	STATUS_INTEGER_OVERFLOW,		TRAP_OF			},

		{	STATUS_ASSERTION_FAILURE,		INT_2C			},
		{	STATUS_ILLEGAL_INSTRUCTION,		SYSCALL_32		},

		{	STATUS_FLOAT_OVERFLOW,			FAULT_XM_OR_XF		},
		{	STATUS_FLOAT_INEXACT_RESULT,		FAULT_XM_OR_XF		},
		{	STATUS_FLOAT_DIVIDE_BY_ZERO,		FAULT_XM_OR_XF		},
		{	STATUS_FLOAT_INVALID_OPERATION,		FAULT_XM_OR_XF		},
		/* -------------------------------------------------------------------- */
	};

	// One way as it's not one to one mapped.
	//
	static constexpr FaultId FromNtStatus( long Status )
	{
		for ( const FaultBinding& Mapping : FaultMap )
			if ( Mapping.Nt == Status )
				return Mapping.Id;
		return FAULT_MAX;
	}
};

using WinFaults::FaultId;
