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
#include <Graphics/GpuFormats.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	class ETPureAbstractHint MeshSource {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct SurfaceDescription {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref SurfaceDescription instance.
			SurfaceDescription(StringView pipeline, GpuPrimitive type, uint32 firstIndex, uint32 primitiveCount, Bounds bounds) ETNoexceptHint;
			//!	Constructs this @ref SurfaceDescription instance.
			SurfaceDescription(const SurfaceDescription&) ETNoexceptHint = default;

			~SurfaceDescription() = default;

			// - DATA MEMBERS ------------------------------------

		public:
			Utf8Char     pipelineName[64u];
			GpuPrimitive type;
			uint32       firstIndex;
			uint32       primitiveCount;
			Bounds       bounds;
		};

		// ---

	public:
		struct Description {
			uint32 surfaceCount;
			uint32 verticesSize;
			uint32 indicesSize;
		};

		// ---

	public:
		struct StreamRequest {
			uint32 firstElement;
			uint32 byteStride;
			void*  target;
		};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref MeshSource instance.
		MeshSource(const MeshSource&) = default;
		//!	Constructs this @ref MeshSource instance.
		MeshSource() = default;

		~MeshSource() = default;

		// ---------------------------------------------------

	public:
		virtual SurfaceDescription GetSurface(uint32 surface) const ETNoexceptHint abstract;

		virtual Description GetDescription() const ETNoexceptHint abstract;

		// ---------------------------------------------------

	public:
		virtual void StreamVertices(const StreamRequest& request) const ETNoexceptHint abstract;

		virtual void StreamIndices(const StreamRequest& request) const ETNoexceptHint abstract;
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/MeshSource.inl>
//------------------------------------------------------------------//
