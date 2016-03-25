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
#include <Scripting/AngelScript/SmartPointers.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {

	class BytecodePackageResourceView : public FileSystem::ResourceView, private AngelScript::BytecodeMetadata {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref BytecodePackageResourceView instance.
		BytecodePackageResourceView( const ::Eldritch2::UTF8Char* const name, ModuleHandle&& module, ::Eldritch2::Allocator& allocator );

		~BytecodePackageResourceView() = default;

	// ---------------------------------------------------

		ETInlineHint const ModuleHandle&	GetScriptModule() const;

	// ---------------------------------------------------

		::Eldritch2::ErrorCode	AttachToPackage( const ::Eldritch2::Range<const char*> bytes, FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) override;

		void					DetachFromPackage( FileSystem::ContentPackage& package, FileSystem::ContentLibrary& library ) const override;

	// ---------------------------------------------------

		static ETNoAliasHint const ::Eldritch2::UTF8Char* const	GetSerializedDataTag();

	// - DATA MEMBERS ------------------------------------

	private:
		ModuleHandle	_module;
	};

}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/AngelScript/BytecodePackageResourceView.inl>
//------------------------------------------------------------------//