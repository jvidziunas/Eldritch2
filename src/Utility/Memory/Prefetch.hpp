/*==================================================================*\
  Prefetch.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template<typename T>
	ETForceInlineHint void	Level0Prefetch( const T& data );

	template<typename T>
	ETForceInlineHint void	Level1Prefetch( const T& data );

	template<typename T>
	ETForceInlineHint void	Level2Prefetch( const T& data );

	template<typename T>
	ETForceInlineHint void	NonTemporalWrite( T& destination, const T& data );

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#if( ET_PLATFORM_WINDOWS )
#	include <Utility/Memory/Prefetch.Win.inl>
#elif( ET_PLATFORM_MAC )
#	include <Utility/Memory/Prefetch.Mac.inl>
#elif( ET_PLATFORM_LINUX )
#	include <Utility/Memory/Prefetch.Linux.inl>
#elif( ET_PLATFORM_XENON )
#	include <Utility/Memory/Prefetch.Xenon.inl>
#elif( ET_PLATFORM_PLAYSTATION3 )
#	include <Utility/Memory/Prefetch.PS3.inl>
#endif
//------------------------------------------------------------------//