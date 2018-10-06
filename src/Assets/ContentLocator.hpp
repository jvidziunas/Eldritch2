/*==================================================================*\
  ContentLocator.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/PackageDatabase.hpp>
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
		AssetReference(const AssetReference&) ETNoexceptHint = default;
		//!	Constructs this @ref AssetReference instance.
		AssetReference(const Asset& asset) ETNoexceptHint;

		~AssetReference() = default;

		// ---------------------------------------------------

	public:
		AssetReference& operator=(const AssetReference&) ETNoexceptHint = default;

		// - DATA MEMBERS ------------------------------------

	private:
		const Asset* _asset;

		// ---------------------------------------------------

		template <typename PublicAsset, bool isChecked = true>
		friend const PublicAsset* Cast(const AssetReference& asset) ETNoexceptHint;

		friend void Swap(AssetReference&, AssetReference&) ETNoexceptHint;
	};

	// ---

	class AsynchronousImport : public PackageLoadClient {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using PackageList = ArrayList<CountedPointer<const Package>, MallocAllocator>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref AsynchronousImport instance.
		AsynchronousImport(const AsynchronousImport&) = delete;
		//!	Constructs this @ref AsynchronousImport instance.
		AsynchronousImport();

		~AsynchronousImport();

		// ---------------------------------------------------

	public:
		bool IsComplete(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ErrorCode InitiateLoad(PackageDatabase& packages, StringView path);

		void CompleteLoad(CountedPointer<const Package> package, ErrorCode finalResult) ETNoexceptHint override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AsynchronousImport& operator=(const AsynchronousImport&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Atomic<size_t> _pendingLoadCount;
		Mutex          _loadedPackagesMutex;
		PackageList    _loadedPackages;

		// ---------------------------------------------------

		friend void Swap(AsynchronousImport&, AsynchronousImport&) = delete;
	};

	// ---

	class ContentLocator {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ContentLocator(const ContentLocator&) = delete;
		//!	Constructs this @ref ContentLocator instance.
		ContentLocator() ETNoexceptHint;

		~ContentLocator() = default;

		// ---------------------------------------------------

	public:
		const Asset* FindAsset(StringView path) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(const AssetDatabase& assets, PackageDatabase& packages);

		void FreeResources();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ContentLocator& operator=(const ContentLocator&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		const AssetDatabase* _assets;
		PackageDatabase*     _packages;
	};

}} // namespace Eldritch2::Assets

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/ContentLocator.inl>
//------------------------------------------------------------------//
