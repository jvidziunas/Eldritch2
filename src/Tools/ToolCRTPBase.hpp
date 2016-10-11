/*==================================================================*\
  ToolCRTPBase.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/Pair.hpp>
#include <functional>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Allocator;
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	class ToolBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class OptionRegistrationVisitor {
		// - TYPE PUBLISHING ---------------------------------

		public:
			using Handler		= std::function<int (const Eldritch2::Utf8Char* const, const Eldritch2::Utf8Char* const)>;
			using KnownOption	= Eldritch2::Pair<const Eldritch2::Utf8Char*, Handler>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//!	Constructs this @ref OptionRegistrationVisitor instance.
		/*!	@param[in] allocator @ref Allocator the @ref OptionRegistrationVisitor should use to perform internal allocations. */
			OptionRegistrationVisitor( Eldritch2::Allocator& allocator );

			~OptionRegistrationVisitor() = default;

		// ---------------------------------------------------

			OptionRegistrationVisitor&	AddArgument( const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const shortName, const Handler& handler );
			OptionRegistrationVisitor&	AddArgument( const Eldritch2::Utf8Char* const name, const Handler& handler );
			
			template <typename Argument>
			OptionRegistrationVisitor&	AddTypedArgument( const Eldritch2::Utf8Char* const name, const Eldritch2::Utf8Char* const shortName, const std::function<int( const Argument )>& handler );
			template <typename Argument>
			OptionRegistrationVisitor&	AddTypedArgument( const Eldritch2::Utf8Char* const name, const std::function<int ( const Argument )>& handler );

			OptionRegistrationVisitor&	AddInputFileHandler( const Handler& handler );

		// ---------------------------------------------------

			int	DispatchProgramArguments( const Eldritch2::Range<Eldritch2::Utf8Char**> arguments );

		// - DATA MEMBERS ------------------------------------

		private:
			Eldritch2::ChildAllocator				_allocator;
			Eldritch2::ResizableArray<KnownOption>	_knownOptions;
			Handler									_inputFileHandler;
		};

	// ---------------------------------------------------

		//!	Registers tool settings with the application command-line argument parser.
		/*!	@param[in] visitor @ref OptionRegistrationVisitor that will collect all tool settings.
			*/
		void	RegisterOptions( OptionRegistrationVisitor& visitor );
	};

}	// namespace Detail

	template <typename ImplementingType>
	class ToolCRTPBase : protected Detail::ToolBase {
	// ---------------------------------------------------

	public:
		int Run( const Eldritch2::Range<Eldritch2::Utf8Char**> options );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
	//!	Constructs this @ref ToolCRTPBase instance.
		ToolCRTPBase() = default;

	//!	Destroys this @ref ToolCRTPBase instance.
		~ToolCRTPBase() = default;
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Tools/ToolCRTPBase.inl>
//------------------------------------------------------------------//