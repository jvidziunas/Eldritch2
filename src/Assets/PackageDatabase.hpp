/*==================================================================*\
  PackageDatabase.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Package.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {
	class AssetDatabase;
}} // namespace Eldritch2::Assets

namespace Eldritch2 { namespace Assets {

	class PackageDatabase {
		// - TYPE PUBLISHING ---------------------------------

	public:
		struct PackageEqual {
			ETPureFunctionHint bool operator()(const Package&, StringView<Utf8Char>) const;
			ETPureFunctionHint bool operator()(const Package&, const Package&) const;
		};

		// ---

	public:
		struct PackageHash {
			ETPureFunctionHint size_t operator()(StringView<Utf8Char>, size_t seed = 0u) const;
			ETPureFunctionHint size_t operator()(const Package&, size_t seed = 0u) const;
		};

		// ---

	public:
		struct LoadRequest {
			Function<void(CountedPointer<const Package>, ErrorCode)> callback;
			CountedPointer<Package>                                  package;
		};

		// ---

	public:
		using LoadableSet = CachingHashSet<Package, PackageHash, PackageEqual>;
		using GcCursor    = typename LoadableSet::Iterator;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		PackageDatabase(const PackageDatabase&) = delete;
		//!	Constructs this @ref PackageDatabase instance.
		PackageDatabase();

		~PackageDatabase() = default;

		// ---------------------------------------------------

	public:
		//!	Performs an incremental garbage collection pass on the set of unreferenced asset packages.
		/*!	@remarks @parblock Note that this function is *not* safe to be called simulaneously on multiple threads, however other methods that operate on the package set
				may be safely called while this function is being executed. @endparblock */
		void DestroyGarbage(AssetDatabase& assetDatabase, size_t destructionLimit);

		// - PACKAGE MANAGEMENT ------------------------------

	public:
		//! Signals to the @ref AssetDatabase that resources in the specified package will be needed in the near future.
		/*! @param[in] path UTF-8-encoded string view containing the path to the package file, without any suffix or extension.
			@remarks Thread-safe.
			@see @ref Package */
		ErrorCode Load(StringView<Utf8Char> path, Function<void(CountedPointer<const Package>, ErrorCode)> callback);

		bool PopRequest(LoadRequest& outRequest);

		// ---------------------------------------------------

	public:
		ErrorCode BindResources(LoadableSet packages);

		void FreeResources();

		// ---------------------------------------------------

	private:
		void PushRequest(LoadRequest request);

		void ClearRequests();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		PackageDatabase& operator=(const PackageDatabase&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		mutable Mutex      _requestsMutex;
		Stack<LoadRequest> _requests;
		mutable Mutex      _packagesMutex;
		LoadableSet        _packages;
		GcCursor           _gcCursor;
	};

}} // namespace Eldritch2::Assets

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/PackageDatabase.inl>
//------------------------------------------------------------------//
