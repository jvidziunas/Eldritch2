/*==================================================================*\
  ObjectGraphResourceView.hpp
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
		namespace AngelScript {
			class	WorldView;
		}
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class ObjectGraphResourceView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ObjectGraphResourceView instance.
		ObjectGraphResourceView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		//!	Destroys this @ref ObjectGraphResourceView instance.
		~ObjectGraphResourceView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	DeserializeIntoWorldView( AngelScript::WorldView& worldView ) const;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2