/*==================================================================*\
  TerrainAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// PRECOMPILED HEADER
//==================================================================//
#include <Common/Precompiled.hpp>
//------------------------------------------------------------------//

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/TerrainAsset.hpp>
#include <Physics/PhysX/SpanInputStream.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <geometry/PxHeightFieldDesc.h>
#include <geometry/PxHeightField.h>
#include <PxRigidActor.h>
#include <PxMaterial.h>
#include <PxPhysics.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::physx;

	// ---------------------------------------------------

	TerrainAsset::TerrainAsset(StringSpan path) ETNoexceptHint : Asset(path) {}

	// ---------------------------------------------------

	Result TerrainAsset::BindResources(Log& log, const AssetBuilder& asset) {
		static ETConstexpr PxReal HeightScale(256.0f);
		static ETConstexpr PxReal ColumnScale(4.0f);
		static ETConstexpr PxReal RowScale(4.0f);

		SpanInputStream          input(asset.bytes);
		PhysxPointer<PxHeightField> heightfield(PxGetPhysics().createHeightField(input));
		ET_ABORT_UNLESS(heightfield ? Result::Success : Result::Unspecified, log, "Unable to create PhysX heightfield for asset '{}'!" ET_NEWLINE, GetPath());

		PhysxPointer<PxMaterial> material(PxGetPhysics().createMaterial(1.0f, 1.0f, 0.1f));
		ET_ABORT_UNLESS(material ? Result::Success : Result::Unspecified, log, "Unable to create PhysX material for asset '{}'!" ET_NEWLINE, GetPath());

		PhysxPointer<PxShape> shape(PxGetPhysics().createShape(PxHeightFieldGeometry(heightfield.Get(), PxMeshGeometryFlags(0), HeightScale, RowScale, ColumnScale), *material));
		ET_ABORT_UNLESS(shape ? Result::Success : Result::Unspecified, log, "Unable to create PhysX shape for asset '{}'!" ET_NEWLINE, GetPath());

		Swap(_shape, shape);

		return Result::Success;
	}

	// ---------------------------------------------------

	void TerrainAsset::FreeResources() ETNoexceptHint {
		_shape.Reset();
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan TerrainAsset::GetExtension() ETNoexceptHint {
		return ".e2terrain";
	}

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
