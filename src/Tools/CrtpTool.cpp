/*==================================================================*\
  CrtpTool.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/Range.hpp>
#include <Tools/CrtpTool.hpp>
#include <Common/Memory.hpp>
//------------------------------------------------------------------//
#ifdef ET_PLATFORM_WINDOWS
#	include <Windows.h>
#endif
#include <simpleopt/SimpleGlob.h>
#include <simpleopt/SimpleOpt.h>
#include <iostream>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	Tool::OptionRegistrar::OptionRegistrar() : _options( MallocAllocator( "Tool Option Registry Allocator" ) ) {}

// ---------------------------------------------------

	Tool::OptionRegistrar& Tool::OptionRegistrar::Register( const Utf8Char* const name, Setter&& setter ) {
		_options.EmplaceBack( eastl::move( name ), eastl::move( setter ) );

		return *this;
	}

// ---------------------------------------------------

	Tool::OptionRegistrar& Tool::OptionRegistrar::Register( const Utf8Char* const name, const Utf8Char* const shortName, Setter&& setter ) {
	//	Create one copy for the long name argument...
		_options.EmplaceBack( eastl::move( name ), Setter( static_cast<const Setter&>( setter ) ) );
	//	... and directly move the argument for the short name.
		_options.EmplaceBack( eastl::move( shortName ), eastl::move( setter ) );

		return *this;
	}

// ---------------------------------------------------

	Tool::OptionRegistrar& Tool::OptionRegistrar::RegisterInputFileHandler( Setter&& setter ) {
		_inputFileHandler = eastl::move( setter );

		return *this;
	}

// ---------------------------------------------------

	int Tool::OptionRegistrar::Dispatch( Range<Utf8Char**> programArguments ) {
		enum : int { ParseFlags = SO_O_SHORTARG | SO_O_CLUMP };

		using OptionGlobber = CSimpleGlobTempl<Utf8Char>;
		using OptionParser	= CSimpleOptTempl<Utf8Char>;

	// ---

	//	Converted option count should be one larger than the number of registered options, as the parser library uses a sentinel value to delimit the registered options.
		ArrayList<OptionParser::SOption>	convertedOptions( MallocAllocator( "OptionRegistrar::Dispatch() Converted Option Allocator" ) );
		int									index( -1 );

		convertedOptions.Reserve( _options.GetSize() + 1 );

		for (const Option& option : _options) {
			convertedOptions.Append( { ++index, option.first, SO_REQ_SEP } );
		}

	//	Terminate the list to be handed off to the option parser.
		convertedOptions.Append( SO_END_OF_OPTIONS );

		{	OptionParser	parser { static_cast<int>(programArguments.GetSize()), programArguments.Begin(), convertedOptions.GetData(), ParseFlags };
			OptionGlobber	globber;

			while (parser.Next()) {
				switch (parser.LastError()) {
					case SO_SUCCESS: {
						const int result( _options[parser.OptionId()].second( { parser.OptionArg(), FindTerminator( parser.OptionArg() ) } ) );

						if (result != 0) {
							return result;
						}

						break;
					}	// case SO_SUCCESS

					case SO_OPT_INVALID: {
						std::cerr << "Unknown option "<<parser.OptionText()<<", ignoring." << std::endl;
						break;
					}	// case SO_OPT_INVALID

					case SO_ARG_MISSING: {
						std::cerr << "Option "<<parser.OptionText()<<" requires an argument, but none was provided!" << std::endl;
						break;
					}	// case SO_OPT_INVALID

					default: {
						std::cerr << "Fatal error parsing command line, aborting!" << std::endl;
						return 1;
					}
				}	// switch( parser.LastError() )
			}	

		//	Add all input files to the globber.
			for (auto inputFile( parser.Files() ), end( inputFile + parser.FileCount() ); inputFile != end; ++inputFile) {
				globber.Add( *inputFile );
			}

			for (auto expandedFile( globber.Files() ), end( expandedFile + globber.FileCount() ); expandedFile != end; ++expandedFile) {
				_inputFileHandler( { *expandedFile, FindTerminator( *expandedFile ) } );
			}
		}

		return 0;
	}

// ---------------------------------------------------

	void Tool::RegisterOptions( OptionRegistrar& /*visitor*/ ) {
	//	Default implementation does nothing.
	}

}	// namespace Detail
}	// namespace Tools
}	// namespace Eldritch2