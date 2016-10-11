/*==================================================================*\
  Cache.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/HashMap.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename Key, typename CachedObject, class LoadPolicy>
	class Cache {
	// - TYPE PUBLISHING ---------------------------------

	private:
		using AccessTicket		= Eldritch2::uint32;
		using LookupTableType	= Eldritch2::HashMap<Key, Eldritch2::Pair<CachedObject, AccessTicket>>;

	public:
		using SizeType			= typename LookupTableType::SizeType;
		using KeyType			= typename LookupTableType::KeyType;
		using MappedType		= CachedObject;
		using AllocatorType		= typename LookupTableType::AllocatorType;
		using LoadPolicyType	= LoadPolicy;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref Cache instance.
		Cache( SizeType maximumSize, LoadPolicyType&& loadPolicy, AllocatorType&& allocator );
	//!	Constructs this @ref Cache instance.
		Cache( LoadPolicyType&& loadPolicy, AllocatorType&& allocator );
	//!	Constructs this @ref Cache instance.
		Cache( AllocatorType&& allocator );

		~Cache() = default;

	// ---------------------------------------------------

	public:
		template <typename AlternateLoadPolicy>
		MappedType&	Find( const KeyType& key, const AlternateLoadPolicy& loadPolicy );
		MappedType&	Find( const KeyType& key );

		MappedType&	Insert( const KeyType& key, const MappedType& value );
		MappedType&	Insert( const KeyType& key, MappedType&& value );

		void		Erase( const KeyType& key );

		void		Clear();

	// ---------------------------------------------------

	public:
		const LoadPolicyType&	GetLoadPolicy();

	// - DATA MEMBERS ------------------------------------

	private:
		AccessTicket	_accessTicket;
		SizeType		_maximumSize;
		LoadPolicyType	_loadPolicy;
		LookupTableType	_lookupTable;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Cache.inl>
//------------------------------------------------------------------//