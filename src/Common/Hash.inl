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
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Mpl/IntTypes.hpp>
#include <Common/Pair.hpp>
//------------------------------------------------------------------//
#include <eastl/type_traits.h>
//------------------------------------------------------------------//

namespace Eldritch2 {

ETPureFunctionHint size_t StringLength(const wchar_t* string);
ETPureFunctionHint size_t StringLength(const char* string);

ETPureFunctionHint size_t HashMemory(const void* memory, size_t sizeInBytes, size_t seed);

} // namespace Eldritch2

namespace Eldritch2 {
namespace Detail {

	//!	HashSelector is responsible for delegating out to the correct hash function for a type based on a specific set of type traits.
	template <typename KeyType>
	struct HashSelector {
		template <typename FunctionSignature>
		struct HasMemberHashFunction {
			static_assert(eastl::integral_constant<FunctionSignature, false>::value, "Template parameter needs to be of function type.");
		};

		// ---

		template <typename DesiredReturn, typename... Arguments>
		struct HasMemberHashFunction<DesiredReturn(Arguments...)> {
		private:
			template <typename T>
			static typename eastl::is_same<DesiredReturn, decltype(eastl::declval<const T>().GetHashCode(eastl::declval<Arguments>()...))>::type Check(T*);

			template <typename>
			static eastl::false_type Check(...);

			// ---

			using Type = decltype(Check<KeyType>(nullptr));

		public:
			static const bool value = Type::value;
		};

		// ---

		template <typename FunctionSignature>
		struct HasFreeHashFunction {
			static_assert(eastl::integral_constant<FunctionSignature, false>::value, "Template parameter needs to be of function type.");
		};

		// ---

		template <typename DesiredReturn, typename... Arguments>
		struct HasFreeHashFunction<DesiredReturn(Arguments...)> {
		private:
			template <typename T>
			static typename eastl::is_same<DesiredReturn, decltype(GetHashCode(eastl::declval<const T>(), eastl::declval<Arguments>()...))>::type Check(T*);

			template <typename>
			static eastl::false_type Check(...);

			// ---

			using Type = decltype(Check<KeyType>(nullptr));

		public:
			static const bool value = Type::value;
		};

		// ---

		template <bool hasMemberHashFunction = HasMemberHashFunction<size_t(size_t)>::value>
		struct GetHashCodeDispatcher {
			static bool Dispatch(const KeyType& /*key*/, size_t /*seed*/);
		};

	public:
		static ETInlineHint ETPureFunctionHint size_t Invoke(const KeyType& key, size_t seed, const eastl::true_type /*hasMemberHash*/) {
			static_assert(!HasFreeHashFunction<size_t(size_t)>::value, "Incorrect hash behavior detected!\n"
																	   "Types to be hashed must have either a member function with signature 'size_t GetHashCode( size_t ) const'\n"
																	   "or a free (optionally friend) function in the containing namespace with signature 'size_t GetHashCode( <Type>&, size_t )'\n"
																	   "as per Koenig lookup.\n"
																	   "\n"
																	   "If you are encountering this error, and you know at least one one such function exists, then there is likely an alternate form \n"
																	   "that exists somewhere in the translation unit, causing ambiguity. Templates will contribute to the overload set!\n");

			// ---

			return key.GetHashCode(seed);
		}

		static ETInlineHint ETPureFunctionHint size_t Invoke(const KeyType& key, size_t seed, const eastl::false_type /*hasMemberHash*/) {
			return GetHashCode(key, seed);
		}
	};

} // namespace Detail

template <typename T>
ETInlineHint ETPureFunctionHint size_t Hash<T>::operator()(const T& key, size_t seed) const {
	static_assert(sizeof(key) != 0, "Hashed types must be complete!");

	// ---

	return Detail::HashSelector<T>::Invoke(key, seed, eastl::integral_constant<bool, Detail::HashSelector<T>::HasMemberHashFunction<size_t(size_t)>::value>());
}

// ---

template <>
class Hash<char*> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	Hash() = default;

	~Hash() = default;

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t operator()(const char* const key, size_t lengthInBytes, size_t seed) const {
		return HashMemory(key, lengthInBytes, seed);
	}

	ETInlineHint ETPureFunctionHint size_t operator()(const char* const key, size_t seed = static_cast<size_t>(0)) const {
		return (*this)(key, StringLength(key), seed);
	}
};

// ---

template <>
class Hash<wchar_t*> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	Hash() = default;

	~Hash() = default;

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t operator()(const wchar_t* const key, size_t lengthInCharacters, size_t seed) const {
		//	The basic hasher wants a size in bytes, so remember to multiply by the number of chars per wchar_t!
		return HashMemory(key, lengthInCharacters * sizeof(wchar_t), seed);
	}

	ETInlineHint ETPureFunctionHint size_t operator()(const wchar_t* const key, size_t seed = 0u) const {
		return (*this)(key, StringLength(key), seed);
	}
};

// ---

template <>
class Hash<void*> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	Hash() = default;

	~Hash() = default;

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t operator()(const void* const key, size_t seed = 0u) const {
		return static_cast<size_t>(reinterpret_cast<uintptr>(key) + seed);
	}
};

// ---

template <>
class Hash<const void*> : public Hash<void*> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	Hash() = default;

	~Hash() = default;

	// ---------------------------------------------------

	using Hash<void*>::operator();
};

// ---

template <>
class Hash<uintptr> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	Hash() = default;

	~Hash() = default;

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t operator()(uintptr key, size_t seed = 0u) const {
		return static_cast<size_t>(key) + seed;
	}
};

// ---

template <>
class Hash<intptr> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	Hash() = default;

	~Hash() = default;

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t operator()(intptr key, size_t seed = 0u) const {
		return static_cast<size_t>(key) + seed;
	}
};

// ---

template <>
class Hash<uint8> : public Hash<uintptr> {};
template <>
class Hash<int8> : public Hash<intptr> {};
template <>
class Hash<uint16> : public Hash<uintptr> {};
template <>
class Hash<int16> : public Hash<intptr> {};
template <>
class Hash<uint32> : public Hash<uintptr> {};
template <>
class Hash<int32> : public Hash<intptr> {};

// ---

template <typename T>
class Hash<T*> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	Hash() = default;

	~Hash() = default;

	// ---------------------------------------------------

	ETInlineHint ETPureFunctionHint size_t operator()(const T* const key, size_t seed = 0u) const {
		return static_cast<size_t>(reinterpret_cast<uintptr>(key) + seed);
	}
};

// ---

template <typename T>
class Hash<const T*> : public Hash<T*> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	Hash() = default;

	~Hash() = default;

	// ---------------------------------------------------

	using Hash<T*>::operator();
};

// ---

template <typename T>
class Hash<const T> : public Hash<T> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

public:
	//!	Constructs this @ref Hash instance.
	Hash() = default;

	~Hash() = default;

	// ---------------------------------------------------

	using Hash<T>::operator();
};

} // namespace Eldritch2
