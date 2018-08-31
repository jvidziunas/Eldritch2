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
/*	(6297) MSVC doesn't like how 32-bit values are shifted before being cast to a 64-bit value. For Morton code generation this is completely benign;
 *	the warning is built around the presumption that the shift is great enough where bits would spill out of the smaller intermediate value. */
ET_PUSH_MSVC_WARNING_STATE(disable : 6297);
#include <morton.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	ETInlineHint ETForceInlineHint MeshInstance::MeshInstance(const Animation::Armature& armature, const MeshSource& source) :
		source(ETAddressOf(source)),
		armature(ETAddressOf(armature)) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Light::Light(Transformation localToWorld, RgbColor color, float16 radius) :
		localToWorld(localToWorld),
		color(color),
		radius(radius) {}

	// ---------------------------------------------------

	template <typename Code>
	ETInlineHint ETForceInlineHint GraphicsScene::LeafExtractor<Code>::LeafExtractor(Vector cellLength) :
		_reciprocalLength(Vector(1.0f, 1.0f, 1.0f, 1.0f) / cellLength) {}

	// ---------------------------------------------------

	template <typename Code>
	ETInlineHint ETForceInlineHint Code GraphicsScene::LeafExtractor<Code>::operator()(const MeshInstance& mesh) const ETNoexceptHint {
		float32 position[4];

		(mesh.armature->GetLocalToWorld().translation * _reciprocalLength).ExtractCoefficients(position);
		return m3D_e_sLUT<Code>(AsInt(position[0]), AsInt(position[1]), AsInt(position[2]));
	}

	// ---------------------------------------------------

	template <typename Code>
	ETInlineHint ETForceInlineHint Code GraphicsScene::LeafExtractor<Code>::operator()(const Light& light) const ETNoexceptHint {
		float32 position[4];

		(light.localToWorld.translation * _reciprocalLength).ExtractCoefficients(position);
		return m3D_e_sLUT<Code>(AsInt(position[0]), AsInt(position[1]), AsInt(position[2]));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const GraphicsScene::PortalViewConcept& GraphicsScene::GetConcept(PortalViewType concept) const ETNoexceptHint {
		ET_ASSERT(concept < PortalViewTypes, "Concept must be valid!");
		ET_SUPPRESS_MSVC_WARNINGS(6385)
		return _portalViewConcepts[concept];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const GraphicsScene::GeometryConcept& GraphicsScene::GetConcept(GeometryType concept) const ETNoexceptHint {
		ET_ASSERT(concept < GeometryTypes, "Concept must be valid!");
		ET_SUPPRESS_MSVC_WARNINGS(6385)
		return _geometryConcepts[concept];
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint const GraphicsScene::LightConcept& GraphicsScene::GetConcept(LightType concept) const ETNoexceptHint {
		ET_ASSERT(concept < LightTypes, "Concept must be valid!");
		ET_SUPPRESS_MSVC_WARNINGS(6385)
		return _lightConcepts[concept];
	}

}} // namespace Eldritch2::Graphics
