/*==================================================================*\
  MeshAsset.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Graphics/MeshSource.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Graphics {
		namespace Vulkan {
			class	Vulkan;
		}
	}
}

namespace Eldritch2 {
namespace Graphics {
namespace Vulkan {
namespace AssetViews {

	class MeshAsset : public Assets::Asset, public MeshSource {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum Attribute {

		};

	// ---

	public:
		class Surface {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Surface instance.
			Surface(
				const Utf8Char* const pipeline,
				uint32 indexOffset,
				uint32 faceCount,
				uint16 indicesPerFace,
				Bounds bounds
			);
		//!	Constructs this @ref Surface instance.
			Surface(const Surface&) = default;

			~Surface() = default;

		// - DATA MEMBERS ------------------------------------

		public:
			Utf8Char	pipelineName[MaxPathLength];
			uint32		indexOffset;
			uint32		faceCount;
			uint16		indicesPerFace;
			Bounds		bounds;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref MeshAsset instance.
	/*!	@param[in] filePath Null-terminated, UTF-8-encoded file system path to the resource this @ref MeshAsset describes. */
		MeshAsset(const Utf8Char* const filePath);
	//!	Disable copy construction.
		MeshAsset(const MeshAsset&) = delete;

		~MeshAsset() override = default;

	// ---------------------------------------------------

	public:
		Dimensions	GetDimensions() const override;

		void		Stream(const VertexStreamRequest& vertices, const IndexStreamRequest& indices) const override;

	// ---------------------------------------------------

	public:
		const ArrayList<Attribute>&	GetAttributes() const;

		const ArrayList<Surface>&	GetSurfaces() const;

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources(const Builder& builder) override;

		void		FreeResources() override;

	// ---------------------------------------------------

	public:
		static Utf8Literal	GetExtension();

	// ---------------------------------------------------

	//!	Disable copy assignment.
		MeshAsset&	operator=(const MeshAsset&) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Attribute>	_attributes;
		ArrayList<Surface>		_surfaces;
	};

}	// namespace AssetViews
}	// namespace Vulkan
}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/AssetViews/MeshAsset.inl>
//------------------------------------------------------------------//