/*==================================================================*\
  PlatformWindowSwapChain.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SwapChain.hpp>
#include <Platform/PlatformWindow.hpp>
#include <Core/ReferenceCountable.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	ApiRegistrar;
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace ScriptComponents {

	class PlatformWindowSwapChain : public Platform::PlatformWindow, public Vulkan::SwapChain, public Core::ReferenceCountable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref PlatformWindowSwapChain instance.
		PlatformWindowSwapChain( VkInstance instance );
	//!	Disable copying.
		PlatformWindowSwapChain( const PlatformWindowSwapChain& ) = delete;

		~PlatformWindowSwapChain() = default;

	// - SCRIPT API REFERENCE ----------------------------

	public:
	//! Registers all script-callable methods for the @ref PlatformWindowSwapChain type with the specified script type registrar.
		static void	RegisterScriptApi( Scripting::ApiRegistrar& registrar );

	// ---------------------------------------------------

	//!	Disable assignment.
		PlatformWindowSwapChain&	operator=( const PlatformWindowSwapChain& ) = delete;

	// ---------------------------------------------------

	protected:
		void	ConfigureForRebuild( Vulkan::SwapChainBuilder& swapChainBuilder, Vulkan::ImageViewBuilder& imageViewBuilder ) override;
	};

}	// namespace ScriptComponents
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2