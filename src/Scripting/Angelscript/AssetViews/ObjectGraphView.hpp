/*==================================================================*\
  ObjectGraphView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/StringLiteral.hpp>
#include <Utility/UniquePointer.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetLibrary;
	}

	namespace Scripting {
		namespace AngelScript {
			class	WorldService;
		}
	}

	class	ErrorCode;
	template <typename>
	class	Result;
	template <typename>
	class	Range;
}

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {
namespace AssetViews {

	class ObjectGraphView : public Assets::AssetView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref ObjectGraphView instance.
		ObjectGraphView( const Eldritch2::Utf8Char* const name );
	//!	Disable copying.
		ObjectGraphView( const ObjectGraphView& ) = delete;

		~ObjectGraphView() = default;

	// ---------------------------------------------------

	public:
		Eldritch2::ErrorCode	InstantiateInto( AngelScript::WorldService& service ) const;

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<Eldritch2::UniquePointer<Assets::AssetView>>	CreateView( Eldritch2::Allocator& allocator, const Assets::AssetLibrary& library, const Eldritch2::Utf8Char* const name, Eldritch2::Range<const char*> assetBytes );

		static ETPureFunctionHint Eldritch2::Utf8Literal						GetExtension();

	// ---------------------------------------------------

	//!	Disable assignment.
		ObjectGraphView&	operator=( const ObjectGraphView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
	};

}	// namespace AssetViews
}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2