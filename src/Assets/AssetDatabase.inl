/*==================================================================*\
  AssetDatabase.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {

	ETInlineHint AssetApiBuilder::AssetApiBuilder() : _factoriesByExtension( MallocAllocator( "Asset Database Factory Bucket Allocator" ) ) {}

// ---------------------------------------------------

	ETInlineHint AssetApiBuilder& AssetApiBuilder::DefineType( Utf8Literal extension, AssetFactory factory ) {
		_factoriesByExtension.Emplace( eastl::move( extension ), eastl::move( factory ) );

		return *this;
	}

// ---------------------------------------------------

	template <typename PublicAsset>
	ETInlineHint AssetApiBuilder& AssetApiBuilder::DefineType( Utf8Literal extension ) {
		static_assert( eastl::is_base_of<Asset, PublicAsset>::value, "PublicAsset must derive from Assets::Asset!" );

	// ---

		return DefineType( eastl::move( extension ), [] ( Allocator& allocator, const Utf8Char* path ) {
			return CreateUnique<PublicAsset>( allocator, path );
		} );
	}

// ---------------------------------------------------

	ETInlineHint AssetApiBuilder::ExtensionMap<AssetApiBuilder::AssetFactory>& AssetApiBuilder::GetFactories() {
		return _factoriesByExtension;
	}

// ---------------------------------------------------

	template <typename PublicAsset>
	ETInlineHint const PublicAsset* AssetDatabase::Require( const Utf8Char* const path ) const {
		static_assert( eastl::is_base_of<Asset, PublicAsset>::value, "AssetType must derive from Assets::Asset!" );

	// ---

		ReadLock	_( _assetsMutex );

		const auto	candidate( _assets.Find( Pair<Type, const Utf8Char*>( typeid(PublicAsset), path ), _assets.GetHashPredicate(), _assets.GetEqualityPredicate() ) );

		return candidate != _assets.End() ? static_cast<PublicAsset*>(*candidate) : nullptr;
	}

}	// namespace Assets
}	// namespace Eldritch2