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
#include <Common/Containers/SoaList.hpp>
#include <Common/Containers/Path.hpp>
#include <Common/FileReader.hpp>
#include <Tools/CrtpTool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Tools {

	class GeometryCompiler : public CrtpTool<GeometryCompiler> {
	public:
		using MeshList = SoaList<Path, FileReader>;

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

		Result ProcessMesh(Logging::Log& log, MeshList::Reference sourceMesh);

		Result Process(Logging::Log& log);

		// - DATA MEMBERS ------------------------------------

	private:
		MeshList _meshes;
		String   _password;
		bool     _skipTextureCoordinates;
		bool     _skipOrientation;
	};

}} // namespace Eldritch2::Tools
