/*==================================================================*\
  ConfigurableVariable.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Configuration {

	namespace Configuration	= ::Eldritch2::Configuration;

// ---------------------------------------------------

	class ETPureAbstractHint ConfigurableVariable {
	// - PUBLIC INTERFACE --------------------------------

	public:
		//! Sets the value of this @ref ConfigurableVariable to a lexically-parsed equivalent of the passed-in string.
		/*! @param[in] string Array of characters to parse the new value from. Is not necessarily null-terminated.
			@remarks @parblock The character array given as an argument to the function is not guaranteed to exist past the initial function call.
				Subclasses that need later access to the string should internally create and maintain a copy as necessary. @endparblock
			*/
		virtual void	SetValue( const ::Eldritch2::Range<const UTF8Char*>& string ) abstract;

		//! Prints the value of this @ref ConfigurableVariable to the provided buffer for possible permanent storage or user display.
		/*! @param[out] destinationBuffer <Parameter Description>
			@param[in] bufferOctetCount <Parameter Description>
			*/
		virtual void	GetValue( ::Eldritch2::UTF8Char* destinationBuffer, size_t bufferOctetCount ) const abstract;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//! Constructs this @ref ConfigurableVariable instance.
		ConfigurableVariable() = default;

		//! Destroys this @ref ConfigurableVariable instance.
		~ConfigurableVariable() = default;
	};

}	// namespace Configuration
}	// namespace Eldritch2