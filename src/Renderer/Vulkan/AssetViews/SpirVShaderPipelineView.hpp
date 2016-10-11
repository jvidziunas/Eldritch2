/*==================================================================*\
  SpirVShaderPipelineView.hpp
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
#include <Utility/Containers/Utf8String.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Utility/StringLiteral.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Renderer {
		namespace Vulkan {
			class	Device;
		}
	}

	namespace Assets {
		class	AssetLibrary;
	}

	template <typename>
	class	Result;
	template <typename>
	class	Range;
}

namespace Eldritch2 {
namespace Renderer {
namespace Vulkan {
namespace AssetViews {

	class SpirVShaderPipelineView : public Assets::AssetView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Usage {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Usage instance.
			Usage( Vulkan::UniquePointer<VkPipeline> pipeline );
		//!	Constructs this @ref Usage instance.
			Usage( Usage&& );
		//!	Provided for compatibility with container classes.
			Usage( const Usage& );

		// ---------------------------------------------------

		public:
			void	Bind( VkCommandBuffer commandBuffer ) const;

		// - DATA MEMBERS ------------------------------------

		private:
			Vulkan::UniquePointer<VkPipeline>	_pipeline;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SpirVShaderPipelineView instance.
		SpirVShaderPipelineView( const Eldritch2::Utf8Char* const name, Vulkan::UniquePointer<VkShaderModule> shaderModule, Eldritch2::HashMap<Eldritch2::Utf8String<>, Usage>&& usages );
	//!	Disable copying.
		SpirVShaderPipelineView( const SpirVShaderPipelineView& ) = delete;

		~SpirVShaderPipelineView() = default;

	// ---------------------------------------------------

	public:
		bool	TryBindUsage( VkCommandBuffer commandBuffer, const Eldritch2::Utf8Char* const usageName ) const;

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<Eldritch2::UniquePointer<Assets::AssetView>>	CreateView( Eldritch2::Allocator&				allocator,
																							Vulkan::Device&						device,
																							const Assets::AssetLibrary&			library,
																							const Eldritch2::Utf8Char* const	name,
																							Eldritch2::Range<const char*>		assetBytes );

		static ETPureFunctionHint Eldritch2::Utf8Literal						GetExtension();

	// ---------------------------------------------------

	//!	Disable assignment.
		SpirVShaderPipelineView&	operator=( const SpirVShaderPipelineView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		Vulkan::UniquePointer<VkShaderModule>				_shader;
		Eldritch2::HashMap<Eldritch2::Utf8String<>, Usage>	_usages;
	};

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Renderer
}	// namespace Eldritch2