/*==================================================================*\
  AngelscriptBytecodePackageView.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/AngelscriptBytecodePackageView.hpp>
#include <utility>
//------------------------------------------------------------------//

using namespace ::Eldritch2::FileSystem;
using namespace ::Eldritch2::Scripting;
using namespace ::Eldritch2;
using namespace ::std;

namespace Eldritch2 {
namespace Scripting {

	AngelscriptBytecodePackageView::AngelscriptBytecodePackageView( unique_ptr<::asIScriptModule>&& ownedModule, const Initializer& initializer, Allocator& allocator ) : ResourceView( initializer, allocator ), AngelscriptBytecodePackage( move( ownedModule ), allocator ) {}

}	// namespace Scripting
}	// namespace Eldritch2