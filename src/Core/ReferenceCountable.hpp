/*==================================================================*\
  ReferenceCountable.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Mpl/Compiler.hpp>
//------------------------------------------------------------------//
#if ET_COMPILER_IS_MSVC && !defined( EA_COMPILER_HAS_C99_FORMAT_MACROS )
//	MSVC complains about *lots* of macro redefinitions in eabase/inttypes.h.
#	define EA_COMPILER_HAS_C99_FORMAT_MACROS
#endif
#include <EABase/eabase.h>
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Core {

	class ReferenceCountable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Constructs this @ref ReferenceCountable instance.
		ReferenceCountable( const ReferenceCountable& );
	//!	Constructs this @ref ReferenceCountable instance.
		ReferenceCountable();

		~ReferenceCountable() = default;

	// - REFERENCE MANAGEMENT ----------------------------

	public:
		size_t	GetReferenceCount() const;

		size_t	AddReference();

		size_t	ReleaseReference();

	// ---------------------------------------------------

	public:
		ReferenceCountable&	operator=( const ReferenceCountable& );

	// - DATA MEMBERS ------------------------------------

	private:
		std::atomic<size_t>	_referenceCount;
	};

}	// namespace Core
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/ReferenceCountable.inl>
//------------------------------------------------------------------//