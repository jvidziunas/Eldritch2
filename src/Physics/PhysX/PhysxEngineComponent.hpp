/*==================================================================*\
  PhysxEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxAllocatorMixin.hpp>
#include <Physics/PhysX/PhysxErrorMixin.hpp>
#include <Core/EngineComponent.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class PhysxEngineComponent : public Core::EngineComponent {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PhysxEngineComponent instance.
		PhysxEngineComponent(const ObjectLocator& services, Logging::Log& log);
		//!	Disable copy construction.
		PhysxEngineComponent(const PhysxEngineComponent&) = delete;

		~PhysxEngineComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		UniquePointer<Core::WorldComponent> CreateWorldComponent(Allocator& allocator, const ObjectLocator& services) override;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void BindResourcesEarly(Scheduling::JobExecutor& executor) override;

		void PublishAssetTypes(Assets::AssetApiBuilder& factories) override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		PhysxEngineComponent& operator=(const PhysxEngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable PhysxErrorMixin<Logging::ChildLog>   _log;
		mutable PhysxAllocatorMixin<MallocAllocator> _allocator;
	};

}}} // namespace Eldritch2::Physics::PhysX
