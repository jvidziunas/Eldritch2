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
#include <Packages/ResourceViewFactory.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Utility/COMPointer.hpp>
//------------------------------------------------------------------//
#if( ET_COMPILER_IS_MSVC )
//	MSVC complains about macro redefinitions, since a few DirectX components separately
//	define some HRESULT values without an include guard. The definitions themselves are consistent,
//	so just disable the warning.
#	pragma warning( push )
#	pragma warning( disable : 4005 )
#endif
#include <D3D11.h>
#if( ET_COMPILER_IS_MSVC )
#	pragma warning( pop )
#endif
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

		~EngineService() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override sealed;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AllocateWorldView( ::Eldritch2::Allocator& allocator, Foundation::World& world ) override;

	// ---------------------------------------------------

	protected:
		void	OnEngineConfigurationBroadcast( Scheduler::WorkerContext& executingContext ) override sealed;

		void	AcceptInitializationVisitor( FileSystem::ResourceViewFactoryPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override sealed;
		void	AcceptInitializationVisitor( Scripting::ScriptAPIRegistrationInitializationVisitor& visitor ) override sealed;

	// ---------------------------------------------------

		void	OnServiceTickStarted( Scheduler::WorkerContext& executingContext ) override sealed;

	// - TYPE PUBLISHING ---------------------------------

	private:
		class ShaderResourceViewFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ShaderResourceViewFactory instance.
			ShaderResourceViewFactory();

			~ShaderResourceViewFactory() = default;

		// ---------------------------------------------------

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );

		// ---------------------------------------------------

			::Eldritch2::ErrorCode AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														 FileSystem::ContentLibrary&			contentLibrary,
														 FileSystem::ContentPackage&			package,
														 const ::Eldritch2::UTF8Char* const		name,
														 const ::Eldritch2::Range<const char*>	sourceAsset ) override;

		// ---------------------------------------------------

			void	SetDevice( const Utility::COMPointer<::ID3D11Device>& device );

		// - DATA MEMBERS ------------------------------------

		private:
			Utility::COMPointer<::ID3D11Device>							_device;

			Configuration::ConfigurablePODVariable<float>				_adaptiveResolutionMaxAreaFraction;
			Configuration::ConfigurablePODVariable<float>				_adaptiveResolutionMinAreaFraction;
			Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_MSAACount;
			Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_MSAAQuality;
		};

	// ---

		class MeshViewFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref MeshViewFactory instance.
			MeshViewFactory();

			~MeshViewFactory() = default;

		// ---------------------------------------------------

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );

		// ---------------------------------------------------

			::Eldritch2::ErrorCode AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														 FileSystem::ContentLibrary&			contentLibrary,
														 FileSystem::ContentPackage&			package,
														 const ::Eldritch2::UTF8Char* const		name,
														 const ::Eldritch2::Range<const char*>	sourceAsset ) override;

		// ---------------------------------------------------

			void	SetDevice( const Utility::COMPointer<::ID3D11Device>& device );

		// - DATA MEMBERS ------------------------------------

		private:
			Utility::COMPointer<::ID3D11Device>	_device;
		};

	// ---

		class PipelineViewFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref PipelineViewFactory instance.
			PipelineViewFactory();

			~PipelineViewFactory() = default;

		// ---------------------------------------------------

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );

		// ---------------------------------------------------

			::Eldritch2::ErrorCode AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														 FileSystem::ContentLibrary&			contentLibrary,
														 FileSystem::ContentPackage&			package,
														 const ::Eldritch2::UTF8Char* const		name,
														 const ::Eldritch2::Range<const char*>	sourceAsset ) override;

		// ---------------------------------------------------

			void	SetDevice( const Utility::COMPointer<::ID3D11Device>& device );

		// - DATA MEMBERS ------------------------------------

		private:
			Utility::COMPointer<::ID3D11Device>	_device;
		};

	// - DATA MEMBERS ------------------------------------

		ShaderResourceViewFactory									_shaderResourceViewFactory;
		MeshViewFactory												_meshResourceViewFactory;
		PipelineViewFactory											_pipelineViewFactory;

		Configuration::ConfigurableUTF8String						_preferredAdapterName;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_VSyncMode;
		Configuration::ConfigurablePODVariable<::Eldritch2::uint32>	_maximumFramesToRenderAhead;
		
		Configuration::ConfigurablePODVariable<bool>				_forceDebugRuntime;
		Configuration::ConfigurablePODVariable<bool>				_allowDriverThreadingOptimizations;

		const Direct3D11::MeshResourceView*							_defaultMeshView;
		Utility::COMPointer<::ID3D11DeviceContext>					_immediateContext;
		Scripting::ObjectHandle<Direct3D11::SwapChain>				_primarySwapChain;
	};

}	// namespace Direct3D11
}	// namespace Renderer
}	// namespace Eldritch2