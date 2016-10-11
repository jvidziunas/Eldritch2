/*==================================================================*\
  AssetPackageLoaderThread.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/AssetPackageLoaderThread.hpp>
#include <Platform/ContentProvider.hpp>
#include <Assets/AssetPackage.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Scheduling;
using namespace ::Eldritch2::Platform;

namespace Eldritch2 {
namespace Assets {
namespace {

	static ETInlineHint Result<MemoryMappedFile> OpenPackageFile( ContentProvider& contentProvider, const AssetPackage& package ) {
		Result<MemoryMappedFile>	result( contentProvider.OpenMemoryMappedFile( WellKnownDirectory::Packages, package.GetName() ) );

		if( result.GetErrorCode() == Error::InvalidParameter ) {
			return contentProvider.OpenMemoryMappedFile( WellKnownDirectory::DownloadedPackages, package.GetName() );
		}

		return result;
	}

}	// anonymous namespace

	AssetPackageLoaderThread::AssetPackageLoaderThread( ContentProvider& contentProvider, Allocator& allocator ) : _contentProvider( contentProvider ),
																												   _wakeEvent( UserWaitableEvent::Signaled ),
																												   _uninitiatedLoads( { allocator, "Deserializer Thread Outstanding Load List Allocator" } ),
																												   _blockedLoads( { allocator, "Deserializer Thread Blocked Load List Allocator" } ),
																												   _terminationBehavior( TerminationBehavior::Continue ) {}

// ---------------------------------------------------

	void AssetPackageLoaderThread::RequestGracefulShutdown() {
		_terminationBehavior.store( TerminationBehavior::Terminate, std::memory_order_release );
		_wakeEvent.Signal();
	}

// ---------------------------------------------------

	Utf8Literal AssetPackageLoaderThread::GetHumanReadableName() const {
		return "Package Loader Thread";
	}

// ---------------------------------------------------

	void AssetPackageLoaderThread::BeginLoad( AssetPackage& package ) {
		{	ScopedWriteLock	_( _uninitiatedLoadMutex );
			_uninitiatedLoads.EmplaceBack( &package );
		}	// End of lock scope.

		_wakeEvent.Signal();
	}

// ---------------------------------------------------

	bool AssetPackageLoaderThread::CanLoadPackage( const Utf8Char* name ) const {
	//	TODO: It might be beneficial to enumerate all the packages in these directories up-front, and avoid hitting the file system sporadically.
		return _contentProvider.CheckFileExists( WellKnownDirectory::Packages, name ) || _contentProvider.CheckFileExists( WellKnownDirectory::DownloadedPackages, name );
	}

// ---------------------------------------------------

	void AssetPackageLoaderThread::Run() {
		while( TerminationBehavior::Continue == _terminationBehavior.load( std::memory_order_consume ) ) {
			const auto package( _uninitiatedLoads.Back() );

			if( !package ) {
			//	Block until there is additional work to be performed.
				_wakeEvent.AwaitSignal();
				continue;
			}

			auto	packageFileResult( OpenPackageFile( _contentProvider, *package ) );

			if( !packageFileResult ) {
				package->NotifyOfLoadFailure( *package );
			}
		}
	}

}	// namespace Assets
}	// namespace Eldritch2