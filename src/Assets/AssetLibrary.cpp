/*==================================================================*\
  AssetLibrary.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/ContentProvider.hpp>
#include <Assets/AssetLibrary.hpp>
#include <Assets/LiveSet.hpp>
#include <Utility/Result.hpp>
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Platform;
using namespace ::Eldritch2::Logging;

namespace Eldritch2 {
namespace Assets {
namespace {

	static ETInlineHint SynchronousFileAppender CreateLogAppender( ContentProvider& contentProvider, const Utf8Char* const logFileName ) {
		Result<SynchronousFileAppender>	createBackingFileResult( contentProvider.CreateSynchronousFileAppender( WellKnownDirectory::Logs, logFileName ) );

		ETRuntimeVerification( createBackingFileResult );

		return eastl::move( *createBackingFileResult );
	}

}	// anonymous namespace

	ETPureFunctionHint size_t AssetLibrary::AssetViewHash::operator()( Pair<const Utf8Char* const, TypeHandle> assetProperties ) const {
		return GetHashCode( assetProperties.second, HashMemory( assetProperties.first, StringLength( assetProperties.first ) ) );
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t AssetLibrary::AssetViewHash::operator()( const UniquePointer<const AssetView>& assetView ) const {
		return operator()( assetView.Get() );
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t AssetLibrary::AssetViewHash::operator()( const AssetView* assetView ) const {
		return operator()( Pair<const Utf8Char* const, TypeHandle>( assetView->GetName(), typeid(assetView) ) );
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t AssetLibrary::AssetViewEquals::operator()( const AssetView* assetView, Eldritch2::Pair<const Utf8Char* const, TypeHandle> assetProperties ) const {
		return (assetView->GetName() == assetProperties.first) && (assetProperties.second == typeid(*assetView));
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t AssetLibrary::AssetViewEquals::operator()( const AssetView* lhs, const AssetView* rhs ) const {
		return (lhs == rhs) || ((typeid(*lhs) == typeid(*rhs)) && (lhs->GetName() == rhs->GetName()));
	}

// ---------------------------------------------------

	AssetLibrary::AssetLibrary( ContentProvider& contentProvider, UniquePointer<AssetPackageLoader> loader, Allocator& allocator ) : _allocator( allocator, "Asset Library Allocator" ),
																																	 _log( eastl::move( CreateLogAppender( contentProvider, "ContentLog.txt" ) ) ),
																																	 _packages( { _allocator, "Content Library Package Bucket Allocator" } ),
																																	 _assetViews( { _allocator, "Content Library Asset View Bucket Allocator" } ),
																																	 _factories( { _allocator, "Content Library Asset View Factory Bucket Allocator" } ),
																																	 _loader( eastl::move( loader ) ) {}

// ---------------------------------------------------

	Result<const AssetPackage*> AssetLibrary::ResolvePackage( const Utf8Char* const name ) {
		ScopedWriteLock	_( _packagesMutex );
	//	Try to locate a package with the same name in the set of previously-found elements.
		auto			candidate( _packages.Find( name, Hash<decltype( name )>(), [] ( const AssetPackage& package, const Utf8Char* const name ) { return package.GetName() == name; } ) );

	//	Allocate a new package object if the package is not in the current live set.
		if( candidate == _packages.End() ) {
			if( !_loader->CanLoadPackage( name ) ) {
				return Error::InvalidParameter;
			}

			candidate = _packages.Emplace( name, _allocator ).first;

		/*	Use of const_cast is gross, but this is one of the suggested solutions in C++ Defect Report 103. We solemnly swear not to modify the package name.
		 *	See http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
			_loader->BeginLoad( const_cast<AssetPackage&>(*candidate) );
		}

		return eastl::addressof( *candidate );
	}

// ---------------------------------------------------

	const AssetView* AssetLibrary::ResolveView( const Utf8Char* const assetName, TypeHandle type ) const {
		ScopedReadLock	_( _assetViewsMutex );

		const auto	candidate( _assetViews.Find( Pair<const Utf8Char* const, TypeHandle>( assetName, type ), _assetViews.GetHashPredicate(), _assetViews.GetEqualityPredicate() ) );
		if( candidate == _assetViews.End() ) {
			return nullptr;
		}

		return *candidate;
	}

// ---------------------------------------------------

	AssetLibrary::CollectionStatistics AssetLibrary::DestroyUnreferencedPackages( const LiveSet& livePackages ) {
	//	Acquire a write lock while we identify and destroy packages. This effectively makes garbage collection work in a 'stop-the-world' manner.
		ScopedWriteLock	_( _packagesMutex );
	//	Stash a bit of bookkeeping for use in the returned GC collection statistics.
		const auto		initialCount( _packages.GetSize() );

	//	Remove all packages not present in the live/working set.
	//	TODO: Parallelize.
		_packages.RemoveIf( [&livePackages, this] ( const AssetPackage& package ) {
		//	Retain packages that are still externally referenced.
			if( livePackages.Contains( package ) ) {
				return false;
			}

		//	Unregister all the asset views in the package.
			{	ScopedWriteLock	_( _assetViewsMutex );
				package.ForEachAssetView( [this] ( const AssetView& view ) {
					Unregister( view );
				} );
			}	// End of lock scope.

			return true;
		} );

	//	TODO: Rehash?

		return { initialCount - _packages.GetSize() };
	}

// ---------------------------------------------------

	void AssetLibrary::CreateViews( AssetPackage& package, const Utf8Char* const name, const Utf8Char* const extension, Range<const char*> rawBytes ) {
		for( const auto& viewFactory : _factories.EqualRange( Utf8Literal( extension ) ) ) {
			Result<UniquePointer<AssetView>>	createViewResult( viewFactory.second( _allocator, *this, name, rawBytes ) );

			if( !createViewResult ) {
				_log( MessageSeverity::Error, "Error creating asset view '{}.{}' : {}.", name, extension, GetErrorString( createViewResult ) );
				continue;
			}

		/*	Publish the view.
		 *	Note that since the handler may attempt to resolve another asset view as a part of its execution, we cannot hold a lock on the live asset view collection. */
			{	ScopedWriteLock	_( _assetViewsMutex );
			
				_assetViews.Insert( createViewResult->Get() );

			}	// End of lock scope.

		//	Do this outside the lock to decrease contention.
			package.TakeOwnership( eastl::move( *createViewResult ) );
		}
	}

// ---------------------------------------------------

	void AssetLibrary::Unregister( const AssetView& assetView ) {
		_assetViews.Erase( eastl::addressof( assetView ) );
	}

}	// namespace Assets
}	// namespace Eldritch2

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif