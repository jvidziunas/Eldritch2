/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  Central context object used to centralize all simulation, effects
  processing and resource management associated with drawing.

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurablePODVariable.hpp>
#include <Configuration/ConfigurableUTF8String.hpp>
#include <Renderer/Direct3D11/SwapChain.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#include <D3D11.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		namespace Direct3D11 {
			class	MeshResourceView;
		}
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Direct3D11 {

	class EngineService : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Initializes this @ref EngineService instance.
		EngineService( Foundation::GameEngine& owningEngine );

		//! Destroys this @ref EngineService instance.
		~EngineService() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

	protected:
		void	AcceptInitializationVisitor( FileSystem::ResourceViewFactoryPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Foundation::WorldViewFactoryPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor ) override sealed;

	// ---------------------------------------------------

		void	AcceptTaskVisitor( ::Eldritch2::Allocator& subtaskAllocator, Scheduler::Task& visitingTask, Scheduler::WorkerContext& executingContext, const PostConfigurationLoadedTaskVisitor ) override sealed;

	// ---------------------------------------------------

		void	InitializeDirect3D();

	// - DATA MEMBERS ------------------------------------

	private:
		Utility::COMPointer<::ID3D11Device>							_device;

		Configuration::ConfigurableUTF8String						_preferredAdapterName;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_VSyncMode;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_maximumFramesToRenderAhead;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_MSAACount;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_MSAAQuality;
		Configuration::ConfigurablePODVariable<float>				_adaptiveResolutionMaxAreaFraction;
		Configuration::ConfigurablePODVariable<float>				_adaptiveResolutionMinAreaFraction;
		Configuration::ConfigurablePODVariable<bool>				_forceDebugRuntime;
		Configuration::ConfigurablePODVariable<bool>				_allowDriverThreadingOptimizations;

		const Direct3D11::MeshResourceView*							_defaultMeshView;
		Scripting::ObjectHandle<Direct3D11::SwapChain>				_primarySwapChain;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2