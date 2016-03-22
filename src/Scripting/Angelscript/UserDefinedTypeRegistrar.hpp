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
#include <Scripting/ScriptAPIRegistrationInitializationVisitor.hpp>
//------------------------------------------------------------------//

class	asIScriptEngine;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class UserDefinedTypeRegistrar : public Scripting::ScriptApiRegistrationInitializationVisitor {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref UserDefinedTypeRegistrar instance.
		UserDefinedTypeRegistrar( ::asIScriptEngine& scriptEngine );

		~UserDefinedTypeRegistrar() = default;
		
	// ---------------------------------------------------

		TypeToken	RegisterEnumType( const char* name ) override;

		void		RegisterEnumValue( TypeToken token, const char* valueName, int bitValue ) override;
		
	// ---------------------------------------------------

		TypeToken	RegisterReferenceType( const char* name ) override;

		void		RegisterReferenceTypeFactory( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterReferenceTypeReferenceCountMethods( TypeToken token, DataTypeDescriptor addReferenceReturnDescriptor, const DataTypeDescriptor* addReferenceArguments, UntypedNativeFunctionPointer addReferenceCodePointer,
																				 DataTypeDescriptor removeReferenceReturnDescriptor, const DataTypeDescriptor* removeReferenceArguments, UntypedNativeFunctionPointer removeReferenceCodePointer ) override;

		void		RegisterReferenceTypeValueCast( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterReferenceTypeReferenceCast( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterReferenceTypeProperty( TypeToken token, const char* name, DataTypeDescriptor typeDescriptor, ::Eldritch2::ptrdiff offsetIntoObject ) override;

		void		RegisterReferenceTypeVirtualProperty( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterReferenceTypeMethod( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterReferenceTypeUnaryOperator( TypeToken token, UnaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterReferenceTypeBinaryOperator( TypeToken token, BinaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;
		
	// ---------------------------------------------------

		TypeToken	RegisterValueType( const char* name, size_t objectSizeInBytes ) override;

		void		RegisterValueTypeConstructor( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterValueTypeDestructor( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterValueTypeValueCast( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterValueTypeProperty( TypeToken token, const char* name, DataTypeDescriptor typeDescriptor, ::Eldritch2::ptrdiff offsetIntoObject ) override;

		void		RegisterValueTypeVirtualProperty( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterValueTypeMethod( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterValueTypeUnaryOperator( TypeToken token, UnaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

		void		RegisterValueTypeBinaryOperator( TypeToken token, BinaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

	// ---------------------------------------------------

		void	RegisterFunction( const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) override;

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
