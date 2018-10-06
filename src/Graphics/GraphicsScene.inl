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

namespace Eldritch2 { namespace Graphics {

	ETInlineHint ETForceInlineHint MeshInstance::MeshInstance(const Animation::Armature& armature, const MeshSource& source) ETNoexceptHint : source(ETAddressOf(source)), armature(ETAddressOf(armature)) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint Light::Light(Transformation localToWorld, RgbColor color, float16 radius) ETNoexceptHint : localToWorld(localToWorld),
																															  color(color),
																															  radius(radius) {}

	// ---------------------------------------------------

	template <typename MortonCode>
	ETInlineHint ETForceInlineHint GraphicsScene::LeafExtractor<MortonCode>::LeafExtractor(Vector cellLength) ETNoexceptHint : _reciprocalLength(~cellLength) {}

	// ---------------------------------------------------

	template <typename MortonCode>
	ETInlineHint ETForceInlineHint MortonCode GraphicsScene::LeafExtractor<MortonCode>::operator()(const Animation::Armature& armature) const ETNoexceptHint {
		return GetMortonCode<MortonCode>(armature.GetLocalToWorld().translation * _reciprocalLength);
	}

	// ---------------------------------------------------

	template <typename MortonCode>
	ETInlineHint ETForceInlineHint MortonCode GraphicsScene::LeafExtractor<MortonCode>::operator()(const Light& light) const ETNoexceptHint {
		return GetMortonCode<MortonCode>(light.localToWorld.translation * _reciprocalLength);
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
