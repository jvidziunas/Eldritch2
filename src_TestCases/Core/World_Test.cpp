/*==================================================================*\
  TestWorld.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/MallocAllocator.hpp>
#include <Assets/Package.hpp>
#include <Core/World.hpp>
//------------------------------------------------------------------//
#include <catch/include/catch.hpp>
//------------------------------------------------------------------//

namespace {

using namespace ::Eldritch2::Logging;
using namespace ::Eldritch2::Assets;
using namespace ::Eldritch2::Core;
using namespace ::Eldritch2;

static ETInlineHint UniquePointer<const Package, NoopDeleter> CreatePackage(Allocator& allocator, const Utf8Char* const path, ResidencyState state) {
	const auto package(new (allocator.Allocate(sizeof(Package))) Package(allocator, path));

	if (state != ResidencyState::QueuedForLoad) {
		package->CompleteLoad(state);
	}

	return { package };
}

// ---------------------------------------------------

class NullLog : public Log {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref NullLog instance.
	NullLog() = default;

	~NullLog() = default;

	// ---------------------------------------------------

public:
	void Write(const Utf8Char* const /*string*/, size_t /*lengthInOctets*/) override {
		//	This space intentionally blank.
	}
};

} // anonymous namespace

#define ETImplementValue(x) \
	case x: return #x

namespace Catch {

template <>
struct StringMaker<World::State> {
	ETPureFunctionHint static std::string convert(World::State state) {
		switch (state) {
			ETImplementValue(World::State::Loading);
			ETImplementValue(World::State::Running);
			ETImplementValue(World::State::ShutDown);
		default: return "<unimplemented>";
		};
	}
};

} // namespace Catch

TEST_CASE("World functionality", "[Worlds]") {
	using namespace ::Eldritch2::Core;
	using namespace ::Eldritch2;

	MallocAllocator allocator("Test Allocator");
	ObjectLocator   locator(allocator);
	NullLog         log;

	GIVEN("a World") {
		World testWorld(allocator, locator, log);

		WHEN("no dependencies are added") {
			THEN("the world should be in an executable state") {
				REQUIRE(testWorld.GetLoadStatus() == World::State::Running);

				AND_WHEN("additional dependencies are added") {
					testWorld.AddRootPackage(CreatePackage(allocator, "TestUnloadedPackage", ResidencyState::QueuedForLoad));

					THEN("the world should no longer be in an executable state") {
						REQUIRE(testWorld.GetLoadStatus() == World::State::Loading);
					}
				}
			}
		}

		WHEN("loaded dependencies are present") {
			testWorld.AddRootPackage(CreatePackage(allocator, "TestLoadedPackage", ResidencyState::Published));

			THEN("the world should be in an executable state") {
				REQUIRE(testWorld.GetLoadStatus() == World::State::Running);

				AND_WHEN("additional dependencies are added") {
					testWorld.AddRootPackage(CreatePackage(allocator, "TestUnloadedPackage", ResidencyState::QueuedForLoad));

					THEN("the world should no longer be in an executable state") {
						REQUIRE(testWorld.GetLoadStatus() == World::State::Loading);
					}
				}
			}
		}

		WHEN("unloaded dependencies are present") {
			auto unloadedDependency(CreatePackage(allocator, "TestUnloadedPackage", ResidencyState::QueuedForLoad));
			auto unloadedPackage(unloadedDependency.Get());

			testWorld.AddRootPackage(CreatePackage(allocator, "TestLoadedPackage", ResidencyState::Published));
			testWorld.AddRootPackage(eastl::move(unloadedDependency));

			THEN("the world should not be in an executable state") {
				REQUIRE(testWorld.GetLoadStatus() == World::State::Loading);

				AND_WHEN("loads are completed") {
					unloadedPackage->CompleteLoad(ResidencyState::Published);

					THEN("the world should be in an executable state") {
						REQUIRE(testWorld.GetLoadStatus() == World::State::Running);
					}
				}
			}
		}

		WHEN("failed dependencies are present") {
			testWorld.AddRootPackage(CreatePackage(allocator, "TestLoadedPackage", ResidencyState::Published));
			testWorld.AddRootPackage(CreatePackage(allocator, "TestUnloadedPackage", ResidencyState::QueuedForLoad));
			testWorld.AddRootPackage(CreatePackage(allocator, "TestFailedPackage", ResidencyState::Failed));

			THEN("the world should cease execution") {
				REQUIRE(testWorld.GetLoadStatus() == World::State::ShutDown);
			}
		}
	}
}
