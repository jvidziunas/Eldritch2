/*==================================================================*\
  AssetViewFactoryRegistrar.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/AssetViewFactoryRegistrar.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	AssetViewFactoryRegistrar::AssetViewFactoryRegistrar( AssetLibrary& contentLibrary ) : _assetLibrary( contentLibrary ) {}

// ---------------------------------------------------

	AssetViewFactoryRegistrar& AssetViewFactoryRegistrar::Publish( Utf8Literal extension, AssetLibrary::AssetViewFactory factory ) {
		_assetLibrary._factories.Emplace( extension, eastl::move( factory ) );

		return *this;
	}

}	// namespace Assets
}	// namespace Eldritch2
