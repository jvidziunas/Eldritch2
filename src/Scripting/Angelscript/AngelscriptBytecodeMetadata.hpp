/*==================================================================*\
  AngelscriptBytecodeMetadata.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/UnorderedMap.hpp>
//------------------------------------------------------------------//

class	asIScriptModule;

using asUINT = unsigned int;

namespace Eldritch2 {
namespace Scripting {

	namespace Scripting = ::Eldritch2::Scripting;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class AngelscriptBytecodeMetadata {
	// - TYPE PUBLISHING ---------------------------------
	public:
		class FunctionMetadata {
		public:
			//! Shared (input/output) serialization entry point.
			/*! @param[in, out] archive The archive to serialize into/from.
				@returns _true_ if serialization was successful, _false_ if serialization was not successful.
				*/
			template <class Archive>
			bool	Serialize( Archive& archive );
		};

	// ---

		class TypeMetadata {
		// - TYPE PUBLISHING ---------------------------------

		public:
			struct PropertyMetadata {
			//! Shared (input/output) serialization entry point.
			/*! @param[in, out] archive The archive to serialize into/from.
				@returns _true_ if serialization was successful, _false_ if serialization was not successful.
				*/
				template <class Archive>
				bool	Serialize( Archive& archive );
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

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

			//! Shared (input/output) serialization entry point.
			/*! @param[in, out] archive The archive to serialize into/from.
				@returns _true_ if serialization was successful, _false_ if serialization was not successful.
				*/
			template <class Archive>
			bool	Serialize( Archive& archive );

		// - DATA MEMBERS ------------------------------------

		private:
			::Eldritch2::UnorderedMap<::asUINT, FunctionMetadata>	_methodMetadata;
			::Eldritch2::UnorderedMap<::asUINT, PropertyMetadata>	_propertyMetadata;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref AngelscriptBytecodeMetadata instance.
		/*! @param[in] allocator @ref Allocator instance this @ref AngelscriptBytecodeMetadata should use to perform internal allocations.
			*/
		AngelscriptBytecodeMetadata( ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref AngelscriptBytecodeMetadata instance.
		~AngelscriptBytecodeMetadata() = default;

	// ---------------------------------------------------

		bool	BindToModule( ::asIScriptModule& module );

		template <typename Archive>
		bool	Serialize( Archive& archive );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator								_rootAllocator;
		::Eldritch2::UnorderedMap<::asUINT, TypeMetadata>		_typeMetadata;
		::Eldritch2::UnorderedMap<::asUINT, FunctionMetadata>	_functionMetadata;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Angelscript/AngelscriptBytecodeMetadata.inl>
//------------------------------------------------------------------//