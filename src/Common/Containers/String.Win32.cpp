/*==================================================================*\
  String.Win32.cpp
  ------------------------------------------------------------------
  Purpose:
  

  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Containers/String.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace {

	Path<> UserDocuments;
	Path<> AppDataLocal;
	Path<> AppDataShared;
	Path<> Packages;
	Path<> DownloadedPackages;
	Path<> WorkingDirectory;
	Path<> Logs;

}

} // namespace Eldritch2::
