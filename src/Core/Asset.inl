/*==================================================================*\
  Asset.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	ETConstexpr ETForceInlineHint AssetBuilder::AssetBuilder(StringSpan package, Span<const byte*> bytes) ETNoexceptHint : package(package), bytes(bytes) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Asset::Asset(StringSpan path) ETNoexceptHint {
		path.Copy(_path, ETCountOf(_path));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint StringSpan Asset::GetPath() const ETNoexceptHint {
		return StringSpan(_path, StringSpan::SizeType(StringLength(_path)));
	}

}} // namespace Eldritch2::Assets
