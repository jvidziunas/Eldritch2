/*==================================================================*\
  AssetLocator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {
	class AssetDatabase;
	class Asset;
}} // namespace Eldritch2::Assets

namespace Eldritch2 { namespace Assets {

	class AssetReference {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref AssetReference instance.
		AssetReference(const AssetReference&) = default;
		//!	Constructs this @ref AssetReference instance.
		AssetReference(const Asset& asset);

		~AssetReference() = default;

		// ---------------------------------------------------

	public:
		AssetReference& operator=(const AssetReference&) = default;

		// - DATA MEMBERS ------------------------------------

	private:
		const Asset* _asset;

		// ---------------------------------------------------

		template <typename PublicAsset, bool isChecked = true>
		friend const PublicAsset* Cast(const AssetReference& asset);

		friend ETConstexpr void Swap(AssetReference&, AssetReference&);
	};

	// ---

	class AssetLocator {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref AssetLocator instance.
		AssetLocator(const AssetDatabase& assets);
		//!	Disable copy construction.
		AssetLocator(const AssetLocator&) = delete;

		~AssetLocator() = default;

		// ---------------------------------------------------

	public:
		const Asset* Find(StringView<Utf8Char> path) const;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AssetLocator& operator=(const AssetLocator&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const AssetDatabase* _assets;
	};

}} // namespace Eldritch2::Assets

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/AssetLocator.inl>
//------------------------------------------------------------------//
