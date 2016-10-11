/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineService.hpp>
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

	class EngineService : public Core::EngineService {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Controller {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : DWORD {
				InvalidControllerIndex = static_cast<DWORD>(-1)
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Controller instance.
			Controller( DWORD index = Controller::InvalidControllerIndex );
		//!	Constructs this @ref Controller instance.
			Controller( const Controller& ) = default;

			~Controller() = default;

		// ---------------------------------------------------

		public:
			void	SampleInput();

		// ---------------------------------------------------

		public:
			Controller&	operator=( const Controller& ) = default;

		// - DATA MEMBERS ------------------------------------

		private:
			DWORD			_controllerIndex;
			XINPUT_STATE	_state;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref EngineService instance.
		EngineService( const Core::Engine& engine );
	//!	Disable copying.
		EngineService( const EngineService& ) = delete;

		~EngineService() = default;

	// ---------------------------------------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// ---------------------------------------------------

	protected:
		void	AcceptVisitor( Scripting::ApiRegistrar& typeRegistrar ) override;
		void	AcceptVisitor( Scheduling::JobFiber& executor, const ServiceTickVisitor ) override;

	// ---------------------------------------------------

	//!	Disable assignment.
		EngineService&	operator=( const EngineService& ) = delete;

	// - DATA MEMBERS ------------------------------------
	
	private:
		Controller	_controllers[XUSER_MAX_COUNT];
	};

}	// namespace XInput
}	// namespace Input
}	// namespace Eldritch2