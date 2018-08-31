/*==================================================================*\
  Package.hpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Assets {
	class Asset;
}} // namespace Eldritch2::Assets

namespace Eldritch2 { namespace Assets {

	class Package {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using AssetList = ArrayList<UniquePointer<Asset>>;

		enum : size_t { MaxPathLength = 128u };

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Package instance.
		/*!	@param[in] path String view containing the file name of the package, without any directory or extension. */
		Package(StringView path);
		//! Disable copy construction.
		Package(const Package&) = delete;

		~Package();

		// - REFERENCE MANAGEMENT ----------------------------

	public:
		size_t AddReference(MemoryOrder order = std::memory_order_relaxed) const ETNoexceptHint;

		size_t ReleaseReference(MemoryOrder order = std::memory_order_release) const ETNoexceptHint;

		bool IsReferenced(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		bool IsLoaded(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		StringView GetPath() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void BindAssets(AssetList assets);

		AssetList FreeAssets();

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Package& operator=(const Package&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Utf8Char               _path[MaxPathLength];
		mutable Atomic<size_t> _referenceCount;
		Atomic<bool>           _isLoaded;
		AssetList              _assets;
	};

}} // namespace Eldritch2::Assets

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Assets/Package.inl>
//------------------------------------------------------------------//
