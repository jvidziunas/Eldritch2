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
#include <Common/Containers/PlatformString.hpp>
#include <Common/ComPointer.hpp>
#include <Tools/CrtpTool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Tools {

	enum ShaderModel : uint32 {
		ShaderModel5_0,
		ShaderModel6_0,
		ShaderModel6_1,
	};

	// ---

	enum ShaderStage : uint32 {
		Vertex,
		Hull,
		TessellationControl = Hull,
		Domain,
		TessellationEval = Domain,
		Geometry,
		Pixel,
		Fragment = Pixel,
		Compute,
	};

	// ---

	class ShaderCompiler : public CrtpTool<ShaderCompiler> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ShaderCompiler(const ShaderCompiler&) = delete;
		//!	Constructs this @ref ShaderCompiler instance.
		ShaderCompiler();

		~ShaderCompiler() = default;

		// ---------------------------------------------------

	public:
		void RegisterOptions(OptionRegistrar& options);

		int Process();

		// - DATA MEMBERS ------------------------------------

	private:
		PlatformString<> _pipelinePath;
		PlatformString<> _hlslPath;
		PlatformString<> _spirvOutPath;
		PlatformString<> _dxbcOutPath;
		ShaderModel      _shaderModel;
		bool             _emitSpirV;
		bool             _emitDxbc;
	};

}} // namespace Eldritch2::Tools
