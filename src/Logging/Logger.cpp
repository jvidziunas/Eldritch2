/*==================================================================*\
  Logger.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Logging/Logger.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Foundation;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Foundation {

	void Logger::WriteString( const UTF8Char* string ) {
		this->WriteString( string, StringLength( string ) );
	}

}	// namespace Foundation
}	// namespace Eldritch2