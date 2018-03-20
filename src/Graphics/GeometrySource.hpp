/*==================================================================*\
  GeometrySource.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/GpuFormats.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {

	class ETPureAbstractHint GeometrySource {
	// - TYPE PUBLISHING ---------------------------------

	public:
		struct Dimensions {
			uint32	vertexCount;
			uint32	indexCount;
		};

	// ---

		struct VertexStreamRequest {
			size_t	elementStrideInBytes;
			uint32	first;
			uint32	last;

			void*	target;
		};

	// ---

		struct IndexStreamRequest {
			uint32	first;
			uint32	last;

			void*	target;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref GeometrySource instance.
		GeometrySource( const GeometrySource& ) = default;
	//!	Constructs this @ref GeometrySource instance.
		GeometrySource() = default;

		~GeometrySource() = default;

	// ---------------------------------------------------

	public:
		virtual Dimensions	GetDimensions() const abstract;

		virtual void		Stream( const VertexStreamRequest& vertices, const IndexStreamRequest& indices ) const abstract;
	};

}	// namespace Graphics
}	// namespace Eldritch2