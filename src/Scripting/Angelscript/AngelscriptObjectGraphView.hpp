/*==================================================================*\
  AngelscriptObjectGraphView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	AngelscriptWorldView;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Scripting {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scripting		= ::Eldritch2::Scripting;

// ---------------------------------------------------

	class AngelscriptObjectGraphView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		// Constructs this @ref AngelscriptObjectGraphView instance.
		AngelscriptObjectGraphView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		// Destroys this @ref AngelscriptObjectGraphView instance.
		~AngelscriptObjectGraphView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	DeserializeIntoWorldView( Scripting::AngelscriptWorldView& worldView ) const;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
	};

}	// namespace Scripting
}	// namespace Eldritch2