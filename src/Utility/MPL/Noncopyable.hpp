/*==================================================================*\
  Noncopyable.hpp
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

#if( ET_COMPILER_IS_MSVC )
#	pragma warning( disable : 4512 )
#endif

namespace Eldritch2 {
namespace Utility {

	namespace Utility	= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ETPureAbstractHint Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this Noncopyable instance.
		ETInlineHint ETNoAliasHint Noncopyable() = default;
		// Constructs this Noncopyable instance.
		ETInlineHint ETNoAliasHint Noncopyable( const Utility::Noncopyable& ) = delete;

		ETInlineHint ETNoAliasHint Noncopyable&	operator=(const Utility::Noncopyable&) = delete;

		// Destroys this Noncopyable instance.
		ETInlineHint ETNoAliasHint ~Noncopyable() = default;
	};

// ---------------------------------------------------

	class ETPureAbstractHint ExplicitCopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		// Constructs this ExplicitCopyable instance.
		ETInlineHint ETNoAliasHint ExplicitCopyable() = default;

		// Constructs this ExplicitCopyable instance.
		ETInlineHint ETNoAliasHint explicit ExplicitCopyable( const Utility::ExplicitCopyable& ) = default;
	};

}	// namespace Utility
}	// namespace Eldritch2