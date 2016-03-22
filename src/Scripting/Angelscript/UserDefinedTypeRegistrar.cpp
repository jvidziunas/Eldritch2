/*==================================================================*\
  UserDefinedTypeRegistrar.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/UserDefinedTypeRegistrar.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#include <cppformat/cppformat/format.h>
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;

namespace {

	static ETNoAliasHint void WriteType( fmt::ArrayWriter& writer, const AngelScript::UserDefinedTypeRegistrar::DataTypeDescriptor& descriptor ) {
		if( descriptor.flags & AngelScript::UserDefinedTypeRegistrar::DataTypeFlags::Const ) {
			writer << "const ";
		}

		writer << descriptor.name;

		if( descriptor.flags & AngelScript::UserDefinedTypeRegistrar::DataTypeFlags::Pointer ) {
			writer << '@';
		}

		if( descriptor.flags & AngelScript::UserDefinedTypeRegistrar::DataTypeFlags::Reference ) {
			writer << '&';
		}
	}

// ---------------------------------------------------

	static ETNoAliasHint void WriteArgumentType( fmt::ArrayWriter& writer, const AngelScript::UserDefinedTypeRegistrar::DataTypeDescriptor& descriptor ) {
		if( descriptor.flags & AngelScript::UserDefinedTypeRegistrar::DataTypeFlags::Const ) {
			writer << "const ";
		}

		writer << descriptor.name;

		if( descriptor.flags & AngelScript::UserDefinedTypeRegistrar::DataTypeFlags::Pointer ) {
			writer << '@';
		}

		if( descriptor.flags & AngelScript::UserDefinedTypeRegistrar::DataTypeFlags::Reference ) {
			writer << (descriptor.flags & AngelScript::UserDefinedTypeRegistrar::DataTypeFlags::Const ? "&in" : "&out");
		}
	}

// ---------------------------------------------------

	struct DeclarationFormatter {
	public:
		DeclarationFormatter( const char* name, AngelScript::UserDefinedTypeRegistrar::DataTypeDescriptor propertyTypeDescriptor ) {
			fmt::ArrayWriter	writer( declaration );

			WriteType( writer, propertyTypeDescriptor );

			writer << ' ' << name << '\0';
		}

		DeclarationFormatter( const char* name, AngelScript::UserDefinedTypeRegistrar::DataTypeDescriptor returnDescriptor, const AngelScript::UserDefinedTypeRegistrar::DataTypeDescriptor* arguments, bool isConst = false ) {
			fmt::ArrayWriter	writer( declaration );

			WriteType( writer, returnDescriptor );

			writer << ' ' << name << '(';

			for( ; arguments->name; ++arguments ) {
				WriteArgumentType( writer, *arguments );
			
				if( arguments[1].name ) {
					writer << ", ";
				}
			}

			writer << ')';

			if( isConst ) {
				writer << " const";
			}

			writer << '\0';
		}

		char	declaration[256u];
	};

}	// anonymous namespace

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	UserDefinedTypeRegistrar::UserDefinedTypeRegistrar( ::asIScriptEngine& scriptEngine ) : _scriptEngine( scriptEngine ) {}

// ---------------------------------------------------

	ScriptApiRegistrationInitializationVisitor::TypeToken UserDefinedTypeRegistrar::RegisterEnumType( const char* name ) {
		const auto	registerResult( _scriptEngine.RegisterEnum( name ) );

		ETRuntimeAssert( ::asSUCCESS <= registerResult );

		return name;
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterEnumValue( TypeToken token, const char* valueName, int bitValue ) {
		const auto	registerResult( _scriptEngine.RegisterEnumValue( static_cast<const char*>(token), valueName, bitValue ) );

		ETRuntimeAssert( ::asSUCCESS <= registerResult );
	}

// ---------------------------------------------------

	ScriptApiRegistrationInitializationVisitor::TypeToken UserDefinedTypeRegistrar::RegisterReferenceType( const char* name ) {
		const auto	registerResult( _scriptEngine.RegisterObjectType( name, 0, ::asEObjTypeFlags::asOBJ_REF ) );

		ETRuntimeAssert( ::asSUCCESS <= registerResult || ::asALREADY_REGISTERED == registerResult );

		return name;
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterReferenceTypeFactory( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		DeclarationFormatter	formatter( "f", returnDescriptor, arguments );

		const auto	registerResult( _scriptEngine.RegisterObjectBehaviour( static_cast<const char*>(token), ::asEBehaviours::asBEHAVE_FACTORY, formatter.declaration, ::asFunctionPtr( codePointer ), ::asECallConvTypes::asCALL_CDECL ) );

		ETRuntimeAssert( ::asSUCCESS <= registerResult );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterReferenceTypeReferenceCountMethods( TypeToken token, DataTypeDescriptor addReferenceReturnDescriptor, const DataTypeDescriptor* addReferenceArguments, UntypedNativeFunctionPointer addReferenceCodePointer, DataTypeDescriptor removeReferenceReturnDescriptor, const DataTypeDescriptor* removeReferenceArguments, UntypedNativeFunctionPointer removeReferenceCodePointer ) {
		{	DeclarationFormatter	formatter( "f", addReferenceReturnDescriptor, addReferenceArguments + 1 );
			const auto	registerResult( _scriptEngine.RegisterObjectBehaviour( static_cast<const char*>(token), ::asEBehaviours::asBEHAVE_ADDREF, formatter.declaration, ::asFunctionPtr( addReferenceCodePointer ), ::asECallConvTypes::asCALL_CDECL_OBJFIRST ) );

			ETRuntimeAssert( ::asSUCCESS <= registerResult );
		}
		{	DeclarationFormatter	formatter( "f", removeReferenceReturnDescriptor, removeReferenceArguments + 1 );
			const auto	registerResult( _scriptEngine.RegisterObjectBehaviour( static_cast<const char*>(token), ::asEBehaviours::asBEHAVE_RELEASE, formatter.declaration, ::asFunctionPtr( removeReferenceCodePointer ), ::asECallConvTypes::asCALL_CDECL_OBJFIRST ) );

			ETRuntimeAssert( ::asSUCCESS <= registerResult );
		}
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterReferenceTypeValueCast( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		UserDefinedTypeRegistrar::RegisterReferenceTypeMethod( token, "opConv", returnDescriptor, arguments, codePointer );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterReferenceTypeReferenceCast( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		UserDefinedTypeRegistrar::RegisterReferenceTypeMethod( token, "opCast", returnDescriptor, arguments, codePointer );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterReferenceTypeProperty( TypeToken token, const char* name, DataTypeDescriptor typeDescriptor, ::Eldritch2::ptrdiff offsetIntoObject ) {
		DeclarationFormatter	formatter( name, typeDescriptor );

		const auto	registerResult( _scriptEngine.RegisterObjectProperty( static_cast<const char*>(token), formatter.declaration, static_cast<int>(offsetIntoObject) ) );

		ETRuntimeAssert( ::asSUCCESS <= registerResult );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterReferenceTypeVirtualProperty( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		// Getter functions only have one parameter-- the 'this' pointer.
		const bool	isGetter( nullptr == arguments[1].name );
		char		accessorName[64u];
		
		fmt::ArrayWriter( accessorName ) << (isGetter ? "get_" : "set_") << name << '\0';

		UserDefinedTypeRegistrar::RegisterReferenceTypeMethod( token, accessorName, returnDescriptor, arguments, codePointer );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterReferenceTypeMethod( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		DeclarationFormatter	formatter( name, returnDescriptor, arguments + 1, (arguments[0].flags & DataTypeFlags::Const ? true : false) );

		const auto	registerResult( _scriptEngine.RegisterObjectMethod( static_cast<const char*>(token), formatter.declaration, asFunctionPtr( codePointer ), ::asECallConvTypes::asCALL_CDECL_OBJFIRST ) );

		ETRuntimeAssert( ::asSUCCESS <= registerResult );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterReferenceTypeUnaryOperator( TypeToken token, UnaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		static const char*	operatorNames[] = {
			"opPreInc",		// PrefixIncrement
			"opPostInc",	// PostfixIncrement
			"opPreDec",		// PrefixDecrement
			"opPostDec",	// PostfixDecrement
			"opNeg",		// Negate
			"opCom",		// Complement
		};

		static_assert( _countof( operatorNames ) == static_cast<size_t>(UnaryOperatorClass::Count), "Angelscript unary operator name table needs updating!" );

		UserDefinedTypeRegistrar::RegisterReferenceTypeMethod( token, operatorNames[static_cast<size_t>(operatorClass)], returnDescriptor, arguments, codePointer );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterReferenceTypeBinaryOperator( TypeToken token, BinaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		static const char*	operatorNames[] = {
			"opAdd",		// Addition
			"opSub",		// Subtraction
			"opMul",		// Multiplication
			"opDiv",		// Division
			"opAssign",		// Assignment
			"opAddAssign",	// AdditionAssignment
			"opSubAssign",	// SubtractionAssignment
			"opMulAssign",	// MultiplicationAssignment
			"opDivAssign",	// DivisionAssignment
		};

		static_assert( _countof( operatorNames ) == static_cast<size_t>(BinaryOperatorClass::Count), "Angelscript binary operator name table needs updating!" );

		UserDefinedTypeRegistrar::RegisterReferenceTypeMethod( token, operatorNames[static_cast<size_t>(operatorClass)], returnDescriptor, arguments, codePointer );
	}

// ---------------------------------------------------

	ScriptApiRegistrationInitializationVisitor::TypeToken UserDefinedTypeRegistrar::RegisterValueType( const char* name, size_t objectSizeInBytes ) {
		const auto	registerResult( _scriptEngine.RegisterObjectType( name, static_cast<int>(objectSizeInBytes), (::asEObjTypeFlags::asOBJ_VALUE | ::asEObjTypeFlags::asOBJ_APP_CLASS_CDAK) ) );

		ETRuntimeAssert( ::asSUCCESS <= registerResult || ::asALREADY_REGISTERED == registerResult );

		return name;
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterValueTypeConstructor( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		const DeclarationFormatter	formatter( "f", returnDescriptor, arguments + 1 );

		const auto	registerResult( _scriptEngine.RegisterObjectBehaviour( static_cast<const char*>(token), asBEHAVE_CONSTRUCT, formatter.declaration, asFunctionPtr( codePointer ), ::asECallConvTypes::asCALL_CDECL_OBJFIRST ) );

		ETRuntimeAssert( ::asSUCCESS <= registerResult );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterValueTypeDestructor( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		const DeclarationFormatter	formatter( "f", returnDescriptor, arguments + 1 );

		const auto	registerResult( _scriptEngine.RegisterObjectBehaviour( static_cast<const char*>(token), asBEHAVE_DESTRUCT, formatter.declaration, asFunctionPtr( codePointer ), ::asECallConvTypes::asCALL_CDECL_OBJFIRST ) );

		ETRuntimeAssert( ::asSUCCESS <= registerResult );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterValueTypeValueCast( TypeToken token, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		UserDefinedTypeRegistrar::RegisterReferenceTypeValueCast( token, returnDescriptor, arguments, codePointer );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterValueTypeProperty( TypeToken token, const char* name, DataTypeDescriptor typeDescriptor, ptrdiff offsetIntoObject ) {
		UserDefinedTypeRegistrar::RegisterReferenceTypeProperty( token, name, typeDescriptor, offsetIntoObject );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterValueTypeVirtualProperty( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		UserDefinedTypeRegistrar::RegisterReferenceTypeVirtualProperty( token, name, returnDescriptor, arguments, codePointer );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterValueTypeMethod( TypeToken token, const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		UserDefinedTypeRegistrar::RegisterReferenceTypeMethod( token, name, returnDescriptor, arguments, codePointer );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterValueTypeUnaryOperator( TypeToken token, UnaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		UserDefinedTypeRegistrar::RegisterReferenceTypeUnaryOperator( token, operatorClass, returnDescriptor, arguments, codePointer );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterValueTypeBinaryOperator( TypeToken token, BinaryOperatorClass operatorClass, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		UserDefinedTypeRegistrar::RegisterReferenceTypeBinaryOperator( token, operatorClass, returnDescriptor, arguments, codePointer );
	}

// ---------------------------------------------------

	void UserDefinedTypeRegistrar::RegisterFunction( const char* name, DataTypeDescriptor returnDescriptor, const DataTypeDescriptor* arguments, UntypedNativeFunctionPointer codePointer ) {
		DeclarationFormatter	formatter( name, returnDescriptor, arguments );

		const auto	registerResult( _scriptEngine.RegisterGlobalFunction( formatter.declaration, asFunctionPtr( codePointer ), ::asECallConvTypes::asCALL_CDECL ) );

		ETRuntimeAssert( ::asSUCCESS <= registerResult );
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2
