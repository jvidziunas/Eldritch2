/*==================================================================*\
  ContentPackage.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardList.hpp>
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Containers/UTF8String.hpp>
#include <Scripting/ReferenceTypeBase.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	PackageDeserializationContext;
		class	ContentLibrary;
	}

	namespace Utility {
		class	ScopedLock;
		template <typename ResultObjectType>
		struct	ResultPair;
	}
}

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scripting		= ::Eldritch2::Scripting;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ContentPackage : public Scripting::ReferenceTypeBase, public Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum class ResidencyState : ::Eldritch2::uint32 {
			FAILED,		//!< The package or one of its dependencies encountered a fatal error while loading. The package contents will never be available.
			LOADING,	//!< The package and/or one of its dependencies are currently in the process of being paged into memory. The contents of the package may be available in the future.
			PUBLISHED	//!< The package, its dependencies and their contents have loaded fully.
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref ContentPackage instance.
		/*!	@param[in] name Null-terminated C string containing the name of the package. A copy of this will be kept internally, and there are no additional lifetime requirements after the constructor call.
			@param[in] owningLibrary The @ref ContentLibrary instance that owns this @ref ContentPackage. The @ref ContentPackage will route additional load requests through this @ref ContentLibrary.
			@param[in] allocator The @ref Allocator instance the @ref ContentPackage will use to allocate memory.
			*/
		ContentPackage( const ::Eldritch2::UTF8Char* const name, FileSystem::ContentLibrary& owningLibrary, ::Eldritch2::Allocator& allocator );
		//! Constructs this @ref ContentPackage instance, with anonymous/editor semantics.
		/*!	@param[in] owningLibrary The ContentLibrary instance that owns this @ref ContentPackage. The @ref ContentPackage will route additional load requests through this @ref ContentLibrary.
			@param[in] allocator The Allocator instance the @ref ContentPackage will use to allocate memory.
			*/
		ContentPackage( FileSystem::ContentLibrary& owningLibrary, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref ContentPackage instance.
		~ContentPackage();

	// ---------------------------------------------------

		//!	Gets the base name of the package.
		/*!	@returns A null-terminated C string containing the name of the package. Both the address and contents are guaranteed to remain constant throughout the life of the package.
			*/
		ETInlineHint const ::Eldritch2::UTF8Char* const	GetName() const;

		//! Queries the current residency state of the package.
		/*!	@returns A @ref ResidencyState enum describing the state of the package. This can be used to check whether or not content is available, i.e. a world resource.
			@remarks Thread-safe.
			@see @ref ResidencyState
			*/
		ETInlineHint ResidencyState	GetResidencyState() const;

	// ---------------------------------------------------

		ETInlineHint const ::Eldritch2::ResizableArray<Scripting::ObjectHandle<FileSystem::ContentPackage>>&	GetReferencedPackageCollection() const;

		ETInlineHint const ::Eldritch2::IntrusiveForwardList<FileSystem::ResourceView>&							GetExportedResourceCollection() const;

	// ---------------------------------------------------

		void	Dispose() override sealed;

	// ---------------------------------------------------

	protected:
		ETInlineHint void	UpdateResidencyState( const ContentPackage::ResidencyState newState );

		//! Deallocates and invokes the destructor on every @ref ResourceView contained in the @ref ContentPackage.
		/*!	@remarks Not thread-safe.
			*/
		void				DeleteContent();

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator															_allocator;
		::Eldritch2::UTF8String<>															_name;
		FileSystem::ContentLibrary&															_owningLibrary;
		::std::atomic<ResidencyState>														_residencyState;
		::Eldritch2::IntrusiveForwardList<FileSystem::ResourceView>							_exportedResources;
		::Eldritch2::ResizableArray<Scripting::ObjectHandle<FileSystem::ContentPackage>>	_referencedPackages;

	// ---------------------------------------------------

		friend class FileSystem::PackageDeserializationContext;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/ContentPackage.inl>
//------------------------------------------------------------------//