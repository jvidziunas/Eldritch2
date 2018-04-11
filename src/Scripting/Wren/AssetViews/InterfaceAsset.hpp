/*==================================================================*\
  InterfaceAsset.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Assets/Asset.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace UserInterface {
		namespace Nuklear {
			class	Interface;
			class	Panel;
		}
	}

	namespace Assets {
		class	AssetApiBuilder;
	}
}

namespace Eldritch2 {
namespace UserInterface {
namespace Nuklear {
namespace AssetViews {

	class InterfaceAsset : public Assets::Asset {
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
			virtual void	Draw( Panel& panel ) const abstract;
		};

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref InterfaceAsset instance.
		InterfaceAsset( const Utf8Char* const assetPath );
	//!	Disable copy construction.
		InterfaceAsset( const InterfaceAsset& ) = delete;

		~InterfaceAsset() override = default;

	// ---------------------------------------------------

	public:
		void	Draw( Nuklear::Interface& nuklear ) const;

	// ---------------------------------------------------

	public:
		ErrorCode	BindResources( const Builder& builder ) override;

		void		FreeResources() override;

	// ---------------------------------------------------

	//!	Disable copy assignment.
		InterfaceAsset&	operator=( const InterfaceAsset& ) = delete;

	// - DATA MEMBERS ------------------------------------

	private:
		mutable MallocAllocator				_allocator;
		ArrayList<UniquePointer<Widget>>	_widgets;
	};

}	// namespace AssetViews
}	// namespace Nuklear
}	// namespace UserInterface
}	// namespace Eldritch2