#pragma once

#include "wave_delineation.h"
#include "../params/params.h"


class PeakZCsIds {
public:
    PeakZCsIds(size_t left_zc_id, size_t center_zc_id, size_t right_zc_id) : left_zc_id(left_zc_id), center_zc_id(center_zc_id), right_zc_id(right_zc_id) {
    }

    size_t left_zc_id;
    size_t center_zc_id;
    size_t right_zc_id;

    bool is_next_zc_exist(std::vector<ZeroCrossing>& zcs, int zc_id, int window) {
        bool result = false;

        if (zc_id < zcs.size()-1)
            if ((zcs[zc_id+1].index - zcs[zc_id].index) < window)
                result = true;
        return static_cast<bool>(result);
    }


    size_t distance_between_zcs(std::vector<ZeroCrossing>& zcs, size_t zc_id_left, size_t zc_id_right) {
        return zcs[zc_id_right].index - zcs[zc_id_left].index;
    }
};

bool is_prev_zc_exist(std::vector<ZeroCrossing>& zcs, size_t zc_id, double window) {
    bool result = false;

    if (zc_id > 0)
        if ((zcs[zc_id].index - zcs[zc_id - 1].index) < window)
            result = true;
    return result;
}

bool is_next_zc_exist(std::vector<ZeroCrossing>& zcs, size_t zc_id, double window) {
    bool result = false;

    if (zc_id < zcs.size()-1)
        if ((zcs[zc_id+1].index - zcs[zc_id].index) < window)
            result = true;
    return static_cast<bool>(result);
}


size_t distance_between_zcs(std::vector<ZeroCrossing>& zcs, size_t zc_id_left, size_t zc_id_right) {
    return zcs[zc_id_right].index - zcs[zc_id_left].index;
}