/*==================================================================*\
  Hash.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Pair.hpp>
//------------------------------------------------------------------//
#include <typeinfo>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template <typename T>
	ETNoAliasHint size_t Hash<T>::operator()( const T& key, const size_t seed ) const {
		return HashBase::operator()( &key, sizeof(key), seed );
	}

// ---

	template <>
	class Hash<::Eldritch2::intptr> : public Detail::HashBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

		ETNoAliasHint size_t operator()( const ::Eldritch2::intptr key, const size_t seed = static_cast<size_t>(0) ) const {
			return static_cast<size_t>(key) ^ seed;
		}
	};

// ---

	template <>
	class Hash<::Eldritch2::uintptr> : public Detail::HashBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t operator()( const ::Eldritch2::uintptr key, const size_t seed = static_cast<size_t>(0) ) const {
			return static_cast<size_t>(key) ^ seed;
		}
	};

// ---

	template <>
	class Hash<char*> : public Detail::HashBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t operator()( const char* const key, const size_t lengthInBytes, const size_t seed ) const {
			return HashBase::operator()( key, lengthInBytes, seed );
		}

		ETNoAliasHint size_t operator()( const char* const key, const size_t seed = static_cast<size_t>(0) ) const {
			return HashBase::operator()( key, ::Eldritch2::StringLength( key ), seed );
		}
	};

	template <>
	class Hash<wchar_t*> : public Detail::HashBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t operator()( const wchar_t* const key, const size_t lengthInCharacters, const size_t seed ) const {
			// The basic hasher wants a size in bytes, so remember to multiply by the number of chars per wchar_t!
			return HashBase::operator()( key, sizeof( wchar_t ) * lengthInCharacters, seed );
		}

		ETNoAliasHint size_t operator()( const wchar_t* const key, const size_t seed = static_cast<size_t>(0) ) const {
			// The basic hasher wants a size in bytes, so remember to multiply by the number of chars per wchar_t!
			return HashBase::operator()( key, sizeof(wchar_t) * ::Eldritch2::StringLength( key ), seed );
		}
	};

	template <>
	class Hash<::std::type_info*> : public Detail::HashBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t operator()( const ::std::type_info* const key, const size_t seed = static_cast<size_t>(0) ) const {
			return key->hash_code() ^ seed;
		}
	};

// ---

	template <typename T>
	class Hash<T*> : public Detail::HashBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t operator()( const T* const key, const size_t seed = static_cast<size_t>(0) ) const {
			// Divide by type alignment in order to improve distributions (low bits are almost always zero otherwise!)
			return static_cast<size_t>( (reinterpret_cast<::Eldritch2::uintptr>(key) / sizeof(*key)) ^ seed );
		}
	};

// ---

	template <typename T>
	class Hash<const T*> : public ::Eldritch2::Hash<T*> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;
	};

// ---

	template <typename T1, typename T2>
	class Hash<::Eldritch2::Pair<T1, T2>> : public Detail::HashBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t operator()( const ::Eldritch2::Pair<T1, T2>& key, const size_t seed = static_cast<size_t>(0) ) const {
			return ::Eldritch2::Hash<T1>()(key.first, ::Eldritch2::Hash<T2>()(key.second, seed));
		}
	};

}	// namespace Eldritch2