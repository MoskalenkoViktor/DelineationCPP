#pragma once

#include "../params/params.h"
#include "wave_delineation.h"
#include "../zero_crossing/zero_crossing.h"

class PeakZCsIds {
public:
    PeakZCsIds(int left_zc_id, int center_zc_id, int right_zc_id);

    bool is_prev_zc_exist(const std::vector<ZeroCrossing>& zcs, int zc_id, int window);
    bool is_next_zc_exist(const std::vector<ZeroCrossing>& zcs, int zc_id, int window);
    size_t distance_between_zcs(const std::vector<ZeroCrossing>& zcs, int zc_id_left, int zc_id_right);

    int left_zc_id;
    int center_zc_id;
    int right_zc_id;
};