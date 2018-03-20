/*==================================================================*\
  Package.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Package.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	Package::Package(
		const Utf8Char* const path
	) : _referenceCount( 0u ),
		_assets( MallocAllocator( "Package Asset List Allocator" ) ) {
		CopyString( _path, path );
	}

// ---------------------------------------------------

	Package::~Package() {
		ET_ASSERT( _assets.IsEmpty(), "Package contains assets! Call FreeAssets() before the package is destroyed!" );
	}

// ---------------------------------------------------

	void Package::BindAssets( AssetList assets ) {
		ET_ASSERT( _assets.IsEmpty(), "Package contains assets! Call FreeAssets() before binding a new asset list!" );

		Swap( _assets, assets );
	}

// ---------------------------------------------------

	Package::AssetList Package::FreeAssets() {
		return eastl::exchange( _assets, AssetList( _assets.GetAllocator() ) );
	}

}	// namespace Assets
}	// namespace Eldritch2