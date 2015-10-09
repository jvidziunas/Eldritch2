/*==================================================================*\
  ReferenceTypeBase.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  �2010-2013 Eldritch Entertainment, LLC.
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

	namespace Scripting	= ::Eldritch2::Scripting;

// ---------------------------------------------------

	class ReferenceTypeBase {
	// - REFERENCE MANAGEMENT ----------------------------

	public:
		ETInlineHint size_t	GetReferenceCount() const;

		ETInlineHint void	AddReference();

		ETInlineHint void	ReleaseReference();

		virtual void		Dispose() abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this ReferenceTypeBase instance.
		ETInlineHint ReferenceTypeBase( const size_t initialReferenceCount = 1u );

		// Destroys this ReferenceTypeBase instance.
		ETInlineHint ~ReferenceTypeBase();

	// - DATA MEMBERS ------------------------------------

	private:
		mutable ::std::atomic<size_t>	_referenceCount;
	};

}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/ReferenceTypeBase.inl>
//------------------------------------------------------------------//