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
	using SymbolType      = typename SymbolTableType::Symbol;
	using FactType        = QueryFact<SymbolType>;
	using FactMapType     = ArrayMap<SymbolType, FactType, LessThan<SymbolType>, AllocatorType>;
	using FactNameType    = AbstractStringView<typename SymbolTableType::CharacterType>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref QueryBuilder instance.
	QueryBuilder(const AllocatorType& allocator, const SymbolTableType& symbols);
	//!	Constructs this @ref QueryBuilder instance.
	QueryBuilder(const QueryBuilder&) = default;

	~QueryBuilder() = default;

	// ---------------------------------------------------

public:
	const FactMapType& GetFacts() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	//!	Publishes a pattern for matching with various @ref Rule instances.
	/*!	@param[in] name String view containing the name of the fact to be added.
		@param[in] value Fact value.
		@returns A reference to *this for method chaining. */
	QueryBuilder& Insert(FactNameType name, SymbolType value);
	QueryBuilder& Insert(FactNameType name, float64 value);
	QueryBuilder& Insert(FactNameType name, uint64 value);
	QueryBuilder& Insert(FactNameType name, int64 value);
	QueryBuilder& Insert(FactNameType name, bool value);
	QueryBuilder& Insert(FactNameType name);

	// - DATA MEMBERS ------------------------------------

private:
	const SymbolTable* _symbols;
	FactMapType        _facts;
};

// ---

template <typename SymbolTable, class Allocator>
class RuleBuilder {
	// - TYPE PUBLISHING ---------------------------------

public:
	using SymbolTableType = SymbolTable;
	using AllocatorType   = Allocator;
	using SymbolType      = typename SymbolTable::Symbol;
	using CriteriaSetType = ArrayMap<SymbolType, Function<bool(const QueryFact<SymbolType>&) ETNoexceptHint>, LessThan<SymbolType>, AllocatorType>;
	using FactNameType    = AbstractStringView<typename SymbolTableType::CharacterType>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref RuleBuilder instance.
	RuleBuilder(const AllocatorType& allocator, SymbolTableType& symbols);
	//!	Constructs this @ref RuleBuilder instance.
	RuleBuilder(const RuleBuilder&) = default;

	~RuleBuilder() = default;

	// ---------------------------------------------------

public:
	const CriteriaSetType& GetCriteria() const ETNoexceptHint;
	CriteriaSetType&       GetCriteria() ETNoexceptHint;

	// ---------------------------------------------------

public:
	RuleBuilder& Insert(FactNameType name, typename CriteriaSetType::MappedType evaluator);
	RuleBuilder& Insert(FactNameType name);

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
	using AllocatorType    = Allocator;
	using SymbolTableType  = SymbolTable<Utf8Char, ChildAllocator>;
	using SymbolType       = typename SymbolTableType::Symbol;
	using QueryBuilderType = QueryBuilder<SymbolTableType, MallocAllocator>;
	using RuleBuilderType  = RuleBuilder<SymbolTableType, MallocAllocator>;
	using Rule             = Pair<typename RuleBuilderType::CriteriaSetType, Response>;
	using RuleIterator     = typename ArrayList<Rule>::ConstIterator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref RuleSet instance.
	RuleSet(const AllocatorType& allocator);
	//!	Constructs this @ref RuleSet instance.
	RuleSet(const RuleSet&) = default;

	~RuleSet() = default;

	// ---------------------------------------------------

public:
	const SymbolTableType& GetSymbols() const ETNoexceptHint;
	SymbolTableType&       GetSymbols() ETNoexceptHint;

	// ---------------------------------------------------

public:
	RuleIterator Begin() const ETNoexceptHint;

	RuleIterator End() const ETNoexceptHint;

	// ---------------------------------------------------

public:
	template <class TemporaryAllocator>
	ArrayList<RuleIterator, TemporaryAllocator> MatchAll(const TemporaryAllocator& allocator, const QueryBuilderType& query) const;

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
