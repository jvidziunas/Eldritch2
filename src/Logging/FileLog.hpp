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
		FileLog() ETNoexceptHint;

		~FileLog() = default;

		// ---------------------------------------------------

	public:
		void Write(const Utf8Char* string, size_t lengthInOctets) ETNoexceptHint override sealed;
		using Log::Write;

		// ---------------------------------------------------

	public:
		Result BindResources(PlatformStringSpan path, StringSpan header = "");

		void FreeResources(StringSpan footer = "") ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable assignment.
		FileLog& operator=(const FileLog&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		FileAppender _sink;

		// ---------------------------------------------------

		friend void Swap(FileLog&, FileLog&);
	};

}} // namespace Eldritch2::Logging

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Logging/FileLog.inl>
//------------------------------------------------------------------//
