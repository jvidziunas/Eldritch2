/*==================================================================*\
  MeshSource.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/GpuAbi.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	template <typename OutputIterator>
	struct MeshElementRequest {
		OutputIterator out;
		uint32         first;
		uint32         last;
	};

	// ---

	template <typename Vertex>
	class ETPureAbstractHint MeshSource {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref MeshSource instance.
		ETConstexpr MeshSource(const MeshSource&) ETNoexceptHint = default;
		//!	Constructs this @ref MeshSource instance.
		ETConstexpr MeshSource() ETNoexceptHint = default;

		~MeshSource() = default;

		// ---------------------------------------------------

	public:
		virtual MeshDescription GetDescription() const ETNoexceptHint abstract;

		virtual MeshSurface GetSurface(uint32 surface) const ETNoexceptHint abstract;

		// ---------------------------------------------------

	public:
		virtual void Stream(MeshElementRequest<StridingIterator<Vertex>> request) const ETNoexceptHint abstract;

		virtual void Stream(MeshElementRequest<MeshIndex*> request) const ETNoexceptHint abstract;
	};

}} // namespace Eldritch2::Graphics
