/*==================================================================*\
  RecastEngineComponent.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineComponent.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Navigation { namespace Recast {
	class NavigationScene;
}}} // namespace Eldritch2::Navigation::Recast

namespace Eldritch2 { namespace Navigation { namespace Recast {

	class RecastEngineComponent : public Core::EngineComponent {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class WorldComponent : public Core::WorldComponent {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref WorldComponent instance.
			WorldComponent(const ObjectInjector& services) ETNoexceptHint;
			//!	Disable copy construction.
			WorldComponent(const WorldComponent&) = delete;

			~WorldComponent() = default;

			// ---------------------------------------------------

		public:
			void OnFixedRateTickLate(Scheduling::JobExecutor& executor, MicrosecondTime duration) ETNoexceptHint override;

			// ---------------------------------------------------

		public:
			void BindResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;

			void FreeResources(Scheduling::JobExecutor& executor) ETNoexceptHint override;
			
			// ---------------------------------------------------

		public:
			void PublishApi(Scripting::Wren::ApiBuilder&) override;

			using Core::WorldComponent::PublishApi;

			// - DATA MEMBERS ------------------------------------

		private:
			NavigationScene* _scene;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RecastEngineComponent instance.
		RecastEngineComponent(const ObjectInjector& services) ETNoexceptHint;
		//!	Disable copy construction.
		RecastEngineComponent(const RecastEngineComponent&) = delete;

		~RecastEngineComponent() = default;

		// ---------------------------------------------------

	public:
		void PublishApi(Core::PropertyApiBuilder& api) override;

		void PublishApi(Core::AssetApiBuilder& api) override;

		using Core::EngineComponent::PublishApi;
	};

}}} // namespace Eldritch2::Navigation::Recast
