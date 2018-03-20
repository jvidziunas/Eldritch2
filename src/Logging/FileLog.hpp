/*==================================================================*\
  FileLog.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/Log.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Logging {

	class FileLog : public Logging::Log {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		FileLog( const FileLog& ) = delete;
	//!	Constructs this @ref FileLog instance.
		FileLog();

		~FileLog() = default;

	// ---------------------------------------------------

	public:
		using Log::Write;

		void	Write( const Utf8Char* const string, size_t lengthInOctets ) override sealed;

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( const PlatformChar* path );

		void		FreeResources();

	// ---------------------------------------------------

	//!	Disable assignment.
		FileLog&	operator=( const FileLog& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		FileAppender	_appender;
	};

}	// namespace Logging
}	// namespace Eldritch2