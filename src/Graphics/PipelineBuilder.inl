/*==================================================================*\
  PipelineBuilder.inl
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

	ETConstexpr ETForceInlineHint AttachmentUsage operator~(AttachmentUsage usage) ETNoexceptHint {
		return AttachmentUsage(~uint32(usage));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint AttachmentUsage operator|(AttachmentUsage lhs, AttachmentUsage rhs) ETNoexceptHint {
		return AttachmentUsage(uint32(lhs) | uint32(rhs));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint AttachmentUsage& operator|=(AttachmentUsage& lhs, AttachmentUsage rhs) ETNoexceptHint {
		return lhs = (lhs | rhs);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint AttachmentUsage operator&(AttachmentUsage lhs, AttachmentUsage rhs)ETNoexceptHint {
		return AttachmentUsage(uint32(lhs) & uint32(rhs));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint AttachmentUsage& operator&=(AttachmentUsage& lhs, AttachmentUsage rhs) ETNoexceptHint {
		return lhs = (lhs & rhs);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint AttachmentUsage operator^(AttachmentUsage lhs, AttachmentUsage rhs) ETNoexceptHint {
		return AttachmentUsage(uint32(lhs) ^ uint32(rhs));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint AttachmentUsage& operator^=(AttachmentUsage& lhs, AttachmentUsage rhs) ETNoexceptHint {
		return lhs = (lhs ^ rhs);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint StageResolution::StageResolution(float32 minWidth, float32 maxWidth, float32 minHeight, float32 maxHeight) ETNoexceptHint : useFixed(false), scale{ minWidth, maxWidth, minHeight, maxHeight } {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint StageResolution::StageResolution(uint32 width, uint32 height, uint32 depth) ETNoexceptHint : useFixed(true), fixed{ width, height, depth } {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint StageResolution::StageResolution(float32 width, float32 height) ETNoexceptHint : StageResolution(/*minWidth =*/width, /*maxWidth =*/width, /*minHeight =*/height, /*maxHeight =*/height) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint bool StageResolution::IsCompatibleWith(const StageResolution& rhs) const ETNoexceptHint {
		return useFixed == rhs.useFixed && // clang-format off
			(useFixed ? ((fixed.width == rhs.fixed.width) & (fixed.height == rhs.fixed.height) & (fixed.depth == rhs.fixed.depth)) :
			((scale.maxWidth == rhs.scale.maxWidth) & (scale.maxHeight == rhs.scale.maxHeight))); // clang-format on
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint uint32 StageResolution::GetWidth(uint32 base) const ETNoexceptHint {
		return useFixed ? fixed.width : uint32(scale.maxWidth * base);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint uint32 StageResolution::GetHeight(uint32 base) const ETNoexceptHint {
		return useFixed ? fixed.height : uint32(scale.maxHeight * base);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint uint32 StageResolution::GetDepth(uint32 base) const ETNoexceptHint {
		return useFixed ? fixed.depth : base;
	}

	// ---------------------------------------------------

	ETConstexpr PipelineAttachment::PipelineAttachment(GpuFormat format, uint32 smallestMip, bool persistent) ETNoexceptHint : resolution(/*width =*/0.0f, /*height =*/0.0f),
																															   format(format),
																															   usages(UnusedAttachment),
																															   samplesPerPixel(0u),
																															   slices(1u),
																															   smallestMip(smallestMip),
																															   firstRead(StageIndex(persistent ? 0u : 0xFFFFFFFFu)),
																															   lastRead(StageIndex(persistent ? 0xFFFFFFFFu : 0u)),
																															   firstWrite(StageIndex(persistent ? 0u : 0xFFFFFFFFu)),
																															   lastWrite(StageIndex(persistent ? 0xFFFFFFFFu : 0u)) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint bool PipelineAttachment::IsUsed() const ETNoexceptHint {
		return usages != UnusedAttachment;
	}

	// ---------------------------------------------------

	ETInlineHint bool PipelineAttachment::TryMarkUsed(AttachmentUsage usage, StageIndex stage, const StageResolution& viewport) ETNoexceptHint {
		if (ET_UNLIKELY(!viewport.IsCompatibleWith(resolution))) {
			return false;
		}

		usages |= usage;
		firstRead  = Minimum(stage, firstRead);
		lastRead   = Maximum(stage, lastRead);
		firstWrite = Minimum(stage, firstWrite);
		lastWrite  = Maximum(stage, lastWrite);

		return true;
	}

	// ---------------------------------------------------

	ETInlineHint bool PipelineAttachment::TryMarkWritten(AttachmentUsage usage, StageIndex stage, const StageResolution& viewport) ETNoexceptHint {
		if (ET_UNLIKELY(!viewport.IsCompatibleWith(resolution))) {
			return false;
		}

		usages |= usage;
		firstWrite = Minimum(stage, firstWrite);
		lastWrite  = Maximum(stage, lastWrite);

		return true;
	}

	// ---------------------------------------------------

	ETInlineHint bool PipelineAttachment::TryMarkRead(AttachmentUsage usage, StageIndex stage, const StageResolution& viewport) ETNoexceptHint {
		if (ET_UNLIKELY(!viewport.IsCompatibleWith(resolution))) {
			return false;
		}

		usages |= usage;
		firstRead = Minimum(stage, firstRead);
		lastRead  = Maximum(stage, lastRead);

		return true;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint PipelineBuffer::PipelineBuffer(GpuSize byteSize, bool persistent) ETNoexceptHint : byteSize(byteSize),
																													 usages(UnusedBuffer),
																													 firstRead(StageIndex(persistent ? 0u : 0xFFFFFFFFu)),
																													 lastRead(StageIndex(persistent ? 0xFFFFFFFFu : 0u)),
																													 firstWrite(StageIndex(persistent ? 0u : 0xFFFFFFFFu)),
																													 lastWrite(StageIndex(persistent ? 0xFFFFFFFFu : 0u)) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint StageAttachment::StageAttachment(AttachmentIndex globalIndex, uint32 mip, uint32 firstSlice, uint32 lastSlice) ETNoexceptHint : globalIndex(globalIndex),
																																								  mip(mip),
																																								  firstSlice(firstSlice),
																																								  lastSlice(lastSlice),
																																								  shouldLoad(true),
																																								  shouldStore(false) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint StageAttachment::StageAttachment() ETNoexceptHint : StageAttachment(/*globalIndex =*/~0u, /*mip =*/0u, /*firstLayer =*/0u, /*layers =*/~0u) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint uint32 StageAttachment::GetArraySlices() const ETNoexceptHint {
		return lastSlice - firstSlice;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint StageBuilder::StageBuilder(Stage type, StringSpan name, StageResolution resolution, uint32 samplesPerPixel) ETNoexceptHint : type(type), resolution(resolution), samplesPerPixel(samplesPerPixel) {
		name.Copy(shader, ETCountOf(shader));
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ResolutionScaler::ResolutionScaler() ETNoexceptHint : _width(1u),
																						_height(1u),
																						_reciprocalTargetTime(1.0f),
																						_minWidth(1u),
																						_maxWidth(1u),
																						_minHeight(1u),
																						_maxHeight(1u) {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint uint32 ResolutionScaler::GetMaxWidth() const ETNoexceptHint {
		return _maxWidth;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint uint32 ResolutionScaler::GetMaxHeight() const ETNoexceptHint {
		return _maxHeight;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint uint32 ResolutionScaler::GetWidth() const ETNoexceptHint {
		return _width;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint uint32 ResolutionScaler::GetHeight() const ETNoexceptHint {
		return _height;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResolutionScaler::SetScale(float32 scale) ETNoexceptHint {
		scale   = Saturate(scale);
		_width  = Maximum(uint32(_maxWidth * scale), _minWidth);
		_height = Maximum(uint32(_maxHeight * scale), _minHeight);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResolutionScaler::UpdateTime(MicrosecondTime time, float32 /*tension*/) ETNoexceptHint {
		SetScale(Sqrt(uint64(time) * _reciprocalTargetTime));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResolutionScaler::ScaleBounds(const StageResolution& pass, uint32 baseWidth, uint32 baseHeight) ETNoexceptHint {
		static ETConstexpr auto CoerceAxis([](float32 axis) ETNoexceptHint {
			return Minimum(uint32(axis), 1u);
		});

		// ---

		if (pass.useFixed) {
			_width  = pass.fixed.width;
			_height = pass.fixed.height;
			ScaleBounds(pass.fixed.width, pass.fixed.height);
		} else {
			_width  = baseWidth;
			_height = baseHeight;
			ScaleBounds(CoerceAxis(pass.scale.minWidth * baseWidth), CoerceAxis(pass.scale.maxWidth * baseWidth), CoerceAxis(pass.scale.minHeight * baseHeight), CoerceAxis(pass.scale.maxHeight * baseHeight));
		}
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETForceInlineHint void ResolutionScaler::ScaleBounds(uint32 minWidth, uint32 maxWidth, uint32 minHeight, uint32 maxHeight) ETNoexceptHint {
		_minWidth  = minWidth;
		_maxWidth  = maxWidth;
		_minHeight = minHeight;
		_maxHeight = maxHeight;
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETForceInlineHint void ResolutionScaler::ScaleBounds(uint32 width, uint32 height) ETNoexceptHint {
		_minWidth = _maxWidth = width;
		_minHeight = _maxHeight = height;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint PipelineBuilder::AttachmentSliceType PipelineBuilder::GetAttachments() const ETNoexceptHint {
		return _attachments;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint PipelineBuilder::BufferSliceType PipelineBuilder::GetBuffers() const ETNoexceptHint {
		return _buffers;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint PipelineBuilder::StageSliceType PipelineBuilder::GetStages() const ETNoexceptHint {
		return _stages;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Result PipelineBuilder::DefineSharedAttachment(GpuFormat format, uint32 smallestMip, bool isPersistent) {
		_attachments.EmplaceBack(format, smallestMip, isPersistent);
		return Result::Success;
	}

	// ---------------------------------------------------

	template <typename... OptimizationPasses>
	ETInlineHint ETForceInlineHint PipelineBuilder& PipelineBuilder::Optimize(OptimizationPasses... passes) {
		DiscardArguments((passes(_attachments, _buffers, _stages), 0)...);
		return *this;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool operator==(const StageAttachment& lhs, const StageAttachment& rhs) ETNoexceptHint {
		return lhs.globalIndex == rhs.globalIndex;
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool operator!=(const StageAttachment& lhs, const StageAttachment& rhs) ETNoexceptHint {
		return !(lhs == rhs);
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint ETPureFunctionHint bool operator<(const StageAttachment& lhs, const StageAttachment& rhs) ETNoexceptHint {
		return lhs.globalIndex < rhs.globalIndex;
	}

}} // namespace Eldritch2::Graphics
