/*==================================================================*\
  GraphicsScene.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Animation/Armature.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
/*	(6297) MSVC doesn't like how 32-bit values are shifted before being cast to a 64-bit value. For Morton code generation this is completely benign;
 *	the warning is built around the presumption that the shift is great enough where bits would spill out of the smaller intermediate value. */
ET_SET_MSVC_WARNING_STATE(disable : 6297);
#include <morton.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	ETInlineHint MeshInstance::MeshInstance(const Animation::Armature& armature, const MeshSource& source) :
		source(&source),
		armature(&armature) {}

	// ---------------------------------------------------

	ETInlineHint Light::Light(Transformation localToWorld, RgbColor color, float16 radius) :
		localToWorld(localToWorld),
		color(color),
		radius(radius) {}

	// ---------------------------------------------------

	ETInlineHint GraphicsScene::LeafExtractor::LeafExtractor(Vector cellLength) :
		_inverseCellLength(Vector(1.0f, 1.0f, 1.0f, 1.0f) / cellLength) {}

	// ---------------------------------------------------

	ETInlineHint uintptr GraphicsScene::LeafExtractor::operator()(const MeshInstance& geometry) const {
		float32 scaledPosition[4];

		(_inverseCellLength * geometry.armature->GetLocalToWorld().translation).ExtractCoefficients(scaledPosition);

		return m3D_e_sLUT<uintptr, int32>(AsInt(scaledPosition[0]), AsInt(scaledPosition[1]), AsInt(scaledPosition[2]));
	}

	// ---------------------------------------------------

	ETInlineHint uintptr GraphicsScene::LeafExtractor::operator()(const Light& light) const {
		float32 scaledPosition[4];

		(_inverseCellLength * light.localToWorld.translation).ExtractCoefficients(scaledPosition);

		return m3D_e_sLUT<uintptr, int32>(AsInt(scaledPosition[0]), AsInt(scaledPosition[1]), AsInt(scaledPosition[2]));
	}

	// ---------------------------------------------------

	ETInlineHint const GraphicsScene::PortalViewConcept& GraphicsScene::GetConcept(PortalViewType concept) const {
		ET_ASSERT(concept < PortalViewType::COUNT, "Concept must be valid!");

		return _portalViewConcepts[static_cast<size_t>(concept)];
	}

	// ---------------------------------------------------

	ETInlineHint const GraphicsScene::GeometryConcept& GraphicsScene::GetConcept(GeometryType concept) const {
		ET_ASSERT(concept < GeometryType::COUNT, "Concept must be valid!");

		return _geometryConcepts[static_cast<size_t>(concept)];
	}

	// ---------------------------------------------------

	ETInlineHint const GraphicsScene::LightConcept& GraphicsScene::GetConcept(LightType concept) const {
		ET_ASSERT(concept < LightType::COUNT, "Concept must be valid!");

		return _lightConcepts[static_cast<size_t>(concept)];
	}

}} // namespace Eldritch2::Graphics
