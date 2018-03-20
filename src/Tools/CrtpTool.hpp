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
#include <Common/Mpl/CharTypes.hpp>
#include <Common/Function.hpp>
#include <Common/Pair.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	template <typename>
	class	Range;
}

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	class Tool {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class OptionRegistrar {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using Setter = Function<int ( Range<const Utf8Char*> /*value*/)>;
			using Option = Pair<const Utf8Char*, Setter>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Disable copy construction.
			OptionRegistrar( const OptionRegistrar& ) = delete;
		//!	Constructs this @ref OptionRegistrar instance.
			OptionRegistrar();

			~OptionRegistrar() = default;

		// ---------------------------------------------------

		public:
			OptionRegistrar&	Register( const Utf8Char* const name, const Utf8Char* const shortName, Setter&& setter );
			template <typename Argument>
			OptionRegistrar&	Register( const Utf8Char* const name, const Utf8Char* const shortName, Argument& argument );
			OptionRegistrar&	Register( const Utf8Char* const name, Setter&& setter );
			template <typename Argument>
			OptionRegistrar&	Register( const Utf8Char* const name, Argument& argument );

			OptionRegistrar&	RegisterInputFileHandler( Setter&& setter );

		// ---------------------------------------------------

		public:
			template <typename ArgumentIterator>
			int	Dispatch( ArgumentIterator argumentBegin, ArgumentIterator argumentEnd );
			int	Dispatch( Range<Utf8Char**> arguments );

		// - DATA MEMBERS ------------------------------------

		private:
			ArrayList<Option>	_options;
			Setter				_inputFileHandler;
		};

	// ---------------------------------------------------

	public:
	//!	Registers tool settings with the application command-line argument parser.
	/*!	@param[in] registrar @ref OptionRegistrar that will collect all tool settings. */
		void	RegisterOptions( OptionRegistrar& registrar );
	};

}	// namespace Detail

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
		int	Run( ArgumentIterator argumentBegin, ArgumentIterator argumentEnd );
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Tools/CrtpTool.inl>
//------------------------------------------------------------------//