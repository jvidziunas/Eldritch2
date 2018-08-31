/*==================================================================*\
  IdentifierPool.inl
  ------------------------------------------------------------------
  Purpose:


  ------------------------------------------------------------------
  ©2010-2015 Eldritch Entertainment, LLC.
\*==================================================================*/
#pragma once

//==================================================================//
// INCLUDES
//==================================================================//
#include <Common/Algorithms.hpp>
#include <Common/Assert.hpp>
//------------------------------------------------------------------//

namespace Eldritch2 {
namespace Detail {

	template <typename Value>
	ETInlineHint ETForceInlineHint ValueRange<Value>::ValueRange(Value begin, Value end) ETNoexceptHint : begin(begin),
																										  end(end) {}

	// ---------------------------------------------------

	template <typename Value>
	ETInlineHint ETForceInlineHint typename ValueRange<Value>::ValueType ValueRange<Value>::GetSize() const ETNoexceptHint {
		return end - begin;
	}

	// ---------------------------------------------------

	template <typename Value>
	ETInlineHint ETForceInlineHint bool ValueRange<Value>::Contains(Value value) const ETNoexceptHint {
		return (begin <= value) & (value < end);
	}

	// ---------------------------------------------------

	template <typename Value>
	ETInlineHint ETForceInlineHint bool ValueRange<Value>::IsEmpty() const ETNoexceptHint {
		return begin == end;
	}

	// ---------------------------------------------------

	template <typename Value>
	ETInlineHint ETForceInlineHint bool IsContiguous(const ValueRange<Value>& lhs, const ValueRange<Value>& rhs) ETNoexceptHint {
		return lhs.end == rhs.begin;
	}

	// ---------------------------------------------------

	template <typename Value>
	ETInlineHint ETForceInlineHint ValueRange<Value> Union(const ValueRange<Value>& lhs, const ValueRange<Value>& rhs) ETNoexceptHint {
		return { lhs.begin, rhs.end };
	}

	// ---------------------------------------------------

	template <typename Value>
	ETInlineHint ETForceInlineHint bool operator<(const ValueRange<Value>& lhs, const ValueRange<Value>& rhs) ETNoexceptHint {
		return lhs.end <= rhs.begin;
	}

} // namespace Detail

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool(const AllocatorType& allocator, std::initializer_list<RangeType> ranges) :
	IdentifierPool(allocator) {
	Assign(ranges);
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool(const AllocatorType& allocator, ValueType begin, ValueType end) :
	IdentifierPool(allocator) {
	Assign(begin, end);
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint IdentifierPool<Identifier, Allocator>::IdentifierPool(const AllocatorType& allocator) :
	_ranges(allocator) {}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint Pair<typename IdentifierPool<Identifier, Allocator>::ValueType, bool> IdentifierPool<Identifier, Allocator>::Allocate() {
	return Allocate(Identifier(1));
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint Pair<typename IdentifierPool<Identifier, Allocator>::ValueType, bool> IdentifierPool<Identifier, Allocator>::Allocate(ValueType count) {
	const auto candidate(FindIf(_ranges.Begin(), _ranges.End(), [count](const RangeType& range) { return range.GetSize() >= count; }));
	if (candidate == _ranges.End()) {
		return { 0, false };
	}

	const auto id(candidate->begin);
	candidate->begin += count;

	if (candidate->IsEmpty() && ((candidate + 1) != _ranges.End())) {
		//	If current range is full and there is another one, that will become the new current range.
		_ranges.Erase(candidate);
	}

	return { id, true };
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint void IdentifierPool<Identifier, Allocator>::Deallocate(ValueType id) {
	return this->Deallocate({ id, id + ValueType(1) });
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint void IdentifierPool<Identifier, Allocator>::Deallocate(RangeType range) {
	const auto candidate(LowerBound(_ranges.Begin(), _ranges.End(), range, LessThan<RangeType>()));
	if (candidate == _ranges.End()) {
		_ranges.Append(range);
		return;
	}

	if (IsContiguous(range, *candidate)) {
		candidate->end = range.end;
		if (candidate + 1 != _ranges.End() && IsContiguous(*candidate, candidate[1])) {
			*candidate = Union(*candidate, candidate[1]);
			_ranges.Erase(candidate + 1);
		}

		return;
	}

	if (IsContiguous(*candidate, range)) {
		candidate->begin = range.begin;
		if (candidate != _ranges.Begin() && IsContiguous(candidate[-1], *candidate)) {
			candidate[-1] = Union(candidate[-1], *candidate);
			_ranges.Erase(candidate);
		}

		return;
	}

	_ranges.Insert(candidate, range);
#if 0
	//	Binary search of the range list
		AvailableRangeCollection::SizeType	i0(0);
		AvailableRangeCollection::SizeType	i1(_availableRanges.Size() - 1);

		for (;;) {
			const AvailableRangeCollection::SizeType	i((i0 + i1) / 2);

			if (releasedRange.firstIdentifier < _availableRanges[i].begin) {
			//	Before current range, check if neighboring
				if (releasedRange.lastIdentifier >= _availableRanges[i].begin) {
				//	Overlaps a range of free IDs, thus (at least partially) invalid IDs
					ETRuntimeAssert(releasedRange.lastIdentifier == _availableRanges[i].begin);

				//	Neighbor id, check if neighboring previous range too
					if (i > i0 && releasedRange.firstIdentifier - 1 == _availableRanges[i - 1].end) {
					//	Merge with previous range
						_availableRanges[i - 1].end = _availableRanges[i].end;
						_availableRanges.Erase(_availableRanges.Begin() + i);
					}
					else {
				 //	Just grow range
						_availableRanges[i].begin = releasedRange.firstIdentifier;
					}

					break;
				}
				else {
			 //	Non-neighbor id
					if (i != i0) {
					//	Cull upper half of list
						i1 = i - 1;
					}
					else {
				 //	Found our position in the list, insert the deleted range here
						_availableRanges.Insert(_availableRanges.Begin() + i, AvailableRange(releasedRange.firstIdentifier, releasedRange.lastIdentifier - 1));
						break;
					}
				}
			}
			else if (releasedRange.firstIdentifier > _availableRanges[i].end) {
		 //	After current range, check if neighboring
				if (releasedRange.firstIdentifier - 1 == _availableRanges[i].end) {
				//	Neighbor id, check if neighboring next range too
					if (i < i1 && releasedRange.lastIdentifier == _availableRanges[i + 1].begin) {
					//	Merge with next range
						_availableRanges[i].end = _availableRanges[i + 1].end;
						_availableRanges.Erase(_availableRanges.Begin() + i + 1);
					}
					else {
					// Just grow range
						_availableRanges[i].end += (releasedRange.lastIdentifier - releasedRange.firstIdentifier);
					}

					break;
				}
				else {
					// Non-neighbor id
					if (i != i1) {
					//	Cull bottom half of list
						i0 = i + 1;
					}
					else {
						// Found our position in the list, insert the deleted range here
						_availableRanges.Insert(_availableRanges.Begin() + i + 1, AvailableRange(releasedRange.firstIdentifier, releasedRange.lastIdentifier - 1));
						break;
					}
				}
			}
			else {
				// Inside a free block, not a valid ID
				ETRuntimeAssert(false);
			}
		}
#endif
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint void IdentifierPool<Identifier, Allocator>::Clear() {
	_ranges.Clear();
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint IdentifierPool<Identifier, Allocator>& IdentifierPool<Identifier, Allocator>::Assign(ValueType begin, ValueType end) {
	_ranges.Clear();
	_ranges.EmplaceBack(begin, end);
	return *this;
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint IdentifierPool<Identifier, Allocator>& IdentifierPool<Identifier, Allocator>::Assign(std::initializer_list<RangeType> ranges) {
	_ranges.Clear();
	_ranges.Assign(ranges);
	return *this;
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint bool IdentifierPool<Identifier, Allocator>::IsEmpty() const ETNoexceptHint {
	return _ranges.IsEmpty();
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint typename IdentifierPool<Identifier, Allocator>::ValueType IdentifierPool<Identifier, Allocator>::GetLargestRange() const ETNoexceptHint {
	return Reduce(_ranges.Begin(), _ranges.End(), [](const RangeType& range, typename RangeList::SizeType longest) ETNoexceptHint {
		return Max(range.GetSize(), longest);
	});
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint typename const IdentifierPool<Identifier, Allocator>::AllocatorType& IdentifierPool<Identifier, Allocator>::GetAllocator() const ETNoexceptHint {
	return _ranges.GetAllocator();
}

// ---------------------------------------------------

template <typename Identifier, class Allocator>
ETInlineHint ETForceInlineHint void Swap(IdentifierPool<Identifier, Allocator>& lhs, IdentifierPool<Identifier, Allocator>& rhs) {
	Swap(lhs._ranges, rhs._ranges);
}

} // namespace Eldritch2
