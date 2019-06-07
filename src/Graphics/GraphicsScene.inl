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

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	ETInlineHint ETForceInlineHint RenderLight::RenderLight(Transformation localToWorld, RgbColor color, float32 radius) ETNoexceptHint : localToWorld(localToWorld), color(color), radius(radius) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint RenderMesh::RenderMesh(const Animation::Armature& armature, const MeshSource& mesh) ETNoexceptHint : armature(ETAddressOf(armature)), mesh(ETAddressOf(mesh)) {}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint RenderView::RenderView(Transformation view, bool isMirror) ETNoexceptHint : viewTransform(view), isMirror(isMirror), viewport{ /*x =*/0.0f, /*y =*/1.0f, /*width =*/1.0f, /*height =*/0.0f } {}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint const GraphicsScene::LightConcept& GraphicsScene::GetInstances(LightType concept) const ETNoexceptHint {
		ETAssert(concept < LightTypes, "Concept must be valid!");
		ET_SUPPRESS_MSVC_WARNINGS(6385)
		return _lightConcepts[concept];
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint const GraphicsScene::MeshConcept& GraphicsScene::GetInstances(MeshType concept) const ETNoexceptHint {
		ETAssert(concept < MeshTypes, "Concept must be valid!");
		ET_SUPPRESS_MSVC_WARNINGS(6385)
		return _meshConcepts[concept];
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint GraphicsScene::LightConcept& GraphicsScene::GetInstances(LightType concept) ETNoexceptHint {
		ETAssert(concept < LightTypes, "Concept must be valid!");
		ET_SUPPRESS_MSVC_WARNINGS(6385)
		return _lightConcepts[concept];
	}

	// ---------------------------------------------------

	ETConstexpr ETForceInlineHint GraphicsScene::MeshConcept& GraphicsScene::GetInstances(MeshType concept) ETNoexceptHint {
		ETAssert(concept < MeshTypes, "Concept must be valid!");
		ET_SUPPRESS_MSVC_WARNINGS(6385)
		return _meshConcepts[concept];
	}

}} // namespace Eldritch2::Graphics
