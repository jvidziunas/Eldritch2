/*==================================================================*\
  ContentLibrary.hpp
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
#include <Utility/Containers/HashMap.hpp>
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Packages/ResourceViewFactory.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/CountedResult.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//
#include <typeinfo>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ResourceViewFactoryPublishingInitializationVisitor;
		class	PackageDeserializationContext;
		class	ContentProvider;
		class	ContentPackage;
		class	LoaderThread;
		class	ResourceView;
	}

	namespace Scheduler {
		class	ThreadScheduler;
	}

	namespace Utility {
		class	ReaderWriterUserMutex;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace FileSystem {

	class ContentLibrary : private Utility::Noncopyable {
	// - TYPE PUBLISHING ---------------------------------
		
	public:
		using ResourceViewFactoryCollection = ::Eldritch2::IntrusiveForwardList<FileSystem::ResourceViewFactory>;

		class ResourceViewKey : public ::Eldritch2::Pair<const ::Eldritch2::UTF8Char*, const ::std::type_info*> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
			//!	Constructs this @ref ResourceViewKey instance.
			/*!	@param[in] resourceName The name of the @ref ResourceView to find.
				@param[in] resourceType The concrete polymorphic type of the @ref ResourceView
				*/
			ETInlineHint ResourceViewKey( const ::Eldritch2::UTF8Char* const resourceName, const ::std::type_info* resourceType );
			//! Constructs this @ref ResourceViewKey instance.
			/*! @param[in] view @ref ResourceView to create a key for.
				*/
			ResourceViewKey( const FileSystem::ResourceView& view );

			//! Constructs this @ref ResourceViewKey instance.
			ETInlineHint ResourceViewKey() = default;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

		//! Constructs this @ref ContentLibrary instance.
		/*! @param[in] contentProvider @ref ContentProvider that will translate package names into operating system file objects.
			@param[in] scheduler @ref ThreadScheduler instance that will be responsible for running the internal @ref LoaderThread instance.
			@param[in] allocator @ref Allocator instance this @ref ContentProvider should use to perform internal allocations.
			@see @ref ContentProvider, @ref LoaderThread, @ref ContentPackage
			*/
		ContentLibrary( FileSystem::ContentProvider& contentProvider, Scheduler::ThreadScheduler& scheduler, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref ContentLibrary instance.
		~ContentLibrary();

	// ---------------------------------------------------

		//! Signals to the @ref ContentLibrary that the resources in the specified package will be needed in the near future.
		/*! @param[in] packageName A null-terminated C string containing the name of the package file, without any suffix or file extension.
			@returns A @ref CountedResult containing the content package, or an @ref ErrorCode indicating why a failure occurred.
			@remarks If the desired package is already resident in memory, then a reference to it is added instead.
			@remarks Thread-safe.
			@see @ref ContentPackage, @ref CreatePackageForEditorWorld()
			*/
		::Eldritch2::CountedResult<const FileSystem::ContentPackage>	ResolvePackageByName( const ::Eldritch2::UTF8Char* const packageName );

		::Eldritch2::CountedResult<const FileSystem::ContentPackage>	CreatePackageForEditorWorld();

	// ---------------------------------------------------

		//!	Locates a resource view in the database.
		/*!	@param[in] name Null-terminated C string containing the name of the resource to search for.
			@returns A pointer to the found view, if successful, or null in the event no views were found.
			@remarks Thread-safe.
			*/
		template <typename View>
		const View*	ResolveViewByName( const ::Eldritch2::UTF8Char* const name ) const;

	// ---------------------------------------------------

		//!	Retrieves the @ref ContentProvider this @ref ContentLibrary should use to satisfy file load requests.
		/*!	@returns A reference to the @ref ContentProvider this @ref ContentLibrary should use to satisfy file load requests.
			@remarks Thread-safe.
			*/
		ETInlineHint FileSystem::ContentProvider&	GetContentProvider() const;

	// ---------------------------------------------------

		ETInlineHint ::Eldritch2::Range<ResourceViewFactoryCollection::Iterator>	GetFactoriesForResourceType( const ::Eldritch2::UTF8Char* const resourceTypeName );

	// - DATA MEMBERS ------------------------------------

	private:
		::Eldritch2::ChildAllocator															_allocator;
		::Eldritch2::ChildAllocator															_deserializationContextAllocator;
		FileSystem::ContentProvider&														_contentProvider;
		
		//! User-space mutex guarding the global content package library. _Not_ responsible for protecting the actual resource views.
		::Eldritch2::AlignedInstancePointer<Utility::ReaderWriterUserMutex>					_contentPackageDirectoryMutex;
		//! User-space mutex guarding the global resource view library. _Not_ responsible for protecting the packages that own the views.
		::Eldritch2::AlignedInstancePointer<Utility::ReaderWriterUserMutex>					_resourceViewDirectoryMutex;		

		::Eldritch2::HashMap<const ::Eldritch2::UTF8Char*, FileSystem::ContentPackage*>		_contentPackageDirectory;
		::Eldritch2::HashMap<ResourceViewKey, const FileSystem::ResourceView*>				_resourceViewDirectory;
		::Eldritch2::HashMap<const ::Eldritch2::UTF8Char*, ResourceViewFactoryCollection>	_resourceFactoryDirectory;

		ResourceViewFactoryCollection														_nullFactoryCollection;

		FileSystem::LoaderThread*															_loaderThread;

	// - FRIEND CLASS DECLARATION ------------------------

		friend class ::Eldritch2::FileSystem::ResourceViewFactoryPublishingInitializationVisitor;
		friend class ::Eldritch2::FileSystem::ContentPackage;
		friend class ::Eldritch2::FileSystem::ResourceView;
	};

// ---------------------------------------------------

	ETInlineHint ETNoAliasHint size_t	GetHashCode( const ContentLibrary::ResourceViewKey& key, const size_t seed );

	//! Tests two @ref ResourceViewKey instances for equality.
	/*! @returns _True_ if the two keys describe the same view, _false_ if they reference different types.
	*/
	ETInlineHint ETNoAliasHint bool		operator==( const ContentLibrary::ResourceViewKey& left, const ContentLibrary::ResourceViewKey& right );

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/ContentLibrary.inl>
//------------------------------------------------------------------//