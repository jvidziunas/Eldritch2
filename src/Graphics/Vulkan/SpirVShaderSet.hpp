/*==================================================================*\
  ShaderPipeline.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vulkan/vulkan_core.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan {
	class GraphicsPipeline;
	class Gpu;
}}} // namespace Eldritch2::Graphics::Vulkan

namespace Eldritch2 { namespace Graphics { namespace Vulkan {

	class SpirVShader {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref SpirVShader instance.
		SpirVShader(const SpirVShader&) ETNoexceptHint = default;
		//!	Constructs this @ref SpirVShader instance.
		SpirVShader() ETNoexceptHint = default;

		~SpirVShader() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		Utf8Char             name[64];
		Range<const uint32*> bytecodeByStage[5];

		struct {
			VkPipelineMultisampleStateCreateFlags flags;
			VkBool32                              shouldSubsampleShading;
			float                                 minSampleShading;
			VkBool32                              shouldSendAlphaToCoverage;
			VkBool32                              shouldForceAlphaToOne;
		} multisample;
		struct {
			VkPipelineRasterizationStateCreateFlags flags;
#if VK_AMD_rasterization_order
			VkRasterizationOrderAMD rasterizationOrder;
#endif
			VkBool32        shouldClampDepth;
			VkBool32        shouldDiscardOutput;
			VkPolygonMode   fill;
			VkCullModeFlags cullMode;
			VkFrontFace     frontFace;
			float           depthBias;
			float           slopeDepthBias;
			float           depthBiasClamp;
		} rasterizer;
		struct {
			VkPipelineDepthStencilStateCreateFlags flags;
			VkBool32                               shouldTest;
			VkBool32                               shouldWrite;
			VkBool32                               shouldClipBounds;
			VkBool32                               shouldTestStencil;
			VkCompareOp                            depthOperator;
			VkStencilOpState                       frontStencilOperator;
			VkStencilOpState                       backStencilOperator;
			float                                  minDepthBounds;
			float                                  maxDepthBounds;
		} depthStencil;
		struct {
			VkPipelineColorBlendStateCreateFlags flags;
			VkBool32                             useBitwiseBlend;
			VkLogicOp                            bitwiseBlend;
			float                                constants[4];
			VkPipelineColorBlendAttachmentState  attachments[4];
		} blending;

		// ---------------------------------------------------

		friend ETCpp14Constexpr size_t GetHashCode(const SpirVShader&, size_t seed = 0u) ETNoexceptHint;

		friend ETPureFunctionHint bool operator==(const SpirVShader&, const SpirVShader&) ETNoexceptHint;
		friend ETPureFunctionHint bool operator==(const SpirVShader&, const StringView&) ETNoexceptHint;
	};

	// ---

	using SpirVShaderSet = HashSet<SpirVShader>;

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/SpirVShaderSet.inl>
//------------------------------------------------------------------//
