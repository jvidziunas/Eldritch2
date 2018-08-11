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

	ETInlineHint Asset::Builder::Builder(Logging::Log& log, Range<const char*> bytes) :
		_log(log),
		_bytes(bytes) {}

	// ---------------------------------------------------

	ETInlineHint const char* Asset::Builder::Begin() const {
		return _bytes.Begin();
	}

	// ---------------------------------------------------

	ETInlineHint const char* Asset::Builder::End() const {
		return _bytes.End();
	}

	// ---------------------------------------------------

	ETInlineHint size_t Asset::Builder::GetSize() const {
		return _bytes.GetSize();
	}

	// ---------------------------------------------------

	template <typename... Arguments>
	ETInlineHint void Asset::Builder::WriteLog(Logging::MessageType severity, StringView<Utf8Char> format, Arguments&&... arguments) const {
		_log.Write(severity, format, eastl::forward<Arguments>(arguments)...);
	}

	// ---------------------------------------------------

	ETInlineHint Asset::Asset(StringView<Utf8Char> path) {
		CopyString(_path, path);
	}

	// ---------------------------------------------------

	ETInlineHint StringView<Utf8Char> Asset::GetPath() const {
		return _path;
	}

}} // namespace Eldritch2::Assets
