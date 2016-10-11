/*==================================================================*\
  ServiceBlackboard.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/ServiceBlackboard.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Platform;

namespace Eldritch2 {
namespace Core {

	ServiceBlackboard::ServiceBlackboard( const ServiceBlackboard& topLevelLocator, Allocator& allocator ) : _services( { allocator, "Service Locator Bucket Allocator" } ) {
		for( const auto& service : topLevelLocator._services ) {
			_services.Insert( service );
		}
	}

// ---------------------------------------------------

	ServiceBlackboard::ServiceBlackboard( Allocator& allocator ) : _services( { allocator, "Service Locator Bucket Allocator" } ) {}

// ---------------------------------------------------

	ServiceBlackboard& ServiceBlackboard::Publish( TypeHandle servicetype, void* service ) {
		ScopedWriteLock	_( _mutex );

		const auto	insertResult( _services.Insert( { servicetype, service } ) );

	//	If this is triggering, you're trying to register the same service type twice.
		ETRuntimeAssert( insertResult.second );

		return *this;
	}

// ---------------------------------------------------

	void* ServiceBlackboard::Locate( TypeHandle serviceType ) const {
		ScopedReadLock	_( _mutex );

		const auto	candidate( _services.Find( serviceType ) );

	//	If this is triggering, you're trying to retrieve a service type that hasn't been registered (yet).
		ETRuntimeAssert( candidate != _services.End() );

		return candidate->second;
	}

}	// namespace Core
}	// namespace Eldritch2