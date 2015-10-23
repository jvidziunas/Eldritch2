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

	class BytecodePackageResourceView : public FileSystem::ResourceView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref BytecodePackageResourceView instance.
		BytecodePackageResourceView( const Initializer& initializer, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref BytecodePackageResourceView instance.
		~BytecodePackageResourceView() = default;

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
		AngelScript::BytecodeMetadata						_metadata;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2