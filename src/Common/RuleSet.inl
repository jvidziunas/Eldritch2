/*==================================================================*\
  RuleSet.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <utility>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename SymbolTable, class Allocator>
	ETInlineHint QueryBuilder<SymbolTable, Allocator>::QueryBuilder(
		const AllocatorType& allocator,
		const SymbolTableType& symbols
	) : _facts( allocator ),
		_symbols( &symbols ) {}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint const typename QueryBuilder<SymbolTable, Allocator>::FactSetType& QueryBuilder<SymbolTable, Allocator>::GetFacts() const {
		return _facts;
	}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact( const CharacterType* name, SymbolType value ) {
		if (const auto nameSymbol = _symbols->Find( name )) {
			typename typename FactSetType::MappedType fact;

			fact.asSymbol = value;

			_facts.Emplace( nameSymbol, fact );
		}

		return *this;
	}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact( const CharacterType* name, float64 value ) {
		if (const auto nameSymbol = _symbols->Find( name )) {
			typename typename FactSetType::MappedType fact;

			fact.asFloat = value;

			_facts.Emplace( nameSymbol, fact );
		}

		return *this;
	}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact( const CharacterType* name, uint64 value ) {
		if (const auto nameSymbol = _symbols->Find( name )) {
			typename typename FactSetType::MappedType fact;

			fact.asUInt = value;

			_facts.Emplace( nameSymbol, fact );
		}

		return *this;
	}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact( const CharacterType* name, int64 value ) {
		if (const auto nameSymbol = _symbols->Find( name )) {
			typename typename FactSetType::MappedType fact;

			fact.asInt = value;

			_facts.Emplace( nameSymbol, fact );
		}

		return *this;
	}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact( const CharacterType* name, bool value ) {
		if (const auto nameSymbol = _symbols->Find( name )) {
			typename typename FactSetType::MappedType fact;

			fact.asBool = value;

			_facts.Emplace( nameSymbol, fact );
		}

		return *this;
	}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact( const CharacterType* factName ) {
		if (const auto nameSymbol = _symbols->Find( factName )) {
			typename typename FactSetType::MappedType fact;

			fact.asSymbol = nullptr;

			_facts.Emplace( nameSymbol, fact );
		}

		return *this;
	}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint RuleBuilder<SymbolTable, Allocator>::RuleBuilder(
		const AllocatorType& allocator,
		SymbolTableType& symbols
	) : _criteria( allocator ),
		_symbols( &symbols ) {}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint typename RuleBuilder<SymbolTable, Allocator>::CriteriaSetType& RuleBuilder<SymbolTable, Allocator>::GetCriteria() {
		return _criteria;
	}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint RuleBuilder<SymbolTable, Allocator>& RuleBuilder<SymbolTable, Allocator>::InsertCriterion( const CharacterType* name, typename CriteriaSetType::MappedType evaluator ) {
		_criteria.Emplace( _symbols->Intern( name ), eastl::move( evaluator ) );

		return *this;
	}

// ---------------------------------------------------

	template <typename SymbolTable, class Allocator>
	ETInlineHint RuleBuilder<SymbolTable, Allocator>& RuleBuilder<SymbolTable, Allocator>::InsertCriterion( const CharacterType* name ) {
		return InsertCriterion( name, [] ( const QueryFact<SymbolType>& /*unused*/ ) { return true; } );
	}

// ---------------------------------------------------

	template <typename Response, class Allocator>
	ETInlineHint RuleSet<Response, Allocator>::RuleSet(
		const AllocatorType& allocator
	) : _allocator( allocator ),
		_symbols( ChildAllocator( _allocator, "Rule Database Symbol Allocator" ) ),
		_rules( ChildAllocator( _allocator, "Rule Database Rule Allocator" ) ) {}

// ---------------------------------------------------

	template <typename Response, class Allocator>
	ETInlineHint typename RuleSet<Response, Allocator>::RuleIterator RuleSet<Response, Allocator>::Begin() const {
		return _rules.ConstBegin();
	}

// ---------------------------------------------------

	template <typename Response, class Allocator>
	ETInlineHint typename RuleSet<Response, Allocator>::RuleIterator	RuleSet<Response, Allocator>::End() const {
		return _rules.ConstEnd();
	}

// ---------------------------------------------------

	template <typename Response, class Allocator>
	template <class TemporaryAllocator, typename UnaryPredicate>
	ETInlineHint typename RuleSet<Response, Allocator>::MatchList<TemporaryAllocator> RuleSet<Response, Allocator>::MatchAll( const TemporaryAllocator& allocator, UnaryPredicate queryBuilder ) const {
		MatchList<TemporaryAllocator>	matches( allocator );
		QueryBuilderType				query( MallocAllocator( "Query Fact Set Allocator" ), _symbols );
	//	We can optimize matching slightly by starting at 1-- this will cause rules that match 0 patterns to fail immediately.
		size_t							threshold( 1u );

	//	Prepare the query by inserting facts and setting search options according to client implementation.
		queryBuilder( query );

		for (RuleIterator rule( Begin() ), end( End() ); rule != end; ++rule) {
			const size_t score( GetRuleScore( rule->second.ConstBegin(), rule->second.ConstEnd(), query.GetFacts() ) );
			if (score < threshold) {
			//	Ignore, we've already found a more specific match.
				continue;
			}

			if (threshold < score) {
			//	Clear out the matches if we've found a more specific response-- it's a better fit than what has come before.
				matches.Clear();
			}

			matches.EmplaceBack( rule );
		}

		return eastl::move( matches );
	}

// ---------------------------------------------------

	template <typename Response, class Allocator>
	template <class TemporaryAllocator, typename UnaryPredicate>
	ETInlineHint typename RuleSet<Response, Allocator>::RuleIterator RuleSet<Response, Allocator>::Match( const TemporaryAllocator& allocator, UnaryPredicate queryBuilder ) const {
		const MatchList<TemporaryAllocator> matches( MatchAll( allocator, eastl::move( queryBuilder ) ) );

		return matches ? matches.Front() : End();
	}

// ---------------------------------------------------

	template <typename Response, class Allocator>
	template <typename UnaryPredicate>
	ETInlineHint void RuleSet<Response, Allocator>::InsertRule( Response response, UnaryPredicate ruleBuilder ) {
		RuleBuilderType	rule( _allocator, _symbols );

		ruleBuilder( rule );

		_rules.EmplaceBack( eastl::move( response ), eastl::move( rule.GetCriteria() ) );
	}

// ---------------------------------------------------

	template <typename Response, class Allocator>
	ETInlineHint void RuleSet<Response, Allocator>::Clear() {
		_rules.Clear();
		_symbols.Clear();
	}

// ---------------------------------------------------

	template <typename InputIterator, typename FactMap>
	ETInlineHint size_t GetRuleScore( InputIterator criteria, InputIterator criteriaEnd, const FactMap& facts ) {
		for (; criteria != criteriaEnd; ++criteria) {
			const auto fact( facts.Find( criteria->first ) );
			if (fact == facts.End() || !criteria->second( fact->second )) {
				return 0u;
			}
		}

		return eastl::distance( criteria, criteriaEnd );
	}

}	// namespace Eldritch2