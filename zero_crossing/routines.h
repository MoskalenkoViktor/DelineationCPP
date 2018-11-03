#pragma once;

#include "../params/params.h"
#include "zero_crossing.h"

std::vector<ZeroCrossing> get_zcs_only_indexes(std::vector<double> wdc, size_t begin_index, size_t end_index);
void init_zcs_with_global_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index);
void init_zcs_with_special_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index, int window);

std::vector<ZeroCrossing> get_zcs_with_global_mms(std::vector<double> wdc, size_t begin_index, size_t end_index) {

    std::vector<ZeroCrossing> zcs = get_zcs_only_indexes(wdc, begin_index, end_index);
    init_zcs_with_global_mms(wdc, zcs, begin_index, end_index);

    return zcs;
}


std::vector<ZeroCrossing> get_zcs_with_local_mms(std::vector<double> wdc, size_t begin_index, size_t end_index) {
    std::vector<ZeroCrossing> zcs = get_zcs_only_indexes(wdc, begin_index, end_index);
    init_zcs_with_local_mms(wdc, zcs, begin_index, end_index);

    return zcs;
}


std::vector<ZeroCrossing> get_zcs_with_special_mms(std::vector<double> wdc, size_t begin_index, size_t end_index, int window) {
    std::vector<ZeroCrossing> zcs = get_zcs_only_indexes(wdc, begin_index, end_index);
    init_zcs_with_special_mms(wdc, zcs, begin_index, end_index, window);

    return zcs;
}


std::vector<ZeroCrossing> get_zcs_only_indexes(std::vector<double> wdc, size_t begin_index, size_t end_index) {

    std::vector<ZeroCrossing> zcs;

    size_t signal_index = begin_index;

    while (signal_index < end_index - 2) {
        double current_wdc = wdc[signal_index];
        double next_wdc = wdc[signal_index + 1];

        if (current_wdc * next_wdc < 0) {
            ZeroCrossing zc = ZeroCrossing(signal_index, wdc);
            zcs.push_back(zc);
            signal_index += 1;
        }
        else if (abs(next_wdc) < EPSILON) {
            int zero_period_size = 0;

            while (abs(next_wdc) < EPSILON and signal_index + 1 + zero_period_size < end_index - 2) {
                zero_period_size += 1;
                next_wdc = wdc[signal_index + 1 + zero_period_size];
            }

            if (current_wdc * next_wdc < 0) {
                ZeroCrossing zc = ZeroCrossing(signal_index + 1 + int((zero_period_size - 1) / 2), wdc);
                zcs.push_back(zc);
            }

            signal_index = signal_index + 1 + zero_period_size;
        }
        else
            signal_index += 1;
    }
    return zcs;
}


void init_zcs_with_global_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index) {

    if (zcs) {

        if (zcs.size() == 1) {
            if (zcs[0].index != begin_index)
                zcs[0].init_global_mm_left(begin_index, wdc);

            if (zcs[0].index != end_index)
                zcs[0].init_global_mm_right(end_index, wdc);
        }
        else if (zcs.size() == 2) {
            if (zcs[0].index != begin_index) {
                zcs[0].init_global_mm_left(begin_index, wdc);
                zcs[0].init_global_mm_right(zcs[1].index, wdc);
            }
            if (zcs[-1].index != end_index) {
                zcs[-1].init_global_mm_right(end_index, wdc);
                zcs[-1].init_global_mm_left(zcs[-2].index, wdc);
            }
        }
        else {
            if (zcs[0].index != begin_index) {
                zcs[0].init_global_mm_left(begin_index, wdc);
                zcs[0].init_global_mm_right(zcs[1].index, wdc);
            }

            for (size_t zero_crossing_id = 1; zero_crossing_id < zcs.size()-1; ++zero_crossing_id) {
                zcs[zero_crossing_id].init_global_mm_left(zcs[zero_crossing_id - 1].index, wdc);
                zcs[zero_crossing_id].init_global_mm_right(zcs[zero_crossing_id + 1].index, wdc);
            }

            if (zcs[-1].index != end_index)
                zcs[-1].init_global_mm_right(end_index, wdc);

            zcs[-1].init_global_mm_left(zcs[-2].index, wdc);
        }
    }
}

void init_zcs_with_local_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index) {

    if (zcs) {
        if (zcs.size() == 1) {
            if (zcs[0].index != begin_index)
                zcs[0].init_local_mm_left(begin_index, wdc);

            if (zcs[0].index != end_index)
                zcs[0].init_local_mm_right(end_index, wdc);
        }
        else if (zcs.size() == 2) {
            if (zcs[0].index != begin_index) {
                zcs[0].init_local_mm_left(begin_index, wdc);
                zcs[0].init_local_mm_right(zcs[1].index, wdc);
            }

            if (zcs[-1].index != end_index) {
                zcs[-1].init_local_mm_right(end_index, wdc);
                zcs[-1].init_local_mm_left(zcs[-2].index, wdc);
            }
        }
        else {
            if (zcs[0].index != begin_index) {
                zcs[0].init_local_mm_left(begin_index, wdc);
                zcs[0].init_local_mm_right(zcs[1].index, wdc);
            }

            for (size_t zero_crossing_id = 1; zero_crossing_id < zcs.size()-1; ++zero_crossing_id) {
                zcs[zero_crossing_id].init_local_mm_left(zcs[zero_crossing_id - 1].index, wdc);
                zcs[zero_crossing_id].init_local_mm_right(zcs[zero_crossing_id + 1].index, wdc);
            }

            if (zcs[-1].index != end_index)
                zcs[-1].init_local_mm_right(end_index, wdc);

            zcs[-1].init_local_mm_left(zcs[-2].index, wdc);
        }
    }
}

void init_zcs_with_special_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index, int window) {

    if (zcs) {
        if (zcs.size() == 1) {
            if (zcs[0].index != begin_index)
                zcs[0].init_special_mm_left(std::max(begin_index, zcs[0].index - window), wdc);

            if (zcs[0].index != end_index)
                zcs[0].init_special_mm_right(std::min(end_index, zcs[0].index + window), wdc);
        }
        else if (zcs.size() == 2) {
            if (zcs[0].index != begin_index) {
                zcs[0].init_special_mm_left(std::max(begin_index, zcs[0].index - window), wdc);
                zcs[0].init_special_mm_right(std::min(zcs[1].index, zcs[0].index + window), wdc);
            }

            if (zcs[-1].index != end_index) {
                zcs[-1].init_special_mm_right(std::min(end_index, zcs[-1].index + window), wdc);
                zcs[-1].init_special_mm_left(std::max(zcs[-2].index, zcs[-1].index - window), wdc);
            }
        }
        else {
            if (zcs[0].index != begin_index) {
                zcs[0].init_special_mm_left(std::max(begin_index, zcs[0].index - window), wdc);
                zcs[0].init_special_mm_right(std::min(zcs[1].index, zcs[0].index + window), wdc);
            }
            for (size_t zero_crossing_id = 1; zero_crossing_id < zcs.size()-1; ++zero_crossing_id) {
                zcs[zero_crossing_id].init_special_mm_left(std::max(zcs[zero_crossing_id - 1].index, zcs[zero_crossing_id].index - window), wdc);
                zcs[zero_crossing_id].init_special_mm_right(std::min(zcs[zero_crossing_id + 1].index, zcs[zero_crossing_id].index + window), wdc);
            }

            if (zcs[-1].index != end_index)
                zcs[-1].init_special_mm_right(std::min(end_index, zcs[-1].index + window), wdc);
            zcs[-1].init_special_mm_left(std::max(zcs[-2].index, zcs[-1].index - window), wdc);
        }
    }
}
