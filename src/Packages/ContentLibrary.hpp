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
#include <Utility/Containers/ResizableArray.hpp>
#include <Utility/Containers/UnorderedMap.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/StringOperators.hpp>
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ResourceViewFactoryPublishingInitializationVisitor;
		class	PackageDeserializationContext;
		class	ContentProvider;
		class	ContentPackage;
		class	LoaderThread;
	}

	namespace Scheduler {
		class	TaskScheduler;
	}

	namespace Utility {
		class	ReaderWriterUserMutex;
		template <typename ResultObjectType>
		struct	DisposingResultPair;
		template <typename ResultObjectType>
		struct	ResultPair;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Scheduler		= ::Eldritch2::Scheduler;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	class ContentLibrary : private Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref ContentLibrary instance.
		/*! @param[in] contentProvider @ref ContentProvider that will translate package names into operating system file objects.
			@param[in] scheduler @ref TaskScheduler instance that will be responsible for running the internal @ref LoaderThread instance.
			@param[in] allocator @ref Allocator instance this @ref ContentProvider should use to perform internal allocations.
			@see @ref ContentProvider, @ContentProvider::LoaderThread, @ref ContentPackage
			*/
		ContentLibrary( FileSystem::ContentProvider& contentProvider, Scheduler::TaskScheduler& scheduler, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref ContentLibrary instance.
		~ContentLibrary();

	// ---------------------------------------------------

		//! Signals to the @ref ContentLibrary that the resources in the specified package will be needed in the near future.
		/*! @param[in] packageName A null-terminated C string containing the name of the package file, without any suffix or file extension.
			@returns a @ref DisposingResultPair containing the content package, or an @ref ErrorCode indicating why a failure occurred.
			@remark If the desired package is already resident in memory, then a reference to it is added instead.
			@see @ref ContentPackage, @ref ContentLibrary::CreatePackageForEditorWorld()
			*/
		Utility::DisposingResultPair<FileSystem::ContentPackage>	ResolvePackageByName( const ::Eldritch2::UTF8Char* const packageName );

		Utility::DisposingResultPair<FileSystem::ContentPackage>	CreatePackageForEditorWorld();

	// ---------------------------------------------------

		template <typename View>
		const View&	ResolveViewByName( const ::Eldritch2::UTF8Char* const name, const View& defaultView ) const;

	// - TYPE PUBLISHING ---------------------------------
	
	private:
		class ResourceViewFactoryKey : public ::std::pair<const ::Eldritch2::UTF8Char*, const ::Eldritch2::UTF8Char*> {
		// - TYPE PUBLISHING ---------------------------------

		public:
			class Hash : public Utility::StringHash {
			public:
				ETInlineHint ETNoAliasHint size_t	operator()( const ResourceViewFactoryKey string, const size_t seed = 0u ) const;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref ResourceViewFactoryKey instance.
			/*! @param[in] begin Pointer to the beginning of a C string containing the name of a resource type. Does not have to be null-terminated.
				@param[in] end Pointer to one past the last character of the string headed by the _begin_ parameter. This does not necessarily have to point to a null character.
				*/
			ETInlineHint ResourceViewFactoryKey( const ::Eldritch2::UTF8Char* const begin, const ::Eldritch2::UTF8Char* const end );

			//! Constructs this @ref ResourceViewFactoryKey instance.
			ETInlineHint ResourceViewFactoryKey();

		// ---------------------------------------------------

			//! Tests two @ref ResourceViewFactoryKey instances for equality.
			/*! @param[in] other The second @ref ResourceViewFactoryKey in the comparison.
				@returns _True_ if the two keys describe the same view type, _false_ if they reference different types.
				*/
			bool	operator==( const ResourceViewFactoryKey& other ) const;
		};

	// ---

		struct ResourceViewFactory {
			Utility::ResultPair<ResourceView> (*factoryFunction)( ::Eldritch2::Allocator&, const FileSystem::ResourceView::Initializer&, void* );
			void*	parameter;
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator																	_allocator;
		::Eldritch2::ChildAllocator																	_deserializationContextAllocator;
		FileSystem::ContentProvider&																_contentProvider;
		
		//! User-space mutex guarding the global content package library. _Not_ responsible for protecting the actual resource views.
		Utility::ReaderWriterUserMutex*																_contentPackageLibraryMutex;

		//! User-space mutex guarding the global resource view library. _Not_ responsible for protecting the packages that own the views.
		Utility::ReaderWriterUserMutex*																_resourceViewLibraryMutex;		

		::Eldritch2::UnorderedMap<const ::Eldritch2::UTF8Char*, FileSystem::ContentPackage*,
								  Utility::StringHash,
								  Utility::StringEqualComparator<>>									_contentPackageLibrary;

		::Eldritch2::UnorderedMap<const ::Eldritch2::UTF8Char*, const FileSystem::ResourceView*,
								  Utility::StringHash,
								  Utility::StringEqualComparator<>>									_resourceViewLibrary;

		::Eldritch2::UnorderedMap<ResourceViewFactoryKey,
								  ::Eldritch2::ResizableArray<ResourceViewFactory>,
								  ResourceViewFactoryKey::Hash>										_resourceViewFactoryLibrary;

		FileSystem::LoaderThread*																	_loaderThread;

	// - FRIEND CLASS DECLARATION ------------------------

		friend class ::Eldritch2::FileSystem::ResourceViewFactoryPublishingInitializationVisitor;
		friend class ::Eldritch2::FileSystem::PackageDeserializationContext;
	};

}	// namespace FileSystem
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Packages/ContentLibrary.inl>
//------------------------------------------------------------------//