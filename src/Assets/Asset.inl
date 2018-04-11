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

namespace Eldritch2 {
namespace Assets {

	ETInlineHint Asset::Builder::Builder( Logging::Log& log, const char* begin, const char* end ) : _log( log ), _begin( begin ), _end( end ) {}

// ---------------------------------------------------

	ETInlineHint const char* Asset::Builder::Begin() const {
		return _begin;
	}

// ---------------------------------------------------

	ETInlineHint const char* Asset::Builder::End() const {
		return _end;
	}

// ---------------------------------------------------

	ETInlineHint size_t Asset::Builder::GetSize() const {
		return _end - _begin;
	}

// ---------------------------------------------------

	template <size_t formatSize, typename... Arguments>
	ETInlineHint void Asset::Builder::WriteLog( Logging::MessageType severity, const Utf8Char (&formatString)[formatSize], Arguments&&... arguments ) const {
		_log.Write( severity, formatString, eastl::forward<Arguments>( arguments )... );
	}

// ---------------------------------------------------

	ETInlineHint Asset::Asset( const Utf8Char* const filePath ) {
		CopyString( _path, filePath );
	}

// ---------------------------------------------------

	ETInlineHint const Utf8Char* const Asset::GetPath() const {
		return eastl::begin( _path );
	}

}	// namespace Assets
}	// namespace Eldritch2