/*==================================================================*\
  AssetLibrary.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/TypeHandle.hpp>
//------------------------------------------------------------------//
#include <EASTL/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	template <typename View>
	ETInlineHint const View* AssetLibrary::ResolveView( const Eldritch2::Utf8Char* const name ) const {
		static_assert( eastl::is_base_of<Assets::AssetView, View>::value, "ResolveView() must be used to convert to AssetView-derived types!" );

	// ---

		return static_cast<const View*>( ResolveView( name, typeid(View) ) );
	}

// ---------------------------------------------------

	ETInlineHint Logging::Log& AssetLibrary::GetLog() const {
		return _log;
	}

}	// namespace Assets
}	// namespace Eldritch2