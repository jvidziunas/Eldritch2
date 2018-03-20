/*==================================================================*\
  PhysxErrorMixin.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxErrorMixin.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Logging;
using namespace ::physx;

namespace Eldritch2 {
namespace Physics {
namespace PhysX {
namespace Detail {

	void WriteLog( Log& log, PxErrorCode::Enum errorCode, const char* message, const char* file, int line ) {
		if (errorCode & PxErrorCode::Enum::eDEBUG_WARNING) {
			log.Write( MessageType::Warning, "PhysX debug warning {}({}): {}" UTF8_NEWLINE, file, line, message );
			return;
		}

		if (errorCode & PxErrorCode::Enum::ePERF_WARNING) {
			log.Write( MessageType::Warning, "PhysX performance warning {}({}): {}" UTF8_NEWLINE, file, line, message );
			return;
		}

		log.Write( MessageType::Error, "PhysX error {}({}): {}" UTF8_NEWLINE, file, line, message );
	}

}	// namespace Detail
}	// namespace PhysX
}	// namespace Physics
}	// namespace Eldritch2