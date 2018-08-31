/*==================================================================*\
  TerrainAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  �2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxPointer.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <PxShape.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	class TerrainAsset : public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		TerrainAsset(const TerrainAsset&) = delete;
		//!	Constructs this @ref TerrainAsset instance.
		/*!	@param[in] path UTF-8-encoded string view containing the file system path to the asset
			the @ref TerrainAsset will be describing. */
		TerrainAsset(StringView path);

		~TerrainAsset() override = default;

		// ---------------------------------------------------

	public:
		physx::PxShape& GetShape() const;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint StringView GetExtension();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		TerrainAsset& operator=(const TerrainAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		PhysxPointer<physx::PxShape> _shape;
	};

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Physics/PhysX/AssetViews/TerrainAsset.inl>
//------------------------------------------------------------------//
