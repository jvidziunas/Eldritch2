/*==================================================================*\
  CrtpTool.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Result.hpp>
//------------------------------------------------------------------//
#include <Tools/ConsoleLog.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Tools {

	ETInlineHint ETForceInlineHint AbstractCrtpTool::OptionRegistrar& AbstractCrtpTool::OptionRegistrar::Register(PlatformStringSpan name, Setter setter) {
		_options.EmplaceBack(Move(name), Move(setter));
		return *this;
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint AbstractCrtpTool::OptionRegistrar& AbstractCrtpTool::OptionRegistrar::Register(PlatformStringSpan name, PlatformStringSpan shortName, Setter setter) {
		//	Create one copy for the long name argument...
		_options.EmplaceBack(name, setter);
		//	... and directly move the argument for the short name.
		return Register(shortName, Move(setter));
	}

	// ---------------------------------------------------

	ETInlineHint ETForceInlineHint AbstractCrtpTool::OptionRegistrar& AbstractCrtpTool::OptionRegistrar::RegisterInputFileHandler(Setter setter) ETNoexceptHint {
		_inputFileHandler = Move(setter);
		return *this;
	}

	// ---------------------------------------------------

	template <typename Option>
	ETInlineHint ETForceInlineHint ETPureFunctionHint AbstractCrtpTool::OptionRegistrar::Setter AbstractCrtpTool::OptionRegistrar::MakePodSetter(Option& option) ETNoexceptHint {
		return [&option](Logging::Log& /*log*/, PlatformStringSpan value) -> Result { return value.Consume(option) ? Result::Success : Result::InvalidParameter; };
	}

	// ---------------------------------------------------

	template <typename ImplementingType>
	ETForceInlineHint Result CrtpTool<ImplementingType>::Run(Span<PlatformChar**> args) {
		Logging::ConsoleLog log;

		{
			OptionRegistrar registrar;
			static_cast<ImplementingType&>(*this).RegisterOptions(registrar);
			ET_ABORT_UNLESS(registrar.Dispatch(log, args));
		}

		return static_cast<ImplementingType&>(*this).Process(log);
	}

}} // namespace Eldritch2::Tools
