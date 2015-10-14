/*==================================================================*\
  ResourceViewFactoryPublishingInitializationVisitor.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Packages/ResourceView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ContentLibrary;
	}

	namespace Utility {
		template <typename ResultObjectType>
		struct	Result;
	}
}

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem = ::Eldritch2::FileSystem;

// ---------------------------------------------------

	class ResourceViewFactoryPublishingInitializationVisitor {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Constructs this @ref ResourceViewFactoryPublishingInitializationVisitor instance.
		/*!	@param[in] contentLibrary The @ref ContentLibrary to which resource view factories will be registered.
			*/
		ResourceViewFactoryPublishingInitializationVisitor( FileSystem::ContentLibrary& contentLibrary );

		//!	Destroys this @ref ResourceViewFactoryPublishingInitializationVisitor instance.
		ETInlineHint ~ResourceViewFactoryPublishingInitializationVisitor() = default;

	// ---------------------------------------------------

		//!	Registers an object creation handler for a specified class of resource view object.
		/*!	@param[in] className Null-terminated C string containing the type name of the resource.
			@param[in] parameter Pointer parameter to be sent to the callback for resource creation.
			@param[in] factory Factory function. This should attempt to allocate a new instance of a @ref ResourceView subclass and return it in the Result for the function.
			@returns A reference to *this for method chaining.
			*/
		ResourceViewFactoryPublishingInitializationVisitor&	PublishFactory( const ::Eldritch2::UTF8Char* const className, void* const parameter, Utility::Result<FileSystem::ResourceView> (*factory)( ::Eldritch2::Allocator&, const FileSystem::ResourceView::Initializer&, void* ) );
		
	// - DATA MEMBERS ------------------------------------

	private:
		FileSystem::ContentLibrary&	_contentLibrary;
	};

}	// namespace Eldritch2
}	// namespace FileSystem
	

