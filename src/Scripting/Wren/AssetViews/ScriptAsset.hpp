/*==================================================================*\
  ScriptAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Scripting { namespace Wren { namespace AssetViews {

	class ScriptAsset : public Assets::Asset {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref ScriptAsset instance.
		ScriptAsset(const Utf8Char* const path);
		//!	Disable copy construction.
		ScriptAsset(const ScriptAsset&) = delete;

		~ScriptAsset() override = default;

		// ---------------------------------------------------

	public:
		const Utf8Char* Begin() const;

		const Utf8Char* End() const;

		size_t GetLength() const;

		// ---------------------------------------------------

	public:
		static ETPureFunctionHint Utf8Literal GetExtension();

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const Builder& builder) override;

		void FreeResources() override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ScriptAsset& operator=(const ScriptAsset&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		String<> _source;
	};

}}}} // namespace Eldritch2::Scripting::Wren::AssetViews

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Scripting/Wren/AssetViews/ScriptAsset.inl>
//------------------------------------------------------------------//
