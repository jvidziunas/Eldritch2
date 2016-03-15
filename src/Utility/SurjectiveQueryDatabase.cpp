/*==================================================================*\
  SurjectiveQueryDatabase.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/SurjectiveQueryDatabase.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Utility;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Utility {
namespace Detail {

	SurjectiveQueryDatabase::Query::Query( const SurjectiveQueryDatabase& database, Allocator& allocator ) : _database( database ), _facts( { allocator, UTF8L("Surjective Query Fact Collection Allocator") } ) {}

// ---------------------------------------------------

	size_t SurjectiveQueryDatabase::Query::EvaluateRuleMatchWeight( const Rule& rule ) const {
		size_t	matchedFacts( 0u );

		for( const auto& criterion : rule.GetCriteria() ) {
			const auto	matchedFact( _facts.Find( criterion.GetTargetFactName() ) );

			if( matchedFact == _facts.End() || !criterion( matchedFact->second ) ) {
				return 0;
			}

			++matchedFacts;
		}

		return matchedFacts;
	}

// ---------------------------------------------------

	SurjectiveQueryDatabase::Rule::Rule( SurjectiveQueryDatabase& database ) : _database( database ) {}

// ---------------------------------------------------

	SurjectiveQueryDatabase::Rule::Rule( Rule&& rule ) : _database( rule._database ), _criteria( ::std::move( rule._criteria ) ) {}

// ---------------------------------------------------

	SurjectiveQueryDatabase::Rule::~Rule() {
		auto&	allocator( _database.GetAllocator() );

		_criteria.ClearAndDispose( [&allocator] ( Criterion& criterion ) {
			allocator.Delete( criterion );
		} );
	}

// ---------------------------------------------------

	SurjectiveQueryDatabase::Rule& SurjectiveQueryDatabase::Rule::AddTagCriterion( const UTF8Char* const tagFactName ) {
		class TagCriterion : public Criterion {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			TagCriterion( InternedFactName targetFactName ) : Criterion( targetFactName ) {}

			~TagCriterion() = default;

		// ---------------------------------------------------

			bool operator()( const Fact& fact ) const override sealed {
				return true;
			}
		};

	// ---

		if( const auto criterion = new(_database.GetAllocator(), Allocator::AllocationDuration::Normal) TagCriterion( _database.InternFactName( tagFactName ) ) ) {
			_criteria.PushFront( *criterion );
		}
		
		return *this;
	}

// ---------------------------------------------------

	SurjectiveQueryDatabase::SurjectiveQueryDatabase( Allocator& allocator ) : _allocator( allocator, UTF8L("Surjective Query Database Root Allocator") ), _internedStrings( { _allocator, UTF8L("Surjective Query Database Interned String Collection Allocator") } ) {}

// ---------------------------------------------------

	SurjectiveQueryDatabase::InternedFactName SurjectiveQueryDatabase::TryInternFactName( const UTF8Char* const factName ) const {
		auto	candidate( _internedStrings.Find( factName ) );

		if( candidate != _internedStrings.End() ) {
			return static_cast<InternedFactName>(candidate->GetCharacterArray());
		}

		return static_cast<InternedFactName>(nullptr);
	}

// ---------------------------------------------------

	SurjectiveQueryDatabase::InternedFactName SurjectiveQueryDatabase::InternFactName( const UTF8Char* const factName ) {
		auto	candidate( _internedStrings.Find( factName ) );

		if( candidate == _internedStrings.End() ) {
			candidate = _internedStrings.Insert( { factName, FindEndOfString( factName ), { GetAllocator(), UTF8L("Interned String Allocator") } } ).first;
		}

		return static_cast<InternedFactName>(candidate->GetCharacterArray());
	}

}	// namespace Detail
}	// namespace Utility
}	// namespace Eldritch2