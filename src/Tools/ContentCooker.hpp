/*==================================================================*\
  ContentCooker.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Tools/CookerOption.hpp>
#if( ET_PLATFORM_WINDOWS )
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <windows.h>
#endif
#include <simpleopt/SimpleGlob.h>
#include <simpleopt/SimpleOpt.h>
//------------------------------------------------------------------//

namespace Eldritch2
{
	class	ErrorCode;

	namespace FileSystem
	{
		class	FileWriteAccessStrategy;
	}

	namespace Tools
	{
		template <typename CookerStrategyType>
		class	CookerOption;

		class	FileReadStrategyFactory;
		class	FileWriteStrategyFactory;
	}
}

namespace Eldritch2Detail
{

	namespace Tools
	{

		namespace Tools	= ::Eldritch2Detail::Tools;

	// ---------------------------------------------------

		class ETPureAbstractHint ContentCookerBase
		{
		protected:
			typedef ::CSimpleGlobTempl<::Eldritch2::SystemChar>	WildcardParser;
			typedef ::CSimpleOptTempl<::Eldritch2::SystemChar>	ArgumentParser;

		// ---------------------------------------------------

			void	WriteString( const ::Eldritch2::SystemChar* const string );

			void	WriteErrorString( const ::Eldritch2::SystemChar* const string );

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			// Constructs this ContentCookerBase instance.
			ContentCookerBase();

			// Destroys this ContentCookerBase instance.
			~ContentCookerBase();
		};

	}	// namespace Tools

}	// namespace Eldritch2Detail

namespace Eldritch2
{

	namespace Tools
	{

		namespace FileSystem	= ::Eldritch2::FileSystem;
		namespace Tools			= ::Eldritch2::Tools;

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		class ContentCooker : public ::Eldritch2Detail::Tools::ContentCookerBase
		{
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			// Constructs this ContentCooker instance.
			ContentCooker();

			// Destroys this ContentCooker instance.
			~ContentCooker();

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	BeginCook( ::Eldritch2::SystemChar**		arguments,
											   ::Eldritch2::SystemChar**		argumentsEnd,
											   Tools::FileReadStrategyFactory&	readStrategyFactory,
											   Tools::FileWriteStrategyFactory&	writeStrategyFactory );

		// ---------------------------------------------------

		private:
			struct Option : public ArgumentParser::SOption
			{
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				// Constructs this Option instance.
				ETInlineHint Option( const Tools::CookerOption<CookerStrategyType>& option, int index );

			// ---------------------------------------------------

				ETInlineHint void	SetEndOfOptions();
			};

		// ---------------------------------------------------

			class OptionConstructionPredicate
			{
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				// Constructs this OptionConstructionPredicate instance.
				ETInlineHint OptionConstructionPredicate( int initialArgID = 0 );

			// ---------------------------------------------------

				ETInlineHint Option operator()( Tools::CookerOption<CookerStrategyType>&	option,
												Tools::CookerOption<CookerStrategyType>*&	optionPointer );

			// ---------------------------------------------------

				ETForceInlineHint int	GetCurrentArgID() const;

			// - DATA MEMBERS ------------------------------------

			private:
				int	_argID;
			};

		// ---------------------------------------------------

			class OutputFileNameCookerOption : public Tools::ArgumentOnlyCookerOption<CookerStrategyType>
			{
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				ETInlineHint OutputFileNameCookerOption();

			// ---------------------------------------------------

				const ::Eldritch2::SystemChar*	GetArgumentString() const override sealed;

			// ---------------------------------------------------

				bool	Parse( CookerStrategyType&				strategy,
							   const ::Eldritch2::SystemChar*	argument,
							   const ::Eldritch2::SystemChar*	argumentEnd ) override sealed;

			// ---------------------------------------------------

				FileSystem::FileWriteAccessStrategy*	GetWriteStrategy( Tools::FileWriteStrategyFactory& factory ) const;

			// - DATA MEMBERS ------------------------------------

			private:
				const ::Eldritch2::SystemChar*	_outputFileName;
			};

		// ---------------------------------------------------

			class ProcessContentExtensionHintPredicate
			{
			// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				ETInlineHint ProcessContentExtensionHintPredicate( CookerStrategyType& cookerStrategy );

			// ---------------------------------------------------

				ETInlineHint void operator()( const ::Eldritch2::SystemChar* filePath ) const;

			// - DATA MEMBERS ------------------------------------

			private:
				CookerStrategyType&	_cookerStrategy;
			};

		// ---------------------------------------------------

			::Eldritch2::ErrorCode	Cook( ::Eldritch2::SystemChar**			arguments,
										  ::Eldritch2::SystemChar**			argumentsEnd,
										  Tools::FileReadStrategyFactory&	readStrategyFactory,
										  Tools::FileWriteStrategyFactory&	writeStrategyFactory );

			::Eldritch2::ErrorCode	ServerModeCook( Tools::FileReadStrategyFactory&		readStrategyFactory,
													Tools::FileWriteStrategyFactory&	writeStrategyFactory );
		};

	}	// namespace Tools

}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Tools/ContentCooker.inl>
//------------------------------------------------------------------//