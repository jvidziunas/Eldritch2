/*==================================================================*\
  Asset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

namespace Eldritch2 {
namespace Assets {

	class ETPureAbstractHint Asset {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Builder {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Builder instance.
			Builder( Logging::Log& log, Range<const char*> assetData );
		//!	Constructs this @ref Builder instance.
			Builder( const Builder& ) = default;

			~Builder() = default;

		// ---------------------------------------------------

		public:
			template <size_t formatSize, typename... Arguments>
			void	WriteLog( Logging::MessageType severity, const Utf8Char (&formatString)[formatSize], Arguments&&... arguments ) const;
		
		// ---------------------------------------------------

		public:
			Range<const char*>	GetRawBytes() const;

		// - DATA MEMBERS ------------------------------------

		private:
			mutable Logging::ChildLog	_log;
			Range<const char*>			_bytes;
		};

	// ---

	public:
		enum : size_t {
			MaxPathLength = 128u
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref Asset instance.
	/*!	@param[in] path Null-terminated, UTF-8-encoded character sequence containing the file system path to the resource the
			@ref Asset describes. */
		Asset( const Utf8Char* const path );
	//!	Disable copy construction.
		Asset( const Asset& ) = delete;
	
		virtual ~Asset() = default;

	// ---------------------------------------------------

	public:
	//! Gets the file system filePath to the asset.
	/*! @returns Null-terminated, UTF-8-encoded character sequence containing the file system
			path to the resource this @ref Asset describes.
		@remarks Thread-safe. */
		const Utf8Char* const	GetPath() const;

	// ---------------------------------------------------

	public:
		virtual ErrorCode	BindResources( const Builder& builder ) abstract;

		virtual void		FreeResources() abstract;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		Asset&	operator=( const Asset& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Utf8Char	_path[MaxPathLength];
	};

}	// namespace Assets
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/Asset.inl>
//------------------------------------------------------------------//