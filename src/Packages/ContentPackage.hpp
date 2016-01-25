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
#include <Scripting/ReferenceCountable.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Scripting/ObjectHandle.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//
#include <atomic>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ContentLibrary;
	}

	namespace Utility {
		class	ScopedLock;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace FileSystem {

	class ContentPackage : public Scripting::ReferenceCountable, public Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------

	public:
		enum class ResidencyState : ::Eldritch2::uint32 {
			Failed,		//!< The package or one of its dependencies encountered a fatal error while loading. The package contents will never be available.
			Loading,	//!< The package and/or one of its dependencies are currently in the process of being paged into memory. The contents of the package may be available in the future.
			Published	//!< The package, its dependencies and their contents have loaded fully.
		};

		using DependencyCollection	= ::Eldritch2::ResizableArray<Scripting::ObjectHandle<const FileSystem::ContentPackage>>;
		using ExportCollection		= ::Eldritch2::IntrusiveForwardList<FileSystem::ResourceView>;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref ContentPackage instance.
		/*!	@param[in] name Null-terminated C string containing the name of the package. A copy of this will be kept internally, and there are no additional lifetime requirements after the constructor call.
			@param[in] owningLibrary The @ref ContentLibrary instance that owns this @ref ContentPackage. The @ref ContentPackage will route additional load requests through this @ref ContentLibrary.
			@param[in] allocator The @ref Allocator instance the @ref ContentPackage will use to allocate memory.
			*/
		ContentPackage( const ::Eldritch2::UTF8Char* const name, FileSystem::ContentLibrary& owningLibrary, ::Eldritch2::Allocator& allocator );
	
		//! Destroys this @ref ContentPackage instance.
		virtual ~ContentPackage();

	// ---------------------------------------------------

		//!	Gets the base name of the package.
		/*!	@returns A null-terminated C string containing the name of the package. Both the address and contents are guaranteed to remain constant throughout the life of the package.
			*/
		ETInlineHint const ::Eldritch2::UTF8Char* const	GetName() const;

		ETInlineHint const FileSystem::ContentLibrary&	GetContentLibrary() const;
		ETInlineHint FileSystem::ContentLibrary&		GetContentLibrary();

		//!	Returns the set of @ref ContentPackage instances the @ref ResourceView instances of the @ref ContentPackage require to be fully resident.
		/*!	@returns A const @ref ResizeableArray reference to the internal list of referenced packages.
			@remarks Not thread-safe.
			*/
		ETInlineHint const DependencyCollection&		GetDependencies() const;
		ETInlineHint DependencyCollection&				GetDependencies();

		ETInlineHint const ExportCollection&			GetExports() const;
		ETInlineHint ExportCollection&					GetExports();

		ETInlineHint ::Eldritch2::Allocator&			GetAllocator();

		//! Queries the current residency state of the package.
		/*!	@returns A @ref ResidencyState enum describing the state of the package. This can be used to check whether or not content is available, i.e. a world resource.
			@remarks Thread-safe.
			@see @ref ResidencyState
			*/
		ETInlineHint ResidencyState						GetResidencyState() const;

	// ---------------------------------------------------

		ETInlineHint void	UpdateResidencyStateOnLoaderThread( const ContentPackage::ResidencyState newState );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator		_allocator;
		const ::Eldritch2::UTF8String<>	_name;
		FileSystem::ContentLibrary&		_owningLibrary;
		::std::atomic<ResidencyState>	_residencyState;
		ExportCollection				_exportedResources;
		DependencyCollection			_referencedPackages;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/ContentPackage.inl>
//------------------------------------------------------------------//