#pragma once

#include "../params/params.h"
#include <cmath>
#include "zero_crossing.h"


//void init_zcs_with_global_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index);
//void init_zcs_with_local_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index);
//void init_zcs_with_special_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index, int window);

int get_closest_zc_id(std::vector<ZeroCrossing> zcs, const std::vector<int>& ids_zcs, size_t index) {
    auto id = ids_zcs[index];
    if (id == -1)
        return 0;
    if (id == zcs.size() - 1)
        return static_cast<int>(zcs.size() - 1);
    else
    if (abs(static_cast<int>(zcs[id].index - index)) < abs(static_cast<int>(zcs[id + 1].index - index)))
        return id;
    else
        return id + 1;
}

unsigned long get_closest_zc_id_left(const std::vector<ZeroCrossing>& zcs, std::vector<size_t>& ids_zcs, size_t index) {
    auto id = ids_zcs[index];
    if (id == -1)
        return 0;
    if (id == zcs.size() - 1)
        return zcs.size() - 1;
    else
        return id;
}

unsigned long get_closest_zc_id_right(const std::vector<ZeroCrossing>& zcs, std::vector<size_t>& ids_zcs, size_t index) {
    auto id = ids_zcs[index];
    if (id == -1)
        return 0;
    if (id == zcs.size() - 1)
        return zcs.size() - 1;
    else
        return id+1;
}

std::vector<ZeroCrossing> get_zcs(const std::vector<double>& wdc,
                                  const std::vector<ModulusMaxima>& mms)
{
    std::vector<size_t> indexes;
    for (size_t i = 1; i < wdc.size(); ++i)
    {
        if (wdc[i] * wdc[i - 1] < 0)
        {
            indexes.push_back(i);
        }
    }

    std::vector<ZeroCrossing> zcs;
    std::vector<ModulusMaxima> r_mms;
    int mm_id = 0;

    for (int id = 0; id < indexes.size(); ++id)
    {
        size_t index = indexes[id];

        // Define list of left mms
        std::vector<ModulusMaxima> l_mms;
        if (id > 0)
        {
            l_mms = r_mms;
        }
        else
        {
            while ((mm_id < mms.size()) && (mms[mm_id].index < index))
            {
                l_mms.push_back(mms[mm_id]);
                ++mm_id;
            }
        }
        std::reverse(l_mms.begin(), l_mms.end());

        // Define list of right mms
        size_t r_index = (id < indexes.size() - 1) ? indexes[id + 1] : wdc.size() - 1;
        r_mms.clear();
        while ((mm_id < mms.size()) && (mms[mm_id].index < r_index))
        {
            r_mms.push_back(mms[mm_id]);
            ++mm_id;
        }

        zcs.emplace_back(index, id, l_mms, r_mms);
    }

    return zcs;
}

ZeroCrossing get_left_zc(std::vector<ZeroCrossing> zcs, std::vector<size_t> ids_zcs, size_t index) {
    size_t id = get_closest_zc_id_left(zcs, ids_zcs, index);
    return zcs[id];
}

ZeroCrossing get_right_zc(std::vector<ZeroCrossing> zcs, std::vector<size_t> ids_zcs, size_t index) {
    size_t id = get_closest_zc_id_right(zcs, ids_zcs, index);
    return zcs[id];
}

std::vector<ZeroCrossing> get_zcs_in_window(const std::vector<double>& wdc,
                                            const std::vector<ZeroCrossing>& zcs, const std::vector<int>& ids_zcs,
                                            size_t begin_index, size_t end_index) {
    size_t begin_index_for_zc = begin_index + 1;
    size_t end_index_for_zc = end_index - 1;

    auto begin_index_for_mm = static_cast<int>(begin_index);
    int end_index_for_mm = static_cast<int>(end_index) - 1;

    int begin_id = get_closest_zc_id(zcs, ids_zcs, begin_index_for_zc);
    if (zcs[begin_id].index < begin_index_for_zc)
    {
        ++begin_id;
    }
    int end_id = get_closest_zc_id(zcs, ids_zcs, end_index_for_zc);
    if (zcs[end_id].index >= end_index_for_zc)
    {
        --end_id;
    }

    std::vector<ZeroCrossing> target_zcs(zcs.begin() + begin_id, zcs.begin() + end_id + 1);

    if (!target_zcs.empty())
    {

        ZeroCrossing left_zc = target_zcs[0];
        if (!left_zc.l_mms.empty())
        {
            size_t num_passed = 0;
            while ((num_passed < left_zc.l_mms.size()) &&
                   (left_zc.l_mms[num_passed].index >= begin_index_for_mm))
            {
                ++num_passed;
            }
            if (num_passed > 0)
            {
                std::vector<ModulusMaxima> tmp_mms(left_zc.l_mms.begin(), left_zc.l_mms.begin() + num_passed);
                if (left_zc.l_mms.size() > num_passed)
                {
                    tmp_mms.emplace_back(begin_index_for_mm, left_zc.l_mms[num_passed].id, wdc);
                }
                left_zc.l_mms = tmp_mms;
                left_zc.zc_proc();
            }
            else
            {
                ModulusMaxima first_mm(static_cast<size_t>(begin_index_for_mm), left_zc.l_mms[0].id, wdc);
                left_zc.l_mms = { first_mm };
                left_zc.zc_proc();
            }
        }

        ZeroCrossing& right_zc = target_zcs.back();
        if (!right_zc.r_mms.empty())
        {
            size_t num_passed = 0;
            while ((num_passed < right_zc.r_mms.size()) &&
                   (right_zc.r_mms[num_passed].index <= end_index_for_mm))
            {
                ++num_passed;
            }
            if (num_passed > 0)
            {
                std::vector<ModulusMaxima> tmp_mms(right_zc.r_mms.begin(), right_zc.r_mms.begin() + num_passed);
                if (right_zc.r_mms.size() > num_passed)
                {
                    tmp_mms.emplace_back(end_index_for_mm, right_zc.r_mms[num_passed].id, wdc);
                }
                right_zc.r_mms = tmp_mms;
                right_zc.zc_proc();
            }
            else
            {
                ModulusMaxima last_mm(static_cast<size_t>(end_index_for_mm), right_zc.r_mms.back().id, wdc);
                right_zc.r_mms = { last_mm };
                right_zc.zc_proc();
            }
        }
    }

    return target_zcs;
}


//std::vector<ZeroCrossing> get_zcs_with_global_mms(std::vector<double> wdc, size_t begin_index, size_t end_index) {
//
//    std::vector<ZeroCrossing> zcs = get_zcs_only_indexes(wdc, begin_index, end_index);
//    init_zcs_with_global_mms(wdc, zcs, begin_index, end_index);
//
//    return zcs;
//}
//
//
//std::vector<ZeroCrossing> get_zcs_with_local_mms(std::vector<double> wdc, size_t begin_index, size_t end_index) {
//    std::vector<ZeroCrossing> zcs = get_zcs_only_indexes(wdc, begin_index, end_index);
//    init_zcs_with_local_mms(wdc, zcs, begin_index, end_index);
//
//    return zcs;
//}
//
//
//std::vector<ZeroCrossing> get_zcs_with_special_mms(std::vector<double> wdc, size_t begin_index, size_t end_index, int window) {
//    std::vector<ZeroCrossing> zcs = get_zcs_only_indexes(wdc, begin_index, end_index);
//    init_zcs_with_special_mms(wdc, zcs, begin_index, end_index, window);
//
//    return zcs;
//}


//std::vector<ZeroCrossing> get_zcs_only_indexes(std::vector<double> wdc, size_t begin_index, size_t end_index) {
//
//    std::vector<ZeroCrossing> zcs;
//
//    size_t signal_index = begin_index;
//
//    while (signal_index < end_index - 2) {
//        double current_wdc = wdc[signal_index];
//        double next_wdc = wdc[signal_index + 1];
//
//        if (current_wdc * next_wdc < 0) {
//            ZeroCrossing zc = ZeroCrossing(signal_index, wdc);
//            zcs.push_back(zc);
//            signal_index += 1;
//        }
//        else if (abs(next_wdc) < EPSILON) {
//            int zero_period_size = 0;
//
//            while (abs(next_wdc) < EPSILON and signal_index + 1 + zero_period_size < end_index - 2) {
//                zero_period_size += 1;
//                next_wdc = wdc[signal_index + 1 + zero_period_size];
//            }
//
//            if (current_wdc * next_wdc < 0) {
//                ZeroCrossing zc = ZeroCrossing(signal_index + 1 + int((zero_period_size - 1) / 2), wdc);
//                zcs.push_back(zc);
//            }
//
//            signal_index = signal_index + 1 + zero_period_size;
//        }
//        else
//            signal_index += 1;
//    }
//    return zcs;
//}


//void init_zcs_with_global_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index) {
//
//    if (!zcs.empty()) {
//
//        if (zcs.size() == 1) {
//            if (zcs[0].index != begin_index)
//                zcs[0].init_global_mm_left(begin_index, wdc);
//
//            if (zcs[0].index != end_index)
//                zcs[0].init_global_mm_right(end_index, wdc);
//        }
//        else if (zcs.size() == 2) {
//            if (zcs[0].index != begin_index) {
//                zcs[0].init_global_mm_left(begin_index, wdc);
//                zcs[0].init_global_mm_right(zcs[1].index, wdc);
//            }
//            if (zcs[-1].index != end_index) {
//                zcs[-1].init_global_mm_right(end_index, wdc);
//                zcs[-1].init_global_mm_left(zcs[-2].index, wdc);
//            }
//        }
//        else {
//            if (zcs[0].index != begin_index) {
//                zcs[0].init_global_mm_left(begin_index, wdc);
//                zcs[0].init_global_mm_right(zcs[1].index, wdc);
//            }
//
//            for (size_t zero_crossing_id = 1; zero_crossing_id < zcs.size()-1; ++zero_crossing_id) {
//                zcs[zero_crossing_id].init_global_mm_left(zcs[zero_crossing_id - 1].index, wdc);
//                zcs[zero_crossing_id].init_global_mm_right(zcs[zero_crossing_id + 1].index, wdc);
//            }
//
//            if (zcs[-1].index != end_index)
//                zcs[-1].init_global_mm_right(end_index, wdc);
//
//            zcs[-1].init_global_mm_left(zcs[-2].index, wdc);
//        }
//    }
//}
//
//void init_zcs_with_local_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index) {
//
//    if (!zcs.empty()) {
//        if (zcs.size() == 1) {
//            if (zcs[0].index != begin_index)
//                zcs[0].init_local_mm_left(begin_index, wdc);
//
//            if (zcs[0].index != end_index)
//                zcs[0].init_local_mm_right(end_index, wdc);
//        }
//        else if (zcs.size() == 2) {
//            if (zcs[0].index != begin_index) {
//                zcs[0].init_local_mm_left(begin_index, wdc);
//                zcs[0].init_local_mm_right(zcs[1].index, wdc);
//            }
//
//            if (zcs[-1].index != end_index) {
//                zcs[-1].init_local_mm_right(end_index, wdc);
//                zcs[-1].init_local_mm_left(zcs[-2].index, wdc);
//            }
//        }
//        else {
//            if (zcs[0].index != begin_index) {
//                zcs[0].init_local_mm_left(begin_index, wdc);
//                zcs[0].init_local_mm_right(zcs[1].index, wdc);
//            }
//
//            for (size_t zero_crossing_id = 1; zero_crossing_id < zcs.size()-1; ++zero_crossing_id) {
//                zcs[zero_crossing_id].init_local_mm_left(zcs[zero_crossing_id - 1].index, wdc);
//                zcs[zero_crossing_id].init_local_mm_right(zcs[zero_crossing_id + 1].index, wdc);
//            }
//
//            if (zcs[-1].index != end_index)
//                zcs[-1].init_local_mm_right(end_index, wdc);
//
//            zcs[-1].init_local_mm_left(zcs[-2].index, wdc);
//        }
//    }
//}
//
//void init_zcs_with_special_mms(std::vector<double> wdc, std::vector<ZeroCrossing> zcs, size_t begin_index, size_t end_index, int window) {
//
//    if (!zcs.empty()) {
//        if (zcs.size() == 1) {
//            if (zcs[0].index != begin_index)
//                zcs[0].init_special_mm_left(std::max(begin_index, zcs[0].index - window), wdc);
//
//            if (zcs[0].index != end_index)
//                zcs[0].init_special_mm_right(std::min(end_index, zcs[0].index + window), wdc);
//        }
//        else if (zcs.size() == 2) {
//            if (zcs[0].index != begin_index) {
//                zcs[0].init_special_mm_left(std::max(begin_index, zcs[0].index - window), wdc);
//                zcs[0].init_special_mm_right(std::min(zcs[1].index, zcs[0].index + window), wdc);
//            }
//
//            if (zcs[-1].index != end_index) {
//                zcs[-1].init_special_mm_right(std::min(end_index, zcs[-1].index + window), wdc);
//                zcs[-1].init_special_mm_left(std::max(zcs[-2].index, zcs[-1].index - window), wdc);
//            }
//        }
//        else {
//            if (zcs[0].index != begin_index) {
//                zcs[0].init_special_mm_left(std::max(begin_index, zcs[0].index - window), wdc);
//                zcs[0].init_special_mm_right(std::min(zcs[1].index, zcs[0].index + window), wdc);
//            }
//            for (size_t zero_crossing_id = 1; zero_crossing_id < zcs.size()-1; ++zero_crossing_id) {
//                zcs[zero_crossing_id].init_special_mm_left(std::max(zcs[zero_crossing_id - 1].index, zcs[zero_crossing_id].index - window), wdc);
//                zcs[zero_crossing_id].init_special_mm_right(std::min(zcs[zero_crossing_id + 1].index, zcs[zero_crossing_id].index + window), wdc);
//            }
//
//            if (zcs[-1].index != end_index)
//                zcs[-1].init_special_mm_right(std::min(end_index, zcs[-1].index + window), wdc);
//            zcs[-1].init_special_mm_left(std::max(zcs[-2].index, zcs[-1].index - window), wdc);
//        }
//    }
//}
