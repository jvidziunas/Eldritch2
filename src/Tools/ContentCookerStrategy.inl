/*==================================================================*\
  ContentCookerStrategy.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2
{

	namespace Tools
	{

		namespace Tools	 = ::Eldritch2::Tools;

	// ---------------------------------------------------

		template <typename ImplementingType>
		ETInlineHint ContentCookerStrategy::OptionIterator<ImplementingType>::OptionIterator( Tools::CookerOption<ImplementingType>** const pointer ) : _pointer( pointer )
		{
		}

	// ---------------------------------------------------

		template <typename ImplementingType>
		ETInlineHint ContentCookerStrategy::OptionIterator<ImplementingType>::OptionIterator( const OptionIterator<ImplementingType>& iterator ) : _pointer( iterator._pointer )
		{
		}

	// ---------------------------------------------------

		template <typename ImplementingType>
		ETInlineHint ContentCookerStrategy::OptionIterator<ImplementingType>::~OptionIterator()
		{
		}

	// ---------------------------------------------------

		template <typename ImplementingType>
		ContentCookerStrategy::OptionIterator<ImplementingType>& ContentCookerStrategy::OptionIterator<ImplementingType>::operator++()
		{
			++_pointer;

			return *this;
		}

	// ---------------------------------------------------

		template <typename ImplementingType>
		ContentCookerStrategy::OptionIterator<ImplementingType> ContentCookerStrategy::OptionIterator<ImplementingType>::operator++( int )
		{
			return ContentCookerStrategy::OptionIterator<ImplementingType>( _pointer++ );
		}

	// ---------------------------------------------------

		template <typename ImplementingType>
		ETInlineHint bool ContentCookerStrategy::OptionIterator<ImplementingType>::operator==( const Tools::ContentCookerStrategy::OptionIterator<ImplementingType>& rhs ) const
		{
			return _pointer == rhs._pointer;
		}

	// ---------------------------------------------------

		template <typename ImplementingType>
		ETInlineHint bool ContentCookerStrategy::OptionIterator<ImplementingType>::operator!=( const Tools::ContentCookerStrategy::OptionIterator<ImplementingType>& rhs ) const
		{
			return _pointer != rhs._pointer;
		}

	// ---------------------------------------------------

		template <typename ImplementingType>
		ETInlineHint Tools::CookerOption<ImplementingType>& ContentCookerStrategy::OptionIterator<ImplementingType>::operator*()
		{
			return **_pointer;
		}

	}	// namespace Tools

}	// namespace Eldritch2