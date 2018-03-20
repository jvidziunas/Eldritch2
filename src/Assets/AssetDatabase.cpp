/*==================================================================*\
  AssetDatabase.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/AssetDatabase.hpp>
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Assets {
namespace {

	ETPureFunctionHint Pair<Type, const Utf8Char*> GetId( const Asset& asset ) {
		return { typeid( asset ), asset.GetPath() };
	}

// ---------------------------------------------------

	ETPureFunctionHint Utf8Literal FindExtension( const Utf8Char* path ) {
		return Utf8Literal( FindLastInstance( path, '.' ) );
	}

}	// anonymous namespace

	ETPureFunctionHint bool	AssetDatabase::AssetEqual::operator()( const Asset* asset, Pair<Type, const Utf8Char*> id ) const {
		return ( typeid( *asset ) == id.first ) && StringsEqual( asset->GetPath(), id.second );
	}
	
// ---------------------------------------------------

	ETPureFunctionHint bool	AssetDatabase::AssetEqual::operator()( const Asset* asset0, const Asset* asset1 ) const {
		return asset0 == asset1 || (*this)( asset0, GetId( *asset1 ) );
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t AssetDatabase::AssetHash::operator()( Pair<Type, const Utf8Char*> id, size_t seed ) const {
		return HashMemory( id.second, StringLength( id.second ), GetHashCode( id.first, seed ) );
	}

// ---------------------------------------------------

	ETPureFunctionHint size_t AssetDatabase::AssetHash::operator()( const Asset* asset, size_t seed ) const {
		return (*this)( GetId( *asset ), seed );
	}

// ---------------------------------------------------

	AssetDatabase::AssetDatabase(
	) : _allocator( "Asset Database Root Allocator" ),
		_assets( MallocAllocator( "Asset Database Asset Bucket Allocator" ) ),
		_factoriesByExtension( MallocAllocator( "Asset Database Factory Bucket Allocator" ) ) {
	}

// ---------------------------------------------------

	AssetDatabase::~AssetDatabase() {
		ET_ASSERT( _assets.IsEmpty(), "Leaking package references!" );

		FreeResources();
	}

// ---------------------------------------------------

	UniquePointer<Asset> AssetDatabase::CreateAsset( const Utf8Char* path ) {
		const ExtensionMap<AssetFactory>::ConstIterator candidate( _factoriesByExtension.Find( FindExtension( path ) ) );

		if (ET_UNLIKELY(candidate == _factoriesByExtension.End())) {
			return nullptr;
		}

		UniquePointer<Asset> asset( candidate->second( _allocator, path ) );

		RegisterAsset( *asset );

		return eastl::move( asset );
	}

// ---------------------------------------------------

	bool AssetDatabase::RegisterAsset( Asset& asset ) {
		Lock	_( _assetsMutex );

		return _assets.Insert( &asset ).second;
	}

// ---------------------------------------------------

	void AssetDatabase::UnregisterAsset( Asset& asset ) {
		Lock	_( _assetsMutex );

		_assets.Erase( _assets.Find( &asset, _assets.GetHashPredicate(), EqualTo<Asset*>() ) );
	}

// ---------------------------------------------------

	ErrorCode AssetDatabase::BindResources( ExtensionMap<AssetFactory> factories ) {
		Swap( _factoriesByExtension, factories );

		return Error::None;
	}

// ---------------------------------------------------

	void AssetDatabase::FreeResources() {
		_factoriesByExtension.Clear();
	}

}	// namespace Assets
}	// namespace Eldritch2