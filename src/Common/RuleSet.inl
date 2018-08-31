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
ETInlineHint QueryBuilder<SymbolTable, Allocator>::QueryBuilder(const AllocatorType& allocator, const SymbolTableType& symbols) :
	_facts(allocator),
	_symbols(ETAddressOf(symbols)) {
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint ETForceInlineHint const typename QueryBuilder<SymbolTable, Allocator>::FactMapType& QueryBuilder<SymbolTable, Allocator>::GetFacts() const ETNoexceptHint {
	return _facts;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::Insert(FactNameType name, SymbolType value) {
	if (const auto id = _symbols->Find(name)) {
		FactType fact;
		fact.asSymbol = value;

		_facts.Emplace(id, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::Insert(FactNameType name, float64 value) {
	if (const auto id = _symbols->Find(name)) {
		FactType fact;
		fact.asFloat = value;

		_facts.Emplace(id, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::Insert(FactNameType name, uint64 value) {
	if (const auto id = _symbols->Find(name)) {
		FactType fact;
		fact.asUInt = value;

		_facts.Emplace(id, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::Insert(FactNameType name, int64 value) {
	if (const auto id = _symbols->Find(name)) {
		FactType fact;
		fact.asInt = value;

		_facts.Emplace(id, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::Insert(FactNameType name, bool value) {
	if (const auto id = _symbols->Find(name)) {
		FactType fact;
		fact.asBool = value;

		_facts.Emplace(id, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint QueryBuilder<SymbolTable, Allocator>& QueryBuilder<SymbolTable, Allocator>::Insert(FactNameType name) {
	if (const auto id = _symbols->Find(name)) {
		FactType fact;
		fact.asSymbol = nullptr;

		_facts.Emplace(id, fact);
	}

	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolTable, Allocator>::RuleBuilder(const AllocatorType& allocator, SymbolTableType& symbols) :
	_criteria(allocator),
	_symbols(ETAddressOf(symbols)) {
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint ETForceInlineHint typename const RuleBuilder<SymbolTable, Allocator>::CriteriaSetType& RuleBuilder<SymbolTable, Allocator>::GetCriteria() const ETNoexceptHint {
	return _criteria;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint ETForceInlineHint typename RuleBuilder<SymbolTable, Allocator>::CriteriaSetType& RuleBuilder<SymbolTable, Allocator>::GetCriteria() ETNoexceptHint {
	return _criteria;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolTable, Allocator>& RuleBuilder<SymbolTable, Allocator>::Insert(FactNameType name, typename CriteriaSetType::MappedType evaluator) {
	_criteria.Emplace(_symbols->Intern(name), eastl::move(evaluator));
	return *this;
}

// ---------------------------------------------------

template <typename SymbolTable, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolTable, Allocator>& RuleBuilder<SymbolTable, Allocator>::Insert(FactNameType name) {
	return Insert(name, [](const QueryFact<SymbolType>& /*unused*/) { return true; });
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint ETForceInlineHint RuleSet<Response, Allocator>::RuleSet(const AllocatorType& allocator) :
	_allocator(allocator),
	_symbols(ChildAllocator(_allocator, "Rule Database Symbol Allocator")),
	_rules(ChildAllocator(_allocator, "Rule Database Rule Allocator")) {
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint ETForceInlineHint typename RuleSet<Response, Allocator>::RuleIterator RuleSet<Response, Allocator>::Begin() const ETNoexceptHint {
	return _rules.ConstBegin();
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint ETForceInlineHint typename RuleSet<Response, Allocator>::RuleIterator RuleSet<Response, Allocator>::End() const ETNoexceptHint {
	return _rules.ConstEnd();
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint ETForceInlineHint typename const RuleSet<Response, Allocator>::SymbolTableType& RuleSet<Response, Allocator>::GetSymbols() const ETNoexceptHint {
	return _symbols;
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint ETForceInlineHint typename RuleSet<Response, Allocator>::SymbolTableType& RuleSet<Response, Allocator>::GetSymbols() ETNoexceptHint {
	return _symbols;
}

// ---------------------------------------------------

template <typename Response, class Allocator>
template <class TemporaryAllocator>
ETInlineHint ArrayList<typename RuleSet<Response, Allocator>::RuleIterator, TemporaryAllocator> RuleSet<Response, Allocator>::MatchAll(const TemporaryAllocator& allocator, const QueryBuilderType& query) const {
	ArrayList<RuleIterator, TemporaryAllocator> matches(allocator);
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
ETInlineHint ETForceInlineHint typename RuleSet<Response, Allocator>::RuleIterator RuleSet<Response, Allocator>::Match(const TemporaryAllocator& allocator, const QueryBuilderType& query) const {
	const ArrayList<RuleIterator, TemporaryAllocator> matches(MatchAll(allocator, query));
	return matches ? matches.Front() : End();
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint ETForceInlineHint void RuleSet<Response, Allocator>::Insert(const RuleBuilderType& ruleBuilder, const Response& response) {
	_rules.EmplaceBack(ruleBuilder.GetCriteria(), response);
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint ETForceInlineHint void RuleSet<Response, Allocator>::Insert(RuleBuilderType&& ruleBuilder, Response&& response) {
	_rules.EmplaceBack(eastl::move(ruleBuilder.GetCriteria()), eastl::move(response));
}

// ---------------------------------------------------

template <typename Response, class Allocator>
ETInlineHint ETForceInlineHint void RuleSet<Response, Allocator>::Clear() {
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
