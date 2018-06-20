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
ET_PUSH_COMPILER_WARNING_STATE()
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_SET_MSVC_WARNING_STATE(disable : 6326)
#	include <PxArticulation.h>
#	include <PxPhysics.h>
#	include <PxScene.h>
ET_POP_COMPILER_WARNING_STATE()
//------------------------------------------------------------------//

using namespace ::Eldritch2::Animation::FlatBuffers;
using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::flatbuffers;
using namespace ::physx;

namespace Eldritch2 {
	namespace Physics {
		namespace PhysX {
			namespace AssetViews {
				namespace {

					ETInlineHint ETPureFunctionHint PxQuat Decompress(const FlatBuffers::CompressedUnitQuaternion& quaternion) {
						return PxQuat(PxIdentity);
					}

				// ---------------------------------------------------

					ETInlineHint ETPureFunctionHint PxTransform AsPxTransform(const Bone& pose) {
						const auto&	translation(pose.Translation());

						return { translation.X(), translation.Y(), translation.Z(), Decompress(pose.Orientation()) };
					}

				}	// anonymous namespace

				ArmatureAsset::ArmatureAsset(const Utf8Char* const filePath) : Asset(filePath), _joints(MallocAllocator("PhysX Armature Asset Joint Definition Allocator")) {}

			// ---------------------------------------------------

				PhysxPointer<PxArticulation> ArmatureAsset::CreateArticulation(PxScene& scene) const {
					const auto	articulation(scene.getPhysics().createArticulation());
					const auto	links(static_cast<PxArticulationLink**>(_alloca(sizeof(PxArticulationLink*) * (_joints.GetSize() + 1))));
					uint32		linkId(1u);

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
						asset.WriteLog(MessageType::Error, "Data integrity check failed for {}, aborting load." UTF8_NEWLINE, GetPath());
						return Error::InvalidParameter;
					}

					ArrayList<Joint>	joints(_joints.GetAllocator());

					for (const JointDescriptor* joint : *GetArmature(asset.Begin())->Joints()) {
						const Bone*	pose(joint->BindPose());

						joints.Append({ AsPxTransform(*pose), joint->ParentIndex() });
					}

					Swap(_joints, joints);

					return Error::None;
				}

			// ---------------------------------------------------

				void ArmatureAsset::FreeResources() {
					_joints.Clear(ReleaseMemorySemantics());
				}

			// ---------------------------------------------------

				Utf8Literal	ArmatureAsset::GetExtension() {
					return Utf8Literal(ArmatureExtension());
				}

			}	// namespace AssetViews
		}	// namespace PhysX
	}	// namespace Physics
}	// namespace Eldritch2