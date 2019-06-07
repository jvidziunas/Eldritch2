/*==================================================================*\
  GraphicsScene.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	GraphicsScene::GraphicsScene(Vector meshCellExtent, Vector lightCellExtent) ETNoexceptHint :
		_meshConcepts {
			{ MallocAllocator("Scene Mesh Concept Allocator"), MeshConcept::SortType(meshCellExtent) },
			{ MallocAllocator("Scene Mesh Concept Allocator"), MeshConcept::SortType(meshCellExtent) },
		},
		_lightConcepts {
			{ MallocAllocator("Scene Light Concept Allocator"), LightConcept::SortType(lightCellExtent) },
			{ MallocAllocator("Scene Light Concept Allocator"), LightConcept::SortType(lightCellExtent) }
		} {}

}} // namespace Eldritch2::Graphics
