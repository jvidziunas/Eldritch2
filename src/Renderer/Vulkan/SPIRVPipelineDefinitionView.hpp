/*==================================================================*\
  SPIRVPipelineDefinitionView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Utility/Containers/FlatMap.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class SPIRVPipelineDefinitionView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref SPIRVPipelineDefinitionView instance.
		SPIRVPipelineDefinitionView( FileSystem::ContentLibrary& owningLibrary, FileSystem::ContentPackage& package, const ::Eldritch2::UTF8Char* const name, ::Eldritch2::Allocator& allocator );

		~SPIRVPipelineDefinitionView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	UpdateFromByteStream( const ::Eldritch2::Range<const char*> bytes ) override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::FlatMap<VkDevice, Vulkan::UniquePointer<VkPipeline>> _pipelines;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2