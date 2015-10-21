/*==================================================================*\
  AssertingNullLogger.hpp
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

	class AssertingNullLogger : public Foundation::Logger {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this @ref AssertingNullLogger instance.
		AssertingNullLogger() = default;

		// Destroys this @ref AssertingNullLogger instance.
		~AssertingNullLogger() = default;

	// ---------------------------------------------------

		void	Write( const ::Eldritch2::UTF8Char* const string, const size_t length ) override sealed;
	};

}	// namespace Foundation
}	// namespace Eldritch2