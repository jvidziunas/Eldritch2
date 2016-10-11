/*==================================================================*\
  ApiRegistrar.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/VirtualMachineContext.hpp>
#include <Scripting/Angelscript/DeclarationFormatter.hpp>
#include <Scripting/Angelscript/ApiRegistrar.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Utility/Mpl/VectorTypes.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//
#define FMT_EXCEPTIONS 0
#include <cppformat/fmt/format.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	ApiRegistrar::ApiRegistrar( asIScriptEngine& scriptEngine, Allocator& allocator ) : _scriptEngine( scriptEngine ), _declarationFormatter( allocator ), _declaration( { allocator, "Angelscript API registrar Declaration String Allocator" } ) {}

// ---------------------------------------------------

	void ApiRegistrar::DefineCoreTypes() {
		_declarationFormatter.OverrideName<Utf8String<>>( "String" );
		_declarationFormatter.OverrideName<Quaternion>( "Orientation" );

		DefineClass<Float4>(	{/* Constructors */},
								{/*	Properties */},
								{/*	Methods */},
								{/*	Operators */},
								{/*	Fields */} );

		DefineClass<Quaternion>(	{/* Constructors */},
									{/*	Properties */},
									{/*	Methods */},
									{/*	Operators */},
									{/*	Fields */} );

		DefineClass<RigidTransform>(	{/* Constructors */},
										{/*	Properties */},
										{/*	Methods */},
										{/*	Operators */},
										{/*	Fields */
											DefineField( "translation", RigidTransform::translation ),
											DefineField( "orientation", RigidTransform::orientation )
										} );

		DefineClass<Utf8String<>>(	{/* Constructors */},
									{/*	Properties */
										DefineGetter( "Length", Utf8String<>::GetLength )
									},
									{/*	Methods */},
									{/*	Operators */},
									{/*	Fields */} );
	}

// ---------------------------------------------------

	void ApiRegistrar::DefineUserAllocatedClass( TypeHandle type, std::initializer_list<Factory> factories, std::initializer_list<Property> properties, std::initializer_list<Method> methods, std::initializer_list<Operator> operators, std::initializer_list<Field> fields ) {
		const auto	typeName( GetTypeName( type ) );
		const auto	registerResult( _scriptEngine.RegisterObjectType( typeName, 0, asEObjTypeFlags::asOBJ_REF ) );

		ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult || asERetCodes::asALREADY_REGISTERED == registerResult );
		
		const asITypeInfo&	typeInfo( *_scriptEngine.GetTypeInfoByName( typeName ) );

	//	Register the factories.
		for( const Factory& factory : factories ) {
			RegisterFactory( typeInfo, factory );
		}

	//	Register the (virtual) properties.
		for( const Property& property : properties ) {
			RegisterProperty( typeInfo, property );
		}

	//	Register the methods.
		for( const Method& method : methods ) {
			RegisterMethod( typeInfo, method );
		}

	//	Register the operators.
		for( const Operator& op : operators ) {
			RegisterOperator( typeInfo, op );
		}

	//	Register the fields.
		for( const Field& field : fields ) {
			RegisterField( typeInfo, field );
		}
	}

// ---------------------------------------------------

	void ApiRegistrar::DefineClass( TypeHandle type, size_t sizeInBytes, std::initializer_list<Constructor> constructors, std::initializer_list<Property> properties, std::initializer_list<Method> methods, std::initializer_list<Operator> operators, std::initializer_list<Field> fields ) {
		const auto	typeName( GetTypeName( type ) );
		const auto	registerResult( _scriptEngine.RegisterObjectType( typeName, static_cast<int>( sizeInBytes ), ( asEObjTypeFlags::asOBJ_VALUE | asEObjTypeFlags::asOBJ_APP_CLASS_CDAK ) ) );

		ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult || asERetCodes::asALREADY_REGISTERED == registerResult );

		const asITypeInfo&	typeInfo( *_scriptEngine.GetTypeInfoByName( typeName ) );

	//	Register the factories.
		for( const Constructor& constructor : constructors ) {
			RegisterConstructor( typeInfo, constructor );
		}

	//	Register the (virtual) properties.
		for( const Property& property : properties ) {
			RegisterProperty( typeInfo, property );
		}

	//	Register the methods.
		for( const Method& method : methods ) {
			RegisterMethod( typeInfo, method );
		}

	//	Register the operators.
		for( const Operator& op : operators ) {
			RegisterOperator( typeInfo, op );
		}

	//	Register the fields.
		for( const Field& field : fields ) {
			RegisterField( typeInfo, field );
		}
	}

// ---------------------------------------------------

	void ApiRegistrar::DefineEnum( TypeHandle type, std::initializer_list<EnumValue> values ) {
		const auto	typeName( GetTypeName( type ) );
		auto		registerResult( _scriptEngine.RegisterEnum( typeName ) );

		ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult );

		for( const EnumValue& value : values ) {
			registerResult = _scriptEngine.RegisterEnumValue( typeName, value.name, value.value );

			ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult );
		}
	}

// ---------------------------------------------------

	void ApiRegistrar::RegisterFunction( const Function& descriptor ) {
		const auto	scriptThunk( [] ( asIScriptGeneric* generic ) {
			VirtualMachineContext	context( *generic );
			reinterpret_cast<NativeFunction*>(generic->GetAuxiliary())( context );
		} );

		_declarationFormatter.RenderDeclaration( _declaration, descriptor );

		const auto	registerResult( _scriptEngine.RegisterGlobalFunction( _declaration, asFunctionPtr( asGENFUNC_t( scriptThunk ) ), asECallConvTypes::asCALL_GENERIC, descriptor.nativeFunction ) );

		ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult );

	//	Clear out the declaration string for the next invocation.
		_declaration.Clear();
	}

// ---------------------------------------------------

	void ApiRegistrar::RegisterFactory( const asITypeInfo& type, const Factory& factory ) {
		const auto	scriptThunk( [] ( asIScriptGeneric* generic ) {
			VirtualMachineContext	context( *generic );
			reinterpret_cast<NativeFunction*>( generic->GetAuxiliary() )( context );
		} );

		_declarationFormatter.RenderDeclaration( _declaration, factory );

		const auto	registerResult( _scriptEngine.RegisterObjectBehaviour( type.GetName(), asEBehaviours::asBEHAVE_FACTORY, _declaration, asFunctionPtr( asGENFUNC_t( scriptThunk ) ), asECallConvTypes::asCALL_GENERIC, factory.nativeFunction ) );

		ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult );

	//	Clear out the declaration string for the next invocation.
		_declaration.Clear();
	}

// ---------------------------------------------------

	void ApiRegistrar::RegisterConstructor( const asITypeInfo& type, const Constructor& constructor ) {
		const auto	scriptThunk( [] ( asIScriptGeneric* generic ) {
			VirtualMachineContext	context( *generic );
			reinterpret_cast<NativeFunction*>( generic->GetAuxiliary() )( context );
		} );

		_declarationFormatter.RenderDeclaration( _declaration, constructor );

		const auto	registerResult( _scriptEngine.RegisterObjectBehaviour( type.GetName(), asEBehaviours::asBEHAVE_CONSTRUCT, _declaration, asFunctionPtr( asGENFUNC_t( scriptThunk ) ), asECallConvTypes::asCALL_GENERIC, constructor.nativeFunction ) );

		ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult );

	//	Clear out the declaration string for the next invocation.
		_declaration.Clear();
	}

// ---------------------------------------------------

	void ApiRegistrar::RegisterProperty( const asITypeInfo& type, const Property& property ) {
		const size_t	nameLength( StringLength( property.name ) + 5u );
		char* const		accessorName( static_cast<char*>(_alloca( nameLength )) );

		if( property.getterFunction ) {
			const Argument	arguments[] = { TypeDescriptor() };

			PrintFormatted( accessorName, nameLength, "get_%s", property.name );
			RegisterMethod( type, { property.type, Utf8Literal( accessorName ), arguments, true, property.getterFunction } );
		}

		if( property.setterFunction ) {
			const Argument	arguments[] = { Argument( property.type ), TypeDescriptor() };

			PrintFormatted( accessorName, nameLength, "set_%s", property.name );
			RegisterMethod( type, { property.type, Utf8Literal( accessorName ), arguments, false, property.setterFunction } );
		}
	}

// ---------------------------------------------------

	void ApiRegistrar::RegisterMethod( const asITypeInfo& type, const Method& method ) {
		const auto scriptThunk( [] ( asIScriptGeneric* generic ) {
			VirtualMachineContext	context( *generic );
			reinterpret_cast<NativeFunction*>(generic->GetAuxiliary())( context );
		} );

		_declarationFormatter.RenderDeclaration( _declaration, method );

		const auto	registerResult( _scriptEngine.RegisterObjectMethod( type.GetName(), _declaration, asFunctionPtr( asGENFUNC_t( scriptThunk ) ), asECallConvTypes::asCALL_GENERIC, method.nativeFunction ) );

		ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult );

	//	Clear out the declaration string for the next invocation.
		_declaration.Clear();
	}

// ---------------------------------------------------

	void ApiRegistrar::RegisterOperator( const asITypeInfo& type, const Operator& op ) {
		static const Utf8Literal	operatorNames[static_cast<size_t>(OperatorType::Count)] = {
			"opPreInc",		// PrefixIncrement
			"opPostInc",	// PostfixIncrement
			"opPreDec",		// PrefixDecrement
			"opPostDec",	// PostfixDecrement
			"opNeg",		// Negate
			"opCom",		// Complement
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

		return RegisterMethod( type, { op.returnDescriptor, operatorNames[static_cast<size_t>( op.type )], op.argumentDescriptors, op.isConst, op.nativeFunction } );
	}

// ---------------------------------------------------

	void ApiRegistrar::RegisterBehavior( const asITypeInfo& type, asEBehaviours behavior, const Function& function ) {
		const auto scriptThunk( [] ( asIScriptGeneric* generic ) {
			VirtualMachineContext	context( *generic );
			reinterpret_cast<NativeFunction*>(generic->GetAuxiliary())( context );
		} );

		_declarationFormatter.RenderDeclaration( _declaration, function );

		const auto	registerResult( _scriptEngine.RegisterObjectMethod( type.GetName(), _declaration, asFunctionPtr( asGENFUNC_t( scriptThunk ) ), asECallConvTypes::asCALL_GENERIC, function.nativeFunction ) );

		ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult );

	//	Clear out the declaration string for the next invocation.
		_declaration.Clear();
	}

// ---------------------------------------------------

	void ApiRegistrar::RegisterBehavior( const asITypeInfo& type, asEBehaviours behavior, const Method& method ) {
		const auto scriptThunk( [] ( asIScriptGeneric* generic ) {
			VirtualMachineContext	context( *generic );
			reinterpret_cast<NativeFunction*>(generic->GetAuxiliary())( context );
		} );

		_declarationFormatter.RenderDeclaration( _declaration, method );

		const auto	registerResult( _scriptEngine.RegisterObjectBehaviour( type.GetName(), behavior, _declaration, asFunctionPtr( asGENFUNC_t( scriptThunk ) ), asECallConvTypes::asCALL_GENERIC, method.nativeFunction ) );

		ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult );

	//	Clear out the declaration string for the next invocation.
		_declaration.Clear();
	}

// ---------------------------------------------------

	void ApiRegistrar::RegisterField( const asITypeInfo& type, const Field& field ) {
		_declarationFormatter.RenderDeclaration( _declaration, field );

		const auto	registerResult( _scriptEngine.RegisterObjectProperty( type.GetName(), _declaration, int( field.offset ) ) );

		ETRuntimeAssert( asERetCodes::asSUCCESS <= registerResult );

	//	Clear out the declaration string for the next invocation.
		_declaration.Clear();
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2
