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
