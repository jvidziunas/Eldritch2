/*==================================================================*\
  PhysicsAsset.cpp
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
#include <Physics/PhysX/AssetViews/PhysicsAsset.hpp>
#include <Physics/PhysX/SpanInputStream.hpp>
#include <Flatbuffers/FlatbufferTools.hpp>
#include <Logging/Log.hpp>
//------------------------------------------------------------------//
#include <FlatBuffers/PhysicsBody_generated.h>
//------------------------------------------------------------------//
//	(6326) MSVC doesn't like some of the compile-time constant comparison PhysX does. We can't fix this, but we can at least disable the warning.
ET_PUSH_MSVC_WARNING_STATE(disable : 6326)
#include <geometry/PxCapsuleGeometry.h>
#include <geometry/PxSphereGeometry.h>
#include <geometry/PxBoxGeometry.h>
#include <PxMaterial.h>
#include <PxPhysics.h>
ET_POP_MSVC_WARNING_STATE()
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Physics { namespace PhysX { namespace AssetViews {

	using namespace ::Eldritch2::Physics::FlatBuffers;
	using namespace ::Eldritch2::Animation;
	using namespace ::Eldritch2::Logging;
	using namespace ::Eldritch2::Core;
	using namespace ::flatbuffers;
	using namespace ::physx;

	namespace {

		ETForceInlineHint ETPureFunctionHint PxVec3 AsPxVec3(const Float3* vector) ETNoexceptHint {
			return PxVec3(vector->X(), vector->Y(), vector->Z());
		}

		// ---------------------------------------------------

		ETForceInlineHint Result CreateRigidShapes(ArrayList<PhysicsAsset::RigidShape>& shapes, const flatbuffers::Vector<Offset<ShapeDescriptor>>* descriptors) {
			const auto ParseMaterial([physX = ETAddressOf(PxGetPhysics())](const ShapeDescriptor& shape) ETNoexceptHint -> PhysxPointer<PxMaterial> {
				return physX->createMaterial(shape.Friction(), shape.RollingFriction(), shape.Restitution());
			});

			const auto ParseSphere([&, physX = ETAddressOf(PxGetPhysics())](const ShapeDescriptor& shape, const SphereShape* sphere) -> Result {
				shapes.EmplaceBack(physX->createShape(PxSphereGeometry(sphere->Radius()), *ParseMaterial(shape)), BoneIndex(shape.BoneIndex()));
				return Result::Success;
			});

			const auto ParseBox([&, physX = ETAddressOf(PxGetPhysics())](const ShapeDescriptor& shape, const BoxShape* box) -> Result {
				shapes.EmplaceBack(physX->createShape(PxBoxGeometry(AsPxVec3(box->HalfExtent())), *ParseMaterial(shape)), BoneIndex(shape.BoneIndex()));
				return Result::Success;
			});

			const auto ParseCapsule([&, physX = ETAddressOf(PxGetPhysics())](const ShapeDescriptor& shape, const CapsuleShape* capsule) -> Result {
				shapes.EmplaceBack(physX->createShape(PxCapsuleGeometry(capsule->Radius(), capsule->Height() * 0.5f), *ParseMaterial(shape)), BoneIndex(shape.BoneIndex()));
				return Result::Success;
			});

			// ---

			for (uoffset_t id(0u); id < descriptors->size(); ++id) {
				const ShapeDescriptor* descriptor(descriptors->Get(id));
				switch (descriptor->Shape_type()) {
				default:
				case CollisionShape::NONE: break;
				case CollisionShape::SphereShape: ET_ABORT_UNLESS(ParseSphere(*descriptor, descriptor->Shape_as<SphereShape>())); break;
				case CollisionShape::BoxShape: ET_ABORT_UNLESS(ParseBox(*descriptor, descriptor->Shape_as<BoxShape>())); break;
				case CollisionShape::CapsuleShape: ET_ABORT_UNLESS(ParseCapsule(*descriptor, descriptor->Shape_as<CapsuleShape>())); break;
				};
			}

			return Result::Success;
		}

		// ---------------------------------------------------

		ETForceInlineHint Result CreateClothShapes(ArrayList<PhysicsAsset::ClothShape>& shapes, const flatbuffers::Vector<Offset<ClothDescriptor>>* bodies) {
			const auto ParseFabric([physX = ETAddressOf(PxGetPhysics())](SpanInputStream bytes) ETNoexceptHint -> PhysxPointer<PxClothFabric> {
				return physX->createClothFabric(bytes);
			});

			const auto ParseCloth([&](const ClothDescriptor& cloth) -> Result {
				shapes.EmplaceBack(ParseFabric(SpanInputStream(AsBytes(cloth.Connectivity()))), nullptr, /*bone =*/0u);
				return Result::Success;
			});

			// ---

			for (uoffset_t id(0u); id < bodies->size(); ++id) {
				ET_ABORT_UNLESS(ParseCloth(*bodies->Get(id)));
			}

			return Result::Success;
		}

	} // anonymous namespace

	PhysicsAsset::RigidShape::RigidShape(PhysxPointer<PxShape> shape, BoneIndex bone) ETNoexceptHint : shape(Move(shape)), bone(bone) {}

	// ---------------------------------------------------

	PhysicsAsset::ClothShape::ClothShape(PhysxPointer<PxClothFabric> fabric, ArrayList<PxClothParticle> particles) ETNoexceptHint : fabric(Move(fabric)), particles(Move(particles)) {}

	// ---------------------------------------------------

	PhysicsAsset::PhysicsAsset(StringSpan path) ETNoexceptHint : Asset(path),
																 _rigidShapes(RigidShapeList::AllocatorType("PhysX Physics Asset Rigid Shape List Allocator")),
																 _clothShapes(ClothShapeList::AllocatorType("PhysX Physics Asset Cloth Shape List Allocator")) {
	}

	// ---------------------------------------------------

	PhysicsAsset::RigidShapeList::ConstSliceType PhysicsAsset::GetRigidShapes() const ETNoexceptHint {
		return _rigidShapes;
	}

	// ---------------------------------------------------

	PhysicsAsset::ClothShapeList::ConstSliceType PhysicsAsset::GetClothShapes() const ETNoexceptHint {
		return _clothShapes;
	}

	// ---------------------------------------------------

	Result PhysicsAsset::BindResources(Log& log, const AssetBuilder& asset) {
		//	Verify the data we are considering can plausibly represent a physics asset.
		const auto physics(GetVerifiedRoot<PhysicsDescriptor>(asset.bytes, PhysicsDescriptorIdentifier()));

		ET_ABORT_UNLESS(physics ? Result::Success : Result::InvalidParameter, log.Write(Error, "Data integrity check failed for {}, aborting load." ET_NEWLINE, GetPath()));

		RigidShapeList rigidShapes(_rigidShapes.GetAllocator(), physics->Shapes()->size());
		ET_ABORT_UNLESS(CreateRigidShapes(rigidShapes, physics->Shapes()), log.Write(Error, "Shape data for {} is malformed!" ET_NEWLINE, GetPath()));

		ClothShapeList clothShapes(_clothShapes.GetAllocator(), physics->ClothShapes()->size());
		ET_ABORT_UNLESS(CreateClothShapes(clothShapes, physics->ClothShapes()), log.Write(Error, "Cloth shape data for {} is malformed!" ET_NEWLINE, GetPath()));

		Swap(_rigidShapes, rigidShapes);
		Swap(_clothShapes, clothShapes);

		return Result::Success;
	}

	// ---------------------------------------------------

	void PhysicsAsset::FreeResources() ETNoexceptHint {
		_rigidShapes.Clear(ReleaseMemorySemantics());
		_clothShapes.Clear(ReleaseMemorySemantics());
	}

	// ---------------------------------------------------

	ETPureFunctionHint StringSpan PhysicsAsset::GetExtension() ETNoexceptHint {
		return StringSpan(PhysicsDescriptorExtension(), StringSpan::SizeType(StringLength(PhysicsDescriptorExtension())));
	}

}}}} // namespace Eldritch2::Physics::PhysX::AssetViews
