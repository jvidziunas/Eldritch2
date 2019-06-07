/*==================================================================*\
  IntrusiveConcurrentQueue.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//

//------------------------------------------------------------------//

namespace Eldritch2 { namespace Detail {

	template <class Value, class ElementPolicy>
	ETInlineHint ETForceInlineHint typename IntrusiveConcurrentQueue<Value, ElementPolicy>::Pointer IntrusiveConcurrentQueue<Value, ElementPolicy>::Pop() ETNoexceptHint {
		const Pointer result(Pointer(ElementPolicy::Pop()));
#if ET_DEBUG_BUILD
		if (result) {
			result->mpNext = result;
		}
#endif
		return result;
	}

	// ---------------------------------------------------

	template <class Value, class ElementPolicy>
	template <typename UnaryPredicate>
	ETInlineHint ETForceInlineHint bool IntrusiveConcurrentQueue<Value, ElementPolicy>::PopAndDispose(UnaryPredicate disposer) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, Reference>()) {
		if (const Pointer element = Pointer(ElementPolicy::Pop())) {
			disposer(*element);
			return true;
		}

		return false;
	}

	// ---------------------------------------------------

	template <class Value, class ElementPolicy>
	ETInlineHint ETForceInlineHint void IntrusiveConcurrentQueue<Value, ElementPolicy>::Push(Reference value) ETNoexceptHint {
		ElementPolicy::Push(value);
	}

	// ---------------------------------------------------

	template <class Value, class ElementPolicy>
	template <typename UnaryPredicate>
	ETInlineHint ETForceInlineHint void IntrusiveConcurrentQueue<Value, ElementPolicy>::ClearAndDispose(UnaryPredicate disposer) ETNoexceptHintIf(IsNoThrowCallable<UnaryPredicate, Reference>()) {
		while (IntrusiveConcurrentQueue<Value, ElementPolicy>::PopAndDispose(disposer)) {
		}
	}

	// ---------------------------------------------------

	template <class Value, class ElementPolicy>
	ETInlineHint ETForceInlineHint void IntrusiveConcurrentQueue<Value, ElementPolicy>::Clear() ETNoexceptHint {
		IntrusiveConcurrentQueue<Value, ElementPolicy>::ClearAndDispose([](Reference) ETNoexceptHint {});
	}

}} // namespace Eldritch2::Detail
