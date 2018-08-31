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
class ErrorCode;
}

namespace Eldritch2 {
namespace Tools {

	class GeometryCompiler : public CrtpTool<GeometryCompiler> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		GeometryCompiler(const GeometryCompiler&) = delete;
		//!	Constructs this @ref GeometryCompiler instance.
		GeometryCompiler();

		~GeometryCompiler() = default;

		// ---------------------------------------------------

	public:
		void RegisterOptions(OptionRegistrar& visitor);

		ErrorCode ProcessMesh(StringView<PlatformChar> path);

		ErrorCode Process();

		// - DATA MEMBERS ------------------------------------

	private:
		HashSet<PlatformString<>> _sourcePaths;
		String<>                  _password;
		bool                      _skipTextureCoordinates;
		bool                      _skipOrientation;
	};

}
} // namespace Eldritch2::Tools
