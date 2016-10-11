/*==================================================================*\
  ServiceBlackboard.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <eastl/memory.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	template <typename Service>
	ETInlineHint Service& ServiceBlackboard::Locate() const {
		return *static_cast<Service*>( Locate( typeid( Service ) ) );
	}

// ---------------------------------------------------

	template <typename Service>
	ETInlineHint ServiceBlackboard& ServiceBlackboard::Publish( Service& service ) {
		return Publish( typeid( Service ), eastl::addressof( service ) );
	}

// ---------------------------------------------------

	ETInlineHint ServiceBlackboard& ServiceBlackboard::operator=( const ServiceBlackboard& other ) {
		_services = other._services;
		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ServiceBlackboard& ServiceBlackboard::operator=( ServiceBlackboard&& other ) {
		_services = eastl::move( other._services );
		return *this;
	}

}	// namespace Core
}	// namespace Eldritch2