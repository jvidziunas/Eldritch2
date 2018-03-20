/*==================================================================*\
  Blackboard.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Blackboard.hpp>
#include <Common/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	Blackboard::Blackboard() : _servicesByType( MallocAllocator( "Blackboard Bucket Allocator" ) ) {}

// ---------------------------------------------------

	Blackboard& Blackboard::Publish( Type servicetype, void* service ) {
		const auto	insertResult( _servicesByType.Insert( { servicetype, service } ) );

		ET_ASSERT( insertResult.second, "Duplicate service registration!" );

		return *this;
	}

// ---------------------------------------------------

	void* Blackboard::Find( Type serviceType ) const {
		const auto	candidate( _servicesByType.Find( serviceType ) );

		ET_ASSERT( candidate != _servicesByType.End(), "Unknown service!" );

		return candidate->second;
	}

}	// namespace Eldritch2