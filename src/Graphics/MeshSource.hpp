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

	struct MeshElementRequest {
		uint32  stream;
		uint32  first;
		uint32  last;
		byte*   out;
	};

	// ---

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
		virtual MeshDescriptor DescribeSelf() const ETNoexceptHint abstract;

		virtual Meshlet GetMeshlet(uint32 meshlet) const ETNoexceptHint abstract;

		// ---------------------------------------------------

	public:
		virtual void Stream(MeshElementRequest request) const ETNoexceptHint abstract;
	};

}} // namespace Eldritch2::Graphics
