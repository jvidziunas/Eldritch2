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
#include <Graphics/ViewConcept.hpp>
#include <Graphics/PortalView.hpp>
#include <Graphics/RgbColor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scheduling {
		class	JobExecutor;
	}

	namespace Animation {
		class	Armature;
	}

	namespace Graphics {
		class	GeometrySource;
	}
}

namespace Eldritch2 {
namespace Graphics {

	enum class GeometryType {
		StaticMeshes,
		Meshes,

		COUNT
	};

// ---

	enum class LightType {
		StaticLights,
		DynamicLights,

		COUNT
	};

// ---

	enum class PortalViewType {
		PortalViews,

		COUNT
	};

// ---

	class Mesh {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Geometry instance.
		Mesh( const Animation::Armature& armature, const GeometrySource& source );
	//!	Constructs this @ref Geometry instance.
		Mesh( const Mesh& ) = default;

		~Mesh() = default;
		
	// - DATA MEMBERS ------------------------------------

	public:
		const Animation::Armature*	armature;
		const GeometrySource*		source;
	};

// ---

	class Light {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Light instance.
		Light( Transformation localToWorld, RgbColor color, float16 radius );
	//!	Constructs this @ref Light instance.
		Light( const Light& ) = default;

		~Light() = default;

	// - DATA MEMBERS ------------------------------------

	public:
		Transformation	localToWorld;
		RgbColor		color;
		float16			radius;
	};

// ---

	class GraphicsScene {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class LeafExtractor {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref LeafExtractor instance.
			LeafExtractor( const LeafExtractor& ) = default;
		//!	Constructs this @ref LeafExtractor instance.
			LeafExtractor( Vector cellLength );

			~LeafExtractor() = default;

		// ---------------------------------------------------

		public:
			uintptr	operator()( const Mesh& geometry ) const;
			uintptr	operator()( const Light& light ) const;

		// - DATA MEMBERS ------------------------------------

		private:
			Vector	_inverseCellLength;
		};

	// ---

	public:
		using PortalViewConcept	= ViewConcept<PortalView, MallocAllocator>;
		using GeometryConcept	= RenderConcept<Mesh, LeafExtractor, MallocAllocator>;
		using LightConcept		= RenderConcept<Light, LeafExtractor, MallocAllocator>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref GraphicsScene instance.
		GraphicsScene( Vector geometryCellExtent, Vector lightCellExtent );
	//!	Disable copy construction.
		GraphicsScene( const GraphicsScene& ) = delete;

		~GraphicsScene() = default;

	// ---------------------------------------------------

	public:
		const PortalViewConcept&	GetConcept( PortalViewType concept ) const;
		const GeometryConcept&		GetConcept( GeometryType concept ) const;
		const LightConcept&			GetConcept( LightType concept ) const;

	// ---------------------------------------------------

	public:
		void	BuildAccelerators( Scheduling::JobExecutor& executor );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		GraphicsScene&	operator=( const GraphicsScene& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		PortalViewConcept	_portalViewConcepts[PortalViewType::COUNT];
		GeometryConcept		_geometryConcepts[GeometryType::COUNT];
		LightConcept		_lightConcepts[LightType::COUNT];
	};

}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/GraphicsScene.inl>
//------------------------------------------------------------------//