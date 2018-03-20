struct Frustum {
};

struct VisibilityCell {
};

static bool FrustumContainsCell( varying Frustum& frustum, varying VisibilityCell& cell ) {
}

export void CullCells( uniform Frustum frusta[], uniform size_t frustumCount, uniform VisibilityCell visibilityCells[], uniform size_t visibilityCellCount ) {
	foreach_tiled( frustum = 0 ... frustumCount, cell = 0 ... visibilityCellCount ) {
		if( FrustumContainsCell( frusta[frustum], visibilityCells[cell] ) ) {
		}
	}
}