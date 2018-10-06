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

	using GpuPrimitive = ::Eldritch2::Graphics::FlatBuffers::GpuPrimitive;
	using GpuFormat    = ::Eldritch2::Graphics::FlatBuffers::GpuFormat;
	using GpuSize      = uint64;
	using MeshIndex    = uint16;

	// ---

	struct MeshSurface {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref MeshSurface instance.
		MeshSurface(GpuPrimitive type, uint32 firstIndex, uint32 primitiveCount, StringView pipeline) ETNoexceptHint;
		//!	Constructs this @ref MeshSurface instance.
		MeshSurface(const MeshSurface&) ETNoexceptHint = default;

		~MeshSurface() = default;

		// - DATA MEMBERS ------------------------------------

	public:
		GpuPrimitive type;
		uint32       firstIndex;
		uint32       primitiveCount;
		Utf8Char     pipeline[64];
	};

	// ---

	struct MeshDescription {
		uint32  surfaceCount;
		GpuSize verticesByteSize;
		GpuSize indicesByteSize;
	};

	// ---

	struct SkinnedVertex {
		float32 positon[4];
		int16   objectToTangent[4];
		uint16  texcoords[2][2];
		uint8   bones[4][2];
		uint8   weights[4][2];
	};

	// ---

	struct StaticVertex {
		float32 positon[4];
		int16   objectToTangent[4];
		uint16  texcoords[2][2];
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
