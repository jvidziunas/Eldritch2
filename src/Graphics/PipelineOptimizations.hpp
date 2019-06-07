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
#include <Graphics/PipelineBuilder.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	class StripUnusedResourcesPass {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref StripUnusedResourcesPass instance.
		ETConstexpr StripUnusedResourcesPass(const StripUnusedResourcesPass&) ETNoexceptHint = default;
		//!	Constructs this @ref StripUnusedResourcesPass instance.
		ETConstexpr StripUnusedResourcesPass() ETNoexceptHint = default;

		~StripUnusedResourcesPass() = default;

		// ---------------------------------------------------

	public:
		void operator()(PipelineBuilder::AttachmentList& attachments, PipelineBuilder::BufferList& buffers, PipelineBuilder::StageList& stages) const;
	};

	// ---

	class OptimizeAttachmentBandwidthPass {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref OptimizeAttachmentBandwidthPass instance.
		ETConstexpr OptimizeAttachmentBandwidthPass(const OptimizeAttachmentBandwidthPass&) ETNoexceptHint = default;
		//!	Constructs this @ref OptimizeAttachmentBandwidthPass instance.
		ETConstexpr OptimizeAttachmentBandwidthPass() ETNoexceptHint = default;

		~OptimizeAttachmentBandwidthPass() = default;

		// ---------------------------------------------------

	public:
		void operator()(PipelineBuilder::AttachmentList& attachments, PipelineBuilder::BufferList& buffers, PipelineBuilder::StageList& stages) const ETNoexceptHint;
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/PipelineOptimizations.inl>
//------------------------------------------------------------------//
