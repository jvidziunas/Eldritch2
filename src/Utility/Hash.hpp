/*==================================================================*\
  Hash.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	class HashBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		HashBase() = default;

		~HashBase() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t	operator()( const void* sourceData, const size_t sourceDataSizeInBytes, const size_t seed = static_cast<size_t>(0) ) const;
	};

}	// namespace Detail

	template <typename T>
	class Hash : public Detail::HashBase {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		Hash() = default;

		~Hash() = default;

	// ---------------------------------------------------

		ETNoAliasHint size_t	operator()( const T& object, const size_t seed = static_cast<size_t>(0) ) const;
	};

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Utility/Hash.inl>
//------------------------------------------------------------------//