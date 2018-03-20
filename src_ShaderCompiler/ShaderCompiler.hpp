/*==================================================================*\
  ShaderCompiler.hpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2017 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/String.hpp>
#include <Tools/CrtpTool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Tools {

	class ShaderCompiler : public CrtpTool<ShaderCompiler> {
	// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
	//!	Disable copy construction.
		ShaderCompiler( const ShaderCompiler& ) = delete;
	//!	Constructs this @ref ShaderCompiler instance.
		ShaderCompiler();

		~ShaderCompiler() = default;

	// ---------------------------------------------------

	public:
		void	RegisterOptions( OptionRegistrar& options );

		int		Process();

	// - DATA MEMBERS ------------------------------------

	private:
	};

}	// namespace Tools
}	// namespace Eldritch2