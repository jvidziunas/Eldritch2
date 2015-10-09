/*==================================================================*\
  CookerOption.hpp
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

namespace Eldritch2Detail
{

	namespace Tools
	{

		namespace Tools	= ::Eldritch2Detail::Tools;

	// ---------------------------------------------------

		template <typename EnumType>
		struct CookerOptionEnumPair
		{
			const ::Eldritch2::SystemChar*	name;
			EnumType						value;
		};

	}	// namespace Tools

}	// namespace Eldritch2Detail

namespace Eldritch2
{

	namespace Tools
	{

		namespace Tools	= ::Eldritch2::Tools;

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		class ETPureAbstractHint CookerOption
		{
		// ---------------------------------------------------		

		public:
			virtual const ::Eldritch2::SystemChar*	GetArgumentString() const abstract;

		// - ARGUMENT PARSING --------------------------------

			virtual bool	Parse( CookerStrategyType&				strategy,
								   const ::Eldritch2::SystemChar*	argument,
								   const ::Eldritch2::SystemChar*	argumentEnd ) abstract;
			virtual bool	Parse( CookerStrategyType&				strategy ) abstract;


		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			// Constructs this CookerOption instance.
			CookerOption();

			// Destroys this CookerOption instance.
			~CookerOption();
		};

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		class ETPureAbstractHint NoArgumentCookerOption : public Tools::CookerOption<CookerStrategyType>
		{
		// - ARGUMENT PARSING --------------------------------

		public:
			bool	Parse( CookerStrategyType&				strategy,
						   const ::Eldritch2::SystemChar*	argument,
						   const ::Eldritch2::SystemChar*	argumentEnd ) override sealed;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			// Constructs this NoArgumentCookerOption instance.
			NoArgumentCookerOption();

			// Destroys this NoArgumentCookerOption instance.
			~NoArgumentCookerOption();
		};

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		class ETPureAbstractHint ArgumentOnlyCookerOption : public Tools::CookerOption<CookerStrategyType>
		{
		// - ARGUMENT PARSING --------------------------------

		public:
			bool	Parse( CookerStrategyType& strategy ) override sealed;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		protected:
			// Constructs this ArgumentOnlyCookerOption instance.
			ArgumentOnlyCookerOption();

			// Destroys this ArgumentOnlyCookerOption instance.
			~ArgumentOnlyCookerOption();
		};

	// ---------------------------------------------------

		template <typename CookerStrategyType, typename VariableType>
		class PODMemberCookerOption : public Tools::ArgumentOnlyCookerOption<CookerStrategyType>
		{
		public:
			typedef	VariableType	CookerStrategyType::*OptionPointer;

		// - ARGUMENT PARSING --------------------------------

			bool	Parse( CookerStrategyType&				strategy,
						   const ::Eldritch2::SystemChar*	argument,
						   const ::Eldritch2::SystemChar*	argumentEnd ) override;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this PODMemberCookerOption instance.
			PODMemberCookerOption( OptionPointer optionPointer );

			// Destroys this PODMemberCookerOption instance.
			~PODMemberCookerOption();

		// ---------------------------------------------------

		protected:
			virtual VariableType	CoerceToValidValue( VariableType value ) const;

		// - DATA MEMBERS ------------------------------------

		private:
			const OptionPointer	_optionPointer;
		};

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		class PODMemberCookerOption<CookerStrategyType, bool> : public Tools::CookerOption<CookerStrategyType>
		{
		public:
			typedef	bool	CookerStrategyType::*OptionPointer;

		// - ARGUMENT PARSING --------------------------------

			bool	Parse( CookerStrategyType&				strategy,
						   const ::Eldritch2::SystemChar*	argument,
						   const ::Eldritch2::SystemChar*	argumentEnd ) override;
			bool	Parse( CookerStrategyType&				strategy ) override;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this PODMemberCookerOption instance.
			PODMemberCookerOption( OptionPointer optionPointer );

			// Destroys this PODMemberCookerOption instance.
			~PODMemberCookerOption();

		// - DATA MEMBERS ------------------------------------

		private:
			const OptionPointer	_optionPointer;
		};

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		class PODMemberCookerOption<CookerStrategyType, const ::Eldritch2::SystemChar*> : public Tools::ArgumentOnlyCookerOption<CookerStrategyType>
		{
		public:
			typedef	const ::Eldritch2::SystemChar* CookerStrategyType::*OptionPointer;

		// - ARGUMENT PARSING --------------------------------

			bool	Parse( CookerStrategyType&				strategy,
						   const ::Eldritch2::SystemChar*	argument,
						   const ::Eldritch2::SystemChar*	argumentEnd ) override;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this PODMemberCookerOption instance.
			PODMemberCookerOption( OptionPointer optionPointer );

			// Destroys this PODMemberCookerOption instance.
			~PODMemberCookerOption();

		// - DATA MEMBERS ------------------------------------

		private:
			const OptionPointer	_optionPointer;
		};

	// ---------------------------------------------------

		template <typename CookerStrategyType, typename EnumType>
		class EnumCookerOption : public Tools::ArgumentOnlyCookerOption<CookerStrategyType>
		{
		public:
			typedef ::Eldritch2Detail::Tools::CookerOptionEnumPair<EnumType>	EnumValueDescriptor;
			typedef EnumType CookerStrategyType::*OptionPointer;

		// - ARGUMENT PARSING --------------------------------

			bool	Parse( CookerStrategyType&				strategy,
						   const ::Eldritch2::SystemChar*	argument,
						   const ::Eldritch2::SystemChar*	argumentEnd ) override;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this EnumCookerOption instance.
			EnumCookerOption( OptionPointer					optionPointer,
							  const EnumValueDescriptor*	values,
							  const EnumValueDescriptor*	valuesEnd );

			// Destroys this EnumCookerOption instance.
			~EnumCookerOption();

		// - DATA MEMBERS ------------------------------------

		private:
			const EnumValueDescriptor* const	_values;
			const EnumValueDescriptor* const	_valuesEnd;
			const OptionPointer					_optionPointer;
		};

	}	// namespace Tools

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Tools/CookerOption.inl>
//------------------------------------------------------------------//