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

	ETInlineHint Range<const char*> Asset::Builder::GetRawBytes() const {
		return _bytes;
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