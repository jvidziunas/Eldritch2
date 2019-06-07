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

	enum class ShaderModel : uint32 {
		_5_0,
		_6_0,
		_6_1,
	};

	// ---

	enum class ShaderStage : uint32 {
		Vertex,
		Hull,
		TessellationControl = Hull,
		Domain,
		TessellationEval = Domain,
		Geometry,
		Pixel,
		Fragment = Pixel,
		Compute,

		MAX_STAGE
	};

	// ---

	class ShaderCompiler : public CrtpTool<ShaderCompiler> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		ShaderCompiler(const ShaderCompiler&) = delete;
		//!	Constructs this @ref ShaderCompiler instance.
		ShaderCompiler() ETNoexceptHint;

		~ShaderCompiler() = default;

		// ---------------------------------------------------

	public:
		void RegisterOptions(OptionRegistrar& options);

		Result Process(Logging::Log& log);

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
