/*==================================================================*\
  MeshView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Renderer/Vulkan/DeviceMemoryPool.hpp>
#include <Utility/Containers/Range.hpp>
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/StringLiteral.hpp>
#include <Utility/UniquePointer.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		namespace Vulkan {
			namespace AssetViews {
				class	SpirVShaderPipelineView;
			}

			class	Device;
		}
	}

	namespace Assets {
		class	AssetLibrary;
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace AssetViews {

	class MeshView : public Assets::AssetView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Submesh {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Submesh instance.
			Submesh( VkBuffer													buffer,
					 VkDeviceSize												vertexBufferOffset,
					 VkDeviceSize												indexBufferOffset,
					 Eldritch2::uint16											indexCount,
					 Eldritch2::uint16											indicesPerPrimitive,
					 Eldritch2::uint32											firstIndex,
					 const AssetViews::SpirVShaderPipelineView&					pipeline,
					 Eldritch2::Range<const VkVertexInputBindingDescription*>	bindings,
					 Eldritch2::Range<const VkVertexInputAttributeDescription*>	attributes );

			~Submesh() = default;

		// ---------------------------------------------------

		public:
			void	IssueDraw( VkCommandBuffer commandBuffer, uint32_t instanceCount, uint32_t firstInstance ) const;
			void	IssueDraw( VkCommandBuffer commandBuffer, volatile VkDrawIndexedIndirectCommand& command ) const;

		// ---------------------------------------------------

		public:
			void	BindMeshBuffers( VkCommandBuffer commandBuffer ) const;

		// ---------------------------------------------------

		public:
			const AssetViews::SpirVShaderPipelineView&	GetShaderPipeline() const;

		// - DATA MEMBERS ------------------------------------

		private:
			VkBuffer													_buffer;
			VkDeviceSize												_vertexBufferOffset;
			VkDeviceSize												_indexBufferOffset;
			Eldritch2::uint16											_indexCount;
			Eldritch2::uint16											_indicesPerPrimitive : 5;
			Eldritch2::uint32											_firstIndex;
			const AssetViews::SpirVShaderPipelineView*					_pipeline;
			Eldritch2::Range<const VkVertexInputBindingDescription*>	_bindings;
			Eldritch2::Range<const VkVertexInputAttributeDescription*>	_attributes;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref MeshView instance.
		MeshView( const Eldritch2::Utf8Char* const								name,
				  Vulkan::UniquePointer<VkBuffer>								buffer,
				  Vulkan::DeviceMemoryPool::Allocation							allocation,
				  Eldritch2::UniquePointer<VkVertexInputAttributeDescription[]>	aggregatedAttributes,
				  Eldritch2::UniquePointer<VkVertexInputBindingDescription[]>	aggregatedBindings,
				  Eldritch2::UniquePointer<Submesh[]>							submeshes );
	//!	Disable copying.
		MeshView( const MeshView& );

		~MeshView() = default;

	// ---------------------------------------------------

	public:
		template <typename Visitor>
		void	VisitSubmeshes( Visitor&& visitor );

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<Eldritch2::UniquePointer<Assets::AssetView>>	CreateView( Eldritch2::Allocator& allocator, Vulkan::Device& device, Vulkan::DeviceMemoryPool& pool, const Assets::AssetLibrary& library, const Eldritch2::Utf8Char* const name, Eldritch2::Range<const char*> assetBytes );

		static ETPureFunctionHint Eldritch2::Utf8Literal						GetExtension();

	// ---------------------------------------------------

	//!	Disable assignment.
		MeshView&	operator=( const MeshView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::UniquePointer<VkBuffer>									_buffer;
		Vulkan::DeviceMemoryPool::Allocation							_allocation;
		Eldritch2::UniquePointer<VkVertexInputAttributeDescription[]>	_aggregatedAttributes;
		Eldritch2::UniquePointer<VkVertexInputBindingDescription[]>		_aggregatedBindings;
		Eldritch2::UniquePointer<Submesh[]>								_submeshes;
	};

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/AssetViews/MeshView.inl>
//------------------------------------------------------------------//
