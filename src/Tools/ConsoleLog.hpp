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
		ConsoleLog(const ConsoleLog&) ETNoexceptHint = default;
		//!	Constructs this @ref ConsoleLog instance.
		ConsoleLog() ETNoexceptHint = default;

		~ConsoleLog() = default;

		// ---------------------------------------------------

	public:
		void Write(const Utf8Char* const string, size_t lengthInOctets) override;
	};

}} // namespace Eldritch2::Logging
