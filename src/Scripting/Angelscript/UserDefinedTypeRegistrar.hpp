/*==================================================================*\
  UserDefinedTypeRegistrar.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
	class	Allocator;
}

class	asIScriptEngine;
struct	asSFuncPtr;

#define SET_TYPE_SCRIPT_NAME( QualifiedTypeName, ScriptTypeNameString ) namespace Eldritch2 { namespace Scripting { namespace AngelScript { template <> const char* const UserDefinedTypeRegistrar::TypeNameGenerator<QualifiedTypeName>::scriptTypeName = ScriptTypeNameString; } } }
#define ETScriptAPICall ETCDecl

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class UserDefinedTypeRegistrar {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum class BinaryOperatorClass {
			Addition,
			Subtraction,
			Multiplication,
			Division,

			Assignment,

			AdditionAssignment,
			SubtractionAssignment,
			MultiplicationAssignment,
			DivisionAssignment,

			COUNT
		};

	// ---

		template <typename Native>
		struct TypeStringGenerator {
			static const char*	GetPrefix();
			static const char*	GetTypeName();
			static const char*	GetSuffix();
		};

	// ---------------------------------------------------

		template <typename Enum>
		class EnumTypeBuilder {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using NativeType	= Enum;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref EnumTypeBuilder instance.
			/*!	@param[in] scriptEngine The AngelScript engine that the new type will be published to.
				*/
			ETInlineHint EnumTypeBuilder( ::asIScriptEngine& scriptEngine );

			~EnumTypeBuilder() = default;

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	ExposeValue( const char* const valueName, NativeType value );

		// - DATA MEMBERS ------------------------------------

		private:
			::asIScriptEngine&	_scriptEngine;
		};

	// ---------------------------------------------------

		template <class Message>
		class MessageTypeBuilder {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using MessageType	= Message;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref MessageTypeBuilder instance.
			/*!	@param[in] scriptEngine The AngelScript engine that the new type will be published to.
				*/
			ETInlineHint MessageTypeBuilder( ::asIScriptEngine& scriptEngine );

			~MessageTypeBuilder() = default;

		// ---------------------------------------------------

			template <typename Property>
			MessageTypeBuilder&	ExposeProperty( const char* const propertyName, Property Message::* propertyPointer );

		// - DATA MEMBERS ------------------------------------

		private:
			::asIScriptEngine&	_scriptEngine;
		};

	// ---------------------------------------------------

		template <typename Native, typename ImplementingType>
		class TypeBuilderCRTPBase {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using NativeType	= Native;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref TypeBuilderCRTPBase instance.
			/*!	@param[in] scriptEngine The AngelScript engine that the new type will be published to.
				*/
			ETInlineHint TypeBuilderCRTPBase( ::asIScriptEngine& scriptEngine );

			~TypeBuilderCRTPBase() = default;

		// ---------------------------------------------------

			template <typename ResultObjectType>
			ImplementingType&	ExposeValueCastTo();

		// ---------------------------------------------------

			template <typename Property>
			ImplementingType&	ExposeProperty( const char* const propertyName, Property Native::* propertyPointer );

		// ---------------------------------------------------

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

		// ---------------------------------------------------

			template <typename Return, typename... Arguments>
			ImplementingType&	ExposeMethod( const char* const methodName, Return (Native::*method)( Arguments... ) );
			template <typename Return, typename... Arguments>
			ImplementingType&	ExposeMethod( const char* const methodName, Return (Native::*method)( Arguments... ) const );
			template <typename Return, typename... Arguments>
			ImplementingType&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( Native*, Arguments... ) );
			template <typename Return, typename... Arguments>
			ImplementingType&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( const Native*, Arguments... ) );

		// ---------------------------------------------------

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
			//!	Constructs this @ref ValueTypeBuilder instance.
			/*!	@param[in] scriptEngine The AngelScript engine that the new type will be published to.
				*/
			ETInlineHint ValueTypeBuilder( ::asIScriptEngine& scriptEngine );

			~ValueTypeBuilder() = default;

		// ---------------------------------------------------

			template <typename... ArgumentTypes>
			ValueTypeBuilder<Native>&	ExposeConstructor( void (ETScriptAPICall* function)( void* const, ArgumentTypes... ) );
		};

	// ---------------------------------------------------

		template <typename Native>
		class ReferenceTypeBuilder : public TypeBuilderCRTPBase<Native, ReferenceTypeBuilder<Native>> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ReferenceTypeBuilder instance.
			/*!	@param[in] scriptEngine The AngelScript engine that the new type will be published to.
				*/
			ETInlineHint ReferenceTypeBuilder( ::asIScriptEngine& scriptEngine );

			~ReferenceTypeBuilder() = default;

		// ---------------------------------------------------

			template <typename ResultObjectType>
			ReferenceTypeBuilder<Native>&	ExposeReferenceCastTo();

		// ---------------------------------------------------

			template <typename... ArgumentTypes>
			ReferenceTypeBuilder<Native>&	ExposeFactory( Native* (ETScriptAPICall *function)( ArgumentTypes... ) );
		};

	// ---

		using UnderlyingTypeRegistrar = ::Eldritch2::Scripting::AngelScript::UserDefinedTypeRegistrar;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref UserDefinedTypeRegistrar instance.
		UserDefinedTypeRegistrar( ::asIScriptEngine& scriptEngine );

		~UserDefinedTypeRegistrar() = default;

	// ---------------------------------------------------

		template <typename Native>
		::Eldritch2::Result<ReferenceTypeBuilder<Native>>	RegisterUserDefinedReferenceType( ::Eldritch2::Allocator& builderAllocator );

		template <typename Native>
		::Eldritch2::Result<ValueTypeBuilder<Native>>		RegisterUserDefinedValueType( ::Eldritch2::Allocator& builderAllocator );

		template <typename Enum>
		::Eldritch2::Result<EnumTypeBuilder<Enum>>			RegisterUserDefinedEnumType( ::Eldritch2::Allocator& builderAllocator );

	// ---------------------------------------------------

		template <typename Native>
		UserDefinedTypeRegistrar&	EnsureReferenceTypeDeclared();

		template <typename Native>
		UserDefinedTypeRegistrar&	EnsureValueTypeDeclared();

		template <typename Enum>
		UserDefinedTypeRegistrar&	EnsureEnumTypeDeclared();

	// ---------------------------------------------------

		template <typename Return, typename... Arguments>
		UserDefinedTypeRegistrar&	ExposeFunction( const char* const functionName, Return (ETScriptAPICall* method)( Arguments... ) );

		template <typename Property>
		UserDefinedTypeRegistrar&	ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)() );

		template <typename Property>
		UserDefinedTypeRegistrar&	ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( Property ) );

	// - DATA MEMBERS ------------------------------------

	private:
		::asIScriptEngine&	_scriptEngine;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Angelscript/UserDefinedTypeRegistrar.inl>
//------------------------------------------------------------------//
