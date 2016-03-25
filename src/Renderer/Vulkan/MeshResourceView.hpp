/*==================================================================*\
  MeshResourceView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//
#include <limits>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		namespace Vulkan {
			class	SPIRVPipelineDefinitionResourceView;
		}
	}
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {

	class MeshResourceView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		MeshResourceView( const ::Eldritch2::UTF8Char* const name );

		~MeshResourceView() = default;

	// ---------------------------------------------------

#if 0
		void	IssueDrawCommands( DrawBatch& batch, ::Eldritch2::uint16 inclusiveMinLod = 0 );
#endif

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AttachToPackage( const ::Eldritch2::Range<const char*> bytes, FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) override;

		void					DetachFromPackage( FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) const override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - TYPE PUBLISHING ---------------------------------

	private:
		struct SubmeshIndexData {
			::Eldritch2::uint16							levelOfDetail;
			bool										bindAsUInt16;

			::VkDeviceSize								offsetIntoIndexBuffer;
			const SPIRVPipelineDefinitionResourceView*	pipeline;
		};

	// - DATA MEMBERS ------------------------------------

		::VkBuffer										_vertexBuffer;
		::VkDeviceSize									_offsetIntoVertexBuffer;

		::VkBuffer										_indexBuffer;
		::Eldritch2::ResizableArray<SubmeshIndexData>	_submeshes;
	};

}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

