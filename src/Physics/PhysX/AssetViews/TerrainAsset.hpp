/*==================================================================*\
  TerrainAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/PhysxPointer.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_SET_MSVC_WARNING_STATE(disable : 6326)
#include <PxShape.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	class TerrainAsset : public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref TerrainAsset instance.
		/*!	@param[in] assetPath Null-terminated, UTF-8-encoded character sequence containing the file system path to the asset
			the @ref TerrainAsset will be describing. */
		TerrainAsset(const Utf8Char* const assetPath);
		//!	Disable copy construction.
		TerrainAsset(const TerrainAsset&) = delete;

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
		static Utf8Literal GetExtension();

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
