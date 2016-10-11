/*==================================================================*\
  SurjectiveQueryDatabase.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/InstanceNew.hpp>
//------------------------------------------------------------------//
#include <type_traits>
#include <utility>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {
namespace Detail {

	template <typename Value, class /*SFINAE*/>
	ETInlineHint SurjectiveQueryDatabase::Fact::Fact( Value&& value ) : _type( typeid(value) ) {
		static_assert( sizeof( Value ) <= sizeof( _allocationSpace ), "Fact type is too large to fit in reserved space! Consider using a pointer." );

		new(_allocationSpace) Value( eastl::move( value ) );
	}

// ---------------------------------------------------

	ETInlineHint SurjectiveQueryDatabase::Fact::Fact( Fact&& fact ) : Fact( static_cast<const Fact&>(fact) ) {}

// ---------------------------------------------------

	ETInlineHint SurjectiveQueryDatabase::Fact::Fact() : _type( typeid(void) ) {}

// ---------------------------------------------------

	template <typename FactValue>
	ETInlineHint bool SurjectiveQueryDatabase::Fact::IsType() const {
		return _type == typeid(FactValue);
	}

// ---------------------------------------------------

	template <typename FactValue>
	ETInlineHint const FactValue& SurjectiveQueryDatabase::Fact::GetValueAs() const {
		return *reinterpret_cast<const FactValue*>(_allocationSpace);
	}

// ---------------------------------------------------

	template <typename FactValue>
	SurjectiveQueryDatabase::Query& SurjectiveQueryDatabase::Query::PublishFact( const Eldritch2::Utf8Char* const name, FactValue&& value ) {
		if( auto internResult = _database.TryInternFactName( name ) ) {
			_facts.Emplace( internResult, Fact( eastl::move( value ) ) );
		}
		
		return *this;
	}

// ---------------------------------------------------

	ETInlineHint SurjectiveQueryDatabase::Query& SurjectiveQueryDatabase::Query::PublishTag( const Eldritch2::Utf8Char* const name ) {
		if( auto internResult = _database.TryInternFactName( name ) ) {
			_facts.Emplace( internResult, Fact() );
		}

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint const SurjectiveQueryDatabase::Fact* SurjectiveQueryDatabase::Query::TryGetFactByName( InternedFactName name ) const {
		const auto candidate( _facts.Find( name ) );

		if( candidate == _facts.End() ) {
			return nullptr;
		}

		return eastl::addressof( candidate->second );
	}

// ---------------------------------------------------

	template <typename Evaluator>
	ETInlineHint SurjectiveQueryDatabase::Rule& SurjectiveQueryDatabase::Rule::AddCriterion( const Eldritch2::Utf8Char* const factName, Evaluator&& evaluator ) {
		_criteria.EmplaceBack( _database.InternFactName( factName ), eastl::forward<Evaluator>( evaluator ) );
		
		return *this;
	}

// ---------------------------------------------------

	ETInlineHint Eldritch2::Allocator& SurjectiveQueryDatabase::GetAllocator() {
		return _allocator;
	}

}	// namespace Detail

	template <typename Response>
	ETInlineHint SurjectiveQueryDatabase<Response>::Rule::Rule( SurjectiveQueryDatabase<Response>& database, Response&& response ) : Detail::SurjectiveQueryDatabase::Rule( database ), _response( eastl::move( response ) ) {}

// ---------------------------------------------------

	template <typename Response>
	ETInlineHint SurjectiveQueryDatabase<Response>::Rule::Rule( Rule&& rule ) : Detail::SurjectiveQueryDatabase::Rule( eastl::move( rule ) ), _response( eastl::move( rule._response ) ) {}

// ---------------------------------------------------

	template <typename Response>
	ETInlineHint const Response& SurjectiveQueryDatabase<Response>::Rule::GetResponse() const {
		return _response;
	}

// ---------------------------------------------------

	template <typename Response>
	ETInlineHint SurjectiveQueryDatabase<Response>::SurjectiveQueryDatabase( Eldritch2::Allocator& allocator ) : Detail::SurjectiveQueryDatabase( allocator ), _rules( { GetAllocator(), "Surjective Query Database Rule Allocator" } ) {}

// ---------------------------------------------------

	template <typename Response>
	ETInlineHint SurjectiveQueryDatabase<Response>::Query::Query( const SurjectiveQueryDatabase<Response>& database, Eldritch2::Allocator& allocator ) : Detail::SurjectiveQueryDatabase::Query( database, allocator ) {}

// ---------------------------------------------------

	template <typename Response>
	template <typename Evaluator>
	ETInlineHint void SurjectiveQueryDatabase<Response>::Query::EvaluateMostSuitableResponse( Eldritch2::Allocator& allocator, Evaluator&& evaluator ) const {
		Eldritch2::ResizableArray<const SurjectiveQueryDatabase<Response>::Rule*>	matchingRules( { allocator, "SurjectiveQueryDatabase::Query::EvaluateMostSuitableResponse() Temporary Allocator" } );

		CollectMatchingRules( matchingRules );

		if( matchingRules ) {
			evaluator( matchingRules.Front()->GetResponse() );
		}
	}

// ---------------------------------------------------

	template <typename Response>
	template <typename Evaluator>
	ETInlineHint void SurjectiveQueryDatabase<Response>::Query::EvaluateAllSuitableResponses( Eldritch2::Allocator& allocator, Evaluator&& evaluator ) const {
		Eldritch2::ResizableArray<const SurjectiveQueryDatabase<Response>::Rule*>	matchingRules( { allocator, "SurjectiveQueryDatabase::Query::EvaluateAllSuitableResponses() Temporary Allocator" } );

		CollectMatchingRules( matchingRules );

		for( const auto& matchingRule : matchingRules ) {
			evaluator( matchingRule->GetResponse() );
		}
	}

// ---------------------------------------------------

	template <typename Response>
	template <typename MatchingRuleCollection>
	ETInlineHint void SurjectiveQueryDatabase<Response>::Query::CollectMatchingRules( MatchingRuleCollection&& collection ) const {
	//	We can optimize matching slightly by starting at 1-- this will cause rules that match 0 patterns to fail immediately instead of checking separately below.
		size_t	matchThreshold( 1u );

		for( const auto& rule : static_cast<const SurjectiveQueryDatabase<Response>&>(_database)._rules ) {
			const auto	matchWeight( rule.EvaluateMatchWeight( *this ) );

			if( matchWeight < matchThreshold ) {
				continue;
			}

		//	Clear out the matches if we've found a more specific response-- it's a better fit than what has come before.
			if( matchThreshold != matchWeight ) {
				collection.Clear();
			}

			collection.PushBack( &rule );
		}
	}

// ---------------------------------------------------

	template <typename Response>
	ETInlineHint typename SurjectiveQueryDatabase<Response>::Rule& SurjectiveQueryDatabase<Response>::AddRule( Response&& response ) {
		_rules.EmplaceBack( *this, eastl::move( response ) );

		return _rules.Back();
	}

}	// namespace Utility
}	// namespace Eldritch2