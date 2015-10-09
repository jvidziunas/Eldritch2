/*==================================================================*\
  AngelscriptBytecodePackageView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/AngelscriptBytecodePackage.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Scripting {
		class	AngelscriptEngine;
	}

	class	ErrorCode;
	template <typename Iterator>
	class	Range;
}

namespace Eldritch2 {
namespace Scripting {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class AngelscriptBytecodePackageView : public FileSystem::ResourceView, public Scripting::AngelscriptBytecodePackage {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref AngelscriptBytecodePackageView instance.
		AngelscriptBytecodePackageView( ::std::unique_ptr<::asIScriptModule>&& ownedModule, const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref AngelscriptBytecodePackageView instance.
		~AngelscriptBytecodePackageView() = default;
	};

}	// namespace Scripting
}	// namespace Eldritch2