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
#include <Core/ContentDatabase.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	class AsynchronousImport : public AbstractPackageClient {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using PackageList = ArrayList<CountedPointer<const Package>>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref AsynchronousImport instance.
		AsynchronousImport(const AsynchronousImport&) = delete;
		//!	Constructs this @ref AsynchronousImport instance.
		AsynchronousImport() ETNoexceptHint;

		~AsynchronousImport();

		// ---------------------------------------------------

	protected:
		void OnLoadSuccess(CountedPointer<const Package> package) ETNoexceptHint override;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		AsynchronousImport& operator=(const AsynchronousImport&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ETCacheLineAligned mutable Mutex _packagesMutex;
		PackageList                      _packages;

		// ---------------------------------------------------

		friend void Swap(AsynchronousImport&, AsynchronousImport&) = delete;
	};

	// ---

	class ContentLocator {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ContentLocator instance.
		ETConstexpr ContentLocator(ContentDatabase& content) ETNoexceptHint;
		//!	Disable copy construction.
		ContentLocator(const ContentLocator&) = delete;

		~ContentLocator() = default;

		// ---------------------------------------------------

	public:
		ETConstexpr ContentDatabase& GetContent() const ETNoexceptHint;

		// ---------------------------------------------------

	public:
		Result BindResources();

		void FreeResources() ETNoexceptHint;

		// ---------------------------------------------------

		//!	Disable copy assignment.
		ContentLocator& operator=(const ContentLocator&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ContentDatabase* _content;

		// ---------------------------------------------------

		friend void Swap(ContentLocator&, ContentLocator&) = delete;
	};

}} // namespace Eldritch2::Core

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Core/ContentLocator.inl>
//------------------------------------------------------------------//
