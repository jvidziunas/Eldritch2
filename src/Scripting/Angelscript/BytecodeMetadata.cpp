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
#include <Utility/Containers/Range.hpp>
#include <Utility/ErrorCode.hpp>
//------------------------------------------------------------------//
#include <Flatbuffers/AngelScriptBytecodePackage_generated.h>
//------------------------------------------------------------------//
#include <angelscript.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	BytecodeMetadata::BytecodeMetadata( Allocator& allocator ) : _typeMetadata( { allocator, "Angelscript Bytecode Package Type Metadata Allocator" } ),
																 _functionMetadata( { allocator, "Angelscript Bytecode Package Function Metadata Allocator" } ),
																 _typePropertyMetadata( { allocator, "Angelscript Bytecode Package Type Property Metadata Allocator" } ),
																 _moduleGlobalPropertyMetadata( { allocator, "Angelscript Bytecode Package Global Property Metadata Allocator" } ) {}

// ---------------------------------------------------

	BytecodeMetadata::BytecodeMetadata( BytecodeMetadata&& other ) : _typeMetadata( eastl::move( other._typeMetadata ) ),
																	 _functionMetadata( eastl::move( other._functionMetadata ) ),
																	 _typePropertyMetadata( eastl::move( other._typePropertyMetadata ) ),
																	 _moduleGlobalPropertyMetadata( eastl::move( other._moduleGlobalPropertyMetadata ) ) {}

// ---------------------------------------------------

	const BytecodeMetadata::FunctionMetadata* BytecodeMetadata::GetMetadata( const asIScriptFunction* function ) const {
		const auto candidate( _functionMetadata.Find( function ) );

		if( candidate == _functionMetadata.End() ) {
			return nullptr;
		}

		return &candidate->second;
	}

// ---------------------------------------------------

	const BytecodeMetadata::TypeMetadata* BytecodeMetadata::GetMetadata( const asITypeInfo* type ) const {
		const auto candidate( _typeMetadata.Find( type ) );

		if( candidate == _typeMetadata.End() ) {
			return nullptr;
		}

		return &candidate->second;
	}

// ---------------------------------------------------

	const BytecodeMetadata::PropertyMetadata* BytecodeMetadata::GetPropertyMetadata( const asITypeInfo* objectType, asUINT propertyIndex ) const {
		const auto candidate( _typePropertyMetadata.Find( { objectType, propertyIndex } ) );

		if( candidate == _typePropertyMetadata.End() ) {
			return nullptr;
		}

		return &candidate->second;
	}

// ---------------------------------------------------

	const BytecodeMetadata::PropertyMetadata* BytecodeMetadata::GetPropertyMetadata( const void* propertyAddress ) const {
		const auto candidate( _moduleGlobalPropertyMetadata.Find( propertyAddress ) );

		if( candidate == _moduleGlobalPropertyMetadata.End() ) {
			return nullptr;
		}

		return &candidate->second;
	}

// ---------------------------------------------------

	ErrorCode BytecodeMetadata::BindToModule( asIScriptModule& module, Range<const char*> sourceBytes ) {
		HashMap<const asITypeInfo*, TypeMetadata>					typeMetadata( _typeMetadata.GetAllocator() );
		HashMap<const asIScriptFunction*, FunctionMetadata>			functionMetadata( _functionMetadata.GetAllocator() );
		HashMap<Pair<const asITypeInfo*, asUINT>, PropertyMetadata>	typePropertyMetadata( _typePropertyMetadata.GetAllocator() );
		HashMap<const void*, PropertyMetadata>						moduleGlobalPropertyMetadata( _moduleGlobalPropertyMetadata.GetAllocator() );
		auto														definitions( FlatBuffers::GetModuleMetadata( sourceBytes.Begin() ) );

		for( const auto& definition : *definitions->Types() ) {
			const auto	scriptType( module.GetObjectTypeByIndex( definition->Index() ) );

		//	Ensure the index is legal and we have a valid script type.
			if( !scriptType ) {
				return Error::InvalidParameter;
			}

			for( const auto& propertyDefinition : *definition->Properties() ) {
				if( propertyDefinition->Index() < scriptType->GetPropertyCount() ) {
					return Error::InvalidParameter;
				}

				_typePropertyMetadata.Emplace( Pair<const asITypeInfo*, asUINT>( scriptType, propertyDefinition->Index() ), PropertyMetadata() );
			}

			_typeMetadata.Emplace( scriptType, TypeMetadata() );
		}

		for( const auto& definition : *definitions->Functions() ) {
			const auto	scriptFunction( module.GetFunctionByIndex( definition->Index() ) );

		//	Ensure the index is legal and we have a valid script type.
			if( !scriptFunction ) {
				return Error::InvalidParameter;
			}

			functionMetadata.Emplace( scriptFunction, FunctionMetadata() );
		}

		for( const auto& definition : *definitions->Properties() ) {
			auto	scriptProperty( module.GetAddressOfGlobalVar( definition->Index() ) );

		//	Ensure the index is legal and we have a valid property.
			if( !scriptProperty ) {
				return Error::InvalidParameter;
			}

			moduleGlobalPropertyMetadata.Emplace( scriptProperty, PropertyMetadata() );
		}

	//	Commit the metadata changes to the view.
		_typeMetadata					= eastl::move( typeMetadata );
		_functionMetadata				= eastl::move( functionMetadata );
		_typePropertyMetadata			= eastl::move( typePropertyMetadata );
		_moduleGlobalPropertyMetadata	= eastl::move( moduleGlobalPropertyMetadata );

		return Error::None;
	}

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2