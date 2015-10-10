/*==================================================================*\
  NullLogger.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Logging/Logger.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;

// ---------------------------------------------------

	class NullLogger : public Foundation::Logger {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this NullLogger instance.
		NullLogger();

		// Destroys this NullLogger instance.
		~NullLogger();

	// ---------------------------------------------------

		void	WriteString( const ::Eldritch2::UTF8Char* const string, const size_t length ) override sealed;
	};

}	// namespace Foundation
}	// namespace Eldritch2