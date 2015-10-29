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

	BytecodeMetadata::TypeMetadata::TypeMetadata( Allocator& allocator ) : _methodMetadata( { allocator, UTF8L("Angelscript Type Metadata Method Metadata Allocator") } ),
																		   _propertyMetadata( { allocator, UTF8L("Angelscript Type Metadata Property Metadata Allocator") } ) {}

// ---------------------------------------------------

	BytecodeMetadata::TypeMetadata::TypeMetadata() : TypeMetadata( NullAllocator::GetInstance() ) {}

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

	const BytecodeMetadata::TypeMetadata* BytecodeMetadata::GetMetadata( const ::asIObjectType& type ) {
		return static_cast<TypeMetadata*>(type.GetUserData());
	}

// ---------------------------------------------------

	const BytecodeMetadata::PropertyMetadata* BytecodeMetadata::GetPropertyMetadata( const ::asIObjectType& objectType, const ::asUINT propertyIndex ) {
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

	void BytecodeMetadata::SetMetadata( ::asIObjectType& type, const TypeMetadata& metadata ) {
		type.SetUserData( const_cast<TypeMetadata*>(&metadata) );
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::BindToModule( ::asIScriptModule& module, Range<const char*> sourceBytes ) {
		SetMetadata( module, *this );

		LoadTypeMetadata( module, sourceBytes );

		return LoadFunctionMetadata( module, sourceBytes );
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::LoadTypeMetadata( ::asIScriptModule& module, Range<const char*> sourceBytes ) {
		if( auto metadataCollection = FlatBuffers::GetModuleMetadata( sourceBytes.first )->types() ) {
			_typeMetadata.SetCapacity( metadataCollection->size() );

			for( auto metadata( metadataCollection->begin() ), end( metadataCollection->end() ); metadata != end; ++metadata ) {
				_typeMetadata.PushBack( { _rootAllocator } );

				auto&	deserializedMetadata( _typeMetadata.Back() );

				if( auto type = module.GetObjectTypeByIndex( metadata->index() ) ) {
					SetMetadata( *type, deserializedMetadata );
				} else {
					return Error::INVALID_PARAMETER;
				}
			}
		}

		return Error::NONE;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::LoadFunctionMetadata( ::asIScriptModule& module, Range<const char*> sourceBytes ) {
		if( auto metadataCollection = FlatBuffers::GetModuleMetadata( sourceBytes.first )->functions() ) {
			_functionMetadata.SetCapacity( metadataCollection->size() );

			for( auto metadata( metadataCollection->begin() ), end( metadataCollection->end() ); metadata != end; ++metadata ) {
				_functionMetadata.PushBack();

				auto&	deserializedMetadata( _functionMetadata.Back() );

				if( auto function = module.GetFunctionByIndex( metadata->index() ) ) {
					SetMetadata( *function, deserializedMetadata );
				} else {
					return Error::INVALID_PARAMETER;
				}
			}
		}

		return Error::NONE;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::LoadPropertyMetadata( ::asIScriptModule& module, Range<const char*> sourceBytes ) {
		#if 0
if( auto metadataCollection = FlatBuffers::GetModuleMetadata( sourceBytes.first )->properties() ) {
			_propertyMetadata.SetCapacity( metadataCollection->size() );

			for( auto metadata( metadataCollection->begin() ), end( metadataCollection->end() ); metadata != end; ++metadata ) {
			}
		}

		return Error::NONE;
#endif
		return Error::OPERATION_NOT_SUPPORTED;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2