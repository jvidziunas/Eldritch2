/*==================================================================*\
  WorldViewFactoryPublishingInitializationVisitor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Foundation {
		class	GameEngine;
		class	World;
	}

	class	Allocator;
	class	ErrorCode;
}

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation = ::Eldritch2::Foundation;

// ---------------------------------------------------

	class WorldViewFactoryPublishingInitializationVisitor {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref WorldViewFactoryPublishingInitializationVisitor instance.
		/*! @param[in] engine The @ref GameEngine this @ref WorldViewFactoryPublishingInitializationVisitor will be adding world view factories to.
			*/
		WorldViewFactoryPublishingInitializationVisitor( Foundation::GameEngine& engine );

		//! Destroys this @ref WorldViewFactoryPublishingInitializationVisitor instance.
		~WorldViewFactoryPublishingInitializationVisitor() = default;

	// ---------------------------------------------------

		//!	Publishes a new world view factory function to the owning @ref GameEngine.
		/*! @param[in] parameter Context pointer that should be passed to the factory function specified by the second parameter.
			@param[in] allocationHintInBytes Memory the @ref GameEngine should reserve for the @ref WorldView instance the function specified in _factoryFunction_ will allocate.
			@param[in] factoryFunction Factory function pointer. This should new() a concrete @ref WorldView subclass instance using the passed-in allocator and register it with the @ref World.
			@returns A reference to *this, for method chaining.
			@see @ref WorldView
			*/
		Foundation::WorldViewFactoryPublishingInitializationVisitor&				PublishFactory( void* const parameter, const size_t allocationHintInBytes, ::Eldritch2::ErrorCode (*factoryFunction)( ::Eldritch2::Allocator&, Foundation::World&, void* ) );
		//!	Publishes a new world view factory function to the owning @ref GameEngine.
		/*! @param[in] allocationHintInBytes Memory the @ref GameEngine should reserve for the @ref WorldView instance the function specified in _factoryFunction_ will allocate.
			@param[in] factoryFunction Factory function pointer. This should new() a concrete @ref WorldView subclass instance using the passed-in allocator and register it with the @ref World.
			@returns A reference to *this, for method chaining.
			@see @ref WorldView
			*/
		ETInlineHint Foundation::WorldViewFactoryPublishingInitializationVisitor&	PublishFactory( const size_t allocationHintInBytes, ::Eldritch2::ErrorCode (*factoryFunction)( ::Eldritch2::Allocator&, Foundation::World&, void* ) );

	// - DATA MEMBERS ------------------------------------

	private:
		Foundation::GameEngine&	_engine;
	};

}	// namespace Foundation
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Foundation/WorldViewFactoryPublishingInitializationVisitor.inl>
//------------------------------------------------------------------//