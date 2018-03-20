/*==================================================================*\
  TestResizableArray.cpp
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
#include <Common/UniquePointer.hpp>
//------------------------------------------------------------------//
#include <catch/include/catch.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2;

namespace {

	class BaseClass {
	public:
		enum {
			TestValue = 3
		};

		BaseClass( const BaseClass& ) = delete;
		BaseClass() = default;

		~BaseClass() = default;

		virtual int TestMethod() {
			return TestValue;
		}
	};

// ---

	class DerivedClass : public BaseClass {
	public:
		enum {
			TestValue = 5
		};

		DerivedClass( const DerivedClass& ) = delete;
		DerivedClass() = default;

		~DerivedClass() = default;

		int TestMethod() override {
			return TestValue;
		}
	};

}	// anonymous namespace

TEST_CASE( "UniquePointer", "[Containers][UniquePointer]" ) {
	static_assert( BaseClass::TestValue != DerivedClass::TestValue, "Test needs updating" );

	MallocAllocator	testAllocator( "Test Allocator" );

	GIVEN( "A UniquePointer to an object" ) {
		const UniquePointer<BaseClass>	testObject( NewUnique<BaseClass>( testAllocator ) );

		REQUIRE( testObject != nullptr );

		WHEN( "methods are invoked" ) {
			THEN( "the correct result is returned" ) {
				REQUIRE( testObject->TestMethod() == BaseClass::TestValue );
			}
		}
	}

	GIVEN( "A UniquePointer to a derived object" ) {
		const UniquePointer<DerivedClass>	testObject( NewUnique<DerivedClass>( testAllocator ) );

		REQUIRE( testObject != nullptr );

		WHEN( "methods are invoked" ) {
			THEN( "the correct result is returned" ) {
				REQUIRE( testObject->TestMethod() == DerivedClass::TestValue );
			}
		}
	}

	GIVEN( "A UniquePointer to a derived object converted to the base" ) {
		const UniquePointer<BaseClass>	testObject( NewUnique<DerivedClass>( testAllocator ) );

		REQUIRE( testObject != nullptr );

		WHEN( "methods are invoked" ) {
			THEN( "the correct result is returned" ) {
				REQUIRE( testObject->TestMethod() == DerivedClass::TestValue );
			}
		}
	}
}