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

namespace Eldritch2 { namespace Logging {

	class FileLog : public Logging::Log {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		FileLog(const FileLog&) = delete;
		//!	Constructs this @ref FileLog instance.
		FileLog();

		~FileLog() = default;

		// ---------------------------------------------------

	public:
		void Write(const Utf8Char* string, size_t lengthInOctets) ETNoexceptHint override sealed;
		using Log::Write;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(PlatformStringView path);

		void FreeResources();

		// ---------------------------------------------------

		//!	Disable assignment.
		FileLog& operator=(const FileLog&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		FileAppender _appender;

		// ---------------------------------------------------

		friend void Swap(FileLog&, FileLog&);
	};

}} // namespace Eldritch2::Logging

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Logging/FileLog.inl>
//------------------------------------------------------------------//
