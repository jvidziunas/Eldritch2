/*==================================================================*\
  AssetPackage.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/AssetPackage.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	AssetPackage::AssetPackage( const Utf8Char* const name, Allocator& allocator ) : _name( name, { allocator, "Package Name Allocator" } ),
																					 _residencyState( ResidencyState::Loading ),
																					 _dependencies( { allocator, "Package Dependency Collection Allocator" } ),
																					 _assetViews( { allocator, "Package Owned Asset View Collection Allocator" } ) {}

// ---------------------------------------------------

	void AssetPackage::NotifyOfLoadFailure( const AssetPackage& failedLoad ) {
		if( !ReferencesPackage( failedLoad ) ) {
			return;
		}

		_residencyState = ResidencyState::Failed;
	}

// ---------------------------------------------------

	bool AssetPackage::IsDeserializable() const {
		for( const auto& importReference : _dependencies ) {
			if( ResidencyState::Published != importReference->GetResidencyState() ) {
				return false;
			}
		}

		return true;
	}

}	// namespace Assets
}	// namespace Eldritch2