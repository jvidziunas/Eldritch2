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
	const AllocatorType&   allocator,
	const SymbolTableType& symbols) :
	_facts(allocator),
	_symbols(eastl::addressof(symbols)) {
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint const typename QueryBuilder<SymbolTable, Allocator>::FactSetType& QueryBuilder<SymbolTable, Allocator>::GetFacts() const {
	return _facts;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact(const CharacterType* nameString, SymbolType value) {
	if (const auto name = _symbols->Find(nameString)) {
		typename typename FactSetType::MappedType fact;

		fact.asSymbol = value;

		_facts.Emplace(name, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact(const CharacterType* nameString, float64 value) {
	if (const auto name = _symbols->Find(nameString)) {
		typename typename FactSetType::MappedType fact;

		fact.asFloat = value;

		_facts.Emplace(name, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact(const CharacterType* nameString, uint64 value) {
	if (const auto name = _symbols->Find(nameString)) {
		typename FactSetType::MappedType fact;

		fact.asUInt = value;

		_facts.Emplace(name, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact(const CharacterType* nameString, int64 value) {
	if (const auto name = _symbols->Find(nameString)) {
		typename FactSetType::MappedType fact;

		fact.asInt = value;

		_facts.Emplace(name, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact(const CharacterType* nameString, bool value) {
	if (const auto name = _symbols->Find(nameString)) {
		typename FactSetType::MappedType fact;

		fact.asBool = value;

		_facts.Emplace(name, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::InsertFact(const CharacterType* nameString) {
	if (const auto name = _symbols->Find(nameString)) {
		typename FactSetType::MappedType fact;

		fact.asSymbol = nullptr;

		_facts.Emplace(name, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint RuleBuilder<SymbolTable, Allocator>::RuleBuilder(
	const AllocatorType& allocator,
	SymbolTableType&     symbols) :
	_criteria(allocator),
	_symbols(eastl::addressof(symbols)) {
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint typename const RuleBuilder<SymbolTable, Allocator>::CriteriaSetType& RuleBuilder<SymbolTable, Allocator>::GetCriteria() const {
	return _criteria;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint typename RuleBuilder<SymbolTable, Allocator>::CriteriaSetType& RuleBuilder<SymbolTable, Allocator>::GetCriteria() {
	return _criteria;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint RuleBuilder<SymbolTable, Allocator>& RuleBuilder<SymbolTable, Allocator>::InsertCriterion(const CharacterType* name, typename CriteriaSetType::MappedType evaluator) {
	_criteria.Emplace(_symbols->Intern(name), eastl::move(evaluator));

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint RuleBuilder<SymbolTable, Allocator>& RuleBuilder<SymbolTable, Allocator>::InsertCriterion(const CharacterType* name) {
	return InsertCriterion(name, [](const QueryFact<SymbolType>& /*unused*/) { return true; });
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint RuleSet<Response, Allocator>::RuleSet(
	const AllocatorType& allocator) :
	_allocator(allocator),
	_symbols(ChildAllocator(_allocator, "Rule Database Symbol Allocator")),
	_rules(ChildAllocator(_allocator, "Rule Database Rule Allocator")) {
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint typename RuleSet<Response, Allocator>::RuleIterator RuleSet<Response, Allocator>::Begin() const {
	return _rules.ConstBegin();
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint typename RuleSet<Response, Allocator>::RuleIterator RuleSet<Response, Allocator>::End() const {
	return _rules.ConstEnd();
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint typename const RuleSet<Response, Allocator>::SymbolTableType& RuleSet<Response, Allocator>::GetSymbols() const {
	return _symbols;
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint typename RuleSet<Response, Allocator>::SymbolTableType& RuleSet<Response, Allocator>::GetSymbols() {
	return _symbols;
}

// ---------------------------------------------------

template <typename Response, class Allocator>
template <class TemporaryAllocator>
ETInlineHint typename RuleSet<Response, Allocator>::MatchList<TemporaryAllocator> RuleSet<Response, Allocator>::MatchAll(const TemporaryAllocator& allocator, const QueryBuilderType& query) const {
	MatchList<TemporaryAllocator> matches(allocator);
	//	We can optimize matching slightly by starting at 1-- this will cause rules that match 0 patterns to fail immediately.
	size_t threshold(1u);

	for (RuleIterator rule(Begin()), end(End()); rule != end; ++rule) {
		const size_t score(GetRuleScore(rule->first.ConstBegin(), rule->first.ConstEnd(), query.GetFacts()));
		if (score < threshold) {
			//	Ignore, we've already found a more specific match.
			continue;
		}

		if (threshold < score) {
			//	Clear out the matches if we've found a more specific response-- it's a better fit than what has come before.
			matches.Clear();
		}

		matches.EmplaceBack(rule);
	}

	return eastl::move(matches);
}

// ---------------------------------------------------

template <typename Response, class Allocator>
template <class TemporaryAllocator>
ETInlineHint typename RuleSet<Response, Allocator>::RuleIterator RuleSet<Response, Allocator>::Match(const TemporaryAllocator& allocator, const QueryBuilderType& query) const {
	const MatchList<TemporaryAllocator> matches(MatchAll(allocator, query));

	return matches ? matches.Front() : End();
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint void RuleSet<Response, Allocator>::Insert(const RuleBuilderType& ruleBuilder, const Response& response) {
	_rules.EmplaceBack(ruleBuilder.GetCriteria(), response);
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint void RuleSet<Response, Allocator>::Insert(RuleBuilderType&& ruleBuilder, Response&& response) {
	_rules.EmplaceBack(eastl::move(ruleBuilder.GetCriteria()), eastl::move(response));
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint void RuleSet<Response, Allocator>::Clear() {
	_rules.Clear();
	_symbols.Clear();
}

// ---------------------------------------------------

template <typename InputIterator, typename FactMap>
ETInlineHint size_t GetRuleScore(InputIterator criteria, InputIterator criteriaEnd, const FactMap& facts) {
	for (; criteria != criteriaEnd; ++criteria) {
		const auto fact(facts.Find(criteria->first));
		if (fact == facts.End() || !criteria->second(fact->second)) {
			return 0u;
		}
	}

	return eastl::distance(criteria, criteriaEnd);
}

} // namespace Eldritch2
