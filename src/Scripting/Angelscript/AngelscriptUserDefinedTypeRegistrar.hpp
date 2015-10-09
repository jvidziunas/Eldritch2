/*==================================================================*\
  AngelscriptUserDefinedTypeRegistrar.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Utility {
		template <typename ResultObjectType>
		struct	ResultPair;
	}

	class	ErrorCode;
	class	Allocator;
}

class	asIScriptEngine;
struct	asSFuncPtr;

#define SET_TYPE_SCRIPT_NAME( QualifiedTypeName, ScriptTypeNameString ) namespace Eldritch2 { namespace Scripting { template <> const char* const AngelscriptUserDefinedTypeRegistrar::TypeNameGenerator<QualifiedTypeName>::scriptTypeName = ScriptTypeNameString; } }
#define ETScriptAPICall ETCDecl

namespace Eldritch2 {
namespace Scripting {

	namespace Scripting	= ::Eldritch2::Scripting;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	enum class BinaryOperatorClass {
		ADDITION,
		SUBTRACTION,
		MULTIPLICATION,
		DIVISION,

		ASSIGNMENT,

		ADDITION_ASSIGNMENT,
		SUBTRACTION_ASSIGNMENT,
		MULTIPLICATION_ASSIGNMENT,
		DIVISION_ASSIGNMENT,

		OPERATOR_COUNT
	};

// ---------------------------------------------------

	class AngelscriptUserDefinedTypeRegistrar {
	public:
		typedef AngelscriptUserDefinedTypeRegistrar	UnderlyingTypeRegistrar;

		template <typename Native>
		struct TypeStringGenerator {
			static const char*	GetPrefix();
			static const char*	GetTypeName();
			static const char*	GetSuffix();
		};

	// ---------------------------------------------------

		static ::Eldritch2::ErrorCode	ErrorCodeFromAngelscriptResult( int angelscriptReturnCode );

	// ---------------------------------------------------

		template <typename Enum>
		class UserDefinedEnumBuilder {
		public:
			typedef Enum	NativeType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			ETInlineHint UserDefinedEnumBuilder( ::asIScriptEngine& scriptEngine );

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	ExposeValue( const char* const valueName, NativeType value );

		// - DATA MEMBERS ------------------------------------

		private:
			::asIScriptEngine&	_scriptEngine;
		};

	// ---------------------------------------------------

		template <typename Native, typename ImplementingType>
		class TypeBuilderCRTPBase {
		public:
			typedef Native	NativeType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this TypeBuilderCRTPBase instance.
			ETInlineHint TypeBuilderCRTPBase( ::asIScriptEngine& scriptEngine );

		// ---------------------------------------------------

			template <typename ResultObjectType>
			ImplementingType&	ExposeValueCastTo();

			template <typename Property>
			ImplementingType&	ExposeProperty( const char* const propertyName, Property Native::* propertyPointer );

			template <typename Property>
			ImplementingType&	ExposeVirtualProperty( const char* const propertyName, Property (Native::*getterFunction)() );
			template <typename Property>
			ImplementingType&	ExposeVirtualProperty( const char* const propertyName, Property (Native::*getterFunction)() const );
			template <typename Property>
			ImplementingType&	ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)( Native* ) );
			template <typename Property>
			ImplementingType&	ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)( const Native* ) );
			template <typename Property>
			ImplementingType&	ExposeVirtualProperty( const char* const propertyName, void (Native::*setterFunction)(Property) );
			template <typename Property>
			ImplementingType&	ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( Native*, Property ) );

			template <typename Return, typename... Arguments>
			ImplementingType&	ExposeMethod( const char* const methodName, Return (Native::*method)( Arguments... ) );
			template <typename Return, typename... Arguments>
			ImplementingType&	ExposeMethod( const char* const methodName, Return (Native::*method)( Arguments... ) const );
			template <typename Return, typename... Arguments>
			ImplementingType&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( Native*, Arguments... ) );
			template <typename Return, typename... Arguments>
			ImplementingType&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( const Native*, Arguments... ) );

			template <typename Return, typename RightHandOperandType>
			ImplementingType&	ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (Native::*operatorMethod)(RightHandOperandType) );
			template <typename Return, typename RightHandOperandType>
			ImplementingType&	ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (Native::*operatorMethod)(RightHandOperandType) const );
			template <typename Return, typename RightHandOperandType>
			ImplementingType&	ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* operatorMethod)( Native*, RightHandOperandType ) );
			template <typename Return, typename RightHandOperandType>
			ImplementingType&	ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* operatorMethod)( const Native*, RightHandOperandType ) );

		// ---------------------------------------------------

		protected:
			ImplementingType&	RegisterFunction( const char* const signature, void (Native::*untypedNativePointer)() );
			ImplementingType&	RegisterFunction( const char* const signature, const ::asSFuncPtr& functionPointer );

		// - DATA MEMBERS ------------------------------------

			::asIScriptEngine&	_scriptEngine;
		};

	// ---------------------------------------------------

		template <typename Native>
		class ValueTypeBuilder : public TypeBuilderCRTPBase<Native, ValueTypeBuilder<Native>> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this ValueTypeBuilder instance.
			ETInlineHint ValueTypeBuilder( ::asIScriptEngine& scriptEngine );

		// ---------------------------------------------------

			template <typename... ArgumentTypes>
			ValueTypeBuilder<Native>&	ExposeConstructor( void (ETScriptAPICall* function)( void* const, ArgumentTypes... ) );
		};

	// ---------------------------------------------------

		template <typename Native>
		class ReferenceTypeBuilder : public TypeBuilderCRTPBase<Native, ReferenceTypeBuilder<Native>> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this ReferenceTypeBuilder instance.
			ETInlineHint ReferenceTypeBuilder( ::asIScriptEngine& scriptEngine );

		// ---------------------------------------------------

			template <typename ResultObjectType>
			ReferenceTypeBuilder<Native>&	ExposeReferenceCastTo();

		// ---------------------------------------------------

			template <typename... ArgumentTypes>
			ReferenceTypeBuilder<Native>&	ExposeFactory( Native* (ETScriptAPICall *function)( ArgumentTypes... ) );
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		// Constructs this AngelscriptUserDefinedTypeRegistrar instance.
		AngelscriptUserDefinedTypeRegistrar( ::asIScriptEngine& scriptEngine );

		// Destroys this AngelscriptUserDefinedTypeRegistrar instance.
		~AngelscriptUserDefinedTypeRegistrar();

	// ---------------------------------------------------

		template <typename Native>
		Utility::ResultPair<ReferenceTypeBuilder<Native>>	RegisterUserDefinedReferenceType( ::Eldritch2::Allocator& builderAllocator );

		template <typename Native>
		Utility::ResultPair<ValueTypeBuilder<Native>>		RegisterUserDefinedValueType( ::Eldritch2::Allocator& builderAllocator );

		template <typename Enum>
		Utility::ResultPair<UserDefinedEnumBuilder<Enum>>	RegisterUserDefinedEnumType( ::Eldritch2::Allocator& builderAllocator );

		template <typename Native>
		Scripting::AngelscriptUserDefinedTypeRegistrar&		EnsureReferenceTypeDeclared();

		template <typename Native>
		Scripting::AngelscriptUserDefinedTypeRegistrar&		EnsureValueTypeDeclared();

		template <typename Enum>
		Scripting::AngelscriptUserDefinedTypeRegistrar&		EnsureEnumDeclared();

		template <typename Return, typename... Arguments>
		Scripting::AngelscriptUserDefinedTypeRegistrar&		ExposeFunction( const char* const functionName, Return (ETScriptAPICall* method)( Arguments... ) );

		template <typename Property>
		Scripting::AngelscriptUserDefinedTypeRegistrar&		ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)() );

		template <typename Property>
		Scripting::AngelscriptUserDefinedTypeRegistrar&		ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( Property ) );

	// - DATA MEMBERS ------------------------------------

	private:
		::asIScriptEngine&	_scriptEngine;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Angelscript/AngelscriptUserDefinedTypeRegistrar.inl>
//------------------------------------------------------------------//
