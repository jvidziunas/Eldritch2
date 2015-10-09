/*==================================================================*\
  ContentCooker.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2012 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/MemStdLib.hpp>
#include <Utility/ErrorCode.hpp>
#include <malloc.h>
//------------------------------------------------------------------//

namespace Eldritch2
{

	namespace Tools
	{

		namespace FileSystem	= ::Eldritch2::FileSystem;
		namespace Tools			= ::Eldritch2::Tools;

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ETInlineHint ContentCooker<CookerStrategyType>::Option::Option( const Tools::CookerOption<CookerStrategyType>& option, int index )
		{
			nId			= index;
			pszArg		= option.GetArgumentString();
			nArgType	= SO_OPT;
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ETInlineHint void ContentCooker<CookerStrategyType>::Option::SetEndOfOptions()
		{
			const ArgumentParser::SOption	endOption = SO_END_OF_OPTIONS;

			static_cast<ArgumentParser::SOption&>( *this ) = endOption;
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ETInlineHint ContentCooker<CookerStrategyType>::OptionConstructionPredicate::OptionConstructionPredicate( int initialArgID ) : _argID( initialArgID )
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ETInlineHint typename ContentCooker<CookerStrategyType>::Option ContentCooker<CookerStrategyType>::OptionConstructionPredicate::operator()( Tools::CookerOption<CookerStrategyType>&	option,
																																					Tools::CookerOption<CookerStrategyType>*&	optionPointer )
		{
			optionPointer = &option;
			return Option( option, _argID++ );
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ETInlineHint int ContentCooker<CookerStrategyType>::OptionConstructionPredicate::GetCurrentArgID() const
		{
			return _argID;
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ETInlineHint ContentCooker<CookerStrategyType>::OutputFileNameCookerOption::OutputFileNameCookerOption() : _outputFileName( nullptr )
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		const ::Eldritch2::SystemChar* ContentCooker<CookerStrategyType>::OutputFileNameCookerOption::GetArgumentString() const
		{
			return SL( "-outputFile" );
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		bool ContentCooker<CookerStrategyType>::OutputFileNameCookerOption::Parse( CookerStrategyType&				strategy,
																				   const ::Eldritch2::SystemChar*	argument,
																				   const ::Eldritch2::SystemChar*	argumentEnd )
		{
			ETUnreferencedParameter( strategy );
			ETUnreferencedParameter( argumentEnd );

			_outputFileName = argument;

			return true;
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		FileSystem::FileWriteAccessStrategy* ContentCooker<CookerStrategyType>::OutputFileNameCookerOption::GetWriteStrategy( Tools::FileWriteStrategyFactory& factory ) const
		{
			return factory.GetAccessor( _outputFileName );
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ETInlineHint ContentCooker<CookerStrategyType>::ProcessContentExtensionHintPredicate::ProcessContentExtensionHintPredicate( CookerStrategyType& cookerStrategy ) : _cookerStrategy( cookerStrategy )
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ETInlineHint void ContentCooker<CookerStrategyType>::ProcessContentExtensionHintPredicate::operator()( const ::Eldritch2::SystemChar* filePath ) const
		{
			const ::Eldritch2::SystemChar* const extensionBegin( ::Eldritch2::FindLastInstance( filePath, SL( '.' ) ) );

			_cookerStrategy.ProcessContentExtensionHint( filePath, extensionBegin ? extensionBegin + 1 : SL( "" ) );
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ContentCooker<CookerStrategyType>::ContentCooker()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		ContentCooker<CookerStrategyType>::~ContentCooker()
		{
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		::Eldritch2::ErrorCode ContentCooker<CookerStrategyType>::BeginCook( ::Eldritch2::SystemChar**			arguments,
																			 ::Eldritch2::SystemChar**			argumentsEnd,
																			 Tools::FileReadStrategyFactory&	readStrategyFactory,
																			 Tools::FileWriteStrategyFactory&	writeStrategyFactory )
		{
			if( 1 == ::std::distance( arguments, argumentsEnd ) )
				return this->ServerModeCook( readStrategyFactory, writeStrategyFactory );

			return this->Cook( arguments + 1,
							   argumentsEnd,
							   readStrategyFactory,
							   writeStrategyFactory );
		}

	// ---------------------------------------------------

		template <typename CookerStrategyType>
		::Eldritch2::ErrorCode ContentCooker<CookerStrategyType>::Cook( ::Eldritch2::SystemChar**			arguments,
																		::Eldritch2::SystemChar**			argumentsEnd,
																		Tools::FileReadStrategyFactory&		readStrategyFactory,
																		Tools::FileWriteStrategyFactory&	writeStrategyFactory )
		{
			typedef ::std::raw_storage_iterator<Option*, Option>	OptionStorageIterator;
			typedef Tools::CookerOption<CookerStrategyType>			StrategyOption;

			enum
			{
				/*
				Current extra options:
				- Output file name
				- Terminator
				*/
				EXTRA_ARGUMENT_COUNT	= 2
			};

		// ---

			OptionConstructionPredicate	optionConstructionPredicate;
			OutputFileNameCookerOption	outputFileNameOption;
			CookerStrategyType			cookerStrategy;
			ArgumentParser				argumentParser;
			WildcardParser				wildcardParser( SG_GLOB_ONLYFILE | SG_GLOB_NOSORT );
			const size_t				optionsCount( static_cast<size_t>( EXTRA_ARGUMENT_COUNT ) + ::std::distance( cookerStrategy.OptionsBegin(),
																													 cookerStrategy.OptionsEnd() ) );
			// Most Vexing Parse, how do I love thee.
			Option* const				options				= static_cast<Option*>( _alloca( sizeof( Option ) * optionsCount ) );
			StrategyOption** const		cookerOptions		= static_cast<StrategyOption**>( _alloca( sizeof( StrategyOption* ) * ( optionsCount - 1u ) ) );
			StrategyOption**			currentCookerOption	= cookerOptions;
			OptionStorageIterator		optionStorageIterator( options );

			{
				// Add all additional options here.
				optionStorageIterator++	= optionConstructionPredicate( outputFileNameOption, *currentCookerOption++ );
			}

			::std::transform( cookerStrategy.OptionsBegin(),
							  cookerStrategy.OptionsEnd(),
							  currentCookerOption,
							  optionStorageIterator,
							  optionConstructionPredicate );

			options[optionConstructionPredicate.GetCurrentArgID()].SetEndOfOptions();

			for( argumentParser.Init( ::std::distance( argumentsEnd, arguments ),
									  arguments,
									  options,
									  ( SO_O_CLUMP | SO_O_ICASE | SO_O_NOERR | SO_O_USEALL ) ); argumentParser.Next(); )
			{
				bool	parsedSuccessfully( false );

				if( SO_SUCCESS == argumentParser.LastError() )
				{
					parsedSuccessfully = argumentParser.OptionArg() ?
											cookerOptions[argumentParser.OptionId()]->Parse( cookerStrategy,
																							 argumentParser.OptionArg(),
																							 ::Eldritch2::FindEndOfString( argumentParser.OptionArg() ) ) :
											cookerOptions[argumentParser.OptionId()]->Parse( cookerStrategy );
				}

				if( !parsedSuccessfully )
				{
					WriteErrorString( SL( "Error parsing command-line arguments. Please see options documentation for help configuring this program." ) );
					return ::Eldritch2::Errors::UNSPECIFIED;
				}
			}

			if( SG_SUCCESS < wildcardParser.Add( argumentParser.FileCount(), argumentParser.Files() ) )
				return ::Eldritch2::Errors::UNSPECIFIED;

			::std::for_each( wildcardParser.Files(),
							 wildcardParser.Files() + wildcardParser.FileCount(),
							 ProcessContentExtensionHintPredicate( cookerStrategy ) );

			if( FileSystem::FileWriteAccessStrategy* const writeStrategy = outputFileNameOption.GetWriteStrategy( writeStrategyFactory ) )
			{
				::Eldritch2::ErrorCode	result( cookerStrategy.Cook( *writeStrategy,
																	 wildcardParser.Files(),
																	 wildcardParser.Files() + wildcardParser.FileCount(),
																	 readStrategyFactory,
																	 writeStrategyFactory ) );

				writeStrategyFactory.DestroyAccessor( *writeStrategy );

				return result;
			}

			return ::Eldritch2::Errors::UNSPECIFIED;
		}

	// ---------------------------------------------------

		template <typename CookStrategyType>
		::Eldritch2::ErrorCode ContentCooker<CookStrategyType>::ServerModeCook( Tools::FileReadStrategyFactory&		readStrategyFactory,
																				Tools::FileWriteStrategyFactory&	writeStrategyFactory )
		{
			for( ; ; )
			{
				this->Cook( nullptr,
							nullptr,
							readStrategyFactory,
							writeStrategyFactory );
			}
		}

	}	// namespace Tools

}	// namespace Eldritch2