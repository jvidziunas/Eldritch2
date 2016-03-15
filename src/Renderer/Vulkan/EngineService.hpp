/*==================================================================*\
  EngineService.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurablePODVariable.hpp>
#include <Configuration/ConfigurableUTF8String.hpp>
#include <Renderer/Vulkan/DeviceContext.hpp>
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Renderer/Vulkan/HostAllocator.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Packages/ResourceViewFactory.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class EngineService : public Foundation::GameEngineService {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref EngineService instance.
		/*!	@param[in] owningEngine @ref GameEngine instance that will manage the lifetime of the new @ref EngineService.
			*/
		EngineService( Foundation::GameEngine& owningEngine );

		~EngineService() = default;

	// ---------------------------------------------------

		const ::Eldritch2::UTF8Char* const	GetName() const override;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AllocateWorldView( ::Eldritch2::Allocator& allocator, Foundation::World& world ) override;

	// ---------------------------------------------------

		void	OnEngineConfigurationBroadcast( Scheduler::WorkerContext& executingContext ) override;

		void	AcceptInitializationVisitor( Scripting::ScriptApiRegistrationInitializationVisitor& visitor ) override;

		void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor ) override;

		void	AcceptInitializationVisitor( FileSystem::ResourceViewFactoryPublishingInitializationVisitor& visitor ) override;

	// ---------------------------------------------------

		void	OnServiceTickStarted( Scheduler::WorkerContext& executingContext ) override;

	// ---------------------------------------------------

	private:
		void	PopulateDevicesSingleGpu();

		void	PopulateDevicesAfr();

	// - TYPE PUBLISHING ---------------------------------

		class PipelineFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref PipelineFactory instance.
			PipelineFactory();

			~PipelineFactory() = default;

		// ---------------------------------------------------

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );

		// ---------------------------------------------------

			static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

		// ---------------------------------------------------

			::Eldritch2::ErrorCode AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														 FileSystem::ContentLibrary&			contentLibrary,
														 FileSystem::ContentPackage&			package,
														 const ::Eldritch2::UTF8Char* const		name,
														 const ::Eldritch2::Range<const char*>	sourceAsset ) override;
		};

	// ---

		class ImageFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref PipelineFactory instance.
			ImageFactory();

			~ImageFactory() = default;

		// ---------------------------------------------------

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );

		// ---------------------------------------------------

			static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

		// ---------------------------------------------------

			::Eldritch2::ErrorCode AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														 FileSystem::ContentLibrary&			contentLibrary,
														 FileSystem::ContentPackage&			package,
														 const ::Eldritch2::UTF8Char* const		name,
														 const ::Eldritch2::Range<const char*>	sourceAsset ) override;

		// ---------------------------------------------------

			void	CreateDeviceResources( Vulkan::DeviceContext& deviceContext );

		// - DATA MEMBERS ------------------------------------

		private:
			Configuration::ConfigurablePODVariable<::VkDeviceSize>	_devicePoolSize;
			Vulkan::UniquePointer<::VkDeviceMemory>					_devicePool;
			Vulkan::UniquePointer<::VkDeviceMemory>					_stagingPool;
		};

	// ---

		class MeshFactory : public FileSystem::ResourceViewFactory {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref MeshFactory instance.
			MeshFactory();

			~MeshFactory() = default;

		// ---------------------------------------------------

			void	AcceptInitializationVisitor( Configuration::ConfigurationPublishingInitializationVisitor& visitor );

		// ---------------------------------------------------

			static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

		// ---------------------------------------------------

			::Eldritch2::ErrorCode AllocateResourceView( ::Eldritch2::Allocator&				allocator,
														 FileSystem::ContentLibrary&			contentLibrary,
														 FileSystem::ContentPackage&			package,
														 const ::Eldritch2::UTF8Char* const		name,
														 const ::Eldritch2::Range<const char*>	sourceAsset ) override;

		// ---------------------------------------------------

			void	CreateDeviceResources( Vulkan::DeviceContext& deviceContext );

		// - DATA MEMBERS ------------------------------------

		private:
			Configuration::ConfigurablePODVariable<::VkDeviceSize>	_devicePoolSize;
			Vulkan::UniquePointer<::VkDeviceMemory>					_devicePool;
			Vulkan::UniquePointer<::VkDeviceMemory>					_stagingPool;
			void*													_mappedStagingPool;
		};

	// - DATA MEMBERS ------------------------------------

		Vulkan::HostAllocator							_allocator;
		Vulkan::UniquePointer<::VkInstance>				_vulkan;

		PipelineFactory									_pipelineFactory;
		ImageFactory									_imageFactory;
		MeshFactory										_meshFactory;

		Vulkan::UniquePointer<::VkFence>				_copyFence;

		Configuration::ConfigurableUTF8String			_enabledInstanceLayers;
		Configuration::ConfigurableUTF8String			_enabledDeviceLayers;
		
		Configuration::ConfigurableUTF8String			_preferredAdapterNameForSingleGpu;
		Configuration::ConfigurablePODVariable<bool>	_allowAfrMultiGpu;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2