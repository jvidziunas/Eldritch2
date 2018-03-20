/*==================================================================*\
  PackageDatabase.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/PackageDatabase.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	ETPureFunctionHint bool PackageDatabase::PackageEqual::operator()( const Package& package, const Utf8Char* name ) const {
		return StringsEqual( package.GetPath(), name );
	}

// ---------------------------------------------------

	ETPureFunctionHint bool PackageDatabase::PackageEqual::operator()( const Package& package0, const Package& package1 ) const {
		return (*this)( package0, package1.GetPath() );
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t PackageDatabase::PackageHash::operator()( const Package& package, size_t seed ) const {
		return (*this)( package.GetPath(), seed );
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t PackageDatabase::PackageHash::operator()( const Utf8Char* name, size_t seed ) const {
		return HashMemory( name, StringLength( name ), seed );
	}

// ---------------------------------------------------

	PackageDatabase::PackageDatabase(
	) : _packages( MallocAllocator( "Asset Database Package Bucket Allocator" ) ),
		_gcCursor( _packages.Begin() ) {}

// ---------------------------------------------------

	CountedPointer<const Package> PackageDatabase::RequirePackage( const Utf8Char* const path ) const {
		ReadLock	_( _packagesMutex );

		const LoadableSet::Iterator candidate( _packages.Find( path, _packages.GetHashPredicate(), _packages.GetEqualityPredicate() ) );
		if (candidate == _packages.End()) {
			return nullptr;
		}

	//	Load the package if this is the first time world(s) have requested it since.
		if (candidate->AddReference() == 0u) {
		/*	Use of const_cast is gross, but this is one of the suggested solutions in C++ Defect Report 103. We solemnly swear not to modify
		 *	the package path. See http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
			_loadFunction( const_cast<Package&>(*candidate) );
		}

		return eastl::addressof( *candidate );
	}

// ---------------------------------------------------

	void PackageDatabase::SetPackages( LoadableSet packages, LoadFunction loadFunction ) {
		using ::Eldritch2::Swap;

		{	Lock	_( _packagesMutex );
			GcCursor gcCursor( packages.Begin() );

			Swap( _loadFunction,	loadFunction );
			Swap( _gcCursor,		gcCursor );
			Swap( _packages,		packages );
		}	// End of lock scope.

		//	The previous resident set will be destroyed at the end of scope, outside the lock.
	}

// ---------------------------------------------------

	void PackageDatabase::DestroyGarbage( size_t maxPackagesToDestroy, AssetDatabase& assetDatabase ) {
		ReadLock	_( _packagesMutex );
		GcCursor	cursor( _gcCursor ), end( _packages.End() );

		for (; maxPackagesToDestroy && cursor != end; ++cursor) {
			if (cursor->IsReferenced()) {
				continue;
			}

		/*	Use of const_cast is gross, but this is one of the suggested solutions in C++ Defect Report 103. We solemnly swear not to modify
		 *	the package path. See http://www.open-std.org/jtc1/sc22/wg21/docs/lwg-defects.html#103 */
			for (UniquePointer<Asset>& asset : const_cast<Package&>(*cursor).FreeAssets()) {
				assetDatabase.UnregisterAsset( *asset );
			}

			--maxPackagesToDestroy;
		}

	//	Wrap the cursor if we've reached the end of the set.
		_gcCursor = (cursor != end) ? cursor : _packages.Begin();
	}

}	// namespace Assets
}	// namespace Eldritch2