/*==================================================================*\
  AngelscriptBytecodePackageView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/AngelscriptBytecodeMetadata.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//
#include <memory>
//------------------------------------------------------------------//

namespace Eldritch2 {
	class	ErrorCode;
}

class	asIScriptEngine;
class	asIScriptModule;

namespace Eldritch2 {
namespace Scripting {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class AngelscriptBytecodePackageView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref AngelscriptBytecodePackageView instance.
		AngelscriptBytecodePackageView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref AngelscriptBytecodePackageView instance.
		~AngelscriptBytecodePackageView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	InstantiateFromByteArray( const ::Eldritch2::Range<const char*>& sourceBytes, ::asIScriptEngine& engine );

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// ---------------------------------------------------

	private:
		struct ModuleDeleter {
			void	operator()( ::asIScriptModule* const module );
		};

		::std::unique_ptr<::asIScriptModule, ModuleDeleter>	_module;
		Scripting::AngelscriptBytecodeMetadata				_metadata;
	};

}	// namespace Scripting
}	// namespace Eldritch2