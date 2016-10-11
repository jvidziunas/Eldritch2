/*==================================================================*\
  LiveSet.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	ETInlineHint LiveSet::LiveSet( Eldritch2::Allocator& allocator ) : _packages( { allocator, "Live Package Set Allocator" } ) {}

// ---------------------------------------------------

	ETInlineHint LiveSet::LiveSet( LiveSet&& other ) : _packages( eastl::move( other._packages ) ) {}

// ---------------------------------------------------

	ETInlineHint bool LiveSet::Contains( const Assets::AssetPackage& package ) const {
		return _packages.Contains( &package );
	}

}	// namespace Assets
}	// namespace Eldritch2