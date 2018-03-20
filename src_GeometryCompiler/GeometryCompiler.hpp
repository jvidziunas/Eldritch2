/*==================================================================*\
  GeometryCompiler.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
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

	class GeometryCompiler : public CrtpTool<GeometryCompiler> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Constructs this @ref GeometryCompiler instance.
		GeometryCompiler();

		~GeometryCompiler() = default;

	// ---------------------------------------------------

	public:
		void	RegisterOptions( OptionRegistrar& visitor );

		int		Process();

	// - DATA MEMBERS ------------------------------------

	private:
		String<>			_password;
		HashSet<String<>>	_sourcePaths;
		bool				_skipTextureCoordinates;
		bool				_skipOrientation;
	};

}	// namespace Tools
}	// namespace Eldritch2