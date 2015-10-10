/*==================================================================*\
  ScriptMarshalTypes.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Utility/Math/MathUtils.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scripting {

	const char* const StringMarshal::scriptTypeName			= "String";
	const char* const Float4Marshal::scriptTypeName			= "Float4";
	const char* const OrientationMarshal::scriptTypeName	= "Orientation";

	static ETThreadLocal	Allocator*	stringAllocator = nullptr;

// ---------------------------------------------------

	StringMarshal::StringMarshal( const EmptyStringSemantics emptyStringSemantics, Allocator& allocator ) : UTF8String<>( emptyStringSemantics, allocator, UTF8L("String Allocator") ) {}

// ---------------------------------------------------

	StringMarshal::StringMarshal( const UTF8Char* const string, const size_t stringLengthInOctets, Allocator& allocator ) : UTF8String<>( string, stringLengthInOctets, allocator, UTF8L("String Allocator") ) {}

// ---------------------------------------------------

	StringMarshal::StringMarshal( const StringMarshal& string, Allocator& allocator ) : StringMarshal( string.GetCharacterArray(), string.Length(), allocator ) {}

// ---------------------------------------------------

	ETNoAliasHint void StringMarshal::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static void ETScriptAPICall Constructor0( void* const thisPointer ) {
				new(thisPointer) StringMarshal( ::Eldritch2::EmptyStringSemantics, *stringAllocator );
			}

			static void ETScriptAPICall Constructor1( void* const thisPointer, const StringMarshal& string ) {
				new(thisPointer) StringMarshal( string, *stringAllocator );
			}

			static StringMarshal& ETScriptAPICall OpAssign( StringMarshal* thisPointer, const StringMarshal& otherString ) {
				return static_cast<StringMarshal&>((*thisPointer).Assign( otherString.GetCharacterArray(), otherString.Length() ));
			}

			static StringMarshal& ETScriptAPICall OpAddAssign( StringMarshal* thisPointer, const StringMarshal& additionalString ) {
				return static_cast<StringMarshal&>((*thisPointer) += additionalString);
			}

			static int ETScriptAPICall OpCompare( const StringMarshal* thisPointer, const StringMarshal& operand ) {
				return thisPointer->Compare( operand );
			}

			static StringMarshal::SizeType ETScriptAPICall GetLength( const StringMarshal* thisPointer ) {
				return thisPointer->Length();
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L( "StringMarshal::ExposeScriptAPI() Temporary Allocator" ) );

		if( const auto registerResult = typeRegistrar.RegisterUserDefinedValueType<StringMarshal>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeBuilder.ExposeConstructor( &FunctionHelper::Constructor0 ).ExposeConstructor( &FunctionHelper::Constructor1 );

			typeBuilder.ExposeVirtualProperty( "Length", &FunctionHelper::GetLength );

			typeBuilder.ExposeBinaryOperator( BinaryOperatorClass::ASSIGNMENT, &FunctionHelper::OpAssign ).ExposeBinaryOperator( BinaryOperatorClass::ADDITION_ASSIGNMENT, &FunctionHelper::OpAddAssign );

			/* typeBuilder.ExposeBinaryOperator( BinaryOperatorClasses::COMPARISON, &FunctionHelper::OpCompare ); */

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	void StringMarshal::AttachWorldAllocator( Allocator& allocator ) {
		stringAllocator = &allocator;
	}

// ---------------------------------------------------

	Float4Marshal::Float4Marshal( const float32 x, const float32 y, const float32 z, const float32 w ) : Float4( x, y, z, w ) {}

// ---------------------------------------------------

	Float4Marshal::Float4Marshal( const Float4Marshal& marshal ) : Float4( marshal ) {}

// ---------------------------------------------------

	Float4Marshal::Float4Marshal( const Float4 initializer ) : Float4( initializer ) {}

// ---------------------------------------------------

	Float4Marshal::~Float4Marshal() {}

// ---------------------------------------------------

	ETNoAliasHint void Float4Marshal::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static void ETScriptAPICall Constructor0( void* const thisPointer, float32 x, float32 y ) {
				new(thisPointer) Float4Marshal( x, y, 0.0f, 0.0f );
			}

			static void ETScriptAPICall Constructor1( void* const thisPointer, float32 x, float32 y, float32 z ) {
				new(thisPointer) Float4Marshal( x, y, z, 0.0f );
			}

			static void ETScriptAPICall Constructor2( void* const thisPointer, float32 x, float32 y, float32 z, float32 w ) {
				new(thisPointer) Float4Marshal( x, y, z, w );
			}

			static void ETScriptAPICall Constructor3( void* const thisPointer, const Float4Marshal& source ) {
				new(thisPointer) Float4Marshal( source );
			}

			static ETInlineHint ETNoAliasHint float32 ETScriptAPICall DotProduct( const Float4Marshal& vector0, const Float4Marshal& vector1 ) {
				return ::Eldritch2::DotProduct( vector0, vector1 );
			}

			static ETInlineHint ETNoAliasHint Float4Marshal	ETScriptAPICall LinearInterpolate( const Float4Marshal& vector0, const Float4Marshal& vector1, float32 alpha ) {
				return ::Eldritch2::LinearInterpolate( vector0, vector1, alpha );
			}

			static ETInlineHint ETNoAliasHint Float4Marshal ETScriptAPICall CrossProduct( const Float4Marshal& vector0, const Float4Marshal& vector1 ) {
				return ::Eldritch2::CrossProduct( vector0, vector1 );
			}

			static ETInlineHint ETNoAliasHint float32 ETScriptAPICall GetSquaredMagnitude( const Float4Marshal* thisPointer ) {
				return ::Eldritch2::DotProduct( *thisPointer, *thisPointer );
			}

			static ETInlineHint ETNoAliasHint float32 ETScriptAPICall GetInverseMagnitude( const Float4Marshal* thisPointer ) {
				return ReciprocalSqrt( GetSquaredMagnitude( thisPointer ) );
			}

			static ETInlineHint ETNoAliasHint float32 ETScriptAPICall GetMagnitude( const Float4Marshal* thisPointer ) {
				return Sqrt( GetSquaredMagnitude( thisPointer ) );
			}

			static ETInlineHint ETNoAliasHint Float4Marshal& ETScriptAPICall Normalize( Float4Marshal* thisPointer ) {
				// Normalize() returns a reference to itself. This conversion cannot be done implicitly, but is safe in this context.
				return static_cast<Float4Marshal&>(thisPointer->Normalize());
			}

			static ETInlineHint ETNoAliasHint Float4Marshal ETScriptAPICall OpAdd( const Float4Marshal* thisPointer, const Float4Marshal& operand ) {
				return { *thisPointer + operand };
			}

			static ETInlineHint ETNoAliasHint Float4Marshal ETScriptAPICall OpSubtract( const Float4Marshal* thisPointer, const Float4Marshal& operand ) {
				return { *thisPointer - operand };
			}

			static ETInlineHint ETNoAliasHint Float4Marshal ETScriptAPICall OpMultiply( const Float4Marshal* thisPointer, float32 operand ) {
				return { *thisPointer * operand };
			}

			static ETInlineHint ETNoAliasHint Float4Marshal ETScriptAPICall OpDivide( const Float4Marshal* thisPointer, float32 operand ) {
				return { *thisPointer / operand };
			}

			static Float4Marshal& ETScriptAPICall OpAssign( Float4Marshal* thisPointer, const Float4Marshal& operand ) {
				return { *thisPointer = operand };
			}

			static ETInlineHint ETNoAliasHint Float4Marshal& ETScriptAPICall OpAddAssign( Float4Marshal* thisPointer, const Float4Marshal& operand ) {
				// the combo assignment operators return a reference to themselves. This can't be done implicitly, but is safe in this context.
				return static_cast<Float4Marshal&>((*thisPointer) += operand);
			}

			static ETInlineHint ETNoAliasHint Float4Marshal& ETScriptAPICall OpSubtractAssign( Float4Marshal* thisPointer, const Float4Marshal& operand ) {
				// the combo assignment operators return a reference to themselves. This can't be done implicitly, but is safe in this context.
				return static_cast<Float4Marshal&>((*thisPointer) -= operand);
			}

			static ETInlineHint ETNoAliasHint Float4Marshal& ETScriptAPICall OpMultiplyAssign( Float4Marshal* thisPointer, float32 operand ) {
				// the combo assignment operators return a reference to themselves. This can't be done implicitly, but is safe in this context.
				return static_cast<Float4Marshal&>((*thisPointer) *= operand);
			}

			static ETInlineHint ETNoAliasHint Float4Marshal& ETScriptAPICall OpDivideAssign( Float4Marshal* thisPointer, float32 operand ) {
				// the combo assignment operators return a reference to themselves. This can't be done implicitly, but is safe in this context.
				return static_cast<Float4Marshal&>((*thisPointer) /= operand);
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("Float4Marshal::ExposeScriptAPI() Temporary Allocator") );
		if( const auto registerResult = typeRegistrar.RegisterUserDefinedValueType<Float4Marshal>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			// Constructor registration
			typeBuilder.ExposeConstructor( &FunctionHelper::Constructor0 ).ExposeConstructor( &FunctionHelper::Constructor1 ).ExposeConstructor( &FunctionHelper::Constructor2 ).ExposeConstructor( &FunctionHelper::Constructor3 );

			// Virtual property registration
			typeBuilder.ExposeVirtualProperty( "SquaredMagnitude", &FunctionHelper::GetSquaredMagnitude ).ExposeVirtualProperty( "InverseMagnitude", &FunctionHelper::GetInverseMagnitude ).ExposeVirtualProperty( "Magnitude", &FunctionHelper::GetMagnitude );

			// Method registration
			typeBuilder.ExposeMethod( "Normalize", &FunctionHelper::Normalize );

			// Operator registration
			typeBuilder.ExposeBinaryOperator( BinaryOperatorClass::ADDITION, &FunctionHelper::OpAdd ).ExposeBinaryOperator( BinaryOperatorClass::SUBTRACTION, &FunctionHelper::OpSubtract );
			typeBuilder.ExposeBinaryOperator( BinaryOperatorClass::MULTIPLICATION, &FunctionHelper::OpMultiply ).ExposeBinaryOperator( BinaryOperatorClass::DIVISION, &FunctionHelper::OpDivide );
			typeBuilder.ExposeBinaryOperator( BinaryOperatorClass::ASSIGNMENT, &FunctionHelper::OpAssign );
			typeBuilder.ExposeBinaryOperator( BinaryOperatorClass::ADDITION_ASSIGNMENT, &FunctionHelper::OpAddAssign ).ExposeBinaryOperator( BinaryOperatorClass::SUBTRACTION_ASSIGNMENT, &FunctionHelper::OpSubtractAssign );
			typeBuilder.ExposeBinaryOperator( BinaryOperatorClass::MULTIPLICATION_ASSIGNMENT, &FunctionHelper::OpMultiplyAssign ).ExposeBinaryOperator( BinaryOperatorClass::DIVISION_ASSIGNMENT, &FunctionHelper::OpDivideAssign );

			// Global function registration
			typeRegistrar.ExposeFunction( "DotProduct", &FunctionHelper::DotProduct ).ExposeFunction( "LinearInterpolate", &FunctionHelper::LinearInterpolate ).ExposeFunction( "CrossProduct", &FunctionHelper::CrossProduct );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

// ---------------------------------------------------

	OrientationMarshal::OrientationMarshal( const float32 x, const float32 y, const float32 z, const float32 w ) : Quaternion( x, y, z, w ) {}

// ---------------------------------------------------

	OrientationMarshal::OrientationMarshal( const Scripting::OrientationMarshal& initializer ) : Quaternion( static_cast<const Quaternion&>(initializer) ) {}

// ---------------------------------------------------

	OrientationMarshal::OrientationMarshal( const ::Eldritch2::Quaternion initializer ) : Quaternion( initializer ) {}

// ---------------------------------------------------

	OrientationMarshal::~OrientationMarshal() {}

// ---------------------------------------------------

	ETNoAliasHint void OrientationMarshal::ExposeScriptAPI( ScriptAPIRegistrationInitializationVisitor& typeRegistrar ) {
		struct FunctionHelper {
			static void ETScriptAPICall Constructor0( void* const thisPointer ) {
				new(thisPointer) OrientationMarshal( 1.0f, 0.0f, 0.0f, 0.0f );
			}

			static void ETScriptAPICall Constructor1( void* const thisPointer, float32 w, float32 x, float32 y, float32 z ) {
				new(thisPointer) OrientationMarshal( x, y, z, w );
			}

			static ETInlineHint ETNoAliasHint OrientationMarshal ETScriptAPICall LinearInterpolate( const OrientationMarshal& orientation0, const OrientationMarshal& orientation1, float32 alpha ) {
				return ::Eldritch2::LinearInterpolate( orientation0, orientation1, alpha );
			}

			static ETInlineHint ETNoAliasHint Float4Marshal	ETScriptAPICall RotateVector( const OrientationMarshal* thisPointer, const Float4Marshal& point ) {
				return Float4Marshal( thisPointer->RotateVector( point ) );
			}

			static ETInlineHint ETNoAliasHint OrientationMarshal ETScriptAPICall GetReverse( const OrientationMarshal* thisPointer ) {
				return OrientationMarshal( thisPointer->Reverse() );
			}

			static ETInlineHint ETNoAliasHint Float4Marshal	ETScriptAPICall GetForwardVector( const OrientationMarshal* thisPointer ) {
				return RotateVector( thisPointer, Float4Marshal( 0.0f, 0.0f, 1.0f, 0.0f ) );
			}

			static ETInlineHint ETNoAliasHint Float4Marshal ETScriptAPICall GetUpVector( const OrientationMarshal* thisPointer ) {
				return RotateVector( thisPointer, Float4Marshal( 0.0f, 1.0f, 0.0f, 0.0f ) );
			}

			static ETInlineHint ETNoAliasHint Float4Marshal ETScriptAPICall GetRightVector( const OrientationMarshal* thisPointer ) {
				return RotateVector( thisPointer, Float4Marshal( 1.0f, 0.0f, 0.0f, 0.0f ) );
			}

			static ETInlineHint ETNoAliasHint OrientationMarshal ETScriptAPICall OrientationFromLookatUpVectors( const Float4Marshal& /*lookAt*/, const Float4Marshal& /*up*/ ) {
				return OrientationMarshal( 0.0f, 0.0f, 0.0f, 1.0f );
			}
		};

	// ---

		FixedStackAllocator<16u>	temporaryAllocator( UTF8L("OrientationMarshal::ExposeScriptAPI() Temporary Allocator") );
		if( const auto registerResult = typeRegistrar.RegisterUserDefinedValueType<OrientationMarshal>( temporaryAllocator ) ) {
			auto&	typeBuilder( *registerResult.object );

			typeRegistrar.EnsureValueTypeDeclared<Float4Marshal>();

			// Constructor registration
			typeBuilder.ExposeConstructor( &FunctionHelper::Constructor0 ).ExposeConstructor( &FunctionHelper::Constructor1 );

			// Virtual property registration
			typeBuilder.ExposeVirtualProperty( "Reverse", &FunctionHelper::GetReverse ).ExposeVirtualProperty( "ForwardVector", &FunctionHelper::GetForwardVector );
			typeBuilder.ExposeVirtualProperty( "UpVector", &FunctionHelper::GetUpVector ).ExposeVirtualProperty( "RightVector", &FunctionHelper::GetRightVector );

			// Method registration
			typeBuilder.ExposeMethod( "RotateVector", &FunctionHelper::RotateVector );

			// Global function registration
			typeRegistrar.ExposeFunction( "LinearInterpolate", &FunctionHelper::LinearInterpolate );

			temporaryAllocator.Delete( typeBuilder );
		}
	}

}	// namespace Scripting
}	// namespace Eldritch2