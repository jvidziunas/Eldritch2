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
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Renderer/Vulkan/HostAllocator.hpp>
#include <Foundation/GameEngineService.hpp>
#include <Packages/ResourceViewFactory.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class EngineService : public Foundation::GameEngineService {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum : size_t {
			MaxAfrDeviceCount	= 4u
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

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

	// - DATA MEMBERS ------------------------------------

		Vulkan::HostAllocator							_allocator;
		Vulkan::UniquePointer<::VkInstance>				_vulkan;

		Configuration::ConfigurableUTF8String			_preferredAdapterNameForSingleGpu;
		Configuration::ConfigurablePODVariable<bool>	_allowAfrMultiGpu;

		Vulkan::UniquePointer<::VkDevice>				_devices[MaxAfrDeviceCount];
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2