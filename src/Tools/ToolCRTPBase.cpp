/*==================================================================*\
  ToolCRTPBase.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Memory/StandardLibrary.hpp>
#include <Utility/Containers/Range.hpp>
#include <Tools/ToolCRTPBase.hpp>
//------------------------------------------------------------------//
#ifdef ET_PLATFORM_WINDOWS
#	include <Windows.h>
#endif
#include <simpleopt/SimpleGlob.h>
#include <simpleopt/SimpleOpt.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	ToolBase::OptionRegistrationVisitor::OptionRegistrationVisitor( Allocator& allocator ) : _allocator( allocator, "Tool Allocator" ), _knownOptions( { _allocator, "Tool Option Registry Allocator" } ) {}

// ---------------------------------------------------

	ToolBase::OptionRegistrationVisitor& ToolBase::OptionRegistrationVisitor::AddArgument( const Utf8Char* const name, const Handler& handler ) {
		_knownOptions.EmplaceBack( name, handler );

		return *this;
	}

// ---------------------------------------------------

	ToolBase::OptionRegistrationVisitor& ToolBase::OptionRegistrationVisitor::AddArgument( const Utf8Char* const name, const Utf8Char* const shortName, const Handler& handler ) {
		_knownOptions.EmplaceBack( name, handler );
		_knownOptions.EmplaceBack( shortName, handler );

		return *this;
	}

// ---------------------------------------------------

	ToolBase::OptionRegistrationVisitor& ToolBase::OptionRegistrationVisitor::AddInputFileHandler( const Handler& handler ) {
		_inputFileHandler = handler;

		return *this;
	}

// ---------------------------------------------------

	int ToolBase::OptionRegistrationVisitor::DispatchProgramArguments( const Range<Utf8Char**> programArguments ) {
		using OptionGlobber = CSimpleGlobTempl<Utf8Char>;
		using OptionParser	= CSimpleOptTempl<Utf8Char>;

	// ---

		ResizableArray<OptionParser::SOption>	convertedOptions( _knownOptions.GetSize() + 1, { _allocator, "OptionRegistrationVisitor::DispatchProgramArguments() Converted Option Allocator" } );
		int										convertedOptionIndex( -1 );

		for( const auto& option : _knownOptions ) {
			convertedOptions.PushBack( { ++convertedOptionIndex, option.first, SO_REQ_SEP } );
		}

	//	Terminate the list to be handed off to the option parser.
		convertedOptions.PushBack( SO_END_OF_OPTIONS );

		{	OptionParser	parser { static_cast<int>(programArguments._end - programArguments._begin), programArguments._begin, convertedOptions.Data(), 0 };
			OptionGlobber	globber;

			while( parser.Next() ) {
				if( parser.LastError() != SO_SUCCESS ) {
					return -1;
				}

				const auto	parseResult( _knownOptions[parser.OptionId()].second( parser.OptionArg(), FindEndOfString( parser.OptionArg() ) ) );

				if( 0 != parseResult ) {
					return parseResult;
				}
			}

		//	Add all input files to the globber.
			for( Utf8Char** inputFile{ parser.Files() }, **end{ inputFile + parser.FileCount() }; inputFile != end; ++inputFile ) {
				globber.Add( *inputFile );
			}

			for( Utf8Char** expandedFile{ globber.Files() }, **end{ expandedFile + globber.FileCount() }; expandedFile != end; ++expandedFile ) {
				_inputFileHandler( *expandedFile, FindEndOfString( *expandedFile ) );
			}
		}

		return 0;
	}

// ---------------------------------------------------

	void ToolBase::RegisterOptions( OptionRegistrationVisitor& /*visitor*/ ) {
	//	Default implementation does nothing.
	}

}	// namespace Detail
}	// namespace Tools
}	// namespace Eldritch2

