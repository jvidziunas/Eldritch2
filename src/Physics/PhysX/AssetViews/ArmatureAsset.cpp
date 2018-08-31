/*==================================================================*\
  ArmatureAsset.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Physics/PhysX/AssetViews/ArmatureAsset.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/Armature_generated.h>
//------------------------------------------------------------------//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <PxArticulation.h>
#include <PxPhysics.h>
#include <PxScene.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	using namespace ::Eldritch2::Animation::FlatBuffers;
	using namespace ::Eldritch2::FlatBuffers;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;
	using namespace ::physx;

	namespace {

		ETInlineHint ETPureFunctionHint PxQuat Decompress(const CompressedUnitQuaternion& orientation) {
			return PxQuat(PxIdentity);
		}

		// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint PxTransform AsPxTransform(const Bone* pose) {
			const auto& translation(pose->Translation());
			return { translation.X(), translation.Y(), translation.Z(), Decompress(pose->Orientation()) };
		}

	} // anonymous namespace

	ArmatureAsset::ArmatureAsset(StringView path) :
		Asset(path),
		_joints(MallocAllocator("PhysX Armature Asset Joint Definition Allocator")) {}

	// ---------------------------------------------------

	PhysxPointer<PxArticulation> ArmatureAsset::CreateArticulation(PxScene& scene) const {
		const auto articulation(scene.getPhysics().createArticulation());
		const auto links(ETStackAlloc(PxArticulationLink*, _joints.GetSize() + 1u));
		uint32     linkId(1u);

		for (const Joint& joint : _joints) {
			links[linkId++] = articulation->createLink(links[joint.parentIndex], joint.bindPose);
		}

		scene.addArticulation(*articulation);

		return articulation;
	}

	// ---------------------------------------------------

	ErrorCode ArmatureAsset::BindResources(const Builder& asset) {
		//	Ensure we're working with data that can plausibly represent an armature.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyArmatureBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		const auto armature(GetArmature(asset.Begin()));
		const auto descriptors(armature->Joints());

		ArrayList<Joint> joints(_joints.GetAllocator(), descriptors->size());
		for (uoffset_t id(0u); id < descriptors->size(); ++id) {
			const auto joint(descriptors->Get(id));
			if (descriptors->size() <= joint->ParentIndex()) {
				asset.WriteLog(Severity::Error, "Armature {} joint {} references out-of-bounds parent {}!" ET_NEWLINE, GetPath(), id, joint->ParentIndex());
			}

			joints.Append({ AsPxTransform(joint->BindPose()), joint->ParentIndex() });
		}

		Swap(_joints, joints);

		return Error::None;
	}

	// ---------------------------------------------------

	void ArmatureAsset::FreeResources() {
		_joints.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView ArmatureAsset::GetExtension() ETNoexceptHint {
		return { ArmatureExtension(), StringLength(ArmatureExtension()) };
	}

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
