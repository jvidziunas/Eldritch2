/*==================================================================*\
  ConsoleLog.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/Log.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Logging {

	class ConsoleLog : public Log {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Disable copy construction.
		ConsoleLog(const ConsoleLog&) = delete;
		//!	Constructs this @ref ConsoleLog instance.
		ConsoleLog();

		~ConsoleLog();

		// ---------------------------------------------------

	public:
		void Write(const Utf8Char* const string, size_t lengthInOctets) override;
	};

}} // namespace Eldritch2::Logging
