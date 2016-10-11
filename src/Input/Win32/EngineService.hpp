/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  Defines a Win32 application decorator object that invokes input
  system event objects when the user presses the corresponding key,
  moves a stick or shifts the mouse.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Input/Win32/RawInputSubscriber.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Core/EngineService.hpp>
#include <Scheduling/Thread.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Core {
		class	Engine;
	}
}

struct HWND__;
struct HHOOK__;

using HWND	= HWND__*;
using HHOOK	= HHOOK__*;

namespace Eldritch2 {
namespace Input {
namespace Win32 {

	class EngineService : public Core::EngineService {
	// - TYPE PUBLISHING ---------------------------------

	private:
		class RawInputPollingThread : public Scheduling::Thread {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref RawInputPollingThread instance.
			RawInputPollingThread( Eldritch2::Allocator& allocator );
		//!	Disable copying.
			RawInputPollingThread( const RawInputPollingThread& ) = delete;

			~RawInputPollingThread() = default;

		// ---------------------------------------------------
			
		public:
			Eldritch2::Utf8Literal	GetHumanReadableName() const override sealed;

		// ---------------------------------------------------

		public:
			void	RequestGracefulShutdown() override;

			void	Run() override sealed;

		// ---------------------------------------------------

		//!	Disable assignment.
			RawInputPollingThread&	operator=( const RawInputPollingThread& ) = delete;

		// - DATA MEMBERS ------------------------------------

		private:
			mutable Eldritch2::ChildAllocator	_allocator;
			std::atomic<HWND>					_window;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref EngineService instance.
		EngineService( const Core::Engine& owningEngine );
	//!	Disable copying.
		EngineService( const EngineService& ) = delete;

	//!	Destroys this @ref EngineService instance.
		~EngineService();

	// ---------------------------------------------------

	public:
		Eldritch2::Utf8Literal	GetName() const override;

	// ---------------------------------------------------

	protected:
		void	AcceptVisitor( Scheduling::JobFiber& executor, const BeginInitializationVisitor ) override;
		void	AcceptVisitor( Scripting::ApiRegistrar& registrar ) override;

	// ---------------------------------------------------

	//!	Disable assignment.
		EngineService&	operator=( const EngineService& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Logging::ChildLog	_log;
		const HHOOK					_keyboardHook;
		RawInputPollingThread		_pollingThread;
	};

}	// namespace Win32
}	// namespace Input
}	// namespace Eldritch2