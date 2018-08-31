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

namespace Eldritch2 { namespace Assets {

	ETInlineHint ETForceInlineHint Asset::Builder::Builder(Logging::Log& log, Range<const char*> bytes) :
		_log(log),
		_bytes(bytes) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const char* Asset::Builder::Begin() const ETNoexceptHint {
		return _bytes.Begin();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const char* Asset::Builder::End() const ETNoexceptHint {
		return _bytes.End();
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint size_t Asset::Builder::GetSize() const ETNoexceptHint {
		return _bytes.GetSize();
	}

	// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint ETForceInlineHint void Asset::Builder::WriteLog(Logging::Severity severity, StringView format, Arguments&&... arguments) const {
		_log.Write(severity, format, eastl::forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Asset::Asset(StringView path) ETNoexceptHint {
		path.Copy(_path, ETCountOf(_path));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint StringView Asset::GetPath() const ETNoexceptHint {
		return { _path, StringLength(_path) };
	}

}} // namespace Eldritch2::Assets
