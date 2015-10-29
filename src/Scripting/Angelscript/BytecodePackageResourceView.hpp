/*==================================================================*\
  BytecodePackageResourceView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Scripting/Angelscript/BytecodeMetadata.hpp>
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
namespace AngelScript {

	class BytecodePackageResourceView : public FileSystem::ResourceView, private AngelScript::BytecodeMetadata {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref BytecodePackageResourceView instance.
		BytecodePackageResourceView( ::asIScriptEngine& engine, const ::Eldritch2::UTF8Char* const name, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref BytecodePackageResourceView instance.
		~BytecodePackageResourceView() = default;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	UpdateFromByteStream( const ::Eldritch2::Range<const char*> bytes ) override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// ---------------------------------------------------

	private:
		struct ModuleDeleter {
			void	operator()( ::asIScriptModule* const module );
		};

	// - DATA MEMBERS ------------------------------------

		::std::unique_ptr<::asIScriptModule, ModuleDeleter>	_module;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2