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

namespace Eldritch2 {
namespace Utility {
namespace Detail {

	SurjectiveQueryDatabase::Query::Query( const SurjectiveQueryDatabase& database, Allocator& allocator ) : _database( database ), _facts( { allocator, "Surjective Query Fact Collection Allocator" } ) {}

// ---------------------------------------------------

	SurjectiveQueryDatabase::Rule::Rule( SurjectiveQueryDatabase& database ) : _database( database ), _criteria( { database.GetAllocator(), "Rule Criteria Allocator" } ) {}

// ---------------------------------------------------

	SurjectiveQueryDatabase::Rule::Rule( Rule&& rule ) : _database( rule._database ), _criteria( eastl::move( rule._criteria ) ) {}

// ---------------------------------------------------

	size_t SurjectiveQueryDatabase::Rule::EvaluateMatchWeight( const Query& query ) const {
		for( const auto& criterion : _criteria ) {
			const auto& fact( query.TryGetFactByName( criterion.first ) );

			if( !fact || !criterion.second( *fact ) ) {
				return 0;
			}
		}

		return _criteria.GetSize();
	}

// ---------------------------------------------------

	SurjectiveQueryDatabase::Rule& SurjectiveQueryDatabase::Rule::AddTagCriterion( const Utf8Char* const tagFactName ) {
		return AddCriterion( tagFactName, [] ( const Fact& /*fact*/ ) { return true; } );
	}

// ---------------------------------------------------

	SurjectiveQueryDatabase::SurjectiveQueryDatabase( Allocator& allocator ) : _allocator( allocator, "Surjective Query Database Root Allocator" ), _internedStrings( { _allocator, "Surjective Query Database Interned String Collection Allocator" } ) {}

// ---------------------------------------------------

	SurjectiveQueryDatabase::InternedFactName SurjectiveQueryDatabase::TryInternFactName( const Utf8Char* const factName ) const {
		const auto	candidate( _internedStrings.Find( factName, Eldritch2::Hash<const Utf8Char*>(), eastl::equal_to_2<Utf8String<>, const Utf8Char*>() ) );

		if( candidate != _internedStrings.End() ) {
			return static_cast<InternedFactName>(candidate->AsCString());
		}

		return static_cast<InternedFactName>(nullptr);
	}

// ---------------------------------------------------

	SurjectiveQueryDatabase::InternedFactName SurjectiveQueryDatabase::InternFactName( const Utf8Char* const factName ) {
		auto	candidate( _internedStrings.Find( factName, Eldritch2::Hash<const Utf8Char*>(), eastl::equal_to_2<Utf8String<>, const Utf8Char*>() ) );

		if( candidate == _internedStrings.End() ) {
			candidate = _internedStrings.Insert( { factName, FindEndOfString( factName ), { GetAllocator(), "Interned String Allocator" } } ).first;
		}

		return static_cast<InternedFactName>(candidate->AsCString());
	}

}	// namespace Detail
}	// namespace Utility
}	// namespace Eldritch2