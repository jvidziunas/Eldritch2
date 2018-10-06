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
#include <Flatbuffers/FlatBufferTools.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/Armature_generated.h>
//------------------------------------------------------------------//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <PxPhysics.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	using namespace ::Eldritch2::Animation::FlatBuffers;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Assets;
	using namespace ::flatbuffers;
	using namespace ::physx;

	namespace {

		ETInlineHint ETForceInlineHint ErrorCode ParseJoint(Joint& joint, const JointDescriptor* descriptor) ETNoexceptHint {
			joint.bindPose = AsTransformation(descriptor->BindPose());
			joint.parent   = descriptor->ParentIndex();

			return Error::None;
		}

		// ---------------------------------------------------

		ETInlineHint ETForceInlineHint ErrorCode ParseJoints(ArrayList<Joint>& joints, const FlatbufferVector<const JointDescriptor*>* descriptors) ETNoexceptHint {
			for (uoffset_t joint(0u); joint < descriptors->size(); ++joint) {
				ET_ABORT_UNLESS(ParseJoint(joints.EmplaceBack(), descriptors->Get(joint)));
			}

			return Error::None;
		}

	} // anonymous namespace

	ArmatureAsset::ArmatureAsset(StringView path) :
		ArmatureDefinition(),
		Asset(path) {}

	// ---------------------------------------------------

	ErrorCode ArmatureAsset::BindResources(const Builder& asset) {
		//	Ensure we're working with data that can plausibly represent an armature.
		Verifier verifier(reinterpret_cast<const uint8_t*>(asset.Begin()), asset.GetSize());
		if (!VerifyArmatureDescriptorBuffer(verifier)) {
			asset.WriteLog(Severity::Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath());
			return Error::InvalidParameter;
		}

		const auto descriptor(GetArmatureDescriptor(asset.Begin()));

		ArmatureDefinition armature;
		ET_ABORT_UNLESS(ParseJoints(armature.joints, descriptor->Joints()));

		Swap(*this, armature);

		return Error::None;
	}

	// ---------------------------------------------------

	void ArmatureAsset::FreeResources() {
		ArmatureDefinition armature;

		Swap(*this, armature);
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringView ArmatureAsset::GetExtension() ETNoexceptHint {
		return { ArmatureDescriptorExtension(), StringLength(ArmatureDescriptorExtension()) };
	}

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
