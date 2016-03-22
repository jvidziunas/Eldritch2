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
#include <Scripting/ScriptApiRegistrationInitializationVisitor.hpp>
#include <Scripting/ScriptMarshalTypes.hpp>
#include <Utility/Math/StandardLibrary.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Scripting {

	static ETThreadLocal Allocator*	stringAllocator							= nullptr;
	const char* const				StringMarshal::scriptTypeName			= "String";
	const char* const				Float4Marshal::scriptTypeName			= "Float4";
	const char* const				OrientationMarshal::scriptTypeName		= "Orientation";
	const char* const				RigidTransformMarshal::scriptTypeName	= "RigidTransform";

// ---------------------------------------------------

	StringMarshal::StringMarshal( Allocator& allocator ) : UTF8String<>( { allocator, UTF8L("String Allocator") } ) {}

// ---------------------------------------------------

	StringMarshal::StringMarshal( const UTF8Char* const string, const size_t stringLengthInOctets, Allocator& allocator ) : UTF8String<>( string, (string + stringLengthInOctets), { allocator, UTF8L("String Allocator") } ) {}

// ---------------------------------------------------

	StringMarshal::StringMarshal( const StringMarshal& string, Allocator& allocator ) : StringMarshal( string.AsCString(), string.GetLength(), allocator ) {}

// ---------------------------------------------------

	ETNoAliasHint void StringMarshal::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& typeRegistrar ) {
		using BinaryOperatorClass = ScriptApiRegistrationInitializationVisitor::BinaryOperatorClass;

	// ---

		auto	builder( typeRegistrar.BeginValueTypeRegistration<StringMarshal>() );

		builder.ExposeConstructor( [] ( void* thisPointer ) -> void {
			new(thisPointer) StringMarshal( *stringAllocator );
		} ).ExposeConstructor<const StringMarshal&>( [] ( void* thisPointer, const StringMarshal& string ) {
			new(thisPointer) StringMarshal( string, *stringAllocator );
		} );

		builder.ExposeVirtualProperty<SizeType>( "Length", [] ( const StringMarshal* thisPointer ) {
			return thisPointer->GetLength();
		} );

		builder.ExposeBinaryOperator<StringMarshal&, const StringMarshal&>( BinaryOperatorClass::Assignment, [] ( StringMarshal* thisPointer, const StringMarshal& otherString ) -> StringMarshal& {
			return static_cast<StringMarshal&>((*thisPointer).Assign( otherString.Begin(), otherString.End() ));
		} ).ExposeBinaryOperator<StringMarshal&, const StringMarshal&>( BinaryOperatorClass::AdditionAssignment, [] ( StringMarshal* thisPointer, const StringMarshal& additionalString ) -> StringMarshal& {
			return static_cast<StringMarshal&>((*thisPointer) += additionalString);
		} );

		/* typeBuilder.ExposeBinaryOperator( BinaryOperatorClasses::COMPARISON, &FunctionHelper::OpCompare ); */
	}

// ---------------------------------------------------

	void StringMarshal::AttachWorldAllocator( Allocator& allocator ) {
		stringAllocator = &allocator;
	}

// ---------------------------------------------------

	Float4Marshal::Float4Marshal( const float32 x, const float32 y, const float32 z, const float32 w ) : Float4( x, y, z, w ) {}

// ---------------------------------------------------

	Float4Marshal::Float4Marshal( const Float4 initializer ) : Float4( initializer ) {}

// ---------------------------------------------------

	ETNoAliasHint void Float4Marshal::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& visitor ) {
		using BinaryOperatorClass = ScriptApiRegistrationInitializationVisitor::BinaryOperatorClass;

	// ---

		auto	builder( visitor.BeginValueTypeRegistration<Float4Marshal>() );
			
		// Constructor registration
		builder.ExposeConstructor( [] ( void* thisPointer ) {
			new(thisPointer) Float4Marshal( 0.0f, 0.0f, 0.0f, 0.0f );
		} ).ExposeConstructor<float32, float32>( [] ( void* thisPointer, float32 x, float32 y ) {
			new(thisPointer) Float4Marshal( x, y, 0.0f, 0.0f );
		} ).ExposeConstructor<float32, float32, float32>( [] ( void* thisPointer, float32 x, float32 y, float32 z ) {
			new(thisPointer) Float4Marshal( x, y, z, 0.0f );
		} ).ExposeConstructor<float32, float32, float32, float32>( [] ( void* thisPointer, float32 x, float32 y, float32 z, float32 w ) {
			new(thisPointer) Float4Marshal( x, y, z, w );
		} ).ExposeConstructor<const Float4Marshal&>( [] ( void* thisPointer, const Float4Marshal& source ) {
			new(thisPointer) Float4Marshal( source );
		} );

		// Virtual property registration
		builder.ExposeVirtualProperty<float32>( "SquaredMagnitude", [] ( const Float4Marshal* thisPointer ) {
			return ::Eldritch2::DotProduct( *thisPointer, *thisPointer );
		} ).ExposeVirtualProperty<float32>( "InverseMagnitude", [] ( const Float4Marshal* thisPointer ) {
			return ::Eldritch2::ReciprocalSqrt( ::Eldritch2::DotProduct( *thisPointer, *thisPointer ) );
		} ).ExposeVirtualProperty<float32>( "Magnitude", [] ( const Float4Marshal* thisPointer ) {
			return ::Eldritch2::Sqrt( ::Eldritch2::DotProduct( *thisPointer, *thisPointer ) );
		} );

		// Method registration
		builder.ExposeMethod<Float4Marshal&>( "Normalize", [] ( Float4Marshal* thisPointer ) -> Float4Marshal& {
			// Normalize() returns a reference to itself. This conversion cannot be done implicitly, but is safe in this context.
			return static_cast<Float4Marshal&>(thisPointer->Normalize());
		} );

		builder.ExposeBinaryOperator<Float4Marshal, const Float4Marshal&>( BinaryOperatorClass::Addition, [] ( const Float4Marshal* thisPointer, const Float4Marshal& operand ) -> Float4Marshal {
			return { *thisPointer + operand };
		} ).ExposeBinaryOperator<Float4Marshal, const Float4Marshal&>( BinaryOperatorClass::Subtraction, [] ( const Float4Marshal* thisPointer, const Float4Marshal& operand ) -> Float4Marshal {
			return { *thisPointer - operand };
		} ).ExposeBinaryOperator<Float4Marshal, float32>( BinaryOperatorClass::Multiplication, [] ( const Float4Marshal* thisPointer, float32 operand ) -> Float4Marshal {
			return { *thisPointer * operand };
		} ).ExposeBinaryOperator<Float4Marshal, float32>( BinaryOperatorClass::Division, [] ( const Float4Marshal* thisPointer, float32 operand ) -> Float4Marshal {
			return { *thisPointer / operand };
		} ).ExposeBinaryOperator<Float4Marshal, const Float4Marshal&>( BinaryOperatorClass::Assignment, [] ( Float4Marshal* thisPointer, const Float4Marshal& operand ) -> Float4Marshal {
			return { *thisPointer = operand };
		} ).ExposeBinaryOperator<Float4Marshal&, const Float4Marshal&>( BinaryOperatorClass::AdditionAssignment, [] ( Float4Marshal* thisPointer, const Float4Marshal& operand ) -> Float4Marshal& {
			// the combo assignment operators return a reference to themselves. This can't be done implicitly, but is safe in this context.
			return static_cast<Float4Marshal&>((*thisPointer) += operand);
		} ).ExposeBinaryOperator<Float4Marshal&, const Float4Marshal&>( BinaryOperatorClass::SubtractionAssignment, [] ( Float4Marshal* thisPointer, const Float4Marshal& operand ) -> Float4Marshal& {
			// the combo assignment operators return a reference to themselves. This can't be done implicitly, but is safe in this context.
			return static_cast<Float4Marshal&>((*thisPointer) -= operand);
		} ).ExposeBinaryOperator<Float4Marshal&, float32>( BinaryOperatorClass::MultiplicationAssignment, [] ( Float4Marshal* thisPointer, float32 operand ) -> Float4Marshal& {
			// the combo assignment operators return a reference to themselves. This can't be done implicitly, but is safe in this context.
			return static_cast<Float4Marshal&>((*thisPointer) *= operand);
		} ).ExposeBinaryOperator<Float4Marshal&, float32>( BinaryOperatorClass::DivisionAssignment, [] ( Float4Marshal* thisPointer, float32 operand ) -> Float4Marshal& {
			// the combo assignment operators return a reference to themselves. This can't be done implicitly, but is safe in this context.
			return static_cast<Float4Marshal&>((*thisPointer) /= operand);
		} );

		// Global function registration
		// visitor.ExposeFunction( "DotProduct", &FunctionHelper::DotProduct ).ExposeFunction( "LinearInterpolate", &FunctionHelper::LinearInterpolate ).ExposeFunction( "CrossProduct", &FunctionHelper::CrossProduct );
	}

// ---------------------------------------------------

	OrientationMarshal::OrientationMarshal( const float32 x, const float32 y, const float32 z, const float32 w ) : Quaternion( x, y, z, w ) {}

// ---------------------------------------------------

	OrientationMarshal::OrientationMarshal( const ::Eldritch2::Quaternion initializer ) : Quaternion( initializer ) {}

// ---------------------------------------------------

	ETNoAliasHint void OrientationMarshal::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& visitor ) {
		visitor.EnsureValueTypeDeclaredToScript<Float4Marshal>();

		auto	builder( visitor.BeginValueTypeRegistration<OrientationMarshal>() );

		// Constructor registration
		builder.ExposeConstructor( [] ( void* thisPointer ) {
			new(thisPointer) OrientationMarshal( 0.0f, 0.0f, 0.0f, 1.0f );
		} ).ExposeConstructor<float32, float32, float32, float32>( [] ( void* thisPointer, float32 x, float32 y, float32 z, float32 w ) {
			new(thisPointer) OrientationMarshal( x, y, z, w );
		} ).ExposeConstructor<const Float4Marshal&, const Float4Marshal&>( [] ( void* thisPointer, const Float4Marshal& lookAt, const Float4Marshal& up ) {
			new(thisPointer) OrientationMarshal( 0.0f, 0.0f, 0.0f, 1.0f );
		} );

		// Virtual property registration
		builder.ExposeVirtualProperty<OrientationMarshal>( "Reverse", [] ( const OrientationMarshal* thisPointer ) -> OrientationMarshal {
			return { thisPointer->GetReverse() };
		} ).ExposeVirtualProperty<Float4Marshal>( "ForwardVector", [] ( const OrientationMarshal* thisPointer ) -> Float4Marshal {
			return { thisPointer->GetReverse().RotateVector( { 0.0f, 0.0f, 1.0f, 0.0f } ) };
		} ).ExposeVirtualProperty<Float4Marshal>( "UpVector", [] ( const OrientationMarshal* thisPointer ) -> Float4Marshal {
			return { thisPointer->GetReverse().RotateVector( { 0.0f, 1.0f, 0.0f, 0.0f } ) };
		} ).ExposeVirtualProperty<Float4Marshal>( "RightVector", [] ( const OrientationMarshal* thisPointer ) -> Float4Marshal {
			return { thisPointer->GetReverse().RotateVector( { 1.0f, 0.0f, 0.0f, 0.0f } ) };
		} );

		// Global function registration
		visitor.ExposeFunction<Float4Marshal, const Float4Marshal&, const OrientationMarshal&>( "RotateWorldToLocal", [] ( const Float4Marshal& point, const OrientationMarshal& localFrame ) -> Float4Marshal {
			return { localFrame.RotateVector( point ) };
		} ).ExposeFunction<OrientationMarshal, const OrientationMarshal&, const OrientationMarshal&, float32>( "LinearInterpolate", [] ( const OrientationMarshal& orientation0, const OrientationMarshal& orientation1, float32 alpha ) -> OrientationMarshal {
			return ::Eldritch2::LinearInterpolate( orientation0, orientation1, alpha );
		} );
	}

// ---------------------------------------------------

	RigidTransformMarshal::RigidTransformMarshal( const Float4Marshal& translation, const OrientationMarshal& orientation ) : translation( translation ), orientation( orientation ) {}

// ---------------------------------------------------

	RigidTransformMarshal::RigidTransformMarshal( const RigidTransform& transform ) : translation( transform.translation ), orientation( transform.orientation ) {}

// ---------------------------------------------------

	ETNoAliasHint void RigidTransformMarshal::ExposeScriptAPI( ScriptApiRegistrationInitializationVisitor& visitor ) {
		visitor.EnsureValueTypeDeclaredToScript<Float4Marshal>();
		visitor.EnsureValueTypeDeclaredToScript<OrientationMarshal>();

		auto	builder( visitor.BeginValueTypeRegistration<RigidTransformMarshal>() );

		// Constructor registration
		builder.ExposeConstructor( [] ( void* const thisPointer ) {
			new(thisPointer) RigidTransformMarshal( { 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f, 0.0f } );
		} ).ExposeConstructor<const Float4Marshal&, const OrientationMarshal&>( [] ( void* const thisPointer, const Float4Marshal& translation, const OrientationMarshal& orientation ) {
			new(thisPointer) RigidTransformMarshal( translation, orientation );
		} );

		// Property registration
		builder.ExposeProperty( "translation", &RigidTransformMarshal::translation ).ExposeProperty( "orientation", &RigidTransformMarshal::orientation );
	}

}	// namespace Scripting
}	// namespace Eldritch2