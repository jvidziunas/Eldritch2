/*==================================================================*\
  AssetManager.hpp
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

		template <typename PublicAsset, bool checkCast = true>
		friend const PublicAsset* Cast(const AssetReference& asset);

		friend void Swap(AssetReference&, AssetReference&);
	};

	// ---

	class AssetManager {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref AssetManger instance.
		AssetManager(const AssetDatabase& assets);
		//!	Disable copy construction.
		AssetManager(const AssetManager&) = delete;

		~AssetManager() = default;

		// ---------------------------------------------------

	public:
		const Asset* Find(const Utf8Char* path) const;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AssetManager& operator=(const AssetManager&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const AssetDatabase* _assets;
	};

}} // namespace Eldritch2::Assets

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/AssetManager.inl>
//------------------------------------------------------------------//
