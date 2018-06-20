/*==================================================================*\
  PackageDatabase.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2018 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 {
	namespace Assets {

		ETInlineHint ETPureFunctionHint bool PackageDatabase::PackageEqual::operator()(const Package& package, const Utf8Char* name) const {
			return StringsEqual(package.GetPath(), name);
		}

	// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint bool PackageDatabase::PackageEqual::operator()(const Package& package0, const Package& package1) const {
			return (*this)(package0, package1.GetPath());
		}

	// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint size_t PackageDatabase::PackageHash::operator()(const Package& package, size_t seed) const {
			return (*this)(package.GetPath(), seed);
		}

	// ---------------------------------------------------

		ETInlineHint ETPureFunctionHint size_t PackageDatabase::PackageHash::operator()(const Utf8Char* name, size_t seed) const {
			return HashMemory(name, StringLength(name), seed);
		}

	}	// namespace Assets
}	// namespace Eldritch2