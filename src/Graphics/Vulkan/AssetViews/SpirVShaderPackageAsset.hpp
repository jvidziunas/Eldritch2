/*==================================================================*\
  SpirVShaderPackageAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace AssetViews {

	class SpirVShaderPackageAsset : public Assets::Asset {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Usage {
		// - TYPE PUBLISHING ---------------------------------

		public:
			enum : size_t {
				MaxColorAttachments	= 8u,
				MaxNameLength		= 128u
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Usage instance.
			Usage(
				const Utf8Char* name,
				const VkPipelineColorBlendAttachmentState (&colorBlends)[MaxColorAttachments],
				VkShaderStageFlagBits enabledStages,
				const VkPipelineMultisampleStateCreateInfo& multisample,
				const VkPipelineRasterizationStateCreateInfo& rasterization,
				const VkPipelineDepthStencilStateCreateInfo& depthStencil
			);
		//!	Constructs this @ref Usage instance.
			Usage( const Usage& ) = default;

			~Usage() = default;

		// - DATA MEMBERS ------------------------------------

		public:
			Utf8Char								name[MaxNameLength];
			VkPipelineColorBlendAttachmentState		colorBlends[MaxColorAttachments];
			VkShaderStageFlagBits					enabledStages;

			VkPipelineMultisampleStateCreateInfo	multisample;
			VkPipelineRasterizationStateCreateInfo	rasterization;
			VkPipelineDepthStencilStateCreateInfo	depthStencil;
			VkPipelineColorBlendStateCreateInfo		colorBlend;
		};

	// ---

	public:
		struct UsageEqual {
			ETPureFunctionHint bool	operator()( const Usage&, const Utf8Char* ) const;
			ETPureFunctionHint bool	operator()( const Usage&, const Usage& ) const;
		};

	// ---

	public:
		struct UsageHash {
			ETPureFunctionHint size_t	operator()( const Utf8Char*, size_t seed = 0u ) const;
			ETPureFunctionHint size_t	operator()( const Usage&, size_t seed = 0u ) const;
		};

	// ---

	public:
		template <typename Value>
		using UsageSet		= CachingHashSet<Value, UsageHash, UsageEqual>;
		template <typename Value>
		using UsageIterator	= typename UsageSet<Value>::ConstIterator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		SpirVShaderPackageAsset( const SpirVShaderPackageAsset& ) = delete;
	//!	Constructs this @ref SpirVShaderPackageAsset instance.
		SpirVShaderPackageAsset( const Utf8Char* const path );

		~SpirVShaderPackageAsset() override = default;

	// ---------------------------------------------------

	public:
		UsageIterator<Usage>	Begin() const;

		UsageIterator<Usage>	End() const;

		UsageIterator<Usage>	Find( const Utf8Char* name ) const;

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( const Builder& builder ) override;

		void		FreeResources() override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		SpirVShaderPackageAsset&	operator=( const SpirVShaderPackageAsset& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		UsageSet<Usage>	_usages;
	};

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/AssetViews/SpirVShaderPackageAsset.inl>
//------------------------------------------------------------------//