#pragma once
#include "../UH_EFM_Ah6jApi.h"

namespace AH6J_UH_EFM {
// Supply this fork's typed parameters to UH_EFM component constructors, then
// combine them into an owned flight-model handle. Destroy via the public SDK.
UhEfmAh6jModel* createAircraft();
}
