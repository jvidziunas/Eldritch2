/*==================================================================*\
  ContentPackage.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace FileSystem {

	namespace FileSystem	= ::Eldritch2::FileSystem;
	namespace Utility		= ::Eldritch2::Utility;

// ---------------------------------------------------

	ETInlineHint const ::Eldritch2::ResizableArray<Scripting::ObjectHandle<FileSystem::ContentPackage>>& ContentPackage::GetReferencedPackageCollection() const {
		return _referencedPackages;
	}

// ---------------------------------------------------

	ETInlineHint const ::Eldritch2::IntrusiveForwardList<FileSystem::ResourceView>& ContentPackage::GetExportedResourceCollection() const {
		return _exportedResources;
	}

// ---------------------------------------------------

	ETInlineHint const ::Eldritch2::UTF8Char* const ContentPackage::GetName() const {
		return _name.GetCharacterArray();
	}

// ---------------------------------------------------

	ETInlineHint ContentPackage::ResidencyState	ContentPackage::GetResidencyState() const {
		return _residencyState.load( ::std::memory_order_acquire );
	}

// ---------------------------------------------------

	ETInlineHint void ContentPackage::UpdateResidencyState( const ContentPackage::ResidencyState newState ) {
		_residencyState.store( newState, ::std::memory_order_release );
	}

}	// namespace FileSystem
}	// namespace Eldritch2