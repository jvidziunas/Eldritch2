/*==================================================================*\
  GpuAbi.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Flatbuffers/GraphicsTypes_generated.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics {

	using GpuPrimitive   = FlatBuffers::GpuPrimitive;
	using GpuFormat      = FlatBuffers::GpuFormat;
	using GpuSize        = uint64;
	using MeshIndex      = uint16;
	using MeshLargeIndex = uint32;

	// ---

	enum class PackedQuaternion : uint64 {
		Identity = 0x03
	};

	// ---

	enum class SurfacePrivacy : uint16 {
		DrawInAllViews,   //!< Surface should be drawn in all views, both the root and any children (mirrors, portals, etc.)
		DrawInRootView,   //!< Surface should be drawn only in the root view (typically first-person geometry)
		DrawInChildViews, //!< Surface should be drawn only in child views, not the root (inverse of root-only)
	};

	// ---

	struct ImageTile {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : uint32 {
			PageCoordinateBits  = 18u,
			MaxImageDimension   = 1u << PageCoordinateBits,
			PageSubresourceBits = 10u
		};

	public:
		uint64 x : PageCoordinateBits;
		uint64 y : PageCoordinateBits;
		uint64 z : PageCoordinateBits;
		uint64 subresource : PageSubresourceBits;
	};

	// ---

	enum MeshStream : uint32 {
		Indices   = 0,
		Vertices0 = 1,
	};

	// ---

	struct ImageDescriptor {
		GpuFormat format;
		uint32    smallestMip;
		uint32    slices;
		struct {
			uint32 width;
			uint32 height;
			uint32 depth;
		} size;
	};

	// ---

	struct Meshlet {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Meshlet instance.
		Meshlet(GpuPrimitive primitive, MeshLargeIndex primitiveCount, MeshLargeIndex firstIndex, StringSpan pipeline) ETNoexceptHint;
		//!	Constructs this @ref Meshlet instance.
		Meshlet(const Meshlet&) ETNoexceptHint = default;

		~Meshlet() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		GpuPrimitive   primitive;
		MeshLargeIndex primitiveCount;
		MeshLargeIndex firstIndex;
		Utf8Char       pipeline[64];
	};

	// ---

	struct MeshDescriptor {
		uint32  meshletCount;
		GpuSize streamSizes[8];
	};

	// ---

	struct SkinnedVertex {
		float32          position[4];
		PackedQuaternion tangentToLocal;
		uint16           texcoords[2][2];
		uint8            bones[2][4];
		uint8            weights[2][4];
	};

	// ---

	struct StaticVertex {
		float32          position[4];
		PackedQuaternion tangentToLocal;
		uint16           texcoords[2][2];
	};

	// ---

	struct UiVertex {
		uint16 position[4];
		uint16 texcoords[2][2];
	};

}} // namespace Eldritch2::Graphics

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/GpuAbi.inl>
//------------------------------------------------------------------//
