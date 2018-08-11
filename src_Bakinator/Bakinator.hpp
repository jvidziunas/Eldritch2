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
#include <Common/Containers/ArrayList.hpp>
#include <Common/Containers/String.hpp>
#include <Tools/CrtpTool.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 { namespace Tools {

	class Bakinator : public CrtpTool<Bakinator> {
		// - CONSTRUCTOR/DESTRUCTOR --------------------------

	public:
		//!	Disable copy construction.
		Bakinator(const Bakinator&) = delete;
		//!	Constructs this @ref Bakinator instance.
		Bakinator();

		~Bakinator() = default;

		// ---------------------------------------------------

	public:
		void RegisterOptions(OptionRegistrar& options);

		int Process();

		// - DATA MEMBERS ------------------------------------

	private:
		ArrayList<Path<>> _exports;
		Path<>            _indexPath;
		Path<>            _blobPath;
	};

}} // namespace Eldritch2::Tools
