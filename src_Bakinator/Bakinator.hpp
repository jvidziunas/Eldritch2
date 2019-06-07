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
#include <Common/Containers/SoaList.hpp>
#include <Common/Containers/Path.hpp>
#include <Common/FileReader.hpp>
#include <Tools/CrtpTool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
enum class Result : int;
} // namespace Eldritch2

namespace Eldritch2 { namespace Tools {

	class Bakinator : public CrtpTool<Bakinator> {
		// - TYPE PUBLISHING ---------------------------------

	public:
		using ExportList = SoaList<Path, FileReader>;

		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Bakinator(const Bakinator&) = delete;
		//!	Constructs this @ref Bakinator instance.
		Bakinator() ETNoexceptHint;

		~Bakinator() = default;

		// ---------------------------------------------------

	public:
		void RegisterOptions(OptionRegistrar& options);

		Result Process(Logging::Log& log);

		// ---------------------------------------------------

		//!	Disable copy assignment.
		Bakinator& operator=(const Bakinator&) = delete;

		// - DATA MEMBERS ------------------------------------

	private:
		ExportList _exports;
		Path       _indexPath;
		Path       _blobPath;
	};

}} // namespace Eldritch2::Tools
