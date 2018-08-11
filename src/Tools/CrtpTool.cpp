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

namespace Eldritch2 { namespace Tools { namespace Detail {

	Tool::OptionRegistrar::OptionRegistrar() :
		_options(MallocAllocator("Tool Option Registry Allocator")) {}

	// ---------------------------------------------------

	Tool::OptionRegistrar& Tool::OptionRegistrar::Register(StringView<Utf8Char> name, Setter setter) {
		_options.EmplaceBack(eastl::move(name), eastl::move(setter));
		return *this;
	}

	// ---------------------------------------------------

	Tool::OptionRegistrar& Tool::OptionRegistrar::Register(StringView<Utf8Char> name, StringView<Utf8Char> shortName, Setter setter) {
		//	Create one copy for the long name argument...
		_options.EmplaceBack(name, setter);
		//	... and directly move the argument for the short name.
		_options.EmplaceBack(eastl::move(shortName), eastl::move(setter));
		return *this;
	}

	// ---------------------------------------------------

	Tool::OptionRegistrar& Tool::OptionRegistrar::RegisterInputFileHandler(Setter setter) {
		_inputFileHandler = eastl::move(setter);
		return *this;
	}

	// ---------------------------------------------------

	int Tool::OptionRegistrar::Dispatch(Range<Utf8Char**> programArguments) {
		using OptionGlobber = CSimpleGlobTempl<Utf8Char>;
		using OptionParser  = CSimpleOptTempl<Utf8Char>;

		// ---

		ArrayList<OptionParser::SOption> convertedOptions(MallocAllocator("OptionRegistrar::Dispatch() Converted Option Allocator"));
		int                              index(0);

		//	Converted option count should be one larger than the number of registered options, as the parser library uses a sentinel to delimit the end of registered options.
		convertedOptions.Reserve(_options.GetSize() + 1);
		for (const Option& option : _options) {
			convertedOptions.Append({ index++, option.first, SO_REQ_SEP });
		}
		//	Terminate the list to be handed off to the option parser.
		convertedOptions.Append(SO_END_OF_OPTIONS);

		{
			OptionParser  parser(int(programArguments.GetSize()), programArguments.Begin(), convertedOptions.GetData(), SO_O_SHORTARG | SO_O_CLUMP);
			OptionGlobber globber;

			while (parser.Next()) {
				switch (parser.LastError()) {
				case SO_SUCCESS: {
					const int result(_options[parser.OptionId()].second({ parser.OptionArg(), StringLength(parser.OptionArg()) }));
					if (result != 0) {
						return result;
					}

					break;
				} // case SO_SUCCESS
				case SO_OPT_INVALID: {
					std::cerr << "Unknown option " << parser.OptionText() << ", ignoring." << std::endl;
					break;
				} // case SO_OPT_INVALID
				case SO_ARG_MISSING: {
					std::cerr << "Option " << parser.OptionText() << " requires an argument, but none was provided!" << std::endl;
					break;
				} // case SO_OPT_INVALID
				default: {
					std::cerr << "Fatal error parsing command line." << std::endl;
					return 1;
				} // default case
				} // switch(parser.LastError())
			}

			for (int file(0); file < parser.FileCount(); ++file) {
				globber.Add(parser.File(file));
			}
			for (int file(0); file < globber.FileCount(); ++file) {
				_inputFileHandler({ globber.File(file), StringLength(globber.File(file)) });
			}
		}

		return 0;
	}

	// ---------------------------------------------------

	void Tool::RegisterOptions(OptionRegistrar& /*visitor*/) {
		//	Default implementation does nothing.
	}

}}} // namespace Eldritch2::Tools::Detail
