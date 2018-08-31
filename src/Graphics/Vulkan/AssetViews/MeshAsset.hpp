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

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	class MeshAsset : public MeshSource, public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		MeshAsset(const MeshAsset&) = delete;
		//!	Constructs this @ref MeshAsset instance.
		/*!	@param[in] path String view containing the file system path to the resource the @ref MeshAsset describes. */
		MeshAsset(StringView path);

		~MeshAsset() override = default;

		// ---------------------------------------------------

	public:
		SurfaceDescription GetSurface(uint32 surface) const ETNoexceptHint override;

		uint32 GetSurfaceCount() const ETNoexceptHint override;

		uint32 GetVerticesSize() const ETNoexceptHint override;

		uint32 GetIndicesSize() const ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		void Stream(const VertexStreamRequest& request) const ETNoexceptHint override;
		void Stream(const IndexStreamRequest& request) const ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		MeshAsset& operator=(const MeshAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Attribute>          _attributes;
		ArrayList<SurfaceDescription> _surfaces;
	};

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/AssetViews/MeshAsset.inl>
//------------------------------------------------------------------//
