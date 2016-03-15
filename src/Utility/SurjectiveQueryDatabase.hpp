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
#include <Utility/Containers/UTF8String.hpp>
#include <Utility/Containers/HashSet.hpp>
#include <Utility/Containers/HashMap.hpp>
#include <Utility/MPL/FloatTypes.hpp>
#include <Utility/MPL/IntTypes.hpp>
//------------------------------------------------------------------//
#include <typeinfo>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Utility {
namespace Detail {

	class SurjectiveQueryDatabase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using InternedFactName	= const void*;

	// ---

		struct Fact {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref Fact instance.
			template <typename Value>
			ETInlineHint Fact( Value&& value );
			//!	Constructs this @ref Fact instance.
			ETInlineHint Fact( const Fact& ) = default;
			//!	Constructs this @ref Fact instance.
			ETInlineHint Fact( Fact&& );
			//!	Constructs this @ref Fact instance.
			ETInlineHint Fact();

			~Fact() = default;

		// ---------------------------------------------------

			template <typename FactValue>
			ETInlineHint const FactValue&	GetValueAs() const;

			template <typename FactValue>
			ETInlineHint bool				IsType() const;

		// - DATA MEMBERS ------------------------------------

		private:
			const ::std::type_info&	_typeInfo;
			char					_allocationSpace[sizeof(void*)];
		};

	// ---

		class Rule {
		// - TYPE PUBLISHING ---------------------------------

		public:
			class Criterion : public ::Eldritch2::IntrusiveForwardListBaseHook {
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			protected:
				ETInlineHint Criterion( InternedFactName targetFactName );

			public:
				virtual ~Criterion() = default;

			// ---------------------------------------------------

				ETInlineHint InternedFactName	GetTargetFactName() const;

			// ---------------------------------------------------

				virtual bool	operator()( const Fact& fact ) const abstract;

			// - DATA MEMBERS ------------------------------------

			protected:
				InternedFactName	_targetFactName;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//!	Constructs this @ref Rule instance.
			/*!	@param[in] database @ref SurjectiveQueryDatabase owning the new @ref Rule.
				*/
			Rule( SurjectiveQueryDatabase& database );
			//!	Constructs this @ref Rule instance.
			Rule( Rule&& );
			//!	Constructs this @ref Rule instance.
			Rule() = default;

			~Rule();

		// ---------------------------------------------------

			ETInlineHint const ::Eldritch2::IntrusiveForwardList<Criterion>&	GetCriteria() const;

		// ---------------------------------------------------

			template <typename Comparator>
			Rule&	AddCriterion( const ::Eldritch2::UTF8Char* const factName, Comparator&& comparator );

			Rule&	AddTagCriterion( const ::Eldritch2::UTF8Char* const tagFactName );

		// - DATA MEMBERS ------------------------------------

		private:
			SurjectiveQueryDatabase&						_database;
			::Eldritch2::IntrusiveForwardList<Criterion>	_criteria;
		};

	// ---

		class Query {
		// ---------------------------------------------------

		public:
			//!	Publishes a pattern for matching with various @ref Rule instances.
			/*!	@param[in] name Name of the pattern.
				@param[in] value Value to add to the @ref Fact collection.
				@returns A reference to *this for method chaining.
				*/
			template <typename FactValue>
			ETInlineHint Query&	PublishFact( const ::Eldritch2::UTF8Char* const name, FactValue&& value );

			ETInlineHint Query&	PublishTag( const ::Eldritch2::UTF8Char* const name );

		// ---------------------------------------------------

			ETInlineHint const SurjectiveQueryDatabase&	GetDatabase() const;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			//!	Constructs this @ref Query instance.
			/*!	@param[in] allocator @ref Allocator instance the new @ref Query should use to allocate @ref Fact data.
				*/
			Query( const SurjectiveQueryDatabase& database, ::Eldritch2::Allocator& allocator );

			~Query() = default;

		// ---------------------------------------------------

			size_t	EvaluateRuleMatchWeight( const Rule& rule ) const;

		// - DATA MEMBERS ------------------------------------

		private:
			const SurjectiveQueryDatabase&					_database;
			::Eldritch2::HashMap<InternedFactName, Fact>	_facts;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		SurjectiveQueryDatabase( ::Eldritch2::Allocator& allocator );

		~SurjectiveQueryDatabase() = default;

	// ---------------------------------------------------

		ETInlineHint ::Eldritch2::Allocator&	GetAllocator();

	// ---------------------------------------------------

		InternedFactName	TryInternFactName( const ::Eldritch2::UTF8Char* const factName ) const;

	protected:
		InternedFactName	InternFactName( const ::Eldritch2::UTF8Char* const factName );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator						_allocator;
		::Eldritch2::HashSet<::Eldritch2::UTF8String<>>	_internedStrings;
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
				@param[in] response @ref Response the new @ref Rule should manage.
				*/
			Rule( SurjectiveQueryDatabase<Response>& database, Response&& response );
			//!	Constructs this @ref Rule instance.
			Rule( Rule&& );

			~Rule() = default;

		// ---------------------------------------------------

			ETInlineHint const Response&	GetResponse() const;

		// - DATA MEMBERS ------------------------------------

		private:
			Response	_response;
		};

	// ---

		class Query : public Detail::SurjectiveQueryDatabase::Query {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			Query( const SurjectiveQueryDatabase<Response>& database, ::Eldritch2::Allocator& allocator );

			~Query() = default;

		// ---------------------------------------------------

			template <typename Evaluator>
			void	EvaluateMostSuitableResponse( ::Eldritch2::Allocator& allocator, Evaluator&& evaluator ) const;

			template <typename Evaluator>
			void	EvaluateAllSuitableResponses( ::Eldritch2::Allocator& allocator, Evaluator&& evaluator ) const;

			template <typename MatchingRuleCollection>
			void	CollectMatchingRules( MatchingRuleCollection&& collection ) const;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		SurjectiveQueryDatabase( ::Eldritch2::Allocator& allocator );

		~SurjectiveQueryDatabase() = default;

	// ---------------------------------------------------

		Rule&	AddRule( Response&& response );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ResizableArray<Rule>	_rules;
	};

}	// namespace Utility
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/SurjectiveQueryDatabase.inl>
//------------------------------------------------------------------//