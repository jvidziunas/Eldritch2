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

	template <typename Value>
	ETInlineHint SurjectiveQueryDatabase::Fact::Fact( Value&& value ) : _typeInfo( typeid(value) ) {
		static_assert( ::std::is_pod<decltype(value)>::value, "Fact type must be POD!" );
		static_assert( sizeof( Value ) <= sizeof( _allocationSpace ), "Fact type is too large to fit in reserved space! Consider using a pointer." );

		new(_allocationSpace) Value( ::std::move( value ) );
	}

// ---------------------------------------------------

	ETInlineHint SurjectiveQueryDatabase::Fact::Fact( Fact&& fact ) : Fact( static_cast<const Fact&>(fact) ) {}

// ---------------------------------------------------

	ETInlineHint SurjectiveQueryDatabase::Fact::Fact() : _typeInfo( typeid(void) ) {}

// ---------------------------------------------------

	template <typename FactValue>
	ETInlineHint bool SurjectiveQueryDatabase::Fact::IsType() const {
		return _typeInfo == typeid(FactValue);
	}

// ---------------------------------------------------

	template <typename FactValue>
	ETInlineHint const FactValue& SurjectiveQueryDatabase::Fact::GetValueAs() const {
		return *reinterpret_cast<const FactValue*>(_allocationSpace);
	}

// ---------------------------------------------------

	ETInlineHint SurjectiveQueryDatabase::Rule::Criterion::Criterion( InternedFactName targetFactName ) : _targetFactName( targetFactName ) {}

// ---------------------------------------------------

	ETInlineHint SurjectiveQueryDatabase::InternedFactName SurjectiveQueryDatabase::Rule::Criterion::GetTargetFactName() const {
		return _targetFactName;
	}

// ---------------------------------------------------

	template <typename FactValue>
	SurjectiveQueryDatabase::Query& SurjectiveQueryDatabase::Query::PublishFact( const ::Eldritch2::UTF8Char* const name, FactValue&& value ) {
		if( auto internResult = _database.TryInternFactName( name ) ) {
			_facts.Insert( { internResult, { ::std::move( value ) } } );
		}
		
		return *this;
	}

// ---------------------------------------------------

	ETInlineHint SurjectiveQueryDatabase::Query& SurjectiveQueryDatabase::Query::PublishTag( const ::Eldritch2::UTF8Char* const name ) {
		if( auto internResult = _database.TryInternFactName( name ) ) {
			_facts.Insert( { internResult, Fact() } );
		}

		return *this;
	}

// ---------------------------------------------------

	ETInlineHint const SurjectiveQueryDatabase& SurjectiveQueryDatabase::Query::GetDatabase() const {
		return _database;
	}

// ---------------------------------------------------

	ETInlineHint const ::Eldritch2::IntrusiveForwardList<SurjectiveQueryDatabase::Rule::Criterion>& SurjectiveQueryDatabase::Rule::GetCriteria() const {
		return _criteria;
	}

// ---------------------------------------------------

	template <typename Comparator>
	SurjectiveQueryDatabase::Rule& SurjectiveQueryDatabase::Rule::AddCriterion( const ::Eldritch2::UTF8Char* const factName, Comparator&& comparator ) {
		class CustomCriterion : public Criterion {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			CustomCriterion( InternedFactName targetFactName, Comparator&& comparator ) : Criterion( targetFactName ), _comparator( ::std::move( comparator ) ) {}
			CustomCriterion( const CustomCriterion& ) = delete;
			CustomCriterion() = delete;

			~CustomCriterion() = default;

		// ---------------------------------------------------

			bool operator()( const Fact& fact ) const override sealed {
				return _comparator( fact );
			}

		// - DATA MEMBERS ------------------------------------

		private:
			const Comparator	_comparator;
		};

	// ---

		if( const auto criterion = new(_database.GetAllocator(), Allocator::AllocationDuration::Normal) CustomCriterion( _database.InternFactName( factName ), ::std::move( comparator ) ) ) {
			_criteria.PushFront( *criterion );
		}
		
		return *this;
	}

// ---------------------------------------------------

	ETInlineHint ::Eldritch2::Allocator& SurjectiveQueryDatabase::GetAllocator() {
		return _allocator;
	}

}	// namespace Detail

	template <typename Response>
	SurjectiveQueryDatabase<Response>::Rule::Rule( SurjectiveQueryDatabase<Response>& database, Response&& response ) : Detail::SurjectiveQueryDatabase::Rule( database ), _response( ::std::move( response ) ) {}

// ---------------------------------------------------

	template <typename Response>
	SurjectiveQueryDatabase<Response>::Rule::Rule( Rule&& rule ) : Detail::SurjectiveQueryDatabase::Rule( ::std::move( rule ) ), _response( ::std::move( rule._response ) ) {}

// ---------------------------------------------------

	template <typename Response>
	ETInlineHint const Response& SurjectiveQueryDatabase<Response>::Rule::GetResponse() const {
		return _response;
	}

// ---------------------------------------------------

	template <typename Response>
	SurjectiveQueryDatabase<Response>::SurjectiveQueryDatabase( ::Eldritch2::Allocator& allocator ) : Detail::SurjectiveQueryDatabase( allocator ), _rules( { GetAllocator(), UTF8L("Surjective Query Database Rule Allocator") } ) {}

// ---------------------------------------------------

	template <typename Response>
	SurjectiveQueryDatabase<Response>::Query::Query( const SurjectiveQueryDatabase<Response>& database, ::Eldritch2::Allocator& allocator ) : Detail::SurjectiveQueryDatabase::Query( database, allocator ) {}

// ---------------------------------------------------

	template <typename Response>
	template <typename Evaluator>
	void SurjectiveQueryDatabase<Response>::Query::EvaluateMostSuitableResponse( ::Eldritch2::Allocator& allocator, Evaluator&& evaluator ) const {
		::Eldritch2::ResizableArray<const SurjectiveQueryDatabase<Response>::Rule*>	matchingRules( { allocator, UTF8L("SurjectiveQueryDatabase::Query::EvaluateMostSuitableResponse() Temporary Allocator") } );

		CollectMatchingRules( matchingRules );

		if( matchingRules ) {
			evaluator( matchingRules.Front()->GetResponse() );
		}
	}

// ---------------------------------------------------

	template <typename Response>
	template <typename Evaluator>
	void SurjectiveQueryDatabase<Response>::Query::EvaluateAllSuitableResponses( ::Eldritch2::Allocator& allocator, Evaluator&& evaluator ) const {
		::Eldritch2::ResizableArray<const SurjectiveQueryDatabase<Response>::Rule*>	matchingRules( { allocator, UTF8L("SurjectiveQueryDatabase::Query::EvaluateAllSuitableResponses() Temporary Allocator") } );

		CollectMatchingRules( matchingRules );

		for( const auto& matchingRule : matchingRules ) {
			evaluator( matchingRule->GetResponse() );
		}
	}

// ---------------------------------------------------

	template <typename Response>
	template <typename MatchingRuleCollection>
	void SurjectiveQueryDatabase<Response>::Query::CollectMatchingRules( MatchingRuleCollection&& collection ) const {
		// We can optimize matching slightly by starting at 1-- this will cause rules that match 0 patterns to fail immediately instead of checking separately below.
		size_t	suitabilityThreshold( 1u );

		for( const auto& rule : static_cast<const SurjectiveQueryDatabase<Response>&>(GetDatabase())._rules ) {
			const auto	suitability( EvaluateRuleMatchWeight( rule ) );

			if( suitabilityThreshold <= suitability ) {
				// Clear out the matches if we've found a more specific response-- it's a better fit than what has come before.
				if( suitabilityThreshold < suitability ) {
					collection.Clear();
				}

				collection.PushBack( &rule );
			}
		}
	}

// ---------------------------------------------------

	template <typename Response>
	typename SurjectiveQueryDatabase<Response>::Rule& SurjectiveQueryDatabase<Response>::AddRule( Response&& response ) {
		_rules.PushBack( { *this, ::std::move( response ) } );

		return _rules.Back();
	}

}	// namespace Utility
}	// namespace Eldritch2