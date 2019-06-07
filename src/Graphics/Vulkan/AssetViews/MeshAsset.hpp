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
#include <Core/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Graphics { namespace Vulkan { namespace AssetViews {

	class MeshAsset : public MeshSource, public Core::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		MeshAsset(const MeshAsset&) = delete;
		//!	Constructs this @ref MeshAsset instance.
		/*!	@param[in] path String view containing the file system path to the resource the @ref MeshAsset describes. */
		MeshAsset(StringSpan path) ETNoexceptHint;

		~MeshAsset() override = default;

		// ---------------------------------------------------

	public:
		Meshlet GetMeshlet(uint32 meshlet) const ETNoexceptHint override;

		MeshDescriptor DescribeSelf() const ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		void Stream(MeshElementRequest request) const ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		Result BindResources(Logging::Log& log, const Core::AssetBuilder& builder) override;

		void FreeResources() ETNoexceptHint override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringSpan GetExtension() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		MeshAsset& operator=(const MeshAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Meshlet> _meshlets;
	};

}}}} // namespace Eldritch2::Graphics::Vulkan::AssetViews

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/Vulkan/AssetViews/MeshAsset.inl>
//------------------------------------------------------------------//
