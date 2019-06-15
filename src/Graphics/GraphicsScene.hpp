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
#include <Animation/AnimationApi.hpp>
#include <Graphics/RenderConcept.hpp>
#include <Graphics/RgbColor.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {
	class MeshSource;
}} // namespace Eldritch2::Graphics

namespace Eldritch2 { namespace Graphics {

	enum ViewType {
		Root,
		Portal,
		Mirror,
	};

	// ---

	using ViewTableTraits  = QueryableTableTraits<0u, Transformation /*localToWorld*/, Transformation /*targetToWorld*/, ViewType /*type*/>;
	using LightTableTraits = QueryableTableTraits<1u, Transformation /*localToWorld*/, RgbColor /*color*/, float32 /*radius*/>;
	using MeshTableTraits  = QueryableTableTraits<2u, Animation::Armature /*armature*/, const MeshSource* /*mesh*/>;

	// ---

	template <typename... Tables>
	class GraphicsScene {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref GraphicsScene instance.
		GraphicsScene(Vector meshCellExtent, Vector lightCellExtent) ETNoexceptHint;
		//!	Disable copy construction.
		GraphicsScene(const GraphicsScene&) = delete;

		~GraphicsScene() = default;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		GraphicsScene& operator=(const GraphicsScene&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Tuple<RenderConcept<Tables>...> _concepts;
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/GraphicsScene.inl>
//------------------------------------------------------------------//
