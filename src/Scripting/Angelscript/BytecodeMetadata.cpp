/*==================================================================*\
  BytecodeMetadata.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/BytecodeMetadata.hpp>
#include <Utility/Memory/NullAllocator.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <Scripting/AngelScript/BytecodeMetadata_generated.h>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {


	ErrorCode BytecodeMetadata::FunctionMetadata::Bind( ::asIScriptFunction& scriptFunction, const FlatBuffers::FunctionMetadata& /*metadata*/ ) {
		SetMetadata( scriptFunction, *this );
		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::PropertyMetadata::Bind( const FlatBuffers::PropertyMetadata& /*metadata*/ ) {
		return Error::None;
	}

// ---------------------------------------------------

	BytecodeMetadata::TypeMetadata::TypeMetadata( Allocator& allocator ) : _methodMetadata( { allocator, UTF8L("Angelscript Type Metadata Method Metadata Allocator") } ),
																		   _propertyMetadata( { allocator, UTF8L("Angelscript Type Metadata Property Metadata Allocator") } ) {}

// ---------------------------------------------------

	BytecodeMetadata::TypeMetadata::TypeMetadata() : TypeMetadata( NullAllocator::GetInstance() ) {}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::TypeMetadata::Bind( ::asITypeInfo& scriptType, const FlatBuffers::TypeMetadata& metadata ) {
		for( auto methodMetadata( metadata.Methods()->begin() ), end( metadata.Methods()->end() ); methodMetadata != end; ++methodMetadata ) {
			auto	scriptFunction( scriptType.GetMethodByIndex( methodMetadata->Index() ) );

			if( !scriptFunction ) {
				return Error::InvalidParameter;
			}

			_methodMetadata.PushBack();
			_methodMetadata.Back().Bind( *scriptFunction, **methodMetadata );
		}

		for( auto propertyMetadata( metadata.Properties()->begin() ), end( metadata.Properties()->end() ); propertyMetadata != end; ++propertyMetadata ) {
			_propertyMetadata.Insert( { propertyMetadata->Index(), PropertyMetadata() } ).first->second.Bind( **propertyMetadata );
		}

		SetMetadata( scriptType, *this );

		return Error::None;
	}

// ---------------------------------------------------

	const BytecodeMetadata::PropertyMetadata* BytecodeMetadata::TypeMetadata::GetPropertyMetadata( const ::asUINT propertyIndex ) const {
		const auto	candidate( _propertyMetadata.Find( propertyIndex ) );

		return candidate != _propertyMetadata.End() ? &(candidate->second) : nullptr;
	}

// ---------------------------------------------------

	BytecodeMetadata::BytecodeMetadata( Allocator& allocator ) : _rootAllocator( { allocator, UTF8L("Angelscript Bytecode Package Metadata Root Allocator") } ),
																 _typeMetadata( { _rootAllocator, UTF8L("Angelscript Bytecode Package Type Metadata Allocator") } ),
																 _functionMetadata( { _rootAllocator, UTF8L("Angelscript Bytecode Package Function Metadata Allocator") } ),
																 _propertyMetadata( { _rootAllocator, UTF8L("Angelscript Bytecode Package Global Property Metadata Allocator") } ) {}

// ---------------------------------------------------

	const BytecodeMetadata::FunctionMetadata* BytecodeMetadata::GetMetadata( const ::asIScriptFunction& function ) {
		return static_cast<FunctionMetadata*>(function.GetUserData());
	}

// ---------------------------------------------------

	const BytecodeMetadata::ModuleMetadata* BytecodeMetadata::GetMetadata( const ::asIScriptModule& module ) {
		return static_cast<ModuleMetadata*>(module.GetUserData());
	}

// ---------------------------------------------------

	const BytecodeMetadata::TypeMetadata* BytecodeMetadata::GetMetadata( const ::asITypeInfo& type ) {
		return static_cast<TypeMetadata*>(type.GetUserData());
	}

// ---------------------------------------------------

	const BytecodeMetadata::PropertyMetadata* BytecodeMetadata::GetPropertyMetadata( const ::asITypeInfo& objectType, const ::asUINT propertyIndex ) {
		const auto typeMetadata( GetMetadata( objectType ) );

		return typeMetadata ? typeMetadata->GetPropertyMetadata( propertyIndex ) : nullptr;
	}

// ---------------------------------------------------

	const BytecodeMetadata::PropertyMetadata* BytecodeMetadata::GetPropertyMetadata( const ::asIScriptModule& module, const void* propertyAddress ) {
		if( const auto moduleMetadata = GetMetadata( module ) ) {
			const auto	candidate( moduleMetadata->_propertyMetadata.Find( propertyAddress ) );

			if( candidate != moduleMetadata->_propertyMetadata.End() ) {
				return &(candidate->second);
			}
		}

		return nullptr;
	}

// ---------------------------------------------------

	void BytecodeMetadata::SetMetadata( ::asIScriptFunction& function, const FunctionMetadata& metadata ) {
		function.SetUserData( const_cast<FunctionMetadata*>(&metadata) );
	}

// ---------------------------------------------------

	void BytecodeMetadata::SetMetadata( ::asIScriptModule& module, const ModuleMetadata& metadata ) {
		module.SetUserData( const_cast<ModuleMetadata*>(&metadata) );
	}

// ---------------------------------------------------

	void BytecodeMetadata::SetMetadata( ::asITypeInfo& type, const TypeMetadata& metadata ) {
		type.SetUserData( const_cast<TypeMetadata*>(&metadata) );
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::BindToModule( ::asIScriptModule& module, Range<const char*> sourceBytes ) {
		auto	metadata( FlatBuffers::GetModuleMetadata( sourceBytes.first ) );

		SetMetadata( module, *this );

		return LoadTypeMetadata( module, *metadata ) && LoadFunctionMetadata( module, *metadata ) && LoadPropertyMetadata( module, *metadata ) ? Error::None : Error::InvalidParameter;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::LoadTypeMetadata( ::asIScriptModule& module, const FlatBuffers::ModuleMetadata& sourceData ) {
		// Early out if there is no type metadata stored in the module.
		if( !sourceData.Types() ) {
			return Error::None;
		}

		// Minimize repeated allocations.
		_typeMetadata.SetCapacity( sourceData.Types()->size() );

		for( auto metadata( sourceData.Types()->begin() ), end( sourceData.Types()->end() ); metadata != end; ++metadata ) {
			auto	scriptType( module.GetObjectTypeByIndex( metadata->Index() ) );

			// Ensure the index is legal and we have a valid script type.
			if( !scriptType ) {
				return Error::InvalidParameter;
			}

			_typeMetadata.PushBack( { _rootAllocator } );
			_typeMetadata.Back().Bind( *scriptType, **metadata );
		}

		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::LoadFunctionMetadata( ::asIScriptModule& module, const FlatBuffers::ModuleMetadata& sourceData ) {
		// Early out if there is no free function metadata stored in the module.
		if( !sourceData.Functions() ) {
			return Error::None;
		}

		// Minimize repeated allocations.
		_functionMetadata.SetCapacity( sourceData.Functions()->size() );

		for( auto metadata( sourceData.Functions()->begin() ), end( sourceData.Functions()->end() ); metadata != end; ++metadata ) {
			auto	scriptFunction( module.GetFunctionByIndex( metadata->Index() ) );

			// Ensure the index is legal and we have a valid script function.
			if( !scriptFunction ) {
				return Error::InvalidParameter;
			}

			_functionMetadata.PushBack();
			_functionMetadata.Back().Bind( *scriptFunction, **metadata );
		}

		return Error::None;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::LoadPropertyMetadata( ::asIScriptModule& module, const FlatBuffers::ModuleMetadata& sourceData ) {
		// Early out if there is no global property metadata stored in the module.
		if( !sourceData.Properties() ) {
			return Error::None;
		}

		// Minimize repeated allocations.
		_propertyMetadata.SetCapacity( sourceData.Properties()->size() );

		for( auto metadata( sourceData.Properties()->begin() ), end( sourceData.Properties()->end() ); metadata != end; ++metadata ) {
			auto	scriptProperty( module.GetAddressOfGlobalVar( metadata->Index() ) );

			// Ensure the index is legal and we have a valid property.
			if( !scriptProperty ) {
				return Error::InvalidParameter;
			}

			_propertyMetadata.Insert( { scriptProperty, PropertyMetadata() } ).first->second.Bind( **metadata );
		}

		return Error::None;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2