/*==================================================================*\
  ResidencyCoordinator.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {

	template <typename Resource>
	ETInlineHint ETPureFunctionHint size_t ResidencyCoordinator::ResourceHash<Resource>::operator()( const Resource& resource, size_t seed ) const {
		return (*this)( resource.GetSource(), seed );
	}

// ---------------------------------------------------

	template <typename Resource>
	template <typename Source>
	ETInlineHint ETPureFunctionHint size_t ResidencyCoordinator::ResourceHash<Resource>::operator()( const Source* source, size_t seed ) const {
		return seed + reinterpret_cast<uintptr>(source);
	}

// ---------------------------------------------------

	template <typename Resource>
	ETInlineHint ETPureFunctionHint bool ResidencyCoordinator::ResourceEquals<Resource>::operator()( const Resource& resource, const Resource& resource1 ) const {
		return (*this)( resource, resource1.GetSource() );
	}

// ---------------------------------------------------

	template <typename Resource>
	template <typename Source>
	ETInlineHint ETPureFunctionHint bool ResidencyCoordinator::ResourceEquals<Resource>::operator()( const Resource& resource, const Source* source ) const {
		return resource.GetSource() == source;
	}

}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2