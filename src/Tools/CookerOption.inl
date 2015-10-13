/*==================================================================*\
  CookerOption.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <algorithm>
//------------------------------------------------------------------//

namespace Eldritch2
{

	namespace Tools
	{

		namespace Tools	= ::Eldritch2::Tools;

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		CookerOption<CookerStrategyType>::CookerOption()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		CookerOption<CookerStrategyType>::~CookerOption()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		NoArgumentCookerOption<CookerStrategyType>::NoArgumentCookerOption() : CookerOption<CookerStrategyType>()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		NoArgumentCookerOption<CookerStrategyType>::~NoArgumentCookerOption()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		bool NoArgumentCookerOption<CookerStrategyType>::Parse( CookerStrategyType&				strategy,
																const ::Eldritch2::SystemChar*	argument,
																const ::Eldritch2::SystemChar*	argumentEnd )
		{
			ETUnreferencedParameter( strategy );
			ETUnreferencedParameter( argument );
			ETUnreferencedParameter( argumentEnd );

			return false;
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ArgumentOnlyCookerOption<CookerStrategyType>::ArgumentOnlyCookerOption()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ArgumentOnlyCookerOption<CookerStrategyType>::~ArgumentOnlyCookerOption()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		bool ArgumentOnlyCookerOption<CookerStrategyType>::Parse( CookerStrategyType& strategy )
		{
			ETUnreferencedParameter( strategy );

			return false;
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType, typename VariableType>
		PODMemberCookerOption<CookerStrategyType, VariableType>::PODMemberCookerOption( OptionPointer optionPointer ) : _optionPointer( optionPointer )
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType, typename VariableType>
		PODMemberCookerOption<CookerStrategyType, VariableType>::~PODMemberCookerOption()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType, typename VariableType>
		bool PODMemberCookerOption<CookerStrategyType, VariableType>::Parse( CookerStrategyType&			strategy,
																			 const ::Eldritch2::SystemChar*	argument,
																			 const ::Eldritch2::SystemChar*	argumentEnd )
		{
			typedef ::boost::iostreams::basic_array_source<::Eldritch2::SystemChar>	StreamSourceType;
			typedef ::boost::iostreams::stream<StreamSourceType>					StreamType;

		// ---

			VariableType	temp;

			if( !( StreamType( argument, ( argumentEnd - argument ) ) >> temp ).fail() )
			{
				strategy.*_optionPointer = this->CoerceToValidValue( temp );
				return true;
			}

			return false;
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType, typename VariableType>
		VariableType PODMemberCookerOption<CookerStrategyType, VariableType>::CoerceToValidValue( VariableType value ) const
		{
			return value;
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		PODMemberCookerOption<CookerStrategyType, bool>::PODMemberCookerOption( OptionPointer optionPointer ) : _optionPointer( optionPointer )
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		PODMemberCookerOption<CookerStrategyType, bool>::~PODMemberCookerOption()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		bool PODMemberCookerOption<CookerStrategyType, bool>::Parse( CookerStrategyType&			strategy,
																	 const ::Eldritch2::SystemChar*	argument,
																	 const ::Eldritch2::SystemChar*	argumentEnd )
		{
			typedef ::boost::iostreams::basic_array_source<::Eldritch2::SystemChar>	StreamSourceType;
			typedef ::boost::iostreams::stream<StreamSourceType>					StreamType;

		// ---

			return !( ( StreamType( argument, ( argumentEnd - argument ) ) >> ( strategy.*_optionPointer ) ).fail() );
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		bool PODMemberCookerOption<CookerStrategyType, bool>::Parse( CookerStrategyType& strategy )
		{
			(strategy.*_optionPointer) = true;

			return true;
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		PODMemberCookerOption<CookerStrategyType, const ::Eldritch2::SystemChar*>::PODMemberCookerOption( OptionPointer optionPointer ) : _optionPointer( optionPointer )
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		PODMemberCookerOption<CookerStrategyType, const ::Eldritch2::SystemChar*>::~PODMemberCookerOption()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		bool PODMemberCookerOption<CookerStrategyType, const ::Eldritch2::SystemChar*>::Parse( CookerStrategyType&				strategy,
																							   const ::Eldritch2::SystemChar*	argument,
																							   const ::Eldritch2::SystemChar*	argumentEnd )
		{
			(strategy.*_optionPointer) = argument;

			return true;
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType, typename EnumType>
		EnumCookerOption<CookerStrategyType, EnumType>::EnumCookerOption( OptionPointer					optionPointer,
																		  const EnumValueDescriptor*	values,
																		  const EnumValueDescriptor*	valuesEnd ) : Tools::ArgumentOnlyCookerOption<CookerStrategyType>(),
																													  _optionPointer( optionPointer ),
																													  _values( values ),
																													  _valuesEnd( valuesEnd )
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType, typename EnumType>
		EnumCookerOption<CookerStrategyType, EnumType>::~EnumCookerOption()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType, typename EnumType>
		bool EnumCookerOption<CookerStrategyType, EnumType>::Parse( CookerStrategyType&				strategy,
																	const ::Eldritch2::SystemChar*	argument,
																	const ::Eldritch2::SystemChar*	argumentEnd )
		{
			class IsStringSamePredicate
			{
			public:
				IsStringSamePredicate( const ::Eldritch2::SystemChar* const	argument,
									   EnumType&							option ) : _argument( argument ),
																					   _option( option )
				{
				}

			// ---------------------------------------------------

				ETInlineHint bool operator()( const EnumValueDescriptor& descriptor )
				{
					if( 0 == ::Eldritch2::StrIEqual( _argument, descriptor.name ) )
					{
						_option = descriptor.value;
						return true;
					}

					return false;
				}

			// - DATA MEMBERS ------------------------------------

				const ::Eldritch2::SystemChar* const	_argument;
				EnumType&								_option;
			};

		// ---

			ETUnreferencedParameter( argumentEnd );

			return _valuesEnd != ::std::find_if( _values, _valuesEnd, IsStringSamePredicate( argument, strategy.*_optionPointer ) );
		}

	}	// namespace Tools

}	// namespace Eldritch2