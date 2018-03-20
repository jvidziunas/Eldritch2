/*==================================================================*\
  Prefetch.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Mpl/Platform.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {

	template<typename T>
	void	Level0Prefetch( const T& data );

	template<typename T>
	void	Level1Prefetch( const T& data );

	template<typename T>
	void	Level2Prefetch( const T& data );

	template<typename T>
	void	NonTemporalWrite( T& destination, const T& data );

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#if ET_PLATFORM_WINDOWS
#	include <Common/Memory/Prefetch.Win.inl>
#elif( ET_PLATFORM_MAC )
#	include <Common/Memory/Prefetch.Mac.inl>
#elif( ET_PLATFORM_LINUX )
#	include <Common/Memory/Prefetch.Linux.inl>
#endif
//------------------------------------------------------------------//