#include "peaks_zcs_ids.h"

PeakZCsIds::PeakZCsIds(int left_zc_id, int center_zc_id, int right_zc_id) {
    this->left_zc_id = left_zc_id;
    this->center_zc_id = center_zc_id;
    this->right_zc_id = right_zc_id;
}

bool PeakZCsIds::is_next_zc_exist(const std::vector<ZeroCrossing> &zcs, int zc_id, int window) {
    bool result = false;

    if (zc_id < zcs.size() - 1)
        if ((zcs[zc_id + 1].index - zcs[zc_id].index) < window)
            result = true;

    return result;
}

bool PeakZCsIds::is_prev_zc_exist(const std::vector<ZeroCrossing> &zcs, int zc_id, int window) {
    bool result = false;

    if (zc_id > 0)
        if ((zcs[zc_id].index - zcs[zc_id - 1].index) < window)
            result = true;

    return result;
}

size_t PeakZCsIds::distance_between_zcs(const std::vector<ZeroCrossing> &zcs, int zc_id_left, int zc_id_right) {
    return zcs[zc_id_right].index - zcs[zc_id_left].index;
}