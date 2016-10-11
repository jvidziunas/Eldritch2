/*==================================================================*\
  UserInterfaceView.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Utility/Containers/ResizableArray.hpp>
#include <UserInterface/Nuklear/Context.hpp>
#include <Utility/Memory/ChildAllocator.hpp>
#include <Utility/UniquePointer.hpp>
#include <Assets/AssetView.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {
namespace AssetViews {

	class UserInterfaceView : public Assets::AssetView {
	// - TYPE PUBLISHING ---------------------------------

	public:
		class ETPureAbstractHint Widget {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

		public:
		//!	Constructs this Widget instance.
			Widget() = default;

			virtual ~Widget() = default;

		// ---------------------------------------------------

		public:
			virtual void	Draw( Nuklear::Context::Panel& panel ) const abstract;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref UserInterfaceView instance.
		UserInterfaceView( const Eldritch2::Utf8Char* const name, Eldritch2::Allocator& allocator );
	//!	Disable copying.
		UserInterfaceView( const UserInterfaceView& ) = delete;

		~UserInterfaceView() = default;

	// ---------------------------------------------------

	public:
		void	Draw( Nuklear::Context& context ) const;

	// ---------------------------------------------------

	//!	Disable assignment.
		UserInterfaceView&	operator=( const UserInterfaceView& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable Eldritch2::ChildAllocator							_allocator;
		Eldritch2::ResizableArray<Eldritch2::UniquePointer<Widget>>	_widgets;
	};

}	// namespace AssetViews
}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2