/*==================================================================*\
  LiveSet.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/AssetPackage.hpp>
#include <Assets/LiveSet.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	void LiveSet::AddPackageAndAllDependencies( const AssetPackage& package ) {
		if( !_packages.Insert( &package ).second ) {
			return;
		}

		package.ForEachDependency( [&] ( const AssetPackage& dependency ) {
			AddPackageAndAllDependencies( dependency );
		} );
	}

}	// namespace Assets
}	// namespace Eldritch2