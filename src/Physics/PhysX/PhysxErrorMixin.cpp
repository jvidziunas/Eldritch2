/*==================================================================*\
  PhysxErrorMixin.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxErrorMixin.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace Detail {

	using namespace ::Eldritch2::Logging;
	using namespace ::physx;

	void WriteLog(Log& log, PxErrorCode::Enum errorCode, const char* message, const char* file, int line) {
		if (errorCode & PxErrorCode::Enum::eDEBUG_WARNING) {
			log.Write(Severity::Warning, "PhysX debug warning {}({}): {}" ET_NEWLINE, file, line, message);
			return;
		}

		if (errorCode & PxErrorCode::Enum::ePERF_WARNING) {
			log.Write(Severity::Warning, "PhysX performance warning {}({}): {}" ET_NEWLINE, file, line, message);
			return;
		}

		log.Write(Severity::Error, "PhysX error {}({}): {}" ET_NEWLINE, file, line, message);
	}

}}}} // namespace Eldritch2::Physics::PhysX::Detail
