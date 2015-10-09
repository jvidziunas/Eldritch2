/*==================================================================*\
  ConfigurablePODVariable.cpp
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2013 Eldritch Entertainment, LLC.
\*==================================================================*/


//==================================================================//
// INCLUDES
//==================================================================//
#include <Configuration/ConfigurablePODVariable.hpp>
//------------------------------------------------------------------//

using namespace ::Eldritch2::Configuration;
using namespace ::Eldritch2;

namespace Eldritch2 {
namespace Configuration {

	template class ConfigurablePODVariable<bool>;
	template class ConfigurablePODVariable<char>;
	template class ConfigurablePODVariable<signed char>;
	template class ConfigurablePODVariable<unsigned char>;
	template class ConfigurablePODVariable<short>;
	template class ConfigurablePODVariable<unsigned short>;
	template class ConfigurablePODVariable<int>;
	template class ConfigurablePODVariable<unsigned int>;
	template class ConfigurablePODVariable<long>;
	template class ConfigurablePODVariable<unsigned long>;
	template class ConfigurablePODVariable<float>;
	template class ConfigurablePODVariable<double>;

}	// namespace Configuration
}	// namespace Eldritch2