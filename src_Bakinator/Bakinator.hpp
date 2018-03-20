/*==================================================================*\
  Bakinator.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/HashSet.hpp>
#include <Common/Containers/String.hpp>
#include <Tools/CrtpTool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	class Bakinator : public CrtpTool<Bakinator> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		Bakinator( const Bakinator& ) = delete;
	//!	Constructs this @ref Bakinator instance.
		Bakinator();

		~Bakinator() = default;

	// ---------------------------------------------------

	public:
		void	RegisterOptions( OptionRegistrar& options );

		int		Process();

	// - DATA MEMBERS ------------------------------------

	private:
		HashSet<String<>>	_exports;
		String<>			_outPath;
	};

}	// namespace Tools
}	// namespace Eldritch2