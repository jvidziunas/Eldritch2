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
#include <Utility/Containers/HashMap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
	template <typename>
	class	Range;
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
		class FunctionMetadata {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref FunctionMetadata instance.
			FunctionMetadata() = default;

			~FunctionMetadata() = default;
		};

	// ---

		class PropertyMetadata {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------
		public:
		//!	Constructs this @ref PropertyMetadata instance.
			PropertyMetadata() = default;

			~PropertyMetadata() = default;
		};

	// ---

		class TypeMetadata {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//! Constructs this @ref TypeMetadata instance.
			TypeMetadata() = default;

			~TypeMetadata() = default;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref BytecodeMetadata instance.
	/*! @param[in] allocator @ref Allocator instance this @ref BytecodeMetadata should use to perform internal allocations. */
		BytecodeMetadata( Eldritch2::Allocator& allocator );
	//! Constructs this @ref BytecodeMetadata instance.
		BytecodeMetadata( BytecodeMetadata&& );

		~BytecodeMetadata() = default;

	// ---------------------------------------------------

	public:
	//!	Extracts metadata for an AngelScript function, if any has been previously associated.
	/*!	@param[in] function The AngelScript function to inspect.
		@returns A pointer to the metadata structure, if found, or a null pointer if no metadata was associated with the function. */
		const FunctionMetadata*	GetMetadata( const asIScriptFunction* function ) const;
	//!	Extracts metadata for an AngelScript type, if any has been previously associated.
	/*!	@param[in] type The AngelScript type to inspect.
		@returns A pointer to the metadata structure, if found, or a null pointer if no metadata was associated with the type. */
		const TypeMetadata*		GetMetadata( const asITypeInfo* type ) const;

	//!	Extracts metadata for an an object property, if any has been previously associated.
	/*!	@param[in] propertyIndex Zero-based index identifying the property to be inspected.
		@returns A pointer to the metadata structure, if found, or a null pointer if no metadata was associated with the property. */
		const PropertyMetadata*	GetPropertyMetadata( const asITypeInfo* objectType, asUINT propertyIndex ) const;
		const PropertyMetadata*	GetPropertyMetadata( const void* propertyAddress ) const;

	// ---------------------------------------------------

	public:
		Eldritch2::ErrorCode	BindToModule( asIScriptModule& module, Eldritch2::Range<const char*> sourceBytes );

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::HashMap<const asITypeInfo*, TypeMetadata>								_typeMetadata;
		Eldritch2::HashMap<const asIScriptFunction*, FunctionMetadata>						_functionMetadata;
		Eldritch2::HashMap<Eldritch2::Pair<const asITypeInfo*, asUINT>, PropertyMetadata>	_typePropertyMetadata;
		Eldritch2::HashMap<const void*, PropertyMetadata>									_moduleGlobalPropertyMetadata;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2