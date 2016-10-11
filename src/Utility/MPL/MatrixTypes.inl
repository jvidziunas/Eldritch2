/*==================================================================*\
  MatrixTypes.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <intrin.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

	ETForceInlineHint Float4x4::Float4x4( const Eldritch2::float32* const values ) {
		_mm_storeu_ps( elements[0].coefficients, _mm_loadu_ps( values ) );
		_mm_storeu_ps( elements[1].coefficients, _mm_loadu_ps( values + 4 ) );
		_mm_storeu_ps( elements[2].coefficients, _mm_loadu_ps( values + 8 ) );
		_mm_storeu_ps( elements[3].coefficients, _mm_loadu_ps( values + 16 ) );
	}

// ---------------------------------------------------

	ETForceInlineHint Float4x4::Float4x4( Eldritch2::Float4 values[4] ) {
		_mm_storeu_ps( elements[0].coefficients, _mm_loadu_ps( values[0].coefficients ) );
		_mm_storeu_ps( elements[1].coefficients, _mm_loadu_ps( values[1].coefficients ) );
		_mm_storeu_ps( elements[2].coefficients, _mm_loadu_ps( values[2].coefficients ) );
		_mm_storeu_ps( elements[3].coefficients, _mm_loadu_ps( values[3].coefficients ) );
	}

// ---------------------------------------------------

	ETForceInlineHint Float4x4::Float4x4( Eldritch2::Float4 row0, Eldritch2::Float4 row1, Eldritch2::Float4 row2, Eldritch2::Float4 row3 ) {
		_mm_storeu_ps( elements[0].coefficients, _mm_loadu_ps( row0.coefficients ) );
		_mm_storeu_ps( elements[1].coefficients, _mm_loadu_ps( row1.coefficients ) );
		_mm_storeu_ps( elements[2].coefficients, _mm_loadu_ps( row2.coefficients ) );
		_mm_storeu_ps( elements[3].coefficients, _mm_loadu_ps( row3.coefficients ) );
	}

// ---------------------------------------------------

	ETForceInlineHint Float4x4::Float4x4( Eldritch2::float32 m00, Eldritch2::float32 m01, Eldritch2::float32 m02, Eldritch2::float32 m03,
										  Eldritch2::float32 m10, Eldritch2::float32 m11, Eldritch2::float32 m12, Eldritch2::float32 m13,
										  Eldritch2::float32 m20, Eldritch2::float32 m21, Eldritch2::float32 m22, Eldritch2::float32 m23,
										  Eldritch2::float32 m30, Eldritch2::float32 m31, Eldritch2::float32 m32, Eldritch2::float32 m33 ) {
		_mm_storeu_ps( elements[0].coefficients, _mm_set_ps( m02, m01, m00, m03 ) );
		_mm_storeu_ps( elements[1].coefficients, _mm_set_ps( m12, m11, m10, m13 ) );
		_mm_storeu_ps( elements[2].coefficients, _mm_set_ps( m22, m21, m20, m23 ) );
		_mm_storeu_ps( elements[3].coefficients, _mm_set_ps( m32, m31, m30, m33 ) );
	}

// ---------------------------------------------------

#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	MSVC complains about writing to uninitialized memory... thus initializing it.
#	pragma warning( disable : 6001 )
#endif
	ETForceInlineHint ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall operator+( Eldritch2::Float4x4 operand0, Eldritch2::Float4x4 operand1 ) {
		Float4x4	temp;

		_mm_storeu_ps( temp.elements[0].coefficients, _mm_add_ps( _mm_loadu_ps( operand0.elements[0].coefficients ), _mm_loadu_ps( operand1.elements[0].coefficients ) ) );
		_mm_storeu_ps( temp.elements[1].coefficients, _mm_add_ps( _mm_loadu_ps( operand0.elements[1].coefficients ), _mm_loadu_ps( operand1.elements[1].coefficients ) ) );
		_mm_storeu_ps( temp.elements[2].coefficients, _mm_add_ps( _mm_loadu_ps( operand0.elements[2].coefficients ), _mm_loadu_ps( operand1.elements[2].coefficients ) ) );
		_mm_storeu_ps( temp.elements[3].coefficients, _mm_add_ps( _mm_loadu_ps( operand0.elements[3].coefficients ), _mm_loadu_ps( operand1.elements[3].coefficients ) ) );

		return temp;
	}
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )	// 6001
#endif

// ---------------------------------------------------

#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	MSVC complains about writing to uninitialized memory... thus initializing it.
#	pragma warning( disable : 6001 )
#endif
	ETForceInlineHint ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall operator-( Eldritch2::Float4x4 operand0, Eldritch2::Float4x4 operand1 ) {
		Float4x4	temp;

		_mm_storeu_ps( temp.elements[0].coefficients, _mm_sub_ps( _mm_loadu_ps( operand0.elements[0].coefficients ), _mm_loadu_ps( operand1.elements[0].coefficients ) ) );
		_mm_storeu_ps( temp.elements[1].coefficients, _mm_sub_ps( _mm_loadu_ps( operand0.elements[1].coefficients ), _mm_loadu_ps( operand1.elements[1].coefficients ) ) );
		_mm_storeu_ps( temp.elements[2].coefficients, _mm_sub_ps( _mm_loadu_ps( operand0.elements[2].coefficients ), _mm_loadu_ps( operand1.elements[2].coefficients ) ) );
		_mm_storeu_ps( temp.elements[3].coefficients, _mm_sub_ps( _mm_loadu_ps( operand0.elements[3].coefficients ), _mm_loadu_ps( operand1.elements[3].coefficients ) ) );

		return temp;
	}
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )	// 6001
#endif

// ---------------------------------------------------

#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	 MSVC complains about writing to uninitialized memory... thus initializing it.
#	pragma warning( disable : 6001 )
#endif
	ETForceInlineHint ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall operator*( Eldritch2::Float4x4 operand, Eldritch2::float32 scalar ) {
		const __m128	scaleVector( _mm_set_ps1( scalar ) );
		Float4x4		temp;

		_mm_storeu_ps( temp.elements[0].coefficients, _mm_mul_ps( _mm_loadu_ps( operand.elements[0].coefficients ), scaleVector ) );
		_mm_storeu_ps( temp.elements[1].coefficients, _mm_mul_ps( _mm_loadu_ps( operand.elements[1].coefficients ), scaleVector ) );
		_mm_storeu_ps( temp.elements[2].coefficients, _mm_mul_ps( _mm_loadu_ps( operand.elements[2].coefficients ), scaleVector ) );
		_mm_storeu_ps( temp.elements[3].coefficients, _mm_mul_ps( _mm_loadu_ps( operand.elements[3].coefficients ), scaleVector ) );

		return temp;
	}
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )	// 6001
#endif

// ---------------------------------------------------

	ETForceInlineHint ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall operator*( Eldritch2::float32 scalar, Eldritch2::Float4x4 operand ) {
		return operand * scalar;
	}

// ---------------------------------------------------

#if ET_COMPILER_IS_MSVC
#	pragma warning( push )
//	MSVC complains about writing to uninitialized memory... thus initializing it.
#	pragma warning( disable : 6001 )
#endif
	ETForceInlineHint ETPureFunctionHint Eldritch2::Float4x4 ETSimdCall operator*( const Eldritch2::Float4x4 operand0, const Eldritch2::Float4x4 operand1 ) {
		enum : int {
			ALL_SOURCE_COMPONENTS	= 0xF0,
			X_MASK					= ALL_SOURCE_COMPONENTS | 0x01,
			Y_MASK					= ALL_SOURCE_COMPONENTS | 0x02,
			Z_MASK					= ALL_SOURCE_COMPONENTS | 0x04,
			W_MASK					= ALL_SOURCE_COMPONENTS | 0x08
		};

	// ---

		Float4x4	temp;
	//	Prepare the operand by performing a matrix transpose.
		__m128		operandRows[4] = { _mm_loadu_ps( operand1.elements[0].coefficients ),
									   _mm_loadu_ps( operand1.elements[1].coefficients ),
									   _mm_loadu_ps( operand1.elements[2].coefficients ),
									   _mm_loadu_ps( operand1.elements[3].coefficients ) };
		_MM_TRANSPOSE4_PS( operandRows[0], operandRows[1], operandRows[2], operandRows[3] );

	//	Since _mm_dp_ps zeroes out all the elements not masked out, we can simply do a bitwise or and work on an individual component at a time instead of playing shuffling games.
	//	This has the added beneficial side effect of allowing the processor to execute several dot products in parallel as there are fewer data dependencies over a naive implementation.

	//	COLUMN 0
		__m128	activeSourceRow( _mm_loadu_ps( operand0.elements[0].coefficients ) );
		__m128	activeDestinationRow( _mm_or_ps( _mm_dp_ps( operandRows[0], activeSourceRow, X_MASK ), _mm_dp_ps( operandRows[1], activeSourceRow, Y_MASK ) ) );
		__m128	activeDestinationRow2( _mm_or_ps( _mm_dp_ps( operandRows[2], activeSourceRow, Z_MASK ), _mm_dp_ps( operandRows[3], activeSourceRow, W_MASK ) ) );

		_mm_storeu_ps( temp.elements[0].coefficients, _mm_or_ps( activeDestinationRow, activeDestinationRow2 ) );

	//	COLUMN 1
		activeSourceRow			= _mm_loadu_ps( operand0.elements[1].coefficients );
		activeDestinationRow	= _mm_or_ps( _mm_dp_ps( operandRows[0], activeSourceRow, X_MASK ), _mm_dp_ps( operandRows[1], activeSourceRow, Y_MASK ) );
		activeDestinationRow2	= _mm_or_ps( _mm_dp_ps( operandRows[2], activeSourceRow, Z_MASK ), _mm_dp_ps( operandRows[3], activeSourceRow, W_MASK ) );

		_mm_storeu_ps( temp.elements[1].coefficients, _mm_or_ps( activeDestinationRow, activeDestinationRow2 ) );

	//	COLUMN 2
		activeSourceRow			= _mm_loadu_ps( operand0.elements[2].coefficients );
		activeDestinationRow	= _mm_or_ps( _mm_dp_ps( operandRows[0], activeSourceRow, X_MASK ), _mm_dp_ps( operandRows[1], activeSourceRow, Y_MASK ) );
		activeDestinationRow2	= _mm_or_ps( _mm_dp_ps( operandRows[2], activeSourceRow, Z_MASK ), _mm_dp_ps( operandRows[3], activeSourceRow, W_MASK ) );

		_mm_storeu_ps( temp.elements[2].coefficients, _mm_or_ps( activeDestinationRow, activeDestinationRow2 ) );

	//	COLUMN 3
		activeSourceRow			= _mm_loadu_ps( operand0.elements[3].coefficients );
		activeDestinationRow	= _mm_or_ps( _mm_dp_ps( operandRows[0], activeSourceRow, X_MASK ), _mm_dp_ps( operandRows[1], activeSourceRow, Y_MASK ) );
		activeDestinationRow2	= _mm_or_ps( _mm_dp_ps( operandRows[2], activeSourceRow, Z_MASK ), _mm_dp_ps( operandRows[3], activeSourceRow, W_MASK ) );

		_mm_storeu_ps( temp.elements[3].coefficients, _mm_or_ps( activeDestinationRow, activeDestinationRow2 ) );

		return temp;
	}
#if ET_COMPILER_IS_MSVC
#	pragma warning( pop )	// 6001
#endif

// ---------------------------------------------------

	ETForceInlineHint Eldritch2::Float4x4& ETSimdCall Float4x4::operator+=( Eldritch2::Float4x4 operand ) {
		_mm_storeu_ps( elements[0].coefficients, _mm_add_ps( _mm_loadu_ps( elements[0].coefficients ), _mm_loadu_ps( operand.elements[0].coefficients ) ) );
		_mm_storeu_ps( elements[1].coefficients, _mm_add_ps( _mm_loadu_ps( elements[1].coefficients ), _mm_loadu_ps( operand.elements[1].coefficients ) ) );
		_mm_storeu_ps( elements[2].coefficients, _mm_add_ps( _mm_loadu_ps( elements[2].coefficients ), _mm_loadu_ps( operand.elements[2].coefficients ) ) );
		_mm_storeu_ps( elements[3].coefficients, _mm_add_ps( _mm_loadu_ps( elements[3].coefficients ), _mm_loadu_ps( operand.elements[3].coefficients ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint Eldritch2::Float4x4& ETSimdCall Float4x4::operator-=( Eldritch2::Float4x4 operand ) {
		_mm_storeu_ps( elements[0].coefficients, _mm_sub_ps( _mm_loadu_ps( elements[0].coefficients ), _mm_loadu_ps( operand.elements[0].coefficients ) ) );
		_mm_storeu_ps( elements[1].coefficients, _mm_sub_ps( _mm_loadu_ps( elements[1].coefficients ), _mm_loadu_ps( operand.elements[1].coefficients ) ) );
		_mm_storeu_ps( elements[2].coefficients, _mm_sub_ps( _mm_loadu_ps( elements[2].coefficients ), _mm_loadu_ps( operand.elements[2].coefficients ) ) );
		_mm_storeu_ps( elements[3].coefficients, _mm_sub_ps( _mm_loadu_ps( elements[3].coefficients ), _mm_loadu_ps( operand.elements[3].coefficients ) ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint Eldritch2::Float4x4& ETSimdCall Float4x4::operator*=( Eldritch2::float32 scalar ) {
		const __m128	scaleVector( _mm_set_ps1( scalar ) );

		_mm_storeu_ps( elements[0].coefficients, _mm_mul_ps( _mm_loadu_ps( elements[0].coefficients ), scaleVector ) );
		_mm_storeu_ps( elements[1].coefficients, _mm_mul_ps( _mm_loadu_ps( elements[1].coefficients ), scaleVector ) );
		_mm_storeu_ps( elements[2].coefficients, _mm_mul_ps( _mm_loadu_ps( elements[2].coefficients ), scaleVector ) );
		_mm_storeu_ps( elements[3].coefficients, _mm_mul_ps( _mm_loadu_ps( elements[3].coefficients ), scaleVector ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint Eldritch2::Float4x4& ETSimdCall Float4x4::operator*=( Eldritch2::Float4x4 matrix ) {
		enum : int {
			ALL_SOURCE_COMPONENTS	= 0xF0,
			X_MASK					= ALL_SOURCE_COMPONENTS | 0x01,
			Y_MASK					= ALL_SOURCE_COMPONENTS | 0x02,
			Z_MASK					= ALL_SOURCE_COMPONENTS | 0x04,
			W_MASK					= ALL_SOURCE_COMPONENTS | 0x08
		};

	// ---

	//	Prepare the operand by performing a matrix transpose.
		__m128	operandRows[4] = { _mm_loadu_ps( matrix.elements[0].coefficients ),
								   _mm_loadu_ps( matrix.elements[1].coefficients ),
								   _mm_loadu_ps( matrix.elements[2].coefficients ),
								   _mm_loadu_ps( matrix.elements[3].coefficients ) };

		_MM_TRANSPOSE4_PS( operandRows[0], operandRows[1], operandRows[2], operandRows[3] );

	/*	Since _mm_dp_ps zeroes out all the elements not masked out, we can simply do a bitwise or and work on an individual component at a time instead of playing shuffling games.
	 *	This has the added beneficial side effect of allowing the processor to execute several dot products in parallel as there are fewer data dependencies over a naive implementation. */

	//	COLUMN 0
		__m128	activeSourceRow( _mm_loadu_ps( elements[0].coefficients ) );
		__m128	activeDestinationRow( _mm_or_ps( _mm_dp_ps( operandRows[0], activeSourceRow, X_MASK ), _mm_dp_ps( operandRows[1], activeSourceRow, Y_MASK ) ) );
		__m128	activeDestinationRow2( _mm_or_ps( _mm_dp_ps( operandRows[2], activeSourceRow, Z_MASK ), _mm_dp_ps( operandRows[3], activeSourceRow, W_MASK ) ) );

		_mm_storeu_ps( elements[0].coefficients, _mm_or_ps( activeDestinationRow, activeDestinationRow2 ) );

	//	COLUMN 1
		activeSourceRow			= _mm_loadu_ps( elements[1].coefficients );
		activeDestinationRow	= _mm_or_ps( _mm_dp_ps( operandRows[0], activeSourceRow, X_MASK ), _mm_dp_ps( operandRows[1], activeSourceRow, Y_MASK ) );
		activeDestinationRow2	= _mm_or_ps( _mm_dp_ps( operandRows[2], activeSourceRow, Z_MASK ), _mm_dp_ps( operandRows[3], activeSourceRow, W_MASK ) );

		_mm_storeu_ps( elements[1].coefficients, _mm_or_ps( activeDestinationRow, activeDestinationRow2 ) );

	//	COLUMN 2
		activeSourceRow			= _mm_loadu_ps( elements[2].coefficients );
		activeDestinationRow	= _mm_or_ps( _mm_dp_ps( operandRows[0], activeSourceRow, X_MASK ), _mm_dp_ps( operandRows[1], activeSourceRow, Y_MASK ) );
		activeDestinationRow2	= _mm_or_ps( _mm_dp_ps( operandRows[2], activeSourceRow, Z_MASK ), _mm_dp_ps( operandRows[3], activeSourceRow, W_MASK ) );

		_mm_storeu_ps( elements[2].coefficients, _mm_or_ps( activeDestinationRow, activeDestinationRow2 ) );

	//	COLUMN 3
		activeSourceRow			= _mm_loadu_ps( elements[3].coefficients );
		activeDestinationRow	= _mm_or_ps( _mm_dp_ps( operandRows[0], activeSourceRow, X_MASK ), _mm_dp_ps( operandRows[1], activeSourceRow, Y_MASK ) );
		activeDestinationRow2	= _mm_or_ps( _mm_dp_ps( operandRows[2], activeSourceRow, Z_MASK ), _mm_dp_ps( operandRows[3], activeSourceRow, W_MASK ) );

		_mm_storeu_ps( elements[3].coefficients, _mm_or_ps( activeDestinationRow, activeDestinationRow2 ) );

		return *this;
	}

// ---------------------------------------------------

	ETForceInlineHint Eldritch2::Float4x4& ETSimdCall Float4x4::operator=( Eldritch2::Float4x4 operand ) {
		_mm_storeu_ps( elements[0].coefficients, _mm_loadu_ps( operand.elements[0].coefficients ) );
		_mm_storeu_ps( elements[1].coefficients, _mm_loadu_ps( operand.elements[1].coefficients ) );
		_mm_storeu_ps( elements[2].coefficients, _mm_loadu_ps( operand.elements[2].coefficients ) );
		_mm_storeu_ps( elements[3].coefficients, _mm_loadu_ps( operand.elements[3].coefficients ) );

		return *this;
	}

}	// namespace Eldritch2