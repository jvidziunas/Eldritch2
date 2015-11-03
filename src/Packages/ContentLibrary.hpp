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
#include <Utility/Memory/InstanceDeleters.hpp>
#include <Utility/DisposingResult.hpp>
#include <Utility/MPL/Noncopyable.hpp>
#include <Utility/StringOperators.hpp>
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
		class	TaskScheduler;
	}

	namespace Utility {
		class	ReaderWriterUserMutex;
	}

	class	ErrorCode;
}

namespace Eldritch2 {
namespace FileSystem {

	class ContentLibrary : private Utility::Noncopyable {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//! Constructs this @ref ContentLibrary instance.
		/*! @param[in] contentProvider @ref ContentProvider that will translate package names into operating system file objects.
			@param[in] scheduler @ref TaskScheduler instance that will be responsible for running the internal @ref LoaderThread instance.
			@param[in] allocator @ref Allocator instance this @ref ContentProvider should use to perform internal allocations.
			@see @ref ContentProvider, @ref LoaderThread, @ref ContentPackage
			*/
		ContentLibrary( FileSystem::ContentProvider& contentProvider, Scheduler::TaskScheduler& scheduler, ::Eldritch2::Allocator& allocator );

		//! Destroys this @ref ContentLibrary instance.
		~ContentLibrary();

	// ---------------------------------------------------

		//! Signals to the @ref ContentLibrary that the resources in the specified package will be needed in the near future.
		/*! @param[in] packageName A null-terminated C string containing the name of the package file, without any suffix or file extension.
			@returns A @ref DisposingResult containing the content package, or an @ref ErrorCode indicating why a failure occurred.
			@remarks If the desired package is already resident in memory, then a reference to it is added instead.
			@remarks Thread-safe.
			@see @ref ContentPackage, @ref CreatePackageForEditorWorld()
			*/
		::Eldritch2::DisposingResult<FileSystem::ContentPackage>	ResolvePackageByName( const ::Eldritch2::UTF8Char* const packageName );

		::Eldritch2::DisposingResult<FileSystem::ContentPackage>	CreatePackageForEditorWorld();

	// ---------------------------------------------------

		//!	Locates a resource view in the database.
		/*!	@param[in] name Null-terminated C string containing the name of the resource to search for.
			@param[in] defaultView A reference to the @ref ResourceView to return in the event the search was unsuccessful.
			@returns A reference to the found view, if successful, or a reference to _defaultView_ if no compatible views were found in the database.
			@remarks Thread-safe.
			*/
		template <typename View>
		const View&	ResolveViewByName( const ::Eldritch2::UTF8Char* const name, const View& defaultView ) const;

	// ---------------------------------------------------

		//!	Retrieves the @ref ContentProvider this @ref ContentLibrary should use to satisfy file load requests.
		/*!	@returns A reference to the @ref ContentProvider this @ref ContentLibrary should use to satisfy file load requests.
			@remarks Thread-safe.
			*/
		ETInlineHint FileSystem::ContentProvider&	GetContentProvider() const;

	// - TYPE PUBLISHING ---------------------------------
	
	private:
		class ResourceViewFactoryKey : public ::std::pair<const ::Eldritch2::UTF8Char*, const ::Eldritch2::UTF8Char*> {
		// - TYPE PUBLISHING ---------------------------------

		public:
			class Hash : public Utility::StringHash {
			public:
				ETInlineHint ETNoAliasHint size_t	operator()( const ResourceViewFactoryKey& key, const size_t seed = 0u ) const;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

			//! Constructs this @ref ResourceViewFactoryKey instance.
			/*! @param[in] begin Pointer to the beginning of a C string containing the name of a resource type. Does not have to be null-terminated.
				@param[in] end Pointer to one past the last character of the string headed by the _begin_ parameter. This does not necessarily have to (but may) point to a null character.
				*/
			ETInlineHint ResourceViewFactoryKey( const ::Eldritch2::UTF8Char* const begin, const ::Eldritch2::UTF8Char* const end );

			//! Constructs this @ref ResourceViewFactoryKey instance.
			ETInlineHint ResourceViewFactoryKey() = default;

		// ---------------------------------------------------

			//! Tests two @ref ResourceViewFactoryKey instances for equality.
			/*! @param[in] other The second @ref ResourceViewFactoryKey in the comparison.
				@returns _True_ if the two keys describe the same view type, _false_ if they reference different types.
				*/
			ETNoAliasHint bool	operator==( const ResourceViewFactoryKey& other ) const;
		};

	// ---

		class ResourceViewKey : public ::std::pair<const ::Eldritch2::UTF8Char*, const ::std::type_info*> {
		public:
			class Hash : public Utility::StringHash {
			public:
				ETInlineHint ETNoAliasHint size_t	operator()( const ResourceViewKey& key, const size_t seed = 0u ) const;
			};

		// - CONSTRUCTOR/DESTRUCTOR --------------------------


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

		// ---------------------------------------------------

			//! Tests two @ref ResourceViewKey instances for equality.
			/*! @param[in] other The second @ref ResourceViewKey in the comparison.
				@returns _True_ if the two keys describe the same view, _false_ if they reference different types.
				*/
			ETInlineHint ETNoAliasHint bool	operator==( const ResourceViewKey& other ) const;
		};

	// ---

		struct ResourceViewFactory {
			using FactoryFunctionPointer	= ::Eldritch2::InstancePointer<FileSystem::ResourceView> (*)( ::Eldritch2::Allocator&, const ::Eldritch2::UTF8Char* const, void* );

			FactoryFunctionPointer	factoryFunction;
			void*					parameter;
		};

	// - DATA MEMBERS ------------------------------------

		::Eldritch2::ChildAllocator															_allocator;
		::Eldritch2::ChildAllocator															_deserializationContextAllocator;
		FileSystem::ContentProvider&														_contentProvider;
		
		//! User-space mutex guarding the global content package library. _Not_ responsible for protecting the actual resource views.
		::Eldritch2::AlignedInstancePointer<Utility::ReaderWriterUserMutex>					_contentPackageCollectionMutex;

		//! User-space mutex guarding the global resource view library. _Not_ responsible for protecting the packages that own the views.
		::Eldritch2::AlignedInstancePointer<Utility::ReaderWriterUserMutex>					_resourceViewCollectionMutex;		

		::Eldritch2::UnorderedMap<const ::Eldritch2::UTF8Char*, FileSystem::ContentPackage*,
								  Utility::StringHash,
								  Utility::StringEqualComparator<>>							_contentPackageCollection;

		//! The value type is left as a void* to prevent slicing for resource views that use multiple inheritance.
		::Eldritch2::UnorderedMap<ResourceViewKey, const void*, ResourceViewKey::Hash>		_resourceViewCollection;

		::Eldritch2::UnorderedMap<ResourceViewFactoryKey,
								  ::Eldritch2::ResizableArray<ResourceViewFactory>,
								  ResourceViewFactoryKey::Hash>								_resourceViewFactoryCollection;

		FileSystem::LoaderThread*															_loaderThread;

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