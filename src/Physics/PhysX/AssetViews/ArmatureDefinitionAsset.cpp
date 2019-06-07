/*==================================================================*\
  ArmatureDefinitionAsset.cpp
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
#include <Physics/PhysX/AssetViews/ArmatureDefinitionAsset.hpp>
#include <Flatbuffers/FlatBufferTools.hpp>
#include <Logging/Log.hpp>
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
	using namespace ::Eldritch2::Core;
	using namespace ::flatbuffers;
	using namespace ::physx;

	// ---------------------------------------------------

	namespace {

		ETForceInlineHint Result ParseJoint(Joint& joint, const JointDescriptor* descriptor) ETNoexceptHint {
			joint.bindPose = AsTransformation(descriptor->BindPose());
			joint.parent   = descriptor->ParentIndex();

			return Result::Success;
		}

		// ---------------------------------------------------

		ETForceInlineHint Result ParseJoints(ArrayList<Joint>& joints, const FlatbufferVector<const JointDescriptor*>* descriptors) ETNoexceptHint {
			for (uoffset_t joint(0u); joint < descriptors->size(); ++joint) {
				ET_ABORT_UNLESS(ParseJoint(joints.EmplaceBack(), descriptors->Get(joint)));
			}

			return Result::Success;
		}

	} // anonymous namespace

	ArmatureDefinitionAsset::ArmatureDefinitionAsset(StringSpan path) ETNoexceptHint : ArmatureDefinition(), Asset(path) {}

	// ---------------------------------------------------

	Result ArmatureDefinitionAsset::BindResources(Log& log, const AssetBuilder& asset) {
		//	Ensure we're working with data that can plausibly represent an armature.
		const auto descriptor(GetVerifiedRoot<ArmatureDescriptor>(asset.bytes, ArmatureDescriptorIdentifier()));

		ET_ABORT_UNLESS(descriptor ? Result::Success : Result::InvalidParameter, log.Write(Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath()));

		ArmatureDefinition armature;
		ET_ABORT_UNLESS(ParseJoints(armature.GetJoints(), descriptor->Joints()));

		Swap(*this, armature);

		return Result::Success;
	}

	// ---------------------------------------------------

	void ArmatureDefinitionAsset::FreeResources() ETNoexceptHint {
		ArmatureDefinition armature;

		Swap(*this, armature);
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan ArmatureDefinitionAsset::GetExtension() ETNoexceptHint {
		return { ArmatureDescriptorExtension(), StringLength(ArmatureDescriptorExtension()) };
	}

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
