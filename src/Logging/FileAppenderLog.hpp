/*==================================================================*\
  FileAppenderLog.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Platform/SynchronousFileAppender.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Logging {

	class FileAppenderLog : public Logging::Log {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref FileAppenderLog instance.
		FileAppenderLog( Platform::SynchronousFileAppender&& appender );
	//!	Disable copying.
		FileAppenderLog( const FileAppenderLog& ) = delete;

	//!	Destroys this @ref FileAppenderLog instance.
		~FileAppenderLog();

	// ---------------------------------------------------

	public:
		void	Write( const Eldritch2::Utf8Char* const string, size_t lengthInOctets ) override sealed;

	// ---------------------------------------------------

	//!	Disable assignment.
		FileAppenderLog&	operator=( const FileAppenderLog& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Platform::SynchronousFileAppender	_appender;
	};

}	// namespace Logging
}	// namespace Eldritch2