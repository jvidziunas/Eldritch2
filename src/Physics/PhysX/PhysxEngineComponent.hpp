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
#include <Physics/PhysX/CpuDispatcher.hpp>
#include <Core/EngineComponent.hpp>
#include <Core/WorldComponent.hpp>
#include <Logging/ChildLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX {
	class PhysicsScene;
}}} // namespace Eldritch2::Physics::PhysX

namespace Eldritch2 { namespace Physics { namespace PhysX {

	class PhysxEngineComponent : public Core::EngineComponent {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class WorldComponent : public Core::WorldComponent, public CpuDispatcher {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref WorldComponent instance.
			WorldComponent(const ObjectInjector& services) ETNoexceptHint;
			//!	Disable copy construction.
			WorldComponent(const WorldComponent&) = delete;

			~WorldComponent() = default;

			// ---------------------------------------------------

		public:
			void OnFixedRateTickEarly(Scheduling::JobExecutor& executor, MicrosecondTime duration) ETNoexceptHint override;

			void OnFixedRateTickLate(Scheduling::JobExecutor& executor, MicrosecondTime duration) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void PublishApi(Scripting::Wren::ApiBuilder& api) override;

			using Core::WorldComponent::PublishApi;

			// ---------------------------------------------------

			//!	Disable copy assignment.
			WorldComponent& operator=(const WorldComponent&) = delete;

			// - DATA MEMBERS ------------------------------------

		private:
			PhysicsScene* _scene;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PhysxEngineComponent instance.
		PhysxEngineComponent(const ObjectInjector& services) ETNoexceptHint;
		//!	Disable copy construction.
		PhysxEngineComponent(const PhysxEngineComponent&) = delete;

		~PhysxEngineComponent() = default;

		// - ENGINE SERVICE SANDBOX METHODS ------------------

	public:
		void BindResourcesEarly(Scheduling::JobExecutor& executor) ETNoexceptHint override;

		void PublishApi(Core::AssetApiBuilder& api) override;

		using Core::EngineComponent::PublishApi;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		PhysxEngineComponent& operator=(const PhysxEngineComponent&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		PhysxErrorMixin<Logging::ChildLog>   _log;
		PhysxAllocatorMixin<MallocAllocator> _allocator;
	};

}}} // namespace Eldritch2::Physics::PhysX
