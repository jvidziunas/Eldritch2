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
#include <Utility/MPL/Compiler.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {

	class ReferenceCountable {
	// - REFERENCE MANAGEMENT ----------------------------

	public:
		ETInlineHint size_t	GetReferenceCount() const;

		ETInlineHint void	AddReference() const;

		ETInlineHint void	ReleaseReference() const;

		virtual void		Dispose() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref ReferenceCountable instance.
		ETInlineHint ReferenceCountable( const size_t initialReferenceCount = 1u );

		//!	Destroys this @ref ReferenceCountable instance.
		ETInlineHint ~ReferenceCountable() = default;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable ::std::atomic<size_t>	_referenceCount;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/ReferenceCountable.inl>
//------------------------------------------------------------------//