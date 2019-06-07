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
#include <Common/Containers/SoaList.hpp>
#include <Common/Containers/String.hpp>
#include <Common/Filesystem.hpp>
#include <Tools/CrtpTool.hpp>
#include <Common/Memory.hpp>
//------------------------------------------------------------------//
#include <simpleopt/SimpleOpt.h>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Tools {

	using namespace ::Eldritch2::Logging;

	// ---------------------------------------------------

	AbstractCrtpTool::OptionRegistrar::OptionRegistrar() ETNoexceptHint : _options(OptionList::AllocatorType("Tool Option Registry Allocator")) {}

	// ---------------------------------------------------

	Result AbstractCrtpTool::OptionRegistrar::Dispatch(Log& log, Span<PlatformChar**> args) {
		using OptionParser = CSimpleOptTempl<PlatformChar>;

		//	Converted option count should be one larger than the number of registered options, as the parser library uses a sentinel to delimit the end of registered options.
		SoaList<PlatformString, OptionParser::SOption> options(MallocAllocator(), _options.GetSize() + 1);
		for (OptionList::ConstReference option : _options) {
			PlatformString path(PlatformString::AllocatorType(), Get<StringSpan&>(option));
			options.EmplaceBack(Move(path), OptionParser::SOption{ int(options.GetSize()), path.AsCString(), SO_REQ_SEP });
		}
		//	Terminate the list to be handed off to the option parser.
		options.EmplaceBack(PlatformString(PlatformString::AllocatorType(), SL("<dummy>")), SO_END_OF_OPTIONS);
		{
			OptionParser parser(int(args.GetSize()), args.Begin(), options.GetData<OptionParser::SOption>(), SO_O_SHORTARG | SO_O_CLUMP);
			while (parser.Next()) {
				switch (parser.LastError()) {
				case SO_SUCCESS: {
					ET_ABORT_UNLESS(Get<Setter&>(_options[parser.OptionId()])(log, PlatformStringSpan(parser.OptionArg(), StringLength(parser.OptionArg()))));
					break;
				} // case SO_SUCCESS
				case SO_OPT_INVALID: {
					log.Write(Severity::Warning, "\tignoring (unknown) option {}" ET_NEWLINE, parser.OptionText());
					break;
				} // case SO_OPT_INVALID
				case SO_ARG_MISSING: {
					log.Write(Severity::Error, "\t{} needs an (missing) argument" ET_NEWLINE, parser.OptionText());
					break;
				} // case SO_OPT_INVALID
				} // switch(parser.LastError())
			}

			for (int file(0); file < parser.FileCount(); ++file) {
				ForEachFile(PlatformStringSpan(parser.File(file), StringLength(parser.File(file))), [&](PlatformStringSpan path) {
					_inputFileHandler(log, path);
				});
			}
		}

		return Result::Success;
	}

	// ---------------------------------------------------

	void AbstractCrtpTool::RegisterOptions(OptionRegistrar& /*options*/) {
		//	Default implementation does nothing.
	}

}} // namespace Eldritch2::Tools
