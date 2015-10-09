/*==================================================================*\
  Direct3D11SwapChain.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/ReferenceTypeBase.hpp>
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

	namespace Renderer {
		class	Direct3D11Renderer;
	}
}

struct	ID3D11RenderTargetView;
struct	IDXGISwapChain;

namespace Eldritch2 {
namespace Renderer {

	namespace Scheduler = ::Eldritch2::Scheduler;
	namespace Scripting = ::Eldritch2::Scripting;
	namespace Renderer	= ::Eldritch2::Renderer;
	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Direct3D11SwapChain : public Scripting::ReferenceTypeBase, public Foundation::Window, public Scheduler::Thread {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this Direct3D11SwapChain instance.
		Direct3D11SwapChain( const Window::PresentationParameters& presentationParameters );

		// Destroys this Direct3D11SwapChain instance.
		~Direct3D11SwapChain();

	// ---------------------------------------------------

		// Retrieves a pointer to a null-terminated character array containing the UTF8-encoded name of this thread.
		const ::Eldritch2::UTF8Char* const	GetHumanReadableName() const override sealed;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	Run() override sealed;

		void					RequestGracefulShutdown() override sealed;

	// - SCRIPT API REFERENCE ----------------------------

		// Registers all script-callable methods for the Direct3D11SwapChain type with the specified script type registrar.
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

}	// namespace Renderer
}	// namespace Eldritch2