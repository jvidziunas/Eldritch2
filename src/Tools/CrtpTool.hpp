/*==================================================================*\
  CrtpTool.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/ArrayList.hpp>
#include <Common/Containers/String.hpp>
#include <Common/Containers/Span.hpp>
#include <Common/Mpl/Tuple.hpp>
#include <Common/Function.hpp>
//------------------------------------------------------------------//
#include <Logging/Log.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
enum class Result : int;
} // namespace Eldritch2

namespace Eldritch2 { namespace Tools {

	class AbstractCrtpTool {
		// - TYPE PUBLISHING ---------------------------------

	public:
		class OptionRegistrar {
			// - TYPE PUBLISHING ---------------------------------

		public:
			using Setter     = Function<Result(Logging::Log& /*log*/, PlatformStringSpan /*value*/)>;
			using OptionList = ArrayList<Tuple<PlatformStringSpan, Setter>>;

			// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Disable copy construction.
			OptionRegistrar(const OptionRegistrar&) = delete;
			//!	Constructs this @ref OptionRegistrar instance.
			OptionRegistrar() ETNoexceptHint;

			~OptionRegistrar() = default;

			// ---------------------------------------------------

		public:
			OptionRegistrar& Register(PlatformStringSpan name, PlatformStringSpan shortName, Setter setter);
			OptionRegistrar& Register(PlatformStringSpan name, Setter setter);

			OptionRegistrar& RegisterInputFileHandler(Setter setter) ETNoexceptHint;

			// ---------------------------------------------------

		public:
			template <typename Option>
			static ETPureFunctionHint Setter MakePodSetter(Option& option) ETNoexceptHint;

			// ---------------------------------------------------

		public:
			Result Dispatch(Logging::Log& log, Span<PlatformChar**> args);

			// - DATA MEMBERS ------------------------------------

		private:
			OptionList _options;
			Setter     _inputFileHandler;
		};

		// ---------------------------------------------------

	public:
		//!	Registers tool settings with the application command-line argument parser.
		/*!	@param[in] options @ref OptionRegistrar that will collect all tool settings. */
		void RegisterOptions(OptionRegistrar& options);
	};

	// ---

	template <typename ImplementingType>
	class CrtpTool : protected AbstractCrtpTool {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref CrtpTool instance.
		CrtpTool() = default;

		~CrtpTool() = default;

		// ---------------------------------------------------

	public:
		Result Run(Span<PlatformChar**> args);
	};

}} // namespace Eldritch2::Tools

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Tools/CrtpTool.inl>
//------------------------------------------------------------------//
