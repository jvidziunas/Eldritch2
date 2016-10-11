/*==================================================================*\
  ImageView.hpp
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
#include <Renderer/Vulkan/SmartPointers.hpp>
#include <Utility/StringLiteral.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetLibrary;
	}

	namespace Renderer {
		namespace Vulkan {
			class	Device;
		}
	}

	template <typename>
	class	Range;
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace AssetViews {

	class ImageView : public Assets::AssetView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ImageView instance.
		ImageView( const Eldritch2::Utf8Char* const name, Vulkan::DeviceMemoryPool::Allocation allocation, Vulkan::UniquePointer<VkImageView> view );
	//!	Disable copying.
		ImageView( const ImageView& ) = delete;

		~ImageView() = default;

	// ---------------------------------------------------

	public:
		VkImageView	GetFramebufferAttachmentView() const;

		VkImageView	GetShaderView() const;

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<Eldritch2::UniquePointer<Assets::AssetView>>	CreateView( Eldritch2::Allocator& allocator, Vulkan::Device& device, Vulkan::DeviceMemoryPool& pool, const Assets::AssetLibrary& library, const Eldritch2::Utf8Char* const name, Eldritch2::Range<const char*> assetBytes );

		static ETPureFunctionHint Eldritch2::Utf8Literal						GetExtension();

	// ---------------------------------------------------

	//!	Disable assignment.
		ImageView&	operator=( const ImageView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::DeviceMemoryPool::Allocation	_allocation;
		Vulkan::UniquePointer<VkImageView>		_view;
	};

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Renderer/Vulkan/AssetViews/ImageView.inl>
//------------------------------------------------------------------//