/*==================================================================*\
  SteamworksEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Networking/Steamworks/SteamTools.hpp>
#include <Core/EngineComponent.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6340) Valve has a few mismatches in their printf specifiers, it seems! We can't fix these, so disable the warning.
ET_SET_MSVC_WARNING_STATE(disable : 6340)
#include <steamclientpublic.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Networking { namespace Steamworks {

	class SteamworksEngineComponent : public Core::EngineComponent {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using Port = uint16;

	public:
		enum : Port {
			DefaultWorldPortBegin = 6670u,
			DefaultWorldPortEnd   = 6689u,
			DefaultSteamPort      = 6690u
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref SteamworksEngineComponent instance.
		SteamworksEngineComponent(const Blackboard& services, Logging::Log& log);
		//!	Disable copy construction.
		SteamworksEngineComponent(const SteamworksEngineComponent&) = delete;

		~SteamworksEngineComponent();

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		Result<UniquePointer<Core::WorldComponent>> CreateWorldComponent(Allocator& allocator, const Core::World& world) override;

		void AcceptVisitor(Scheduling::JobExecutor& executor, const ConfigurationBroadcastVisitor) override;
		void AcceptVisitor(Scheduling::JobExecutor& executor, const ServiceTickVisitor) override;
		void AcceptVisitor(Core::PropertyRegistrar& properties) override;

		void AddLocalUser();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		SteamworksEngineComponent& operator=(const SteamworksEngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		//!	Mutable so logs can be written in const methods.
		mutable Logging::ChildLog _log;
		//!	Port used to communicate user state to the Steam master servers.
		Port _steamPort;
		//!	Pool of ports that may be assigned to worlds for communication with Steam servers.
		IdentifierPool<Port> _worldPorts;
		HashSet<CSteamID>    _bannedIds;
	};

}}} // namespace Eldritch2::Networking::Steamworks
