/*==================================================================*\
  TestSurjectiveQueries.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/SurjectiveQueryDatabase.hpp>
#include <Utility/Memory/MallocAllocator.hpp>
//------------------------------------------------------------------//
#include <catch/include/catch.hpp>
//------------------------------------------------------------------//

TEST_CASE( "Basic SurjectiveQueryDatabase functionality", "[SurjectiveQueries]" ) {
	using namespace ::Eldritch2::Utility;
	using namespace ::Eldritch2;
	using namespace ::std;

	using Database	= SurjectiveQueryDatabase<int>;
	using Query		= Database::Query;
	using Fact		= Database::Fact;

	enum FactValue : int64 {
		Fact0Value,
		Fact1Value,
		Fact2Value,
		Fact3Value,
		NoMatchingValue
	};

	GIVEN( "A SurjectiveQueryDatabase" ) {
		MallocAllocator	allocator( "Database Test Allocator" );
		Database		database( allocator );

		database.AddRule( 0 ).AddCriterion( "Fact0", [] ( const Fact& fact ) {
			return fact.IsType<decltype(Fact0Value)>() && (fact.GetValueAs<decltype(Fact0Value)>() == Fact0Value);
		} );

		database.AddRule( 1 ).AddCriterion( "Fact0", [] ( const Fact& fact ) {
			return fact.IsType<decltype(Fact0Value)>() && (fact.GetValueAs<decltype(Fact0Value)>() == Fact0Value);
		} ).AddCriterion( "Fact1", [] ( const Fact& fact ) {
			return fact.IsType<decltype(Fact1Value)>() && (fact.GetValueAs<decltype(Fact1Value)>() == Fact1Value);
		} );

		database.AddRule( 2 ).AddCriterion( "Fact0", [] ( const Fact& fact ) {
			return fact.IsType<decltype(Fact0Value)>() && (fact.GetValueAs<decltype(Fact0Value)>() == Fact0Value);
		} ).AddCriterion( "Fact1", [] ( const Fact& fact ) {
			return fact.IsType<decltype(Fact1Value)>() && (fact.GetValueAs<decltype(Fact1Value)>() == Fact1Value);
		} ).AddCriterion( "Fact2", [] ( const Fact& fact ) {
			return fact.IsType<decltype(Fact2Value)>() && (fact.GetValueAs<decltype(Fact2Value)>() == Fact2Value);
		} );

		database.AddRule( 3 ).AddCriterion( "Fact0", [] ( const Fact& fact ) {
			return fact.IsType<decltype(Fact0Value)>() && (fact.GetValueAs<decltype(Fact0Value)>() == Fact0Value);
		} ).AddCriterion( "Fact1", [] ( const Fact& fact ) {
			return fact.IsType<decltype(Fact1Value)>() && (fact.GetValueAs<decltype(Fact1Value)>() == Fact1Value);
		} ).AddCriterion( "Fact2", [] ( const Fact& fact ) {
			return fact.IsType<decltype(Fact2Value)>() && (fact.GetValueAs<decltype(Fact2Value)>() == Fact2Value);
		} ).AddCriterion( "Fact3", [] ( const Fact& fact ) {
			// This fact intentionally untrue.
			return fact.IsType<decltype(Fact3Value)>() && (fact.GetValueAs<decltype(Fact3Value)>() != Fact3Value);
		} );

		WHEN( "a matching query is made" ) {
			Query	query( database, allocator );
			int		selectedRule( -1 );

			query.PublishFact( "Fact0", Fact0Value ).PublishFact( "Fact1", Fact1Value ).PublishFact( "Fact2", Fact2Value ).PublishFact( "Fact3", Fact3Value );

			query.EvaluateMostSuitableResponse( allocator, [&selectedRule] ( const int& selection ) {
				selectedRule = selection;
			} );

			THEN( "the database selects the most specific response" ) {
			//	While there are more criteria in the third rule, the comparison should fail in one of the conditions.
				REQUIRE( selectedRule == 2 );
			}
		}
		WHEN( "no criteria pass" ) {
			Query	query( database, allocator );
			int		selectedRule( -1 );

			query.PublishFact( "Fact0", Fact1Value ).PublishFact( "Fact1", Fact2Value ).PublishFact( "Fact2", Fact3Value ).PublishFact( "Fact3", Fact3Value );

			query.EvaluateMostSuitableResponse( allocator, [&selectedRule] ( const int& selection ) {
				selectedRule = selection;
			} );

			THEN( "no rules are matched" ) {
				REQUIRE( selectedRule == -1 );
			}
		}
		WHEN( "no facts are submitted" ) {
			int	selectedRule( -1 );

			Query( database, allocator ).EvaluateMostSuitableResponse( allocator, [&selectedRule] ( const int& selection ) {
				selectedRule = selection;
			} );

			THEN( "no rules are matched" ) {
				REQUIRE( selectedRule == -1 );
			}
		}
	}
}