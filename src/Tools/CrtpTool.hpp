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
#include <Common/Function.hpp>
#include <Common/Pair.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Tools {
	namespace Detail {

		class Tool {
			// - TYPE PUBLISHING ---------------------------------

		public:
			class OptionRegistrar {
				// - TYPE PUBLISHING ---------------------------------

			public:
				using Setter = Function<int(PlatformStringView /*value*/)>;
				using Option = Pair<PlatformStringView, Setter>;

				// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				//!	Disable copy construction.
				OptionRegistrar(const OptionRegistrar&) = delete;
				//!	Constructs this @ref OptionRegistrar instance.
				OptionRegistrar();

				~OptionRegistrar() = default;

				// ---------------------------------------------------

			public:
				template <typename Option>
				OptionRegistrar& Register(PlatformStringView name, PlatformStringView shortName, Option& option);
				OptionRegistrar& Register(PlatformStringView name, PlatformStringView shortName, Setter setter);
				template <typename Option>
				OptionRegistrar& Register(PlatformStringView name, Option& option);
				OptionRegistrar& Register(PlatformStringView name, Setter setter);

				OptionRegistrar& RegisterInputFileHandler(Setter setter);

				// ---------------------------------------------------

			public:
				int Dispatch(int argc, PlatformChar** argv);

				// - DATA MEMBERS ------------------------------------

			private:
				ArrayList<Option> _options;
				Setter            _inputFileHandler;
			};

			// ---------------------------------------------------

		public:
			//!	Registers tool settings with the application command-line argument parser.
			/*!	@param[in] registrar @ref OptionRegistrar that will collect all tool settings. */
			void RegisterOptions(OptionRegistrar& registrar);
		};

	} // namespace Detail

	template <typename ImplementingType>
	class CrtpTool : protected Detail::Tool {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref CrtpTool instance.
		CrtpTool() = default;

		~CrtpTool() = default;

		// ---------------------------------------------------

	public:
		int Run(int argc, PlatformChar** argv);
	};

}} // namespace Eldritch2::Tools

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Tools/CrtpTool.inl>
//------------------------------------------------------------------//
