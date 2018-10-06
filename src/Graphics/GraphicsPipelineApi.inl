/*==================================================================*\
  GraphicsPipelineApi.inl
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

	ETConstexpr ETInlineHint ETForceInlineHint PassDimensions::PassDimensions(float32 minWidth, float32 maxWidth, float32 minHeight, float32 maxHeight) ETNoexceptHint : useFixed(false), scale { minWidth, maxWidth, minHeight, maxHeight } {}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint PassDimensions::PassDimensions(uint32 width, uint32 height, uint32 depth) ETNoexceptHint : useFixed(true), fixed { width, height, depth } {}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint PassDimensions::PassDimensions(float32 width, float32 height) ETNoexceptHint : PassDimensions(/*minWidth =*/width, /*maxWidth =*/width, /*minHeight =*/height, /*maxHeight =*/height) {}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint bool PassDimensions::IsCompatible(const PassDimensions& rhs) const ETNoexceptHint {
		return useFixed == rhs.useFixed && // clang-format off
			(useFixed ? ((fixed.width == rhs.fixed.width) & (fixed.height == rhs.fixed.height) & (fixed.depth == rhs.fixed.depth)) :
			((scale.maxWidth == rhs.scale.maxWidth) & (scale.maxHeight == rhs.scale.maxHeight))); // clang-format on
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint uint32 PassDimensions::GetMaxWidth(uint32 base) const ETNoexceptHint {
		return useFixed ? fixed.width : uint32(scale.maxWidth * base);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint uint32 PassDimensions::GetMaxHeight(uint32 base) const ETNoexceptHint {
		return useFixed ? fixed.height : uint32(scale.maxHeight * base);
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint uint32 PassDimensions::GetMaxDepth(uint32 base) const ETNoexceptHint {
		return useFixed ? fixed.depth : base;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint PipelineAttachmentReference::PipelineAttachmentReference(uint32 globalIndex, uint32 firstLayer, uint32 layers) ETNoexceptHint : globalIndex(globalIndex), firstLayer(firstLayer), layers(layers) {}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint PipelineAttachmentReference::PipelineAttachmentReference() ETNoexceptHint : PipelineAttachmentReference(/*globalIndex =*/~0u, /*firstLayer =*/0u, /*layers =*/~0u) {}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint PipelineFramebufferDescription::PipelineFramebufferDescription(PassDimensions resolution, uint32 samplesPerPixel) ETNoexceptHint : resolution(resolution), samplesPerPixel(samplesPerPixel) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint PipelinePassDescription::PipelinePassDescription(PassType type, PassDimensions resolution, uint32 samplesPerPixel, StringView name) ETNoexceptHint : type(type), framebuffer(resolution, samplesPerPixel) {
		name.Copy(shader, ETCountOf(shader));
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint ResolutionScaler::ResolutionScaler() ETNoexceptHint : _width(1u),
																									 _height(1u),
																									 _reciprocalTargetTimeMicroseconds(1.0f),
																									 _minWidth(1u),
																									 _maxWidth(1u),
																									 _minHeight(1u),
																									 _maxHeight(1u) {}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint uint32 ResolutionScaler::GetMaxWidth() const ETNoexceptHint {
		return _maxWidth;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint uint32 ResolutionScaler::GetMaxHeight() const ETNoexceptHint {
		return _maxHeight;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint uint32 ResolutionScaler::GetCurrentWidth() const ETNoexceptHint {
		return _width;
	}

	// ---------------------------------------------------

	ETConstexpr ETInlineHint ETForceInlineHint uint32 ResolutionScaler::GetCurrentHeight() const ETNoexceptHint {
		return _height;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResolutionScaler::SetScale(float32 scale) ETNoexceptHint {
		scale   = Saturate(scale);
		_width  = Max(uint32(_maxWidth * scale), _minWidth);
		_height = Max(uint32(_maxHeight * scale), _minHeight);
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResolutionScaler::UpdateTime(uint64 timeInMicroseconds, float32 /*tightness*/) ETNoexceptHint {
		SetScale(Sqrt(timeInMicroseconds * _reciprocalTargetTimeMicroseconds));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint void ResolutionScaler::SetBounds(const PassDimensions& pass, uint32 baseWidth, uint32 baseHeight) ETNoexceptHint {
		const auto CoerceAxis([](float32 axis) { return Min(uint32(axis), 1u); });
		if (pass.useFixed) {
			_width  = pass.fixed.width;
			_height = pass.fixed.height;
			SetBounds(pass.fixed.width, pass.fixed.height);
		} else {
			_width  = baseWidth;
			_height = baseHeight;
			SetBounds(CoerceAxis(pass.scale.minWidth * baseWidth), CoerceAxis(pass.scale.maxWidth * baseWidth), CoerceAxis(pass.scale.minHeight * baseHeight), CoerceAxis(pass.scale.maxHeight * baseHeight));
		}
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETInlineHint ETForceInlineHint void ResolutionScaler::SetBounds(uint32 minWidth, uint32 maxWidth, uint32 minHeight, uint32 maxHeight) ETNoexceptHint {
		_minWidth  = minWidth;
		_maxWidth  = maxWidth;
		_minHeight = minHeight;
		_maxHeight = maxHeight;
	}

	// ---------------------------------------------------

	ETCpp14Constexpr ETInlineHint ETForceInlineHint void ResolutionScaler::SetBounds(uint32 width, uint32 height) ETNoexceptHint {
		_minWidth = _maxWidth = width;
		_minHeight = _maxHeight = height;
	}

}} // namespace Eldritch2::Graphics
