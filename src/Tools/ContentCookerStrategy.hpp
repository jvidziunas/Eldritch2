/*==================================================================*\
  ContentCookerStrategy.hpp
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
#include <iterator>
//------------------------------------------------------------------//

namespace Eldritch2
{
	namespace Tools
	{
		template <typename CookerStrategyType>
		class	CookerOption;
	}
}

namespace Eldritch2
{

	namespace Tools
	{

		namespace Tools	= ::Eldritch2::Tools;

	// ---------------------------------------------------

		class ContentCookerStrategy
		{
		public:
			template <typename ImplementingType>
			class OptionIterator : public ::std::iterator< ::std::input_iterator_tag,
														   Tools::CookerOption<ImplementingType> >
			{
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				// Constructs this OptionIterator instance.
				ETInlineHint OptionIterator( Tools::CookerOption<ImplementingType>** const	pointer );
				// Constructs this OptionIterator instance.
				ETInlineHint OptionIterator( const OptionIterator<ImplementingType>&		iterator );

				// Destroys this OptionIterator instance.
				ETInlineHint ~OptionIterator();

			// - ITERATOR ADVANCEMENT ----------------------------

				OptionIterator<ImplementingType>&	operator++();
				OptionIterator<ImplementingType>	operator++( int );

			// - ITERATOR COMPARISON -----------------------------

				ETInlineHint bool	operator==( const OptionIterator<ImplementingType>& rhs ) const;
				ETInlineHint bool	operator!=( const OptionIterator<ImplementingType>& rhs ) const;

			// - ITERATOR DEREFERENCING --------------------------

				ETInlineHint Tools::CookerOption<ImplementingType>&	operator*();

			// - DATA MEMBERS ------------------------------------

			private:
				Tools::CookerOption<ImplementingType>* const *	_pointer;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this ContentCookerStrategy instance.
			ContentCookerStrategy();

			// Destroys this ContentCookerStrategy instance.
			~ContentCookerStrategy();

		// ---------------------------------------------------

			void	ProcessContentExtensionHint( const ::Eldritch2::SystemChar*	contentPath,
												 const ::Eldritch2::SystemChar*	contentExtension );
		};

	}	// namespace Tools

}	// namespace Eldritch2

#include <Tools/ContentCookerStrategy.inl>