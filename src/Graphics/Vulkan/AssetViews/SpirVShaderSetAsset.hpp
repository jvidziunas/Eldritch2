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
		class UsageAsset {
			// - TYPE PUBLISHING ---------------------------------

		public:
			enum : size_t {
				MaxColorAttachments = 8u,
				MaxNameLength       = 64u
			};

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref Usage instance.
			UsageAsset(
				const Utf8Char* name,
				const VkPipelineColorBlendAttachmentState (&blends)[MaxColorAttachments],
				VkShaderStageFlagBits                         enabledShaders,
				const VkPipelineMultisampleStateCreateInfo&   multisampleInfo,
				const VkPipelineRasterizationStateCreateInfo& rasterizationInfo,
				const VkPipelineDepthStencilStateCreateInfo&  depthStencilInfo);
			//!	Constructs this @ref Usage instance.
			UsageAsset(const UsageAsset&) = default;

			~UsageAsset() = default;

			// ---------------------------------------------------

			const uint32* GetBytecode() const;

			uint32 GetBytecodeSize() const;

			// - DATA MEMBERS ------------------------------------

		public:
			Utf8Char                               name[MaxNameLength];
			VkPipelineColorBlendAttachmentState    blends[MaxColorAttachments];
			VkShaderStageFlagBits                  enabledShaders;
			VkPipelineMultisampleStateCreateInfo   multisampleInfo;
			VkPipelineRasterizationStateCreateInfo rasterizationInfo;
			VkPipelineDepthStencilStateCreateInfo  depthStencilInfo;
			VkPipelineColorBlendStateCreateInfo    colorBlendInfo;
			const uint32*                          bytecodeBegin;
			const uint32*                          bytecodeEnd;
		};

		// ---

	public:
		struct UsageEqual {
			ETPureFunctionHint bool operator()(const UsageAsset&, const Utf8Char*) const;
			ETPureFunctionHint bool operator()(const UsageAsset&, const UsageAsset&) const;
		};

		// ---

	public:
		struct UsageHash {
			ETPureFunctionHint size_t operator()(const Utf8Char*, size_t seed = 0u) const;
			ETPureFunctionHint size_t operator()(const UsageAsset&, size_t seed = 0u) const;
		};

		// ---

	public:
		using UsageSet      = CachingHashSet<UsageAsset, UsageHash, UsageEqual>;
		using UsageIterator = typename UsageSet::ConstIterator;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		SpirVShaderSetAsset(const SpirVShaderSetAsset&) = delete;
		//!	Constructs this @ref SpirVShaderPackageAsset instance.
		SpirVShaderSetAsset(const Utf8Char* const path);

		~SpirVShaderSetAsset() override = default;

		// ---------------------------------------------------

	public:
		UsageIterator Find(const Utf8Char* name) const;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		SpirVShaderSetAsset& operator=(const SpirVShaderSetAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<uint32> _bytecode;
		UsageSet          _usages;
	};

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/AssetViews/SpirVShaderSetAsset.inl>
//------------------------------------------------------------------//
