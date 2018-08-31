/*==================================================================*\
  SpirVShaderSetAsset.hpp
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

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	class SpirVShaderSetAsset : public Assets::Asset {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class Usage {
			// - TYPE PUBLISHING ---------------------------------

		public:
			enum : size_t {
				MaxColorAttachments = 4u
			};

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Usage instance.
			Usage(
				const VkPipelineColorBlendAttachmentState (&blends)[MaxColorAttachments],
				VkShaderStageFlagBits                         enabledShaders,
				const VkPipelineMultisampleStateCreateInfo&   multisampleInfo,
				const VkPipelineRasterizationStateCreateInfo& rasterizationInfo,
				const VkPipelineDepthStencilStateCreateInfo&  depthStencilInfo);
			//!	Constructs this @ref Usage instance.
			Usage(const Usage&) = default;

			~Usage() = default;

			// ---------------------------------------------------

		public:
			const uint32* GetBytecode() const;

			uint32 GetBytecodeSizeInBytes() const;

			// - DATA MEMBERS ------------------------------------

		public:
			VkPipelineColorBlendAttachmentState    blends[MaxColorAttachments];
			VkPipelineMultisampleStateCreateInfo   multisampleInfo;
			VkPipelineRasterizationStateCreateInfo rasterizationInfo;
			VkPipelineDepthStencilStateCreateInfo  depthStencilInfo;
			VkPipelineColorBlendStateCreateInfo    colorBlendInfo;
			VkShaderStageFlagBits                  enabledShaders;
			const uint32*                          bytecode;
			const uint32*                          bytecodeEnd;
		};

		// ---

	public:
		enum : uint32 {
			InvalidUsageId = ~0u
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		SpirVShaderSetAsset(const SpirVShaderSetAsset&) = delete;
		//!	Constructs this @ref SpirVShaderPackageAsset instance.
		SpirVShaderSetAsset(StringView path);

		~SpirVShaderSetAsset() override = default;

		// ---------------------------------------------------

	public:
		const Usage& operator[](uint32 usage) const;

		uint32 Find(StringView name) const;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		SpirVShaderSetAsset& operator=(const SpirVShaderSetAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<uint32>              _bytecode;
		ArrayList<Usage>               _usages;
		CachingHashMap<String, uint32> _indexByName;
	};

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/AssetViews/SpirVShaderSetAsset.inl>
//------------------------------------------------------------------//
