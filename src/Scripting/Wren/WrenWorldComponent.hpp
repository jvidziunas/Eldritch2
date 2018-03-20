/*==================================================================*\
  WrenWorldComponent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Wren/Context.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		namespace Wren {
			class	EventCoordinator;
			class	Game;
		}
	}
}

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class WrenWorldComponent : public Core::WorldComponent {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t {
			WrenHeapSizeInBytes = 2u * 1024u * 1024u	// 2MB
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		WrenWorldComponent( const WrenWorldComponent& ) = delete;
	//!	Constructs this @ref WrenWorldComponent instance.
		WrenWorldComponent( const Core::World& owner );

		~WrenWorldComponent() = default;

	// - WORLD COMPONENT SANDBOX METHODS -----------------

	public:
		void	AcceptVisitor( Scheduling::JobExecutor& executor, const EarlyInitializationVisitor ) override;
		void	AcceptVisitor( Scheduling::JobExecutor& executor, const LateInitializationVisitor ) override;
		void	AcceptVisitor( Scheduling::JobExecutor& executor, const StandardTickVisitor& ) override;
		void	AcceptVisitor( Scheduling::JobExecutor& executor, const TearDownVisitor ) override;
		void	AcceptVisitor( Scripting::Wren::ApiBuilder& api ) override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		WrenWorldComponent&	operator=( const WrenWorldComponent& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	//!	Container for the Wren virtual machine hosted by the world.
		Context				_wren;
	//!	Central dispatch point for in-world events.
		EventCoordinator*	_eventCoordinator;
		Game*				_game;
	};

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2