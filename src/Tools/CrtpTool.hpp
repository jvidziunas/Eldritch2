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
#include <Common/Containers/StringView.hpp>
#include <Common/Containers/ArrayList.hpp>
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
				using Setter = Function<int(StringView<Utf8Char> /*value*/)>;
				using Option = Pair<StringView<Utf8Char>, Setter>;

				// - CONSTRUCTOR/DESTRUCTOR --------------------------

			public:
				//!	Disable copy construction.
				OptionRegistrar(const OptionRegistrar&) = delete;
				//!	Constructs this @ref OptionRegistrar instance.
				OptionRegistrar();

				~OptionRegistrar() = default;

				// ---------------------------------------------------

			public:
				OptionRegistrar& Register(StringView<Utf8Char> name, StringView<Utf8Char> shortName, Setter setter);
				template <typename Option>
				OptionRegistrar& Register(StringView<Utf8Char> name, StringView<Utf8Char> shortName, Option& podOption);
				template <typename Option>
				OptionRegistrar& Register(StringView<Utf8Char> name, Option& podOption);
				OptionRegistrar& Register(StringView<Utf8Char> name, Setter setter);

				OptionRegistrar& RegisterInputFileHandler(Setter setter);

				// ---------------------------------------------------

			public:
				template <typename ArgumentIterator>
				int Dispatch(ArgumentIterator begin, ArgumentIterator end);

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
		template <typename ArgumentIterator>
		int Run(ArgumentIterator argumentBegin, ArgumentIterator argumentEnd);
	};

}} // namespace Eldritch2::Tools

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Tools/CrtpTool.inl>
//------------------------------------------------------------------//
