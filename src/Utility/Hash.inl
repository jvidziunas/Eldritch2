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
#include <type_traits>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	//!	HashSelector is responsible for delegating out to the correct hash function for a type based on a specific set of type traits.
	template <typename KeyType>
	struct HashSelector {
		template <typename FunctionSignature>
		struct HasMemberHashFunction {
			static_assert( ::std::integral_constant<FunctionSignature, false>::value, "Template parameter needs to be of function type." );
		};

	// ---

		template <typename DesiredReturn, typename... Arguments>
		struct HasMemberHashFunction<DesiredReturn ( Arguments... )> {
		private:
			template <typename T>
			static typename ::std::is_same<DesiredReturn, decltype(::std::declval<const T>().GetHashCode( ::std::declval<Arguments>()... ))>::type Check( T* );

			template <typename>
			static ::std::false_type Check( ... );

		// ---

			using Type	= decltype(Check<KeyType>(nullptr));

		public:
			static const bool	value = Type::value;
		};

	// ---

		template <typename FunctionSignature>
		struct HasFreeHashFunction {
			static_assert( ::std::integral_constant<FunctionSignature, false>::value, "Template parameter needs to be of function type." );
		};

	// ---

		template <typename DesiredReturn, typename... Arguments>
		struct HasFreeHashFunction<DesiredReturn ( Arguments... )> {
		private:
			template <typename T>
			static typename ::std::is_same<DesiredReturn, decltype(GetHashCode( ::std::declval<const T>(), ::std::declval<Arguments>()... ))>::type Check( T* );

			template <typename>
			static ::std::false_type Check( ... );

		// ---

			using Type = decltype(Check<KeyType>( nullptr ));

		public:
			static const bool	value = Type::value;
		};

	// ---

		template <bool hasMemberHashFunction = HasMemberHashFunction<size_t ( const size_t )>::value>
		struct GetHashCodeDispatcher {
			static bool Dispatch( const KeyType& /*key*/, const size_t /*seed*/ );
		};

	public:
		static ETForceInlineHint ETNoAliasHint size_t Invoke( const KeyType& key, const size_t seed, const ::std::true_type /*hasMemberHash*/ ) {
			static_assert( !HasFreeHashFunction<size_t( const size_t )>::value,
						   "Incorrect hash behavior detected!\n"
						   "Types to be hashed must have either a member function with signature 'size_t GetHashCode( const size_t ) const'\n"
						   "or a free (optionally friend) function in the containing namespace with signature 'size_t GetHashCode( <Type>&, const size_t )'\n"
						   "as per Koenig lookup.\n"
						   "\n"
						   "If you are encountering this error, and you know at least one one such function exists, then there is likely an alternate form \n"
						   "that exists somewhere in the translation unit, causing ambiguity. Templates will contribute to the overload set!\n" );

		// ---

			return key.GetHashCode( seed );
		}

		static ETForceInlineHint ETNoAliasHint size_t Invoke( const KeyType& key, const size_t seed, const ::std::false_type /*hasMemberHash*/ ) {
			return GetHashCode( key, seed );
		}
	};

}	// namespace Detail

	template <typename T>
	ETNoAliasHint size_t Hash<T>::operator()( const T& key, const size_t seed ) const {
		static_assert( sizeof(key) != 0, "Hashed types must be complete!" );

	// ---

		return Detail::HashSelector<T>::Invoke( key, seed, ::std::integral_constant<bool, Detail::HashSelector<T>::HasMemberHashFunction<size_t( const size_t )>::value>() );
	}

// ---

	template <>
	class Hash<char*> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t operator()( const char* const key, const size_t lengthInBytes, const size_t seed ) const {
			return ::Eldritch2::HashMemory( key, lengthInBytes, seed );
		}

		ETNoAliasHint size_t operator()( const char* const key, const size_t seed = static_cast<size_t>(0) ) const {
			return (*this)( key, ::Eldritch2::StringLength( key ), seed );
		}
	};

// ---

	template <>
	class Hash<wchar_t*> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t operator()( const wchar_t* const key, const size_t lengthInCharacters, const size_t seed ) const {
			// The basic hasher wants a size in bytes, so remember to multiply by the number of chars per wchar_t!
			return ::Eldritch2::HashMemory( key, lengthInCharacters * sizeof(wchar_t), seed );
		}

		ETNoAliasHint size_t operator()( const wchar_t* const key, const size_t seed = static_cast<size_t>(0) ) const {
			return (*this)( key, ::Eldritch2::StringLength( key ), seed );
		}
	};

// ---

	template <typename T>
	class Hash<T*> {
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
	class Hash<::Eldritch2::Pair<T1, T2>> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t operator()( const ::Eldritch2::Pair<T1, T2>& key, const size_t seed = static_cast<size_t>(0) ) const {
			return ::Eldritch2::Hash<T2>()( key.second, ::Eldritch2::Hash<T1>()( key.first, seed ) );
		}
	};

// ---

	template <typename T1, typename T2>
	class Hash<const ::Eldritch2::Pair<T1, T2>> : public ::Eldritch2::Hash<::Eldritch2::Pair<T1, T2>> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;
	};

}	// namespace Eldritch2