/*==================================================================*\
  PipelineBuilder.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/GpuAbi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	enum class Stage : uint8 {
		Draw,
		Compute,
		Copy,
		PathTrace
	};

	// ---

	enum AttachmentUsage : uint32 {
		UnusedAttachment       = 0x00u,
		InputAttachment        = 0x01u,
		ColorAttachment        = 0x02u,
		DepthStencilAttachment = 0x04u,
	};

	// ---

	enum BufferUsage : uint32 {
		UnusedBuffer = 0x00u,
	};

	// ---

	class StageResolution {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref StageResolution instance.
		ETConstexpr StageResolution(float32 minWidth, float32 maxWidth, float32 minHeight, float32 maxHeight) ETNoexceptHint;
		//!	Constructs this @ref StageResolution instance.
		ETConstexpr StageResolution(uint32 width, uint32 height, uint32 depth) ETNoexceptHint;
		//!	Constructs this @ref StageResolution instance.
		ETConstexpr StageResolution(float32 width, float32 height) ETNoexceptHint;
		//!	Constructs this @ref StageResolution instance.
		ETConstexpr StageResolution(const StageResolution&) ETNoexceptHint = default;

		~StageResolution() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr bool IsCompatibleWith(const StageResolution&) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr uint32 GetWidth(uint32 base) const ETNoexceptHint;

		ETConstexpr uint32 GetHeight(uint32 base) const ETNoexceptHint;

		ETConstexpr uint32 GetDepth(uint32 base) const ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	public:
		bool useFixed;
		union {
			struct {
				uint32 width;
				uint32 height;
				uint32 depth;
			} fixed;
			struct {
				float32 minWidth;
				float32 maxWidth;
				float32 minHeight;
				float32 maxHeight;
			} scale;
		};
	};

	// ---

	class PipelineAttachment {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using StageIndex = uint32;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref PipelineAttachment instance.
		ETConstexpr PipelineAttachment(GpuFormat format, uint32 smallestMip, bool persistent) ETNoexceptHint;
		//! Constructs this @ref PipelineAttachment instance.
		ETConstexpr PipelineAttachment(const PipelineAttachment&) ETNoexceptHint = default;

		~PipelineAttachment() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr bool IsUsed() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		bool TryMarkWritten(AttachmentUsage usage, StageIndex stage, const StageResolution& viewport) ETNoexceptHint;

		bool TryMarkRead(AttachmentUsage usage, StageIndex stage, const StageResolution& viewport) ETNoexceptHint;

		bool TryMarkUsed(AttachmentUsage usage, StageIndex stage, const StageResolution& viewport) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	public:
		StageResolution resolution;
		GpuFormat       format;
		AttachmentUsage usages;
		uint32          samplesPerPixel;
		uint32          slices;
		uint32          smallestMip;
		StageIndex      firstRead;
		StageIndex      lastRead;
		StageIndex      firstWrite;
		StageIndex      lastWrite;
	};

	// ---

	class PipelineBuffer {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using StageIndex = uint32;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref PipelineBuffer instance.
		ETConstexpr PipelineBuffer(GpuSize byteSize, bool persistent) ETNoexceptHint;
		//! Constructs this @ref PipelineBuffer instance.
		ETConstexpr PipelineBuffer(const PipelineBuffer&) ETNoexceptHint = default;

		~PipelineBuffer() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		GpuSize     byteSize;
		BufferUsage usages;
		StageIndex  firstRead;
		StageIndex  lastRead;
		StageIndex  firstWrite;
		StageIndex  lastWrite;
	};

	// ---

	class StageAttachment {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using AttachmentIndex = uint32;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref StageAttachment instance.
		ETConstexpr StageAttachment(AttachmentIndex globalIndex, uint32 mip, uint32 firstSlice = 0u, uint32 lastSlice = 1u) ETNoexceptHint;
		//!	Constructs this @ref StageAttachment instance.
		ETConstexpr StageAttachment(const StageAttachment&) ETNoexceptHint = default;
		//!	Constructs this @ref StageAttachment instance.
		ETConstexpr StageAttachment() ETNoexceptHint;

		~StageAttachment() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr uint32 GetArraySlices() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr StageAttachment& operator=(const StageAttachment&) ETNoexceptHint = default;

		// - DATA MEMBERS ------------------------------------

	public:
		AttachmentIndex globalIndex;
		uint32          mip;
		uint32          firstSlice;
		uint32          lastSlice;
		bool            shouldLoad;
		bool            shouldStore;
	};

	// ---

	class StageBuilder {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref StageBuilder instance.
		StageBuilder(Stage type, StringSpan name, StageResolution resolution, uint32 samplesPerPixel) ETNoexceptHint;
		//!	Constructs this @ref StageBuilder instance.
		ETConstexpr StageBuilder(const StageBuilder&) ETNoexceptHint = default;

		~StageBuilder() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		Stage           type;
		Utf8Char        shader[64];
		StageResolution resolution;
		uint32          samplesPerPixel;
		StageAttachment depthAttachment;
		StageAttachment inputAttachments[4];
		StageAttachment colorAttachments[4];
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

		ETConstexpr uint32 GetWidth() const ETNoexceptHint;

		ETConstexpr uint32 GetHeight() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void UpdateTime(MicrosecondTime time, float32 tension = 0.75f) ETNoexceptHint;

		void SetScale(float32 scale) ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void                  ScaleBounds(const StageResolution& resolution, uint32 baseWidth, uint32 baseHeight) ETNoexceptHint;
		ETCpp14Constexpr void ScaleBounds(uint32 minWidth, uint32 maxWidth, uint32 minHeight, uint32 maxHeight) ETNoexceptHint;
		ETCpp14Constexpr void ScaleBounds(uint32 width, uint32 height) ETNoexceptHint;

		// - DATA MEMBERS ------------------------------------

	private:
		uint32  _width;
		uint32  _height;
		float32 _reciprocalTargetTime;
		uint32  _minWidth;
		uint32  _maxWidth;
		uint32  _minHeight;
		uint32  _maxHeight;

		// ---------------------------------------------------

		friend void Swap(ResolutionScaler&, ResolutionScaler&) ETNoexceptHint;
	};

	// ---

	class PipelineBuilder {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using AttachmentList      = ArrayList<PipelineAttachment>;
		using BufferList          = ArrayList<PipelineBuffer>;
		using StageList           = ArrayList<StageBuilder>;
		using AttachmentIndex     = AttachmentList::SizeType;
		using StageIndex          = StageList::SizeType;
		using AttachmentSliceType = AttachmentList::ConstSliceType;
		using BufferSliceType     = BufferList::ConstSliceType;
		using StageSliceType      = StageList::ConstSliceType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref PipelineBuilder instance.
		PipelineBuilder(const PipelineBuilder&) = default;
		//!	Constructs this @ref PipelineBuilder instance.
		PipelineBuilder(PipelineBuilder&&) ETNoexceptHint;
		//!	Constructs this @ref PipelineBuilder instance.
		PipelineBuilder() ETNoexceptHint;

		~PipelineBuilder() = default;

		// ---------------------------------------------------

	public:
		AttachmentSliceType GetAttachments() const ETNoexceptHint;

		BufferSliceType GetBuffers() const ETNoexceptHint;

		StageSliceType GetStages() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		Result DefineSharedAttachment(GpuFormat format, uint32 smallestMip, bool isPersistent);

		Result DefineDrawStage(StringSpan name, StageResolution resolution, uint32 samplesPerPixel, StageAttachment depthAttachment, Span<const StageAttachment*> inputAttachments, Span<const StageAttachment*> colorAttachments);
		Result DefineDrawStage(StringSpan name, StageResolution resolution, uint32 samplesPerPixel, Span<const StageAttachment*> inputAttachments, Span<const StageAttachment*> colorAttachments);

		Result DefineComputeStage(StringSpan name, StageResolution dimensions, Span<const StageAttachment*> attachments);

		Result DefineCopyStage(StringSpan name, Span<const StageAttachment*> targets, Span<const StageAttachment*> sources);

		// ---------------------------------------------------

	public:
		template <typename... OptimizationPasses>
		PipelineBuilder& Optimize(OptimizationPasses... passes);

		// ---------------------------------------------------

		//!	Disable copy construction.
		PipelineBuilder& operator=(const PipelineBuilder&) = default;

		// - DATA MEMBERS ------------------------------------

	private:
		AttachmentList _attachments;
		BufferList     _buffers;
		StageList      _stages;

		// ---------------------------------------------------

		friend void Swap(PipelineBuilder&, PipelineBuilder&) ETNoexceptHint;
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/PipelineBuilder.inl>
//------------------------------------------------------------------//
