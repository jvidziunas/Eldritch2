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

namespace Eldritch2 { namespace Core {
	class Asset;
}} // namespace Eldritch2::Core

namespace Eldritch2 { namespace Core {

	class Package {
		// - TYPE PUBLISHING ---------------------------------

	public:
		enum : StringSpan::SizeType { MaxPathLength = 128u };
		using AssetHandle = const Asset*;
		using AssetList   = ArrayList<AssetHandle>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref Package instance.
		/*!	@param[in] path String view containing the file name of the package, without any directory or extension. */
		Package(StringSpan path) ETNoexceptHint;
		//! Constructs this @ref Package instance.
		Package(const Package&) ETNoexceptHint;

		~Package();

		// - REFERENCE MANAGEMENT ----------------------------

	public:
		size_t AddReference(MemoryOrder order = std::memory_order_relaxed) const ETNoexceptHint;

		size_t ReleaseReference(MemoryOrder order = std::memory_order_release) const ETNoexceptHint;

		bool ShouldCollect(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		bool IsReferenced(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		ETConstexpr StringSpan GetPath() const ETNoexceptHint;

		FileTime GetReadTime(MemoryOrder order = std::memory_order_consume) const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		void BindAssets(FileTime readTime, AssetList& assets) ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Package& operator=(const Package&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		Utf8Char               _path[MaxPathLength];
		mutable Atomic<size_t> _referenceCount;
		Atomic<FileTime>       _readTime;
		AssetList              _assets;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/Package.inl>
//------------------------------------------------------------------//
