/*==================================================================*\
  DialogueSet.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	ETInlineHint DialogueSet::DialogueSet(
		const AssetViews::DialogueSetAsset& asset
	) : RuleSet<DialogueResponse>( AllocatorType( "Wren Dialogue Set Allocator" ) ),
		_asset( asset ) {}

// ---------------------------------------------------

	template <class Allocator>
	ETInlineHint DialogueSet::MatchList<Allocator> DialogueSet::MatchAll( const Allocator& allocator, WrenHandle* facts ) const {}

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2