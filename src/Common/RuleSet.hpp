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
#include <Common/Memory/ChildAllocator.hpp>
#include <Common/Containers/ArrayList.hpp>
#include <Common/Containers/ArrayMap.hpp>
#include <Common/Mpl/FloatTypes.hpp>
#include <Common/SymbolTable.hpp>
#include <Common/Function.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

template <typename Symbol>
union QueryFact {
	void*   asPointer;
	Symbol  asSymbol;
	float64 asFloat;
	uint64  asUInt;
	int64   asInt;
	bool    asBool;
};

// ---

template <typename SymbolTable, class Allocator>
class QueryBuilder {
	// - TYPE PUBLISHING ---------------------------------

public:
	using SymbolTableType = SymbolTable;
	using AllocatorType   = Allocator;
	using CharacterType   = typename SymbolTableType::CharacterType;
	using SymbolType      = typename SymbolTable::Symbol;
	using FactSetType     = ArrayMap<SymbolType, QueryFact<SymbolType>, LessThan<SymbolType>, AllocatorType>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref QueryBuilder instance.
	QueryBuilder(const AllocatorType& allocator, const SymbolTableType& symbols);
	//!	Constructs this @ref QueryBuilder instance.
	QueryBuilder(const QueryBuilder&) = default;

	~QueryBuilder() = default;

	// ---------------------------------------------------

public:
	const FactSetType& GetFacts() const;

	// ---------------------------------------------------

public:
	//!	Publishes a pattern for matching with various @ref Rule instances.
	/*!	@param[in] name Null-terminated C string containing the name of the fact to be added.
		@param[in] value Fact value.
		@returns A reference to *this for method chaining. */
	QueryBuilder& InsertFact(const CharacterType* name, SymbolType value);
	QueryBuilder& InsertFact(const CharacterType* name, float64 value);
	QueryBuilder& InsertFact(const CharacterType* name, uint64 value);
	QueryBuilder& InsertFact(const CharacterType* name, int64 value);
	QueryBuilder& InsertFact(const CharacterType* name, bool value);
	QueryBuilder& InsertFact(const CharacterType* name);

	// - DATA MEMBERS ------------------------------------

private:
	const SymbolTable* _symbols;
	FactSetType        _facts;
};

// ---

template <typename SymbolTable, class Allocator>
class RuleBuilder {
	// - TYPE PUBLISHING ---------------------------------

public:
	using SymbolTableType = SymbolTable;
	using AllocatorType   = Allocator;
	using CharacterType   = typename SymbolTableType::CharacterType;
	using SymbolType      = typename SymbolTable::Symbol;
	using CriteriaSetType = ArrayMap<SymbolType, Function<bool(const QueryFact<SymbolType>&)>, LessThan<SymbolType>, AllocatorType>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref RuleBuilder instance.
	RuleBuilder(const AllocatorType& allocator, SymbolTableType& symbols);
	//!	Constructs this @ref RuleBuilder instance.
	RuleBuilder(const RuleBuilder&) = default;

	~RuleBuilder() = default;

	// ---------------------------------------------------

public:
	const CriteriaSetType& GetCriteria() const;
	CriteriaSetType&       GetCriteria();

	// ---------------------------------------------------

public:
	RuleBuilder& InsertCriterion(const CharacterType* name, typename CriteriaSetType::MappedType evaluator);
	RuleBuilder& InsertCriterion(const CharacterType* name);

	// - DATA MEMBERS ------------------------------------

private:
	SymbolTableType* _symbols;
	CriteriaSetType  _criteria;
};

// ---

template <typename Response, class Allocator = MallocAllocator>
class RuleSet {
	// - TYPE PUBLISHING ---------------------------------

public:
	using SymbolTableType  = SymbolTable<Utf8Char, ChildAllocator>;
	using SymbolType       = typename SymbolTableType::Symbol;
	using QueryBuilderType = QueryBuilder<SymbolTableType, MallocAllocator>;
	using RuleBuilderType  = RuleBuilder<SymbolTableType, MallocAllocator>;
	using Rule             = Pair<typename RuleBuilderType::CriteriaSetType, Response>;
	using AllocatorType    = Allocator;
	using RuleIterator     = typename ArrayList<Rule>::ConstIterator;
	template <typename Allocator>
	using MatchList = ArrayList<RuleIterator, Allocator>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref RuleSet instance.
	RuleSet(const AllocatorType& allocator);
	//!	Constructs this @ref RuleSet instance.
	RuleSet(const RuleSet&) = default;

	~RuleSet() = default;

	// ---------------------------------------------------

public:
	const SymbolTableType& GetSymbols() const;
	SymbolTableType&       GetSymbols();

	// ---------------------------------------------------

public:
	RuleIterator Begin() const;

	RuleIterator End() const;

	// ---------------------------------------------------

public:
	template <class TemporaryAllocator>
	MatchList<TemporaryAllocator> MatchAll(const TemporaryAllocator& allocator, const QueryBuilderType& query) const;

	template <class TemporaryAllocator>
	RuleIterator Match(const TemporaryAllocator& allocator, const QueryBuilderType& query) const;

	// ---------------------------------------------------

public:
	void Insert(const RuleBuilderType& ruleBuilder, const Response& response);
	void Insert(RuleBuilderType&& ruleBuilder, Response&& response);

	void Clear();

	// - DATA MEMBERS ------------------------------------

private:
	mutable AllocatorType           _allocator;
	SymbolTableType                 _symbols;
	ArrayList<Rule, ChildAllocator> _rules;
};

// ---

template <typename InputIterator, typename FactMap>
size_t GetRuleScore(InputIterator criteria, InputIterator criteriaEnd, const FactMap& facts);

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/RuleSet.inl>
//------------------------------------------------------------------//
