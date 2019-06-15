/*==================================================================*\
  RuleSet.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Memory/MallocAllocator.hpp>
#include <Common/Containers/ArrayList.hpp>
#include <Common/Containers/ArrayMap.hpp>
#include <Common/Mpl/FloatTypes.hpp>
#include <Common/Function.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename SymbolSet, class Allocator>
class RuleBuilder {
	// - TYPE PUBLISHING ---------------------------------

public:
	union Fact {
		Fact(typename SymbolSet::Symbol) ETNoexceptHint;
		Fact(float64) ETNoexceptHint;
		Fact(uint64) ETNoexceptHint;
		Fact(int64) ETNoexceptHint;
		Fact(bool) ETNoexceptHint;
		Fact() ETNoexceptHint;

		// ---

		typename SymbolSet::Symbol asSymbol;
		float64                    asFloat;
		uint64                     asUInt;
		int64                      asInt;
		bool                       asBool;
	};

	// ---

public:
	using SymbolSetType = SymbolSet;
	using CriteriaSet   = ArrayMap<typename SymbolSet::Symbol, Function<bool(Fact)>, LessThan<typename SymbolSet::Symbol>, Allocator>;
	using SymbolType    = typename CriteriaSet::KeyType;
	using AllocatorType = typename CriteriaSet::AllocatorType;
	using FactNameType  = AbstractStringSpan<typename SymbolSetType::CharacterType>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref RuleBuilder instance.
	RuleBuilder(const AllocatorType& allocator, SymbolSetType& symbols) ETNoexceptHint;
	//!	Constructs this @ref RuleBuilder instance.
	RuleBuilder(const RuleBuilder&) = default;

	~RuleBuilder() = default;

	// ---------------------------------------------------

public:
	const CriteriaSet& GetCriteria() const ETNoexceptHint;
	CriteriaSet&       GetCriteria() ETNoexceptHint;

	// ---------------------------------------------------

public:
	template <typename UnaryPredicate>
	RuleBuilder& Insert(FactNameType name, UnaryPredicate evaluator);
	RuleBuilder& Insert(FactNameType name);

	// - DATA MEMBERS ------------------------------------

private:
	SymbolSetType* _symbols;
	CriteriaSet    _criteria;
};

// ---

template <typename Builder, typename Response, typename Allocator = MallocAllocator>
class RuleSet : public Builder::SymbolSetType {
	// - TYPE PUBLISHING ---------------------------------

public:
	using RuleBuilderType = Builder;
	using RuleList        = ArrayList<Pair<typename RuleBuilderType::CriteriaSetType, Response>, Allocator>;
	using AllocatorType   = typename RuleList::AllocatorType;
	using RuleType        = typename RuleList::ValueType;
	using MatchType       = typename RuleList::ConstIterator;
	using SizeType        = typename RuleList::SizeType;
	using ScoreType       = typename RuleList::SizeType;

	// ---

public:
	class Query {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using FactMap       = ArrayMap<typename RuleBuilderType::SymbolType, typename RuleBuilderType::FactType, LessThan<typename RuleBuilderType::SymbolType>, Allocator>;
		using FactNameType  = typename RuleBuilderType::SymbolTableType;
		using AllocatorType = typename FactMap::AllocatorType;
		using FactType      = typename FactMap::MappedType;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref Query instance.
		Query(const AllocatorType& allocator, const RuleSet<Builder, Response, Allocator>& rules) ETNoexceptHint;
		//!	Constructs this @ref Query instance.
		Query(const Query&) = default;
		//!	Constructs this @ref Query instance.
		Query(Query&&) ETNoexceptHint = default;

		~Query() = default;

		// ---------------------------------------------------

	public:
		//!	Publishes a pattern for matching with various @ref Rule instances.
		/*!	@param[in] fact String view containing the name of the fact to be added.
			@param[in] value Fact value.
			@returns A reference to *this for method chaining. */
		template <typename Value>
		Query& Insert(FactNameType fact, Value value);
		Query& Insert(FactNameType fact);

		// ---------------------------------------------------

	public:
		template <class Allocator2>
		ArrayList<MatchType, Allocator2> GetMatches(const Allocator2& allocator) const;

		template <class Allocator2>
		MatchType GetBestMatch(const Allocator2& allocator) const;

		// - DATA MEMBERS ------------------------------------

	private:
		const RuleSet* _rules;
		FactMap        _facts;
	};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref RuleSet instance.
	template <typename... SymbolTableArgs>
	RuleSet(const AllocatorType& allocator, SymbolTableArgs&&... args) ETNoexceptHint;
	//!	Constructs this @ref RuleSet instance.
	RuleSet(const RuleSet&) = default;

	~RuleSet() = default;

	// ---------------------------------------------------

public:
	template <typename FactMap>
	ScoreType Score(const FactMap& facts) const ETNoexceptHint;

	void Emplace(const RuleBuilderType& rule, const Response& response);
	void Emplace(RuleBuilderType&& rule, Response&& response);

	Query BeginQuery(const AllocatorType& allocator) const;

	// - DATA MEMBERS ------------------------------------

private:
	RuleList _rules;
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/RuleSet.inl>
//------------------------------------------------------------------//
