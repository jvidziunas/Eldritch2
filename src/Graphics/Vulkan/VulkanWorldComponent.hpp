/*==================================================================*\
  VulkanWorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			class	VulkanGraphicsScene;
			class	OutputCoordinator;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class VulkanWorldComponent : public Core::WorldComponent {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		VulkanWorldComponent( const VulkanWorldComponent& ) = delete;
	//!	Constructs this @ref VulkanWorldComponent instance.
		VulkanWorldComponent( const Core::World& owner );

		~VulkanWorldComponent() = default;

	// ---------------------------------------------------

	public:
		void	AcceptVisitor( Scheduling::JobExecutor& executor, const LateInitializationVisitor ) override;
		void	AcceptVisitor( Scheduling::JobExecutor& executor, const VariableTickVisitor& ) override;
		void	AcceptVisitor( Scheduling::JobExecutor& executor, const TearDownVisitor ) override;
		void	AcceptVisitor( Scripting::Wren::ApiBuilder& api ) override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		VulkanWorldComponent&	operator=( const VulkanWorldComponent& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		VulkanGraphicsScene*	_scene;
		OutputCoordinator*		_outputCoordinator;
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2