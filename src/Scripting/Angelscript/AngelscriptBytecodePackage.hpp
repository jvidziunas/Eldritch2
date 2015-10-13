/*==================================================================*\
  AngelscriptBytecodePackage.hpp
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
#include <angelscript.h>
//------------------------------------------------------------------//
#include <memory>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Utility {
		class	MessagePackReader;
		class	MessagePackWriter;
	}
}

namespace std {

	template<>
	struct default_delete<::asIScriptModule> {
		ETInlineHint void operator()( ::asIScriptModule* const module ) {
			module->Discard();
		}
	};

}	// namespace std

namespace Eldritch2 {
namespace Scripting {

	namespace Scripting = ::Eldritch2::Scripting;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class AngelscriptBytecodePackage {
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
		//! Constructs this @ref AngelscriptBytecodePackage instance.
		/*! @param[in] ownedModule Movable object handle to an @ref asIScriptModule. This @ref AngelscriptBytecodePackage will assume responsibility for freeing resources.
			@param[in] allocator @ref Allocator instance this @ref AngelscriptBytecodePackage should use to perform internal allocations.
			*/
		AngelscriptBytecodePackage( ::std::unique_ptr<::asIScriptModule>&& ownedModule, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref AngelscriptBytecodePackage instance.
		~AngelscriptBytecodePackage() = default;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// ---------------------------------------------------

		bool	SerializeAndBindToModule( Utility::MessagePackReader&& reader );

		template <typename Archive>
		bool	Serialize( Archive& archive );

	// - DATA MEMBERS ------------------------------------

	private:
		::std::unique_ptr<::asIScriptModule>					_module;
		::Eldritch2::ChildAllocator								_rootAllocator;
		::Eldritch2::UnorderedMap<::asUINT, TypeMetadata>		_typeMetadata;
		::Eldritch2::UnorderedMap<::asUINT, FunctionMetadata>	_functionMetadata;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Angelscript/AngelscriptBytecodePackage.inl>
//------------------------------------------------------------------//