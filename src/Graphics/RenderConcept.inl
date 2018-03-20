/*==================================================================*\
  Scene.inl
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Graphics {

	template <typename Instance, typename Extractor, class Allocator>
	ETInlineHint RenderConcept<Instance, Extractor, Allocator>::RenderConcept(
		const AllocatorType& allocator,
		const Extractor& extractor
	) : _shouldRebuildHierarchy( false ),
		_hierarchy( extractor, allocator ) {
	}

// ---------------------------------------------------

	template <typename Instance, typename Extractor, class Allocator>
	ETInlineHint void RenderConcept<Instance, Extractor, Allocator>::Insert( const ValueType& value ) {
		Lock	_( _hierarchyMutex );

		_hierarchy.Insert( value );

		_shouldRebuildHierarchy.store( true, std::memory_order_release );
	}

// ---------------------------------------------------

	template <typename Instance, typename Extractor, class Allocator>
	ETInlineHint void RenderConcept<Instance, Extractor, Allocator>::Erase( const ValueType& value ) {
		Lock	_( _hierarchyMutex );

		_hierarchy.Erase( _hierarchy.Find( value ) );

		_shouldRebuildHierarchy.store( true, std::memory_order_release );
	}

// ---------------------------------------------------

	template <typename Instance, typename Extractor, class Allocator>
	ETInlineHint bool RenderConcept<Instance, Extractor, Allocator>::ShouldRebuildHierarchy() {
		return _shouldRebuildHierarchy.load( std::memory_order_consume );
	}

// ---------------------------------------------------

	template <typename Instance, typename Extractor, class Allocator>
	ETInlineHint void RenderConcept<Instance, Extractor, Allocator>::BuildHierarchy( Scheduling::JobExecutor& executor ) {
	//	_hierarchy.Build( executor );
		_shouldRebuildHierarchy.store( false, std::memory_order_release );
	}

}	// namespace Graphics
}	// namespace Eldritch2