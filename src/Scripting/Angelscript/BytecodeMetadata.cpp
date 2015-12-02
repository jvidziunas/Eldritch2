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
		return Error::NONE;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::PropertyMetadata::Bind( const FlatBuffers::PropertyMetadata& /*metadata*/ ) {
		return Error::NONE;
	}

// ---------------------------------------------------

	BytecodeMetadata::TypeMetadata::TypeMetadata( Allocator& allocator ) : _methodMetadata( { allocator, UTF8L("Angelscript Type Metadata Method Metadata Allocator") } ),
																		   _propertyMetadata( { allocator, UTF8L("Angelscript Type Metadata Property Metadata Allocator") } ) {}

// ---------------------------------------------------

	BytecodeMetadata::TypeMetadata::TypeMetadata() : TypeMetadata( NullAllocator::GetInstance() ) {}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::TypeMetadata::Bind( ::asITypeInfo& scriptType, const FlatBuffers::TypeMetadata& metadata ) {
		for( auto methodMetadata( metadata.methods()->begin() ), end( metadata.methods()->end() ); methodMetadata != end; ++methodMetadata ) {
			auto	scriptFunction( scriptType.GetMethodByIndex( methodMetadata->index() ) );

			if( !scriptFunction ) {
				return Error::INVALID_PARAMETER;
			}

			_methodMetadata.PushBack();
			_methodMetadata.Back().Bind( *scriptFunction, **methodMetadata );
		}

		for( auto propertyMetadata( metadata.properties()->begin() ), end( metadata.properties()->end() ); propertyMetadata != end; ++propertyMetadata ) {
			_propertyMetadata.Insert( { propertyMetadata->index(), PropertyMetadata() } ).first->second.Bind( **propertyMetadata );
		}

		SetMetadata( scriptType, *this );

		return Error::NONE;
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

		return LoadTypeMetadata( module, *metadata ) && LoadFunctionMetadata( module, *metadata ) && LoadPropertyMetadata( module, *metadata ) ? Error::NONE : Error::INVALID_PARAMETER;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::LoadTypeMetadata( ::asIScriptModule& module, const FlatBuffers::ModuleMetadata& sourceData ) {
		// Early out if there is no type metadata stored in the module.
		if( !sourceData.types() ) {
			return Error::NONE;
		}

		// Minimize repeated allocations.
		_typeMetadata.SetCapacity( sourceData.types()->size() );

		for( auto metadata( sourceData.types()->begin() ), end( sourceData.types()->end() ); metadata != end; ++metadata ) {
			auto	scriptType( module.GetObjectTypeByIndex( metadata->index() ) );

			// Ensure the index is legal and we have a valid script type.
			if( !scriptType ) {
				return Error::INVALID_PARAMETER;
			}

			_typeMetadata.PushBack( { _rootAllocator } );
			_typeMetadata.Back().Bind( *scriptType, **metadata );
		}

		return Error::NONE;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::LoadFunctionMetadata( ::asIScriptModule& module, const FlatBuffers::ModuleMetadata& sourceData ) {
		// Early out if there is no free function metadata stored in the module.
		if( !sourceData.functions() ) {
			return Error::NONE;
		}

		// Minimize repeated allocations.
		_functionMetadata.SetCapacity( sourceData.functions()->size() );

		for( auto metadata( sourceData.functions()->begin() ), end( sourceData.functions()->end() ); metadata != end; ++metadata ) {
			auto	scriptFunction( module.GetFunctionByIndex( metadata->index() ) );

			// Ensure the index is legal and we have a valid script function.
			if( !scriptFunction ) {
				return Error::INVALID_PARAMETER;
			}

			_functionMetadata.PushBack();
			_functionMetadata.Back().Bind( *scriptFunction, **metadata );
		}

		return Error::NONE;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::LoadPropertyMetadata( ::asIScriptModule& module, const FlatBuffers::ModuleMetadata& sourceData ) {
		// Early out if there is no global property metadata stored in the module.
		if( !sourceData.properties() ) {
			return Error::NONE;
		}

		// Minimize repeated allocations.
		_propertyMetadata.SetCapacity( sourceData.properties()->size() );

		for( auto metadata( sourceData.properties()->begin() ), end( sourceData.properties()->end() ); metadata != end; ++metadata ) {
			auto	scriptProperty( module.GetAddressOfGlobalVar( metadata->index() ) );

			// Ensure the index is legal and we have a valid property.
			if( !scriptProperty ) {
				return Error::INVALID_PARAMETER;
			}

			_propertyMetadata.Insert( { scriptProperty, PropertyMetadata() } ).first->second.Bind( **metadata );
		}

		return Error::NONE;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2