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
#include <microprofile/microprofile.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {

		using namespace ::Eldritch2::Scheduling;

		GraphicsScene::GraphicsScene(
			Vector geometryCellExtent,
			Vector lightCellExtent
		) : _geometryCellExtent(geometryCellExtent),
			_lightCellExtent(lightCellExtent),
			_portalViewConcepts{
				{ MallocAllocator("Scene Portal View Concept Allocator") }
		},
			_geometryConcepts{
				{ MallocAllocator("Scene Geometry Concept Allocator") },
				{ MallocAllocator("Scene Geometry Concept Allocator") },
		},
		_lightConcepts{
			{ MallocAllocator("Scene Light Concept Allocator") },
			{ MallocAllocator("Scene Light Concept Allocator") }
		} {
		}

// ---------------------------------------------------

		void GraphicsScene::BuildAccelerators(JobExecutor& executor) {
			MICROPROFILE_SCOPEI("Graphics", "Build Acceleration Structures", 0xFFFFFF);

			executor.AwaitWork(
				[this](JobExecutor& /*executor*/) {
				for (PortalViewConcept& concept : _portalViewConcepts) {}
			},
				[this](JobExecutor& executor) {
				for (GeometryConcept& concept : _geometryConcepts) {
					if (concept.ShouldRebuildHierarchy()) {
						MICROPROFILE_SCOPEI("Graphics/AccelerationStructures", "Build Geometry Hierarchy", 0xFFAAFF);
						Rebuild(executor, concept, LeafExtractor(_geometryCellExtent));
					}
				}
			},
				[this](JobExecutor& executor) {
				for (LightConcept& concept : _lightConcepts) {
					if (concept.ShouldRebuildHierarchy()) {
						MICROPROFILE_SCOPEI("Graphics/AccelerationStructures", "Build Light Hierarchy", 0xFFAAFF);
						Rebuild(executor, concept, LeafExtractor(_lightCellExtent));
					}
				}
			}
			);
		}

	}	// namespace Graphics
}	// namespace Eldritch2