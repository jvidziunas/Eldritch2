/*==================================================================*\
  Direct3D11Renderer.hpp
  ------------------------------------------------------------------
  Purpose:
  Central context object used to centralize all simulation, effects
  processing and resource management associated with drawing.

  ------------------------------------------------------------------
  �2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/D3D11/D3D11ForwardDeclarations.hpp>
#include <Configuration/ConfigurablePODVariable.hpp>
#include <Configuration/ConfigurableUTF8String.hpp>
#include <Renderer/D3D11/Direct3D11SwapChain.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#include <D3D11.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		class	Direct3D11MeshView;
	}
}

namespace Eldritch2 {
namespace Renderer {

	namespace Configuration	= ::Eldritch2::Configuration;
	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Foundation	= ::Eldritch2::Foundation;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Renderer		= ::Eldritch2::Renderer;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class Direct3D11Renderer : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Initializes this @ref Direct3D11Renderer instance.
		Direct3D11Renderer( Foundation::GameEngine& owningEngine );

		//! Destroys this @ref Direct3D11Renderer instance.
		~Direct3D11Renderer() = default;

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
		Utility::COMPointer<::IDXGIFactory1>						_factory;
		Utility::COMPointer<::ID3D11Device>							_device;
		Utility::COMPointer<::ID3D11DeviceContext>					_immediateContext;

		Configuration::ConfigurableUTF8String						_defaultMeshName;

		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_VSyncMode;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_adapterIndex;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_maximumFramesToRenderAhead;

		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_MSAACount;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_MSAAQuality;
		Configuration::ConfigurablePODVariable<float>				_adaptiveResolutionMaxAreaFraction;
		Configuration::ConfigurablePODVariable<float>				_adaptiveResolutionMinAreaFraction;
		Configuration::ConfigurablePODVariable<bool>				_forceDebugRuntime;

		const Renderer::Direct3D11MeshView*							_defaultMeshView;
		Scripting::ObjectHandle<Renderer::Direct3D11SwapChain>		_primarySwapChain;
	};

}	// namespace Renderer
}	// namespace Eldritch2