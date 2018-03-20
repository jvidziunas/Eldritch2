/*==================================================================*\
  XInputEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineComponent.hpp>
//------------------------------------------------------------------//
#if ET_PLATFORM_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#endif
#include <XInput.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Core {
		class	Engine;
	}
}

namespace Eldritch2 {
namespace Input {
namespace XInput {

	class XInputEngineComponent : public Core::EngineComponent {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		XInputEngineComponent( const XInputEngineComponent& ) = delete;
	//!	Constructs this @ref XInputEngineComponent instance.
		XInputEngineComponent( const Blackboard& services );

		~XInputEngineComponent() = default;

	// - ENGINE SERVICE SANDBOX METHODS ------------------

	protected:
		Result<UniquePointer<Core::WorldComponent>>	CreateWorldComponent( Allocator& allocator, const Core::World& world ) override;

		void										AcceptVisitor( Scheduling::JobExecutor& executor, const ServiceTickVisitor ) override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		XInputEngineComponent&	operator=( const XInputEngineComponent& ) = delete;

	// - DATA MEMBERS ------------------------------------
	
	private:
		XINPUT_STATE	_gamepads[XUSER_MAX_COUNT];
	};

}	// namespace XInput
}	// namespace Input
}	// namespace Eldritch2