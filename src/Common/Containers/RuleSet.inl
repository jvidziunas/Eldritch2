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

//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename SymbolSet, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolSet, Allocator>::Fact::Fact(typename SymbolSet::Symbol value) ETNoexceptHint {
	this->asSymbol = value;
}

// ---------------------------------------------------

template <typename SymbolSet, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolSet, Allocator>::Fact::Fact(float64 value) ETNoexceptHint {
	this->asFloat = value;
}

// ---------------------------------------------------

template <typename SymbolSet, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolSet, Allocator>::Fact::Fact(uint64 value) ETNoexceptHint {
	this->asUInt = value;
}

// ---------------------------------------------------

template <typename SymbolSet, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolSet, Allocator>::Fact::Fact(int64 value) ETNoexceptHint {
	this->asInt = value;
}

// ---------------------------------------------------

template <typename SymbolSet, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolSet, Allocator>::Fact::Fact(bool value) ETNoexceptHint {
	this->asBool = value;
}

// ---------------------------------------------------

template <typename SymbolSet, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolSet, Allocator>::Fact::Fact() ETNoexceptHint {
	this->asSymbol = SymbolSet::InvalidSymbol;
}

// ---------------------------------------------------

template <typename SymbolSet, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolSet, Allocator>::RuleBuilder(const AllocatorType& allocator, SymbolSetType& symbols) ETNoexceptHint : _criteria(allocator), _symbols(ETAddressOf(symbols)) {}

// ---------------------------------------------------

template <typename SymbolSet, class Allocator>
ETInlineHint ETForceInlineHint typename const RuleBuilder<SymbolSet, Allocator>::CriteriaSet& RuleBuilder<SymbolSet, Allocator>::GetCriteria() const ETNoexceptHint {
	return _criteria;
}

// ---------------------------------------------------

template <typename SymbolSet, class Allocator>
ETInlineHint ETForceInlineHint typename RuleBuilder<SymbolSet, Allocator>::CriteriaSet& RuleBuilder<SymbolSet, Allocator>::GetCriteria() ETNoexceptHint {
	return _criteria;
}

// ---------------------------------------------------

template <typename SymbolSet, class Allocator>
template <typename UnaryPredicate>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolSet, Allocator>& RuleBuilder<SymbolSet, Allocator>::Insert(FactNameType name, UnaryPredicate evaluator) {
	_criteria.Emplace(_symbols->Intern(name), Move(evaluator));
	return *this;
}

// ---------------------------------------------------

template <typename SymbolSet, class Allocator>
ETInlineHint ETForceInlineHint RuleBuilder<SymbolSet, Allocator>& RuleBuilder<SymbolSet, Allocator>::Insert(FactNameType name) {
	return Insert(name, [](Fact /*unused*/) ETNoexceptHint {
		return true;
	});
}

// ---------------------------------------------------

template <typename Builder, typename Response, typename Allocator>
ETInlineHint ETForceInlineHint RuleSet<Builder, Response, Allocator>::Query::Query(const AllocatorType& allocator, const RuleSet<Builder, Response, Allocator>& rules) ETNoexceptHint : _facts(allocator), _rules(ETAddressOf(rules)) {}

// ---------------------------------------------------

template <typename Builder, typename Response, typename Allocator>
template <typename Value>
ETInlineHint ETForceInlineHint typename RuleSet<Builder, Response, Allocator>::Query& RuleSet<Builder, Response, Allocator>::Query::Insert(FactNameType name, Value value) {
	if (const auto fact = _rules->Find(name)) {
		_facts.Emplace(fact, FactType(value));
	}

	return *this;
}

// ---------------------------------------------------

template <typename Builder, typename Response, typename Allocator>
ETInlineHint ETForceInlineHint typename RuleSet<Builder, Response, Allocator>::Query& RuleSet<Builder, Response, Allocator>::Query::Insert(FactNameType name) {
	if (const auto fact = _rules->Find(name)) {
		_facts.Emplace(fact, FactType());
	}

	return *this;
}

// ---------------------------------------------------

template <typename Builder, typename Response, typename Allocator>
template <class Allocator2>
ETInlineHint ArrayList<typename RuleSet<Builder, Response, Allocator>::MatchType, Allocator2> RuleSet<Builder, Response, Allocator>::Query::GetMatches(const Allocator2& allocator) const {
	ArrayList<MatchType, Allocator2> matches(allocator);
	//	We can optimize matching slightly by starting at 1-- this will cause rules that match 0 patterns to fail immediately.
	ScoreType threshold(1u);

	for (typename RuleList::ConstReference rule : _rules->_rules) {
		const auto score(ScoreCriteria(rule->first, query.GetFacts()));
		if (score < threshold) {
			continue; // Ignore, we've already found a more specific match.
		}

		if (threshold < score) {
			matches.Clear(); //	Clear previous findings if a better-quality match is found.
		}

		matches.EmplaceBack(rule);
	}

	return Move(matches);
}

// ---------------------------------------------------

template <typename Builder, typename Response, typename Allocator>
template <class Allocator2>
ETInlineHint ETForceInlineHint typename RuleSet<Builder, Response, Allocator>::MatchType RuleSet<Builder, Response, Allocator>::Query::GetBestMatch(const Allocator2& allocator) const {
	const ArrayList<MatchType, Allocator2> matches(MatchAll(allocator, query));
	return matches ? matches.Front() : End();
}

// ---------------------------------------------------

template <typename Builder, typename Response, typename Allocator>
template <typename... SymbolTableArgs>
ETInlineHint ETForceInlineHint RuleSet<Builder, Response, Allocator>::RuleSet(const AllocatorType& allocator, SymbolTableArgs&&... symbolTableArgs) ETNoexceptHint : Builder::SymbolTableType(Forward<SymbolTableArgs>(symbolTableArgs)...), _rules(allocator) {}

// ---------------------------------------------------

template <typename Builder, typename Response, typename Allocator>
ETInlineHint ETForceInlineHint void RuleSet<Builder, Response, Allocator>::Emplace(const RuleBuilderType& rule, const Response& response) {
	_rules.EmplaceBack(rule.GetCriteria(), response);
}

// ---------------------------------------------------

template <typename Builder, typename Response, typename Allocator>
ETInlineHint ETForceInlineHint void RuleSet<Builder, Response, Allocator>::Emplace(RuleBuilderType&& rule, Response&& response) {
	_rules.EmplaceBack(Move(rule.GetCriteria()), Move(response));
}

// ---------------------------------------------------

template <typename Builder, typename Response, typename Allocator>
ETInlineHint ETForceInlineHint typename RuleSet<Builder, Response, Allocator>::Query RuleSet<Builder, Response, Allocator>::BeginQuery(const AllocatorType& allocator) const {
	return Query(allocator, *this);
}

// ---------------------------------------------------

template <typename Builder, typename Response, typename Allocator>
template <typename FactMap>
ETInlineHint typename RuleSet<Builder, Response, Allocator>::ScoreType RuleSet<Builder, Response, Allocator>::Score(const FactMap& facts) const ETNoexceptHint {
	ScoreType score(0u);
	for (typename FactMap::ConstReference criterion : _rules) {
		const auto fact(_rules.Find(criteria->first));
		if (fact == facts.End() || !criteria->second(fact->second)) {
			return 0u;
		}

		++score;
	}

	return score;
}

} // namespace Eldritch2
