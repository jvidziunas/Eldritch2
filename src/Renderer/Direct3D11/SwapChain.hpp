/*==================================================================*\
  SwapChain.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ReferenceCountable.hpp>
#include <Utility/COMPointer.hpp>
#include <Foundation/Window.hpp>
#include <Scheduler/Thread.hpp>
//------------------------------------------------------------------//
#include <Windows.h>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ScriptAPIRegistrationInitializationVisitor;
	}
}

struct	ID3D11RenderTargetView;
struct	IDXGISwapChain;

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class SwapChain : public Scripting::ReferenceCountable, public Foundation::Window, public Scheduler::Thread {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref SwapChain instance.
		SwapChain( const Window::PresentationParameters& presentationParameters );

		//! Destroys this @ref SwapChain instance.
		~SwapChain();

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const override sealed;

	// ---------------------------------------------------

		void	Run() override sealed;

		void	RequestGracefulShutdown() override sealed;

	// - SCRIPT API REFERENCE ----------------------------

		//!	Registers all script-callable methods for the @ref SwapChain type with the specified script type registrar.
		static void	ExposeScriptAPI( Scripting::ScriptAPIRegistrationInitializationVisitor& typeRegistrar );

	// ---------------------------------------------------

		void	Dispose() override sealed;

	// ---------------------------------------------------

		Window::PresentationParameters	GetPresentationParameters() const override sealed;

	// ---------------------------------------------------

		void	SetCaption( const ::Eldritch2::UTF8Char* const captionString ) override sealed;

	// ---------------------------------------------------

		Utility::COMPointer<::ID3D11RenderTargetView>	GetRenderTargetViewAndIncrementPresentCounter();

		void											DecrementPresentCounter();

	// ---------------------------------------------------

		static ::ATOM	RegisterWindowClass();

		::LRESULT		MessageProc( ::UINT messageID, ::WPARAM wParam, ::LPARAM lParam );

	// - DATA MEMBERS ------------------------------------

		static const char* const				scriptTypeName;

	private:
		Window::PresentationParameters			_presentationParameters;
		::std::atomic<::HWND>					_windowHandle;
		::Eldritch2::uint32						_displayedFrameCounter;
		Utility::COMPointer<::IDXGISwapChain>	_swapChain;
		::std::atomic<size_t>					_drawingCounter;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2