/*==================================================================*\
  RenderConcept.hpp
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
	namespace Scheduling {
		class	JobExecutor;
	}
}

namespace Eldritch2 {
namespace Graphics {

	template <typename Instance, typename Extractor, class Allocator = Eldritch2::MallocAllocator>
	class RenderConcept {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using HierarchyType	= ArrayBvh<Instance, Extractor, Allocator>;
		using AllocatorType = typename HierarchyType::AllocatorType;
		using ValueType		= typename HierarchyType::ValueType;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref RenderConcept instance.
		RenderConcept( const AllocatorType& allocator, const Extractor& extractor );
	//!	Disable copy construction.
		RenderConcept( const RenderConcept& ) = delete;
	//!	Constructs this @ref RenderConcept instance.
		RenderConcept( RenderConcept&& ) ETNoexceptHint = default;

		~RenderConcept() = default;

	// ---------------------------------------------------

	public:
		void	Insert( const ValueType& value );

		void	Erase( const ValueType& value );

	// ---------------------------------------------------

	public:
		bool	ShouldRebuildHierarchy();

		void	BuildHierarchy( Scheduling::JobExecutor& executor );

	// ---------------------------------------------------

	//!	Disable copy assignment.
		RenderConcept&	operator=( const RenderConcept& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		std::atomic<bool>	_shouldRebuildHierarchy;
		mutable Mutex		_hierarchyMutex;
		HierarchyType		_hierarchy;
	};

}	// namespace Graphics
}	// namespace Eldritch2

//==================================================================//
// INLINE FUNCTION DEFINITIONS
//==================================================================//
#include <Graphics/RenderConcept.inl>
//------------------------------------------------------------------//