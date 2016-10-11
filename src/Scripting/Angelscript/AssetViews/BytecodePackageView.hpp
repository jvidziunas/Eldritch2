/*==================================================================*\
  BytecodePackageView.hpp
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
#include <Utility/StringLiteral.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//
#include <EASTL/functional.h>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {
		class	AssetLibrary;
	}

	template <typename>
	class	Result;
}

namespace Eldritch2 {
namespace Scripting {
namespace AngelScript {
namespace AssetViews {

	class BytecodePackageView : public Assets::AssetView {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//! Constructs this @ref BytecodePackageView instance.
		BytecodePackageView( const Eldritch2::Utf8Char* const name, AngelScript::UniquePointer<asIScriptModule> module, AngelScript::BytecodeMetadata&& metadata );
	//!	Disable copying.
		BytecodePackageView( const BytecodePackageView& ) = delete;

		~BytecodePackageView() = default;

	// ---------------------------------------------------

	public:
		const AngelScript::BytecodeMetadata&	GetMetadata() const;

		const asIScriptModule&					GetScriptModule() const;

	// ---------------------------------------------------

	public:
		static Eldritch2::Result<Eldritch2::UniquePointer<Assets::AssetView>>	CreateView( Eldritch2::Allocator& allocator, asIScriptEngine& engine, const Assets::AssetLibrary& library, const Eldritch2::Utf8Char* const name, Eldritch2::Range<const char*> assetBytes );

		static ETPureFunctionHint Eldritch2::Utf8Literal						GetExtension();

	// ---------------------------------------------------

	//!	Disable assignment.
		BytecodePackageView&	operator=( const BytecodePackageView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		AngelScript::UniquePointer<asIScriptModule>	_module;
		AngelScript::BytecodeMetadata				_metadata;
	};

}	// namespace AssetViews
}	// namespace AngelScript
}	// namespace Scripting
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/AngelScript/AssetViews/BytecodePackageView.inl>
//------------------------------------------------------------------//