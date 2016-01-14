/*==================================================================*\
  TestResizableArray.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Memory/ArenaAllocator.hpp>
//------------------------------------------------------------------//
#include <catch/include/catch.hpp>
//------------------------------------------------------------------//

TEST_CASE( "Basic ResizableArray functionality", "[Containers][ResizableArray]" ) {
	GIVEN( "A ResizableArray" ) {
		::Eldritch2::ResizableArray<unsigned int, ::Eldritch2::ExternalArenaAllocator>	testArray( 5, { _alloca( 128u ), 128u, UTF8L("ResizableArray Test Allocator") } );

		REQUIRE( testArray.Size() == 0 );
		REQUIRE( testArray.GetCapacity() >= 5 );

		WHEN( "the size is increased" ) {
			testArray.Resize( 10 );

			THEN( "the size and capacity change" ) {
				REQUIRE( testArray.Size() == 10 );
				REQUIRE( testArray.GetCapacity() >= 10 );
			}
		}
		WHEN( "the size is reduced" ) {
			testArray.Resize( 0 );

			THEN( "the size changes but not capacity" ) {
				REQUIRE( testArray.Size() == 0 );
				REQUIRE( testArray.GetCapacity() >= 5 );
			}
		}
		WHEN( "more capacity is reserved" ) {
			testArray.Reserve( 10 );

			THEN( "the capacity changes but not the size" ) {
				REQUIRE( testArray.Size() == 0 );
				REQUIRE( testArray.GetCapacity() >= 10 );
			}
		}
		WHEN( "less capacity is reserved" ) {
			testArray.Reserve( 0 );

			THEN( "neither size nor capacity are changed" ) {
				REQUIRE( testArray.Size() == 0 );
				REQUIRE( testArray.GetCapacity() >= 5 );
			}
		}
		WHEN( "an element is added" ) {
			testArray.PushBack( 5u );

			THEN( "the size changes" ) {
				REQUIRE( testArray.Size() == 1 );
			}
			AND_THEN( "the back element is updated" ) {
				REQUIRE( testArray.Back() == 5u );
			}
		}
	}
}