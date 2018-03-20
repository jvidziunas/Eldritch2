/*==================================================================*\
  RuleDatabase_Test.cpp
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
#include <Common/RuleDatabase.hpp>
//------------------------------------------------------------------//
#include <catch/include/catch.hpp>
//------------------------------------------------------------------//

TEST_CASE( "Basic RuleDatabase functionality", "[RuleDatabase]" ) {
	using namespace ::Eldritch2;
	using namespace ::std;

	enum DatabaseResponse : int {
		Response1,
		Response2,
		Response3,
		Response4,
	};

	static constexpr uint64	Fact0Value	=  0;
	static constexpr uint64	Fact1Value	=  1;
	static constexpr uint64	Fact2Value	=  2;
	static constexpr uint64	Fact3Value	=  3;

	using TestDatabase	= RuleDatabase<DatabaseResponse>;
	using QueryBuilder	= TestDatabase::QueryBuilder;
	using RuleBuilder	= TestDatabase::RuleBuilder;
	using Fact			= TestDatabase::Fact;

	GIVEN( "A RuleDatabase" ) {
		MallocAllocator	allocator( "Database Test Allocator" );
		TestDatabase	database( { allocator, "Database Allocator" } );

		database.InsertRule( Response1, [] ( RuleBuilder& rule ) {
			rule.InsertCriterion( "Fact0", [] ( const Fact& fact ) {
				return fact.asUInt == Fact0Value;
			} );
		} );
		database.InsertRule( Response2, [] ( RuleBuilder& rule ) {
			rule.InsertCriterion( "Fact0", [] ( const Fact& fact ) {
				return fact.asUInt == Fact0Value;
			} );
			rule.InsertCriterion( "Fact1", [] ( const Fact& fact ) {
				return fact.asUInt == Fact1Value;
			} );
		} );
		database.InsertRule( Response3, [] ( RuleBuilder& rule ) {
			rule.InsertCriterion( "Fact0", [] ( const Fact& fact ) {
				return fact.asUInt == Fact0Value;
			} );
			rule.InsertCriterion( "Fact1", [] ( const Fact& fact ) {
				return fact.asUInt == Fact1Value;
			} );
			rule.InsertCriterion( "Fact2", [] ( const Fact& fact ) {
				return fact.asUInt == Fact2Value;
			} );
		} );
		database.InsertRule( Response4, [] ( RuleBuilder& rule ) {
			rule.InsertCriterion( "Fact0", [] ( const Fact& fact ) {
				return fact.asUInt == Fact0Value;
			} );
			rule.InsertCriterion( "Fact1", [] ( const Fact& fact ) {
				return fact.asUInt == Fact1Value;
			} );
			rule.InsertCriterion( "Fact2", [] ( const Fact& fact ) {
				return fact.asUInt == Fact2Value;
			} );
			rule.InsertCriterion( "Fact3", [] ( const Fact& fact ) {
			//	This fact intentionally untrue.
				return fact.asUInt != Fact3Value;
			} );
		} );

		WHEN( "several matching rules exist" ) {
			THEN( "the database selects the most specific response" ) {
				const auto match( database.FindBestMatch( allocator, [] ( QueryBuilder& query ) {
					query.InsertFact( "Fact0", Fact0Value );
					query.InsertFact( "Fact1", Fact1Value );
					query.InsertFact( "Fact2", Fact2Value );
					query.InsertFact( "Fact3", Fact3Value );
				} ) );

			//	While there are more criteria in the third rule, the comparison should fail in one of the conditions.
				REQUIRE( match != database.ConstEnd() );
				REQUIRE( match->first == Response3 );
			}
		}
		WHEN( "no criteria pass" ) {
			THEN( "no rules are matched" ) {
				const auto match( database.FindBestMatch( allocator, [] ( QueryBuilder& query ) {
					query.InsertFact( "Fact0", Fact1Value );
					query.InsertFact( "Fact1", Fact2Value );
					query.InsertFact( "Fact2", Fact3Value );
					query.InsertFact( "Fact3", Fact0Value );
				} ) );

				REQUIRE( match == database.ConstEnd() );
			}
		}
		WHEN( "no facts are submitted" ) {
			THEN( "no rules are matched" ) {
				REQUIRE( database.FindBestMatch( allocator, [] ( QueryBuilder& /*query*/ ) {} ) == database.ConstEnd() );
			}
		}
	}
}