/*==================================================================*\
  GraphicsScene.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scheduling/JobExecutor.hpp>
#include <Graphics/GraphicsScene.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	using namespace ::Eldritch2::Scheduling;

	GraphicsScene::GraphicsScene(Vector geometryCellExtent, Vector lightCellExtent) :
		_geometryCellExtent(geometryCellExtent),
		_lightCellExtent(lightCellExtent),
		_portalViewConcepts {
			{ MallocAllocator("Scene Portal View Concept Allocator") }
		},
		_geometryConcepts {
			{ MallocAllocator("Scene Geometry Concept Allocator") },
			{ MallocAllocator("Scene Geometry Concept Allocator") },
		},
		_lightConcepts {
			{ MallocAllocator("Scene Light Concept Allocator") },
			{ MallocAllocator("Scene Light Concept Allocator") }
		} {
	}

	// ---------------------------------------------------

	void GraphicsScene::UpdateAccelerators(JobExecutor& executor) {
		ET_PROFILE_SCOPE("Graphics", "Build Acceleration Structures", 0xFFFFFF);
		executor.AwaitWork(
			[this](JobExecutor& executor) {
				executor.ForEach<1u>(Begin(_portalViewConcepts), End(_portalViewConcepts), [this](JobExecutor& executor, PortalViewConcept& concept) {
					ET_PROFILE_SCOPE("Graphics/AccelerationStructures", "Update Portal View Hierarchy", 0xFFAAFF);
				});
			},
			[this](JobExecutor& executor) {
				executor.ForEach<1u>(Begin(_geometryConcepts), End(_geometryConcepts), [this](JobExecutor& executor, GeometryConcept& concept) {
					ET_PROFILE_SCOPE("Graphics/AccelerationStructures", "Update Geometry Hierarchy", 0xFFAAFF);
					if (!concept.ShouldRebuildHierarchy()) {
						return;
					}
					Rebuild(executor, concept, LeafExtractor<uint64>(_geometryCellExtent));
				});
			},
			[this](JobExecutor& executor) {
				executor.ForEach<1u>(Begin(_lightConcepts), End(_lightConcepts), [this](JobExecutor& executor, LightConcept& concept) {
					ET_PROFILE_SCOPE("Graphics/AccelerationStructures", "Update Light Hierarchy", 0xFFAAFF);
					if (!concept.ShouldRebuildHierarchy()) {
						return;
					}
					Rebuild(executor, concept, LeafExtractor<uint64>(_lightCellExtent));
				});
			});
	}

}} // namespace Eldritch2::Graphics
