/*==================================================================*\
  GraphicsPipelineOptimizations.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	class StripUnusedResourcesPass {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref StripUnusedResourcesPass instance.
		StripUnusedResourcesPass(const StripUnusedResourcesPass&) ETNoexceptHint = default;
		//!	Constructs this @ref StripUnusedResourcesPass instance.
		StripUnusedResourcesPass() ETNoexceptHint = default;

		~StripUnusedResourcesPass() = default;

		// ---------------------------------------------------

	public:
		template <class AttachmentsList, class BuffersList, class PassesList>
		void operator()(AttachmentsList& attachments, BuffersList& buffers, PassesList& passes) const;
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/GraphicsPipelineOptimizations.inl>
//------------------------------------------------------------------//
