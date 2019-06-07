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
namespace Animation {
	class Armature;
} // namespace Animation

namespace Graphics {
	class MeshSource;
} // namespace Graphics
} // namespace Eldritch2

namespace Eldritch2 { namespace Graphics {

	enum MeshType {
		WorldMesh,
		DynamicMesh,

		MeshTypes
	};

	// ---

	enum LightType {
		WorldLight,
		DynamicLight,

		LightTypes
	};

	// ---

	class RenderMesh {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RenderMesh instance.
		RenderMesh(const Animation::Armature& armature, const MeshSource& mesh) ETNoexceptHint;
		//!	Constructs this @ref RenderMesh instance.
		RenderMesh(const RenderMesh&) = default;

		~RenderMesh() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		const Animation::Armature* armature;
		const MeshSource*          mesh;
	};

	// ---

	class RenderLight {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RenderLight instance.
		RenderLight(Transformation localToWorld, RgbColor color, float32 radius) ETNoexceptHint;
		//!	Constructs this @ref RenderLight instance.
		RenderLight(const RenderLight&) = default;

		~RenderLight() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		Transformation localToWorld;
		RgbColor       color;
		float32        radius;
	};

	// ---

	class RenderView {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref RenderView instance.
		RenderView(Transformation view, bool isMirror) ETNoexceptHint;
		//!	Constructs this @ref RenderView instance.
		RenderView(const RenderView&) ETNoexceptHint = default;

		~RenderView() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		Transformation viewTransform;
		bool           isMirror;
		struct {
			float32 x, y, width, height;
		} viewport;
	};

	// ---

	class GraphicsScene {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using LightConcept = RenderConcept<RenderLight>;
		using MeshConcept  = RenderConcept<RenderMesh>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref GraphicsScene instance.
		GraphicsScene(Vector meshCellExtent, Vector lightCellExtent) ETNoexceptHint;
		//!	Disable copy construction.
		GraphicsScene(const GraphicsScene&) = delete;

		~GraphicsScene() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr const LightConcept& GetInstances(LightType concept) const ETNoexceptHint;
		ETConstexpr const MeshConcept& GetInstances(MeshType concept) const ETNoexceptHint;
		ETConstexpr LightConcept& GetInstances(LightType concept) ETNoexceptHint;
		ETConstexpr MeshConcept& GetInstances(MeshType concept) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GraphicsScene& operator=(const GraphicsScene&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		MeshConcept  _meshConcepts[MeshTypes];
		LightConcept _lightConcepts[LightTypes];
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/GraphicsScene.inl>
//------------------------------------------------------------------//
