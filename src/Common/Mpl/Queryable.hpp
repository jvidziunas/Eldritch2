/*==================================================================*\
  Queryable.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2019 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Tuple.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
template <typename... Columns>
class QueryableSelectTraits;

template <typename... Tables>
class QueryableJoinedTableTraits;

template <typename SelectTraits, typename TableTraits>
class QueryableProjectionTraits;
} // namespace Eldritch2

namespace Eldritch2 {

template <typename... Columns>
class QueryableSelectTraits {
public:
	using RowTupleType = Tuple<typename Columns::ValueType...>;
	template <size_t Column>
	using ValueType = typename TupleElement<Column, RowTupleType>;
	template <size_t Column>
	using Reference = typename TupleElement<Column, RowTupleType>&;
	template <size_t Column>
	using ConstReference = typename ConstTupleElement<Column, RowTupleType>&;
};

// ---

template <size_t Id, typename... Columns>
class QueryableTableTraits : public QueryableSelectTraits<Columns...> {
	// - TYPE PUBLISHING ---------------------------------

public:
	using ThisType = QueryableTableTraits<Id, Columns...>;
	template <size_t /*TableIndex*/>
	using TableType = ThisType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref QueryableTableTraits instance.
	ETConstexpr QueryableTableTraits(const QueryableTableTraits&) ETNoexceptHint = default;
	//!	Constructs this @ref QueryableTableTraits instance.
	ETConstexpr QueryableTableTraits() ETNoexceptHint = default;

	~QueryableTableTraits() = default;

	// ---------------------------------------------------

public:
	template <typename TableType>
	ETConstexpr ETForceInlineHint QueryableJoinedTableTraits<TableType, ThisType> Join(TableType) const ETNoexceptHint {
		return QueryableJoinedTableTraits<TableType, ThisType>();
	}

	//! Conditions are disabled on single tables; there is nothing to join.
	template <typename... Conditions>
	void Where(Conditions...) const = delete;
};

// ---

template <typename... Tables>
class QueryableJoinedTableTraits {
	// - TYPE PUBLISHING ---------------------------------

public:
	using ThisType = QueryableJoinedTableTraits<Tables...>;
	using RowType  = TupleCat<typename Tables::RowTupleType...>;
	template <size_t TableIndex>
	using TableType = typename TupleElement<TableIndex, Tuple<Tables...>>;
	template <size_t Column>
	using ValueType = typename TupleElement<Column, RowType>;
	template <size_t Column>
	using Reference = typename TupleElement<Column, Tuple>&;
	template <size_t Column>
	using ConstReference = typename ConstTupleElement<Column, Tuple>&;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref QueryableJoinedTableTraits instance.
	ETConstexpr QueryableJoinedTableTraits(const QueryableJoinedTableTraits&) ETNoexceptHint = default;
	//!	Constructs this @ref QueryableJoinedTableTraits instance.
	ETConstexpr QueryableJoinedTableTraits() ETNoexceptHint = default;

	~QueryableJoinedTableTraits() = default;

	// ---------------------------------------------------

public:
	template <typename TableType>
	ETConstexpr ETForceInlineHint QueryableJoinedTableTraits<TableType, Tables...> Join(TableType) const ETNoexceptHint {
		return QueryableJoinedTableTraits<TableType, Tables...>();
	}

	template <typename... Conditions>
	ETConstexpr ETForceInlineHint QueryableSelectTraits<ThisType, > Where(Conditions...) const ETNoexceptHint {
	}
};

// ---

template <typename ProjectionTraits, typename Table>
class TableQuery {
	// - TYPE PUBLISHING ---------------------------------

public:
	using ProjectionTraitsType = ProjectionTraits;
	using TableType            = Table;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref TableQuery instance.
	TableQuery(const TableQuery&) = default;
	//!	Constructs this @ref TableQuery instance.
	TableQuery(Table& table) ETNoexceptHint = default;

	~TableQuery() = default;

	// - DATA MEMBERS ------------------------------------

private:
};

} // namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Common/Containers/Queryable.inl>
//------------------------------------------------------------------//
