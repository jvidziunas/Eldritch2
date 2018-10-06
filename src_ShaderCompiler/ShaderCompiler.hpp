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
#include <Common/Containers/Path.hpp>
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
		Path        _pipelinePath;
		Path        _hlslPath;
		Path        _spirvOutPath;
		Path        _dxbcOutPath;
		ShaderModel _shaderModel;
		bool        _emitSpirV;
		bool        _emitDxbc;
	};

}} // namespace Eldritch2::Tools
