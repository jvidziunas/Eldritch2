/*==================================================================*\
  GraphicsScene.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/RenderConcept.hpp>
#include <Graphics/RgbColor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scheduling {
	class JobExecutor;
}

namespace Animation {
	class Armature;
}

namespace Graphics {
	template <typename Vertex>
	class MeshSource;
}
} // namespace Eldritch2

namespace Eldritch2 { namespace Graphics {

	enum LightType {
		StaticLights,
		DynamicLights,

		LightTypes
	};

	// ---

	enum PortalViewType {
		PortalViews,

		PortalViewTypes
	};

	// ---

	template <typename Vertex>
	class MeshInstance {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref MeshInstance instance.
		MeshInstance(const Animation::Armature& armature, const MeshSource<Vertex>& mesh) ETNoexceptHint;
		//!	Constructs this @ref MeshInstance instance.
		MeshInstance(const MeshInstance&) = default;

		~MeshInstance() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		const Animation::Armature* armature;
		const MeshSource<Vertex>*  mesh;
	};

	// ---

	class Light {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Light instance.
		Light(Transformation localToWorld, RgbColor color, float16 radius) ETNoexceptHint;
		//!	Constructs this @ref Light instance.
		Light(const Light&) = default;

		~Light() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		Transformation localToWorld;
		RgbColor       color;
		float16        radius;
	};

	// ---

	class PortalView {
	public:
		const MeshSource<StaticVertex>* mask;
		Plane                           portalPlane;
	};

	// ---

	class GraphicsScene {
		// - TYPE PUBLISHING ---------------------------------

	public:
		template <typename MortonCode>
		class LeafExtractor {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref LeafExtractor instance.
			LeafExtractor(const LeafExtractor&) ETNoexceptHint = default;
			//!	Constructs this @ref LeafExtractor instance.
			LeafExtractor(Vector cellLength) ETNoexceptHint;

			~LeafExtractor() = default;

			// ---------------------------------------------------

		public:
			MortonCode operator()(const Animation::Armature&) const ETNoexceptHint;
			MortonCode operator()(const Light& light) const ETNoexceptHint;

			// - DATA MEMBERS ------------------------------------

		private:
			Vector _reciprocalLength;
		};

		// ---

	public:
		template <typename Vertex>
		using GeometryConcept   = RenderConcept<MeshInstance<Vertex>, MallocAllocator>;
		using PortalViewConcept = ViewConcept<PortalView, MallocAllocator>;
		using LightConcept      = RenderConcept<Light, MallocAllocator>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref GraphicsScene instance.
		GraphicsScene(Vector geometryCellExtent, Vector lightCellExtent) ETNoexceptHint;
		//!	Disable copy construction.
		GraphicsScene(const GraphicsScene&) = delete;

		~GraphicsScene() = default;

		// ---------------------------------------------------

	public:
		const PortalViewConcept& GetConcept(PortalViewType concept) const ETNoexceptHint;
		const GeometryConcept&   GetConcept(GeometryType concept) const ETNoexceptHint;
		const LightConcept&      GetConcept(LightType concept) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void UpdateAccelerators(Scheduling::JobExecutor& executor);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GraphicsScene& operator=(const GraphicsScene&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Vector            _geometryCellExtent;
		Vector            _lightCellExtent;
		PortalViewConcept _portalViewConcepts[PortalViewTypes];
		GeometryConcept   _geometryConcepts[GeometryTypes];
		LightConcept      _lightConcepts[LightTypes];
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/GraphicsScene.inl>
//------------------------------------------------------------------//
