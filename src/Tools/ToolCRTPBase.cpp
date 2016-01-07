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

using namespace ::Eldritch2::Tools;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	ToolBase::OptionRegistrationVisitor::OptionRegistrationVisitor( Allocator& allocator ) : _allocator( allocator, UTF8L("Tool Allocator") ), _knownOptions( { _allocator, UTF8L("Tool Option Registry Allocator") } ) {}

// ---------------------------------------------------

	ToolBase::OptionRegistrationVisitor& ToolBase::OptionRegistrationVisitor::AddArgument( const UTF8Char* const name, const Handler& handler ) {
		_knownOptions.PushBack( { name, handler } );

		return *this;
	}

// ---------------------------------------------------

	ToolBase::OptionRegistrationVisitor& ToolBase::OptionRegistrationVisitor::AddArgument( const UTF8Char* const name, const UTF8Char* const shortName, const Handler& handler ) {
		_knownOptions.PushBack( { name, handler } );
		_knownOptions.PushBack( { shortName, handler } );

		return *this;
	}

// ---------------------------------------------------

	ToolBase::OptionRegistrationVisitor& ToolBase::OptionRegistrationVisitor::AddInputFileHandler( const Handler& handler ) {
		_inputFileHandler = handler;

		return *this;
	}

// ---------------------------------------------------

	int ToolBase::OptionRegistrationVisitor::DispatchProgramArguments( const Range<UTF8Char**> programArguments ) {
		using OptionGlobber = ::CSimpleGlobTempl<UTF8Char>;
		using OptionParser	= ::CSimpleOptTempl<UTF8Char>;

	// ---

		ResizableArray<OptionParser::SOption>	convertedOptions( _knownOptions.Size() + 1, { _allocator, UTF8L("OptionRegistrationVisitor::DispatchProgramArguments() Converted Option Allocator") } );
		int										convertedOptionIndex( -1 );

		for( const auto& option : _knownOptions ) {
			convertedOptions.PushBack( { ++convertedOptionIndex, option.first, ::SO_REQ_SEP } );
		}

		// Terminate the list to be handed off to the option parser.
		convertedOptions.PushBack( SO_END_OF_OPTIONS );

		{	OptionParser	parser { static_cast<int>(programArguments.onePastLast - programArguments.first), programArguments.first, convertedOptions.Data(), 0 };
			OptionGlobber	globber;

			while( parser.Next() ) {
				if( parser.LastError() != ::SO_SUCCESS ) {
					return -1;
				}

				const auto	parseResult( _knownOptions[parser.OptionId()].second( parser.OptionArg(), FindEndOfString( parser.OptionArg() ) ) );

				if( 0 != parseResult ) {
					return parseResult;
				}
			}

			// Add all input files to the globber
			for( UTF8Char** inputFile{ parser.Files() }, **end{ inputFile + parser.FileCount() }; inputFile != end; ++inputFile ) {
				globber.Add( *inputFile );
			}

			for( UTF8Char** expandedFile{ globber.Files() }, **end{ expandedFile + globber.FileCount() }; expandedFile != end; ++expandedFile ) {
				_inputFileHandler( *expandedFile, FindEndOfString( *expandedFile ) );
			}
		}

		return 0;
	}

// ---------------------------------------------------

	void ToolBase::RegisterOptions( OptionRegistrationVisitor& /*visitor*/ ) {
		// Default implementation does nothing.
	}

}	// namespace Detail
}	// namespace Tools
}	// namespace Eldritch2

