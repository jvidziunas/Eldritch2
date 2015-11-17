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
#include <Utility/Containers/UnorderedMap.hpp>
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/Pair.hpp>
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
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref OptionRegistrationVisitor instance.
			/*!	@param[in] allocator @ref Allocator the @ref OptionRegistrationVisitor should use to perform internal allocations.
				*/
			OptionRegistrationVisitor( ::Eldritch2::Allocator& allocator );

			~OptionRegistrationVisitor() = default;

		// ---------------------------------------------------

			template <typename Option>
			OptionRegistrationVisitor&	AddOption( const ::Eldritch2::SystemChar* const name, const ::Eldritch2::SystemChar shortName, Option& option );

			template <typename Option>
			OptionRegistrationVisitor&	AddOption( const ::Eldritch2::SystemChar* const name, Option& option );

		// ---------------------------------------------------

			int	DispatchOptions( const ::Eldritch2::Range<const ::Eldritch2::SystemChar**> options ) const;

		// ---------------------------------------------------

		private:
			using SettingHandler	= ::Eldritch2::Pair<void*, int (*)( const ::Eldritch2::SystemChar*, const ::Eldritch2::SystemChar*, void* const )>;

		// - DATA MEMBERS ------------------------------------

			::Eldritch2::UnorderedMap<const ::Eldritch2::SystemChar*, SettingHandler>	_settings;
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
		int	Run( const ::Eldritch2::Range<const ::Eldritch2::SystemChar**> options );

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