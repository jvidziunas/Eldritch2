/*==================================================================*\
  GraphicsPipelineApi.hpp
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

	class PassDimensions {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PassResolution instance.
		ETConstexpr PassDimensions(float32 minWidth, float32 maxWidth, float32 minHeight, float32 maxHeight) ETNoexceptHint;
		//!	Constructs this @ref PassResolution instance.
		ETConstexpr PassDimensions(uint32 width, uint32 height, uint32 depth) ETNoexceptHint;
		//!	Constructs this @ref PassResolution instance.
		ETConstexpr PassDimensions(float32 width, float32 height) ETNoexceptHint;
		//!	Constructs this @ref PassResolution instance.
		ETConstexpr PassDimensions(const PassDimensions&) ETNoexceptHint = default;

		// ---------------------------------------------------

	public:
		ETConstexpr bool IsCompatible(const PassDimensions& rhs) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr uint32 GetMaxWidth(uint32 base) const ETNoexceptHint;

		ETConstexpr uint32 GetMaxHeight(uint32 base) const ETNoexceptHint;

		ETConstexpr uint32 GetMaxDepth(uint32 base) const ETNoexceptHint;

		// ---------------------------------------------------

		union {
			struct {
				uint32 width, height, depth;
			} fixed;
			struct {
				float32 minWidth, maxWidth, minHeight, maxHeight;
			} scale;
		};
		bool useFixed;
	};

	// ---

	using AttachmentDimensions = PassDimensions;

	// ---

	class PipelineAttachmentReference {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PipelineAttachmentReference instance.
		ETConstexpr PipelineAttachmentReference(uint32 globalIndex, uint32 firstLayer, uint32 layers) ETNoexceptHint;
		//!	Constructs this @ref PipelineAttachmentReference instance.
		ETConstexpr PipelineAttachmentReference(const PipelineAttachmentReference&) ETNoexceptHint = default;
		//!	Constructs this @ref PipelineAttachmentReference instance.
		ETConstexpr PipelineAttachmentReference() ETNoexceptHint;

		~PipelineAttachmentReference() = default;

		// ---------------------------------------------------

	public:
		PipelineAttachmentReference& operator=(const PipelineAttachmentReference&) ETNoexceptHint = default;

		// - DATA MEMBERS ------------------------------------

	public:
		uint32 globalIndex;
		uint32 firstLayer;
		uint32 layers;
	};

	// ---

	class PipelineFramebufferDescription {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PipelineFramebufferDescription instance.
		ETConstexpr PipelineFramebufferDescription(PassDimensions resolution, uint32 samplesPerPixel) ETNoexceptHint;
		//!	Constructs this @ref PipelineFramebufferDescription instance.
		ETConstexpr PipelineFramebufferDescription(const PipelineFramebufferDescription&) ETNoexceptHint = default;

		~PipelineFramebufferDescription() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		PassDimensions              resolution;
		uint32                      samplesPerPixel;
		PipelineAttachmentReference colorAttachments[4];
		PipelineAttachmentReference inputAttachments[4];
		PipelineAttachmentReference depthAttachment;
	};

	// ---

	enum class PassType : uint8 {
		Draw,
		Compute,
		Copy
	};

	// ---

	class PipelinePassDescription {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PipelinePassDescription instance.
		PipelinePassDescription(PassType type, PassDimensions resolution, uint32 samplesPerPixel, StringView name) ETNoexceptHint;
		//!	Constructs this @ref PipelinePassDescription instance.
		ETConstexpr PipelinePassDescription(const PipelinePassDescription&) ETNoexceptHint = default;

		~PipelinePassDescription() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		PassType                       type;
		PipelineFramebufferDescription framebuffer;
		Utf8Char                       shader[64];
	};

	// ---

	class ResolutionScaler {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ResolutionScaler instance.
		ETConstexpr ResolutionScaler(const ResolutionScaler&) ETNoexceptHint = default;
		//!	Constructs this @ref ResolutionScaler instance.
		ETConstexpr ResolutionScaler() ETNoexceptHint;

		~ResolutionScaler() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr uint32 GetMaxWidth() const ETNoexceptHint;

		ETConstexpr uint32 GetMaxHeight() const ETNoexceptHint;

		ETConstexpr uint32 GetCurrentWidth() const ETNoexceptHint;

		ETConstexpr uint32 GetCurrentHeight() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void UpdateTime(uint64 timeInMicroseconds, float32 tightness = 0.75f) ETNoexceptHint;

		void SetScale(float32 scale) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void                  SetBounds(const PassDimensions& resolution, uint32 baseWidth, uint32 baseHeight) ETNoexceptHint;
		ETCpp14Constexpr void SetBounds(uint32 minWidth, uint32 maxWidth, uint32 minHeight, uint32 maxHeight) ETNoexceptHint;
		ETCpp14Constexpr void SetBounds(uint32 width, uint32 height) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		uint32  _width;
		uint32  _height;
		float32 _reciprocalTargetTimeMicroseconds;
		uint32  _minWidth;
		uint32  _maxWidth;
		uint32  _minHeight;
		uint32  _maxHeight;

		// ---------------------------------------------------

		friend void Swap(ResolutionScaler&, ResolutionScaler&) ETNoexceptHint;
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/GraphicsPipelineApi.inl>
//------------------------------------------------------------------//
