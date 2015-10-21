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

	void Logger::operator()( ETFormatStringHint( const UTF8Char* formatString ), ... ) {
		UTF8Char	submittedString[256u];
		va_list		args;

		va_start( args, formatString );
			PrintFormatted( submittedString, formatString, args );
		va_end( args );

		this->Write( submittedString, StringLength( submittedString ) );
	}

}	// namespace Foundation
}	// namespace Eldritch2