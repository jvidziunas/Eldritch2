/*==================================================================*\
  ScriptApiRegistrationInitializationVisitor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//

#define ETScriptAPICall ETCDecl

namespace Eldritch2 {
namespace Scripting {

	template <typename Native>
	struct TypeStringGenerator {
		static const char*	GetTypeName();
	};

// ---

	class ScriptApiRegistrationInitializationVisitor {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using UntypedNativeFunctionPointer	= void (ETScriptAPICall*)();
		using TypeToken						= const void*;

	// ---

		enum DataTypeFlags : ::Eldritch2::uint32 {
			Normal		= 0,
			Const		= 1,
			Pointer		= 2,
			Reference	= 4
		};

	// ---

		enum class UnaryOperatorClass {
			PrefixIncrement,
			PostfixIncrement,
			PrefixDecrement,
			PostfixDecrement,
			Negate,
			Complement,

			Count
		};

	// ---

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

			Count
		};

	// ---

		struct DataTypeDescriptor {
			const char*		name;
			DataTypeFlags	flags;
		};

	// ---

		template <typename RealEnum>
		class EnumTypeBuilder {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref EnumTypeBuilder instance.
			EnumTypeBuilder( ScriptApiRegistrationInitializationVisitor& owner, TypeToken token );
			//!	Constructs this @ref EnumTypeBuilder instance.
			EnumTypeBuilder( const EnumTypeBuilder& ) = delete;
			//!	Constructs this @ref EnumTypeBuilder instance.
			EnumTypeBuilder( EnumTypeBuilder&& );

			~EnumTypeBuilder();

		// ---------------------------------------------------

			EnumTypeBuilder&	ExposeValue( const char* const valueName, RealEnum value );

		// - DATA MEMBERS ------------------------------------

		private:
			ScriptApiRegistrationInitializationVisitor&	_owner;
			TypeToken									_token;
		};

	// ---

		template <typename RealClass>
		class ReferenceTypeBuilder {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ReferenceTypeBuilder instance.
			ReferenceTypeBuilder( ScriptApiRegistrationInitializationVisitor& owner, TypeToken token );
			//!	Constructs this @ref ReferenceTypeBuilder instance.
			ReferenceTypeBuilder( const ReferenceTypeBuilder& ) = delete;
			//!	Constructs this @ref ReferenceTypeBuilder instance.
			ReferenceTypeBuilder( ReferenceTypeBuilder&& );

			~ReferenceTypeBuilder();

		// ---------------------------------------------------

			ReferenceTypeBuilder&	ExposeFactory( RealClass* (ETScriptAPICall* factory)() );
			template <typename... Arguments>
			ReferenceTypeBuilder&	ExposeFactory( RealClass* (ETScriptAPICall* factory)( Arguments... ) );

		// ---------------------------------------------------

			ReferenceTypeBuilder&	ExposeReferenceCountingMethods( void (ETScriptAPICall* addReferenceFunction)( RealClass* ), void (ETScriptAPICall* removeReferenceFunction)( RealClass* ) );

		// ---------------------------------------------------

			template <typename Result>
			ReferenceTypeBuilder&	ExposeValueCast( Result (ETScriptAPICall* castFunction)( RealClass* ) );
			template <typename Result>
			ReferenceTypeBuilder&	ExposeValueCast( Result (ETScriptAPICall* castFunction)( const RealClass* ) );

			template <typename Result>
			ReferenceTypeBuilder&	ExposeReferenceCast( Result* (ETScriptAPICall* castFunction)( RealClass* ) );
			template <typename Result>
			ReferenceTypeBuilder&	ExposeReferenceCast( const Result* (ETScriptAPICall* castFunction)( const RealClass* ) );

		// ---------------------------------------------------

			template <typename Property>
			ReferenceTypeBuilder&	ExposeProperty( const char* const propertyName, Property RealClass::* propertyPointer );

		// ---------------------------------------------------

			template <typename Property>
			ReferenceTypeBuilder&	ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)( RealClass* ) );
			template <typename Property>
			ReferenceTypeBuilder&	ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)( const RealClass* ) );
			template <typename Property>
			ReferenceTypeBuilder&	ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( RealClass*, Property ) );
			template <typename Property>
			ReferenceTypeBuilder&	ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( const RealClass*, Property ) );

		// ---------------------------------------------------

			template <typename Return>
			ReferenceTypeBuilder&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( RealClass* ) );
			template <typename Return>
			ReferenceTypeBuilder&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( const RealClass* ) );
			template <typename Return, typename... Arguments>
			ReferenceTypeBuilder&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( RealClass*, Arguments... ) );
			template <typename Return, typename... Arguments>
			ReferenceTypeBuilder&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( const RealClass*, Arguments... ) );

		// ---------------------------------------------------

			template <typename Return>
			ReferenceTypeBuilder&	ExposeUnaryOperator( UnaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( RealClass* ) );
			template <typename Return>
			ReferenceTypeBuilder&	ExposeUnaryOperator( UnaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( const RealClass* ) );

			template <typename Return, typename RightHandOperandType>
			ReferenceTypeBuilder&	ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( RealClass*, RightHandOperandType ) );
			template <typename Return, typename RightHandOperandType>
			ReferenceTypeBuilder&	ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( const RealClass*, RightHandOperandType ) );

		// - DATA MEMBERS ------------------------------------

		private:
			ScriptApiRegistrationInitializationVisitor&	_owner;
			TypeToken									_token;
		};

	// ---

		template <typename RealClass>
		class ValueTypeBuilder {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ValueTypeBuilder instance.
			ValueTypeBuilder( ScriptApiRegistrationInitializationVisitor& owner, TypeToken token );
			//!	Constructs this @ref ValueTypeBuilder instance.
			ValueTypeBuilder( const ValueTypeBuilder& ) = delete;
			//!	Constructs this @ref ValueTypeBuilder instance.
			ValueTypeBuilder( ValueTypeBuilder&& );

			~ValueTypeBuilder();

		// ---------------------------------------------------

			ValueTypeBuilder&	ExposeConstructor( void (ETScriptAPICall* constructor)( void* ) );
			template <typename... Arguments>
			ValueTypeBuilder&	ExposeConstructor( void (ETScriptAPICall* constructor)( void*, Arguments... ) );

		// ---------------------------------------------------

			template <typename Result>
			ValueTypeBuilder&	ExposeValueCast( Result (ETScriptAPICall* castFunction)( RealClass* ) );
			template <typename Result>
			ValueTypeBuilder&	ExposeValueCast( Result (ETScriptAPICall* castFunction)( const RealClass* ) );

		// ---------------------------------------------------

			template <typename Property>
			ValueTypeBuilder&	ExposeProperty( const char* const propertyName, Property RealClass::* propertyPointer );

		// ---------------------------------------------------

			template <typename Property>
			ValueTypeBuilder&	ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)( RealClass* ) );
			template <typename Property>
			ValueTypeBuilder&	ExposeVirtualProperty( const char* const propertyName, Property (ETScriptAPICall* getterFunction)( const RealClass* ) );
			template <typename Property>
			ValueTypeBuilder&	ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( RealClass*, Property ) );
			template <typename Property>
			ValueTypeBuilder&	ExposeVirtualProperty( const char* const propertyName, void (ETScriptAPICall* setterFunction)( const RealClass*, Property ) );

		// ---------------------------------------------------

			template <typename Return>
			ValueTypeBuilder&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( RealClass* ) );
			template <typename Return>
			ValueTypeBuilder&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( const RealClass* ) );
			template <typename Return, typename... Arguments>
			ValueTypeBuilder&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( RealClass*, Arguments... ) );
			template <typename Return, typename... Arguments>
			ValueTypeBuilder&	ExposeMethod( const char* const methodName, Return (ETScriptAPICall* method)( const RealClass*, Arguments... ) );

		// ---------------------------------------------------

			template <typename Return>
			ValueTypeBuilder&	ExposeUnaryOperator( UnaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( RealClass* ) );
			template <typename Return>
			ValueTypeBuilder&	ExposeUnaryOperator( UnaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( const RealClass* ) );

			template <typename Return, typename RightHandOperandType>
			ValueTypeBuilder&	ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( RealClass*, RightHandOperandType ) );
			template <typename Return, typename RightHandOperandType>
			ValueTypeBuilder&	ExposeBinaryOperator( BinaryOperatorClass operatorClass, Return (ETScriptAPICall* method)( const RealClass*, RightHandOperandType ) );

		// - DATA MEMBERS ------------------------------------

		private:
			ScriptApiRegistrationInitializationVisitor&	_owner;
			TypeToken									_token;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref ScriptApiRegistrationInitializationVisitor instance.
		ScriptApiRegistrationInitializationVisitor() = default;

		~ScriptApiRegistrationInitializationVisitor() = default;

	// ---------------------------------------------------

		template <typename RealEnum>
		EnumTypeBuilder<RealEnum>		BeginEnumTypeRegistration();

		template <typename RealClass>
		ReferenceTypeBuilder<RealClass>	BeginReferenceTypeRegistration();

		template <typename RealClass>
		ValueTypeBuilder<RealClass>		BeginValueTypeRegistration();

	// ---------------------------------------------------

		template <typename Return>
		ScriptApiRegistrationInitializationVisitor&	ExposeFunction( const char* const methodName, Return (ETScriptAPICall* method)() );
		template <typename Return, typename... Arguments>
		ScriptApiRegistrationInitializationVisitor&	ExposeFunction( const char* const methodName, Return (ETScriptAPICall* method)( Arguments... ) );

	// ---------------------------------------------------

		template <typename RealEnum>
		void	EnsureEnumTypeDeclaredToScript();

		template <typename RealClass>
		void	EnsureReferenceTypeDeclaredToScript();

		template <typename RealClass>
		void	EnsureValueTypeDeclaredToScript();

	// ---------------------------------------------------

	protected:
		virtual void	DeclareEnumType( const char* name );

		virtual void	DeclareReferenceType( const char* name );

		virtual void	DeclareValueType( const char* name, size_t objectSizeInBytes );

	// ---------------------------------------------------

		virtual TypeToken	RegisterEnumType( const char* name ) abstract;

		virtual void		RegisterEnumValue( TypeToken token, const char* valueName, int bitValue ) abstract;

		virtual void		CompleteEnumTypeRegistration( TypeToken token );

	// ---------------------------------------------------

		virtual TypeToken	RegisterReferenceType( const char* name ) abstract;

		virtual void		RegisterReferenceTypeFactory( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterReferenceTypeReferenceCountMethods( TypeToken token, DataTypeDescriptor addReferenceReturnDescriptor, const DataTypeDescriptor* addReferenceArguments, UntypedNativeFunctionPointer addReferenceCodePointer,
																						 DataTypeDescriptor removeReferenceReturnDescriptor, const DataTypeDescriptor* removeReferenceArguments, UntypedNativeFunctionPointer removeReferenceCodePointer ) abstract;

		virtual void		RegisterReferenceTypeValueCast( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterReferenceTypeReferenceCast( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterReferenceTypeProperty( TypeToken token, const char* name, DataTypeDescriptor typeDescriptor, ::Eldritch2::ptrdiff offsetIntoObject ) abstract;

		virtual void		RegisterReferenceTypeVirtualProperty( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterReferenceTypeMethod( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterReferenceTypeUnaryOperator( TypeToken token, UnaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterReferenceTypeBinaryOperator( TypeToken token, BinaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		CompleteReferenceTypeRegistration( TypeToken token );

	// ---------------------------------------------------

		virtual TypeToken	RegisterValueType( const char* typeName, size_t objectSizeInBytes ) abstract;

		virtual void		RegisterValueTypeConstructor( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterValueTypeDestructor( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterValueTypeValueCast( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterValueTypeProperty( TypeToken token, const char* name, DataTypeDescriptor typeDescriptor, ::Eldritch2::ptrdiff offsetIntoObject ) abstract;

		virtual void		RegisterValueTypeVirtualProperty( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterValueTypeMethod( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterValueTypeUnaryOperator( TypeToken token, UnaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		RegisterValueTypeBinaryOperator( TypeToken token, BinaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;

		virtual void		CompleteValueTypeRegistration( TypeToken token );

	// ---------------------------------------------------

		virtual void	RegisterFunction( const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) abstract;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/ScriptApiRegistrationInitializationVisitor.inl>
//------------------------------------------------------------------//
