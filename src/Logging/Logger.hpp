/*==================================================================*\
  Logger.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Foundation {

	namespace Foundation	= ::Eldritch2::Foundation;

// ---------------------------------------------------

	class ETPureAbstractHint Logger {
	// ---------------------------------------------------

	public:
		virtual void	WriteString( const ::Eldritch2::UTF8Char* const string, const size_t lengthInOctets ) abstract;
		void			WriteString( const ::Eldritch2::UTF8Char* const string );

	// ---------------------------------------------------

	protected:
		//! Constructs this @ref Logger instance.
		/*!	Designed to be called from subclasses.
			*/
		ETInlineHint Logger() = default;

		//! Destroys this @ref Logger instance.
		ETInlineHint ~Logger() = default;
	};

}	// namespace Foundation
}	// namespace Eldritch2