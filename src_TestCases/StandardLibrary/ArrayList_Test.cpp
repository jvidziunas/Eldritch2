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
#include <Common/Containers/ArrayList.hpp>
//------------------------------------------------------------------//
#include <catch/include/catch.hpp>
//------------------------------------------------------------------//

TEST_CASE( "Basic ArrayList functionality", "[Containers][ArrayList]" ) {
	GIVEN( "A ArrayList" ) {
		Eldritch2::ArrayList<unsigned int, Eldritch2::MallocAllocator>	testArray( { "Test Allocator" } );

		REQUIRE( testArray.GetSize() == 0 );
		REQUIRE( testArray.IsEmpty() );

		testArray.Reserve( 5 );

		WHEN( "the size is increased" ) {
			testArray.Resize( 10 );

			THEN( "the size and capacity change" ) {
				REQUIRE( testArray.GetSize() == 10 );
				REQUIRE( testArray.GetCapacity() >= 10 );
			}

			AND_THEN( "the container is not empty" ) {
				REQUIRE( !testArray.IsEmpty() );
				REQUIRE( !!testArray );
			}
		}
		WHEN( "more capacity is reserved" ) {
			testArray.Reserve( 10 );

			THEN( "the capacity changes but not the size" ) {
				REQUIRE( testArray.GetSize() == 0 );
				REQUIRE( testArray.GetCapacity() >= 10 );
			}

			AND_THEN( "the container is still empty" ) {
				REQUIRE( testArray.IsEmpty() );
				REQUIRE( !testArray );
			}
		}
		WHEN( "less capacity is reserved" ) {
			testArray.Reserve( 0 );

			THEN( "neither size nor capacity are changed" ) {
				REQUIRE( testArray.GetSize() == 0 );
				REQUIRE( testArray.GetCapacity() >= 5 );
			}
		}
		WHEN( "an element is added" ) {
			testArray.PushBack( 5u );

			THEN( "the size changes" ) {
				REQUIRE( testArray.GetSize() == 1 );
			}

			AND_THEN( "the back element is updated" ) {
				REQUIRE( testArray.Back() == 5u );
			}

			AND_THEN( "the container is not empty" ) {
				REQUIRE( !testArray.IsEmpty() );
				REQUIRE( !!testArray );
			}
		}
	}
}