/*==================================================================*\
  BytecodeMetadata.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Containers/FlatOrderedMap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		namespace AngelScript {
			namespace FlatBuffers {
				struct	FunctionMetadata;
				struct	PropertyMetadata;
				struct	ModuleMetadata;
				struct	TypeMetadata;
			}
		}
	}

	class	ErrorCode;
}

class	asIScriptFunction;
class	asIScriptModule;
class	asITypeInfo;

using asUINT	= unsigned int;

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class BytecodeMetadata {
	// - TYPE PUBLISHING ---------------------------------
	
	public:
		using ModuleMetadata	= AngelScript::BytecodeMetadata;

		class FunctionMetadata {
		public:
			::Eldritch2::ErrorCode Bind( ::asIScriptFunction& function, const FlatBuffers::FunctionMetadata& metadata );
		};

	// ---

		class PropertyMetadata {
		public:
			::Eldritch2::ErrorCode Bind( const FlatBuffers::PropertyMetadata& metadata );
		};

	// ---

		class TypeMetadata {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//! Constructs this @ref TypeMetadata instance.
			/*! @param[in] allocator @ref Allocator the @ref TypeMetadata instance should use to perform allocations.
				*/
			TypeMetadata( ::Eldritch2::Allocator& allocator );
			//! Constructs this @ref TypeMetadata instance.
			/*! @remarks @parblock This overload is provided only for container compatibility. _Always_ use
					TypeMetadata(Allocator&) when constructing real instances of the type. @endparblock
				*/
			TypeMetadata();

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	Bind( ::asITypeInfo& scriptType, const FlatBuffers::TypeMetadata& metadata );

		// ---------------------------------------------------

			//!	Extracts metadata for an an object property, if any has been previously associated.
			/*!	@param[in] propertyIndex Zero-based index identifying the property to be inspected.
				@returns A pointer to the metadata structure, if found, or a null pointer if no metadata was associated with the property.
				*/
			const PropertyMetadata*	GetPropertyMetadata( const ::asUINT propertyIndex ) const;

		// - DATA MEMBERS ------------------------------------

		private:
			::Eldritch2::ResizableArray<FunctionMetadata>			_methodMetadata;
			::Eldritch2::FlatOrderedMap<::asUINT, PropertyMetadata>	_propertyMetadata;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref BytecodeMetadata instance.
		/*! @param[in] allocator @ref Allocator instance this @ref BytecodeMetadata should use to perform internal allocations.
			*/
		BytecodeMetadata( ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref BytecodeMetadata instance.
		~BytecodeMetadata() = default;

	// ---------------------------------------------------

		//!	Extracts metadata for an AngelScript function, if any has been previously associated.
		/*!	@param[in] function The AngelScript function to inspect.
			@returns A pointer to the metadata structure, if found, or a null pointer if no metadata was associated with the function.
			*/
		static const FunctionMetadata*	GetMetadata( const ::asIScriptFunction& function );
		//!	Extracts metadata for an AngelScript module, if any has been previously associated.
		/*!	@param[in] module The AngelScript module to inspect.
			@returns A pointer to the metadata structure, if found, or a null pointer if no metadata was associated with the module.
			*/
		static const ModuleMetadata*	GetMetadata( const ::asIScriptModule& module );
		//!	Extracts metadata for an AngelScript type, if any has been previously associated.
		/*!	@param[in] type The AngelScript type to inspect.
			@returns A pointer to the metadata structure, if found, or a null pointer if no metadata was associated with the type.
			*/
		static const TypeMetadata*		GetMetadata( const ::asITypeInfo& type );

		//!	Extracts metadata for an an object property, if any has been previously associated.
		/*!	@param[in] propertyIndex Zero-based index identifying the property to be inspected.
			@returns A pointer to the metadata structure, if found, or a null pointer if no metadata was associated with the property.
			*/
		static const PropertyMetadata*	GetPropertyMetadata( const ::asITypeInfo& objectType, const ::asUINT propertyIndex );
		static const PropertyMetadata*	GetPropertyMetadata( const ::asIScriptModule& module, const void* propertyAddress );

	// ---------------------------------------------------

	protected:
		static void	SetMetadata( ::asIScriptFunction& function, const FunctionMetadata& metadata );
		static void	SetMetadata( ::asIScriptModule& module, const ModuleMetadata& metadata );
		static void	SetMetadata( ::asITypeInfo& objectType, const TypeMetadata& metadata );

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	BindToModule( ::asIScriptModule& module, ::Eldritch2::Range<const char*> sourceBytes );

	private:
		::Eldritch2::ErrorCode	LoadTypeMetadata( ::asIScriptModule& module, const FlatBuffers::ModuleMetadata& sourceData );

		::Eldritch2::ErrorCode	LoadFunctionMetadata( ::asIScriptModule& module, const FlatBuffers::ModuleMetadata& sourceData );

		::Eldritch2::ErrorCode	LoadPropertyMetadata( ::asIScriptModule& module, const FlatBuffers::ModuleMetadata& sourceData );

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator									_rootAllocator;
		::Eldritch2::ResizableArray<TypeMetadata>					_typeMetadata;
		::Eldritch2::ResizableArray<FunctionMetadata>				_functionMetadata;
		::Eldritch2::FlatOrderedMap<const void*, PropertyMetadata>	_propertyMetadata;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2