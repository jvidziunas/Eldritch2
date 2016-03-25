/*==================================================================*\
  ResourceViewFactory.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/IntrusiveForwardListHook.hpp>
#include <Utility/MPL/CharTypes.hpp>
#include <Utility/Result.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace FileSystem {
		class	ContentLibrary;
		class	ContentPackage;
		class	ResourceView;
	}

	class	Allocator;
	class	ErrorCode;
}

namespace Eldritch2 {
namespace FileSystem {

	class ETPureAbstractHint ResourceViewFactory : public ::Eldritch2::IntrusiveForwardListBaseHook {
	public:
		virtual ::Eldritch2::Result<FileSystem::ResourceView>	AllocateResourceView( ::Eldritch2::Allocator& allocator, const ::Eldritch2::UTF8Char* const name ) const abstract;

		virtual ::Eldritch2::ErrorCode							AllocateEngineResources( ::Eldritch2::Allocator& allocator, FileSystem::ContentLibrary& contentLibrary, FileSystem::ContentPackage& package );

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	protected:
		//!	Constructs this @ref ResourceViewFactory instance.
		/*!	@remarks Designed to be called only by subclasses.
			*/
		ResourceViewFactory() = default;

		//!	Destroys this @ref ResourceViewFactory instance.
		/*!	@remarks Designed to be called only by subclasses.
			*/
		~ResourceViewFactory() = default;
	};

}	// namespace FileSystem
}	// namespace Eldritch2