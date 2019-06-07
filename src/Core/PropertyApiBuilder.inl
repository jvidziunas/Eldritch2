/*==================================================================*\
  PropertyApiBuilder.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2016 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Core {

	ETConstexpr PropertyDatabase& PropertyApiBuilder::GetProperties() ETNoexceptHint {
		return _properties;
	}

	// ---------------------------------------------------

	template <typename T>
	ETConstexpr ETPureFunctionHint PropertyDatabase::Setter PropertyApiBuilder::MakeSetter(T& variable) {
		return[&](StringSpan value) ETNoexceptHint{ 
			value.Consume(variable);
		};
	}

}} // namespace Eldritch2::Core
