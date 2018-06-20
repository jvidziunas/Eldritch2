/*==================================================================*\
  PhysxErrorMixin.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Logging {
	class Log;
}} // namespace Eldritch2::Logging

namespace Eldritch2 { namespace Physics { namespace PhysX {
	namespace Detail {

		void WriteLog(Logging::Log& log, physx::PxErrorCode::Enum errorCode, const Utf8Char* message, const Utf8Char* file, int line);

	} // namespace Detail

	template <typename BackingLog>
	template <typename... ConstructorArguments>
	ETInlineHint PhysxErrorMixin<BackingLog>::PhysxErrorMixin(
		ConstructorArguments&&... constructorArguments) :
		BackingLog(eastl::forward<ConstructorArguments>(constructorArguments)...) {}

	// ---------------------------------------------------

	template <typename BackingLog>
	ETInlineHint void PhysxErrorMixin<BackingLog>::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) {
		return Detail::WriteLog(*this, code, message, file, line);
	}

}}} // namespace Eldritch2::Physics::PhysX
