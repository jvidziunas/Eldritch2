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

	class SpirVShader {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum class Opcode : uint32 {};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref SpirVShader instance.
		SpirVShader(const SpirVShader&) ETNoexceptHint = default;
		//!	Constructs this @ref SpirVShader instance.
		SpirVShader() ETNoexceptHint = default;

		~SpirVShader() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		struct {
			VkPipelineMultisampleStateCreateFlags flags;
			VkBool32                              shouldSubsampleShading;
			float                                 minSampleShading;
			VkBool32                              shouldSendAlphaToCoverage;
		} multisample;
		struct {
			VkPipelineRasterizationStateCreateFlags flags;
#if VK_AMD_rasterization_order
			VkRasterizationOrderAMD rasterizationOrder;
#endif
			VkBool32        shouldClampDepth;
			VkBool32        shouldDiscardOutput;
			VkCullModeFlags cullMode;
			float           depthBias;
			float           slopeDepthBias;
			float           depthBiasClamp;
		} rasterizer;
		struct {
			VkPipelineDepthStencilStateCreateFlags flags;
			VkBool32                               shouldTest;
			VkBool32                               shouldWrite;
			VkCompareOp                            depthOperator;
			VkStencilOpState                       frontStencilOperator;
			VkStencilOpState                       backStencilOperator;
		} depthStencil;
		struct {
			VkPipelineColorBlendStateCreateFlags flags;
			VkBool32                             useBitwiseBlend;
			VkLogicOp                            bitwiseBlend;
			float                                constants[4];
			VkPipelineColorBlendAttachmentState  attachments[4];
		} blending;
		Span<const Opcode*> bytecodeByStage[5];
	};

	// ---

	class SpirVShaderSet {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using BytecodeList = ArrayList<SpirVShader::Opcode>;
		using SubshaderMap = CachingHashMap<String, SpirVShader>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref SpirVShaderSet instance.
		SpirVShaderSet(const SpirVShaderSet&) = default;
		//!	Constructs this @ref SpirVShaderSet instance.
		SpirVShaderSet(SpirVShaderSet&&) ETNoexceptHint;
		//!	Constructs this @ref SpirVShaderSet instance.
		SpirVShaderSet() ETNoexceptHint;

		~SpirVShaderSet() = default;

		// ---------------------------------------------------

	public:
		BytecodeList::ConstSliceType GetBytecode() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		Result BindResources(BytecodeList bytecode, SubshaderMap shaders) ETNoexceptHint;

		void FreeResources() ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		BytecodeList _combinedBytecode;
		SubshaderMap _subshaderByPass;

		// ---------------------------------------------------

		friend void Swap(SpirVShaderSet&, SpirVShaderSet&) ETNoexceptHint;
	};

}}} // namespace Eldritch2::Graphics::Vulkan

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/SpirVShaderSet.inl>
//------------------------------------------------------------------//
