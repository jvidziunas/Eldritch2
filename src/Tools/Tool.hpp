/*==================================================================*\
  Tool.hpp
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
#include <Utility/Containers/UnorderedMap.hpp>
#include <Utility/MPL/CharTypes.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	Allocator;
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Tools {
namespace Detail {

	class OptionRegistrationVisitorBase {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ParseFunction = void (*)( const ::Eldritch2::SystemChar*, const ::Eldritch2::SystemChar*, void* const );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		OptionRegistrationVisitorBase( ::Eldritch2::Allocator& allocator );

	// ---------------------------------------------------

		int	DispatchOptions( const ::Eldritch2::Range<const ::Eldritch2::SystemChar**> options ) const;

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::UnorderedMap<const ::Eldritch2::UTF8Char*, ::rde::pair<void*, ParseFunction>>	_settings;
	};

}	// namespace Detail

	class Tool {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class OptionRegistrationVisitor : Detail::OptionRegistrationVisitorBase {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref OptionRegistrationVisitor instance.
			OptionRegistrationVisitor( ::Eldritch2::Allocator& allocator );

			//!	Destroys this @ref OptionRegistrationVisitor instance.
			~OptionRegistrationVisitor() = default;

		// ---------------------------------------------------

			template <typename Option>
			OptionRegistrationVisitor&	AddOption( const ::Eldritch2::UTF8Char* const name, Option& option );
		};

	// ---------------------------------------------------

		int	Run( const ::Eldritch2::Range<const ::Eldritch2::SystemChar**> options );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref Tool instance.
		Tool() = default;

		//!	Destroys this @ref Tool instance.
		~Tool() = default;

	// ---------------------------------------------------

		//!	Registers tool settings with the application command-line argument parser.
		/*!	@param[in] visitor @ref OptionRegistrationVisitor that will collect all tool settings.
			*/
		virtual void	RegisterOptions( OptionRegistrationVisitor& visitor );

		virtual int		ProcessInputFiles( const ::Eldritch2::Range<const ::Eldritch2::SystemChar**> inputFiles ) abstract;
	};

}	// namespace Tools
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Tools/Tool.inl>
//------------------------------------------------------------------//