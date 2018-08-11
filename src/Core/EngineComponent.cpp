/*==================================================================*\
  EngineComponent.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Core/EngineComponent.hpp>
#include <Core/WorldComponent.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	using namespace ::Eldritch2::Scheduling;
	using namespace ::Eldritch2::Scripting;
	using namespace ::Eldritch2::Assets;

	UniquePointer<WorldComponent> EngineComponent::CreateWorldComponent(Allocator& /*allocator*/, const ObjectLocator& /*services*/) {
		//	Default implementation should not do anything.
		return nullptr;
	}

	// ---------------------------------------------------

	void EngineComponent::TickEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/Tick", "Default early tick", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::Tick(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/Tick", "Default standard tick", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::BindResourcesEarly(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Default early-phase initialization", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::BindConfigurableResources(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Default configuration broadcast", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::BindResources(JobExecutor& /*executor*/) {
		ET_PROFILE_SCOPE("Engine/LateInitialization", "Default late-phase initialization", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::PublishConfiguration(PropertyRegistrar& /*properties*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Default property registration", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::PublishAssetTypes(AssetApiBuilder& /*factories*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Default asset factory registration", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

	// ---------------------------------------------------

	void EngineComponent::PublishServices(ObjectLocator& /*services*/) {
		ET_PROFILE_SCOPE("Engine/Initialization", "Default shared service registration", 0xBBBBBB);
		//	Default implementation should not do anything.
	}

}} // namespace Eldritch2::Core
