/*==================================================================*\
  TestAssetDatabase.cpp
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
#include <Common/Containers/ArrayMap.hpp>
#include <Platform/ContentProvider.hpp>
#include <Assets/PackageProvider.hpp>
#include <Assets/AssetDatabase.hpp>
//------------------------------------------------------------------//
#include <catch/include/catch.hpp>
//------------------------------------------------------------------//

namespace {

using namespace ::Eldritch2::Assets;
using namespace ::Eldritch2;

class MockDatabase : public PackageProvider, public AssetDatabase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref MockLibrary instance.
	MockDatabase(
		Allocator& allocator) :
		PackageProvider(allocator, *this),
		AssetDatabase(allocator, *this),
		_mockLoaders({ allocator, "Mock Loader Collection Allocator" }) {}

	~MockDatabase() = default;

	// ---------------------------------------------------

public:
	template <typename MockLoadHandler>
	void InstallHandler(Utf8Literal packageName, MockLoadHandler&& handler) {
		_mockLoaders.Emplace(packageName, eastl::forward<MockLoadHandler>(handler));
	}

	// ---------------------------------------------------

public:
	void RequestLoad(Package& package) override sealed {
		const auto candidate(_mockLoaders.Find(Utf8Literal(package.GetPath())));

		if (candidate == _mockLoaders.End()) {
			package.CompleteLoad(ResidencyState::Failed);
			return;
		}

		candidate->second(package, *this);
	}

	// ---------------------------------------------------

	using AssetDatabase::EnumeratePackages;

	CachingHashSet<Package> EnumeratePackages(Allocator& allocator) override {
		CachingHashSet<Package> loadableSet({ allocator, "Mock Loadable Package Collection Allocator" });

		for (const auto& mock : _mockLoaders) {
			loadableSet.Emplace(allocator, mock.first);
		}

		return eastl::move(loadableSet);
	}

	// - DATA MEMBERS ------------------------------------

private:
	ArrayMap<Utf8Literal, Function<void(Package& /*package*/, MockDatabase& /*loader*/)>> _mockLoaders;
};

} // anonymous namespace

TEST_CASE("AssetLibrary functionality", "[AssetLibrary]") {
	using namespace ::Eldritch2::Platform;
	using namespace ::Eldritch2::Assets;
	using namespace ::Eldritch2;

	// ---

	GIVEN("an AssetLibrary") {
		MallocAllocator allocator("Test Allocator");
		MockDatabase    loader(allocator);

		WHEN("an unloadable package is requested") {
			loader.EnumeratePackages();

			auto result(loader.FindPackage("TestPackage"));

			THEN("the AssetLibrary should reject the operation") {
				REQUIRE(result.GetErrorCode() == Error::InvalidFileName);
			}
		}

		WHEN("a loadable package is requested") {
			loader.InstallHandler("TestPackage", [](Package& package, MockDatabase& /*loader*/) {
				package.CompleteLoad(ResidencyState::Published);
			});

			loader.EnumeratePackages();

			auto result(loader.FindPackage("TestPackage"));

			THEN("the AssetLibrary should complete the operation") {
				REQUIRE(result.GetErrorCode() == Error::None);
				REQUIRE(*result != nullptr);
			}
		}

		WHEN("a simple garbage collection pass is performed") {
			loader.InstallHandler("TestPackage", [](Package& package, MockDatabase& /*loader*/) {
				package.CompleteLoad(ResidencyState::Published);
			});

			loader.EnumeratePackages();

			auto testPackage(loader.FindPackage("TestPackage"));

			loader.SweepGarbage(5u);

			THEN("the AssetLibrary should complete the operation") {
			}
		}

		WHEN("a complex garbage collection pass is performed") {
			loader.InstallHandler("TestPackage", [](Package& package, MockDatabase& /*loader*/) {
				package.CompleteLoad(ResidencyState::Published);
			});
			loader.InstallHandler("TestPackage2", [](Package& package, MockDatabase& /*loader*/) {
				package.CompleteLoad(ResidencyState::Published);
			});
			loader.InstallHandler("TestPackage1", [](Package& package, MockDatabase& loader) {
				package.InsertDependency(**loader.FindPackage("TestPackage2"));

				package.CompleteLoad(ResidencyState::Published);
			});

			loader.EnumeratePackages();

			auto testPackage(loader.FindPackage("TestPackage"));
			auto testPackage1(loader.FindPackage("TestPackage1"));

			loader.SweepGarbage(5u);

			THEN("the AssetLibrary should complete the operation") {
			}
		}

		WHEN("an alternate complex garbage collection pass is performed") {
			loader.InstallHandler("TestPackage", [](Package& package, MockDatabase& /*loader*/) {
				package.CompleteLoad(ResidencyState::Published);
			});
			loader.InstallHandler("TestPackage2", [](Package& package, MockDatabase& /*loader*/) {
				package.CompleteLoad(ResidencyState::Published);
			});
			loader.InstallHandler("TestPackage1", [](Package& package, MockDatabase& loader) {
				package.InsertDependency(**loader.FindPackage("TestPackage2"));

				package.CompleteLoad(ResidencyState::Published);
			});

			loader.EnumeratePackages();

			auto testPackage(loader.FindPackage("TestPackage"));
			auto testPackage1(loader.FindPackage("TestPackage1"));
			loader.SweepGarbage(5u);

			THEN("the AssetLibrary should complete the operation") {
			}
		}
	}
}
