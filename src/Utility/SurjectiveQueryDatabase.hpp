/*==================================================================*\
  SurjectiveQueryEngine.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardList.hpp>
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Containers/Utf8String.hpp>
#include <Utility/Containers/HashSet.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
#include <Utility/TypeHandle.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {
namespace Detail {

	class SurjectiveQueryDatabase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using InternedFactName	= const void*;

	// ---

		class Fact {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Fact instance.
			template <typename Value, class = eastl::enable_if<eastl::is_trivially_destructible<Value>::value>>
			Fact( Value&& value );
		//!	Constructs this @ref Fact instance.
			Fact( const Fact& ) = default;
		//!	Constructs this @ref Fact instance.
			Fact( Fact&& );
		//!	Constructs this @ref Fact instance.
			Fact();

			~Fact() = default;

		// ---------------------------------------------------

		public:
			template <typename FactValue>
			const FactValue&	GetValueAs() const;

			template <typename FactValue>
			bool				IsType() const;

		// - DATA MEMBERS ------------------------------------

		private:
			Eldritch2::TypeHandle	_type;
			char					_allocationSpace[sizeof(void*)];
		};

	// ---

		class ETPureAbstractHint Query {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
		//!	Constructs this @ref Query instance.
		/*!	@param[in] allocator @ref Allocator instance the new @ref Query should use to allocate @ref Fact data. */
			Query( const SurjectiveQueryDatabase& database, Eldritch2::Allocator& allocator );

			~Query() = default;

		// ---------------------------------------------------

		public:
		//!	Publishes a pattern for matching with various @ref Rule instances.
		/*!	@param[in] name Name of the pattern.
			@param[in] value Value to add to the @ref Fact collection.
			@returns A reference to *this for method chaining. */
			template <typename FactValue>
			Query&	PublishFact( const Eldritch2::Utf8Char* const name, FactValue&& value );

			Query&	PublishTag( const Eldritch2::Utf8Char* const name );

		// ---------------------------------------------------

		public:
			const Fact*	TryGetFactByName( InternedFactName name ) const;

		// - DATA MEMBERS ------------------------------------

		protected:
			const SurjectiveQueryDatabase&				_database;
			Eldritch2::HashMap<InternedFactName, Fact>	_facts;
		};

	// ---

		class Rule {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using Criterion = Eldritch2::Pair<InternedFactName, std::function<bool ( const Fact& )>>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Rule instance.
		/*!	@param[in] database @ref SurjectiveQueryDatabase owning the new @ref Rule. */
			Rule( SurjectiveQueryDatabase& database );
		//!	Constructs this @ref Rule instance.
			Rule( const Rule& ) = default;
		//!	Constructs this @ref Rule instance.
			Rule( Rule&& );
		//!	Constructs this @ref Rule instance.
			Rule() = default;

			~Rule() = default;

		// ---------------------------------------------------

		public:
			size_t	EvaluateMatchWeight( const Query& query ) const;

		// ---------------------------------------------------

		public:
			template <typename Evaluator>
			Rule&	AddCriterion( const Eldritch2::Utf8Char* const factName, Evaluator&& comparator );

			Rule&	AddTagCriterion( const Eldritch2::Utf8Char* const tagFactName );

		// - DATA MEMBERS ------------------------------------

		private:
			SurjectiveQueryDatabase&				_database;
			Eldritch2::ResizableArray<Criterion>	_criteria;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SurjectiveQueryDatabase instance.
		SurjectiveQueryDatabase( Eldritch2::Allocator& allocator );
	//!	Constructs this @ref SurjectiveQueryDatabase instance.
		SurjectiveQueryDatabase( const SurjectiveQueryDatabase& ) = default;

		~SurjectiveQueryDatabase() = default;

	// ---------------------------------------------------

	public:
		Eldritch2::Allocator&	GetAllocator();

	// ---------------------------------------------------

	public:
		InternedFactName	TryInternFactName( const Eldritch2::Utf8Char* const factName ) const;

		InternedFactName	InternFactName( const Eldritch2::Utf8Char* const factName );

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator			_allocator;
		Eldritch2::HashSet<Eldritch2::Utf8String<>>	_internedStrings;
	};

}	// namespace Detail

	template <typename Response>
	class SurjectiveQueryDatabase : public Detail::SurjectiveQueryDatabase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class Rule : public Detail::SurjectiveQueryDatabase::Rule {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Rule instance.
		/*!	@param[in] database @ref SurjectiveQueryDatabase that will own the new @ref Rule.
			@param[in] response @ref Response the new @ref Rule should manage. */
			Rule( SurjectiveQueryDatabase<Response>& database, Response&& response );
		//!	Constructs this @ref Rule instance.
			Rule( Rule&& );

			~Rule() = default;

		// ---------------------------------------------------

		public:
			const Response&	GetResponse() const;

		// - DATA MEMBERS ------------------------------------

		private:
			Response	_response;
		};

	// ---

		class Query : public Detail::SurjectiveQueryDatabase::Query {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this @ref Query instance.
			Query( const SurjectiveQueryDatabase<Response>& database, Eldritch2::Allocator& allocator );
		//!	Constructs this @ref Query instance.
			Query( const Query& ) = default;

			~Query() = default;

		// ---------------------------------------------------

		public:
			template <typename Evaluator>
			void	EvaluateMostSuitableResponse( Eldritch2::Allocator& allocator, Evaluator&& evaluator ) const;

			template <typename Evaluator>
			void	EvaluateAllSuitableResponses( Eldritch2::Allocator& allocator, Evaluator&& evaluator ) const;

			template <typename MatchingRuleCollection>
			void	CollectMatchingRules( MatchingRuleCollection&& collection ) const;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref SurjectiveQueryDatabase instance.
		SurjectiveQueryDatabase( Eldritch2::Allocator& allocator );
	//!	Constructs this @ref SurjectiveQueryDatabase instance.
		SurjectiveQueryDatabase( const SurjectiveQueryDatabase& ) = default;

		~SurjectiveQueryDatabase() = default;

	// ---------------------------------------------------

	public:
		Rule&	AddRule( Response&& response );

	// - DATA MEMBERS ------------------------------------

	private:
		Eldritch2::ResizableArray<Rule>	_rules;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/SurjectiveQueryDatabase.inl>
//------------------------------------------------------------------//