/*==================================================================*\
  GameObject.hpp
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

struct	WrenHandle;
struct	WrenVM;

namespace Eldritch2 {
namespace Scripting {
namespace Wren {

	class GameObject {
	// - TYPE PUBLISHING ---------------------------------

	public:
		using ComponentList	= ArrayList<WrenHandle*>;
		using Iterator		= typename ComponentList::Iterator;

	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		GameObject( const GameObject& ) = delete;
	//!	Constructs this @ref GameObject instance.
		GameObject();

		~GameObject();

	// ---------------------------------------------------

	public:
		Iterator	FindComponentByType( WrenVM* vm, WrenHandle* type );

		bool		HasComponentOfType( WrenVM* vm, WrenHandle* type );

	// ---------------------------------------------------

	public:
		void	InsertComponent( WrenVM* vm, WrenHandle* component );

		void	FreeComponents( WrenVM* vm );

	// - DATA MEMBERS ------------------------------------

	private:
		ComponentList	_components;
	};

}	// namespace Wren
}	// namespace Scripting
}	// namespace Eldritch2