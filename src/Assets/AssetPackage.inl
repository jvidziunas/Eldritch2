/*==================================================================*\
  AssetPackage.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	ETInlineHint bool AssetPackage::ReferencesPackage( const AssetPackage& package ) const {
		return this == &package || _dependencies.Contains( &package );
	}

// ---------------------------------------------------

	ETInlineHint Assets::ResidencyState AssetPackage::GetResidencyState() const {
		return _residencyState.load( std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint const Eldritch2::Utf8String<>& AssetPackage::GetName() const {
		return _name;
	}

// ---------------------------------------------------

	ETInlineHint void AssetPackage::TakeOwnership( Eldritch2::UniquePointer<Assets::AssetView> assetView ) {
		_assetViews.PushBack( eastl::move( assetView ) );
	}

// ---------------------------------------------------

	ETInlineHint void AssetPackage::AddDependency( const AssetPackage& package ) {
		ETRuntimeAssertWithMessage( this != &package, "Package is attempting to add itself as a dependency." );

		_dependencies.Insert( &package );
	}

// ---------------------------------------------------

	template <typename UnaryPredicate>
	ETInlineHint void AssetPackage::ForEachAssetView( UnaryPredicate&& predicate ) const {
		for( const auto& asset : _assetViews ) {
			predicate( const_cast<const AssetView&>(*asset) );
		}
	}

// ---------------------------------------------------

	template <typename UnaryPredicate>
	ETInlineHint void AssetPackage::ForEachDependency( UnaryPredicate&& predicate ) const {
		for( const auto& dependency : _dependencies ) {
			predicate( *dependency );
		}
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator==( const AssetPackage& lhs, const AssetPackage& rhs ) {
		return (&lhs == &rhs) || (lhs.GetName() == rhs.GetName());
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint bool operator!=( const AssetPackage& lhs, const AssetPackage& rhs ) {
		return lhs.GetName() != rhs.GetName();
	}

// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t GetHashCode( const AssetPackage& package, size_t seed = 0u ) {
		return GetHashCode( package.GetName(), seed );
	}

}	// namespace Assets
}	// namespace Eldritch2