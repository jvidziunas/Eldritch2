/*==================================================================*\
  ShaderPipeline.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <vulkan/vulkan.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			namespace AssetViews {
				class	SpirVShaderPackageAsset;
			}

			class	Gpu;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	class ShaderPipeline {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		ShaderPipeline( const ShaderPipeline& ) = delete;
	//!	Constructs this @ref ShaderPipeline instance.
		ShaderPipeline( ShaderPipeline&& ) ETNoexceptHint;
	//!	Constructs this @ref ShaderPipeline instance.
		ShaderPipeline();

		~ShaderPipeline();

	// ---------------------------------------------------

	public:
		VkResult	BindResources( Gpu& gpu, const AssetViews::SpirVShaderPackageAsset& asset );

		void		FreeResources( Gpu& gpu );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		ShaderPipeline&	operator=( const ShaderPipeline& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		VkPipeline	_pipeline;

	// ---------------------------------------------------

		friend void	Swap( ShaderPipeline&, ShaderPipeline& );
	};

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2