#include "zero_crossing.h"
#include "routines.h"
#include <algorithm>
#include <cmath>

ZeroCrossing::ZeroCrossing(size_t index, int id,
                           const std::vector<ModulusMaxima>& l_mms, const std::vector<ModulusMaxima>& r_mms) :
        index(index), id(id), l_mms(l_mms), r_mms(r_mms), extremum_sign(ExtremumSign::UNKNOWN)
{
    zc_proc();
}


void ZeroCrossing::zc_proc()
{
    if (l_mms.size() > 0)
    {
        auto it = std::max_element(l_mms.begin(), l_mms.end(),
                                   [](const ModulusMaxima& first, const ModulusMaxima& second) {
                                       return std::abs(first.value) < std::abs(second.value);
                                   });
        g_l_mm = std::make_shared<ModulusMaxima>(*it);
        l_l_mm = std::make_shared<ModulusMaxima>(l_mms[0]);
    }
    if (r_mms.size() > 0)
    {
        auto it = std::max_element(r_mms.begin(), r_mms.end(),
                                   [](const ModulusMaxima& first, const ModulusMaxima& second) {
                                       return std::abs(first.value) < std::abs(second.value);
                                   });
        g_r_mm = std::make_shared<ModulusMaxima>(*it);
        l_r_mm = std::make_shared<ModulusMaxima>(r_mms[0]);
    }

    if (g_l_mm && g_r_mm)
    {
        g_ampl = std::abs(g_l_mm->value) + std::abs(g_r_mm->value);
        if ((g_l_mm->value < 0) && (g_r_mm->value > 0))
        {
            extremum_sign = ExtremumSign::POSITIVE;
        }
        else
        {
            extremum_sign = ExtremumSign::NEGATIVE;
        }
    }
    if (l_l_mm && l_r_mm)
    {
        l_ampl = std::abs(l_l_mm->value) + std::abs(l_r_mm->value);
    }

//    g_l_mm = l_mms[np.argmax([abs(mm.value) for mm in self.l_mms])] if len(self.l_mms) > 0 else None
//        self.g_r_mm = self.r_mms[np.argmax([abs(mm.value) for mm in self.r_mms])] if len(self.r_mms) > 0 else None
//        if self.g_l_mm is not None and self.g_r_mm is not None:
//    self.g_ampl = abs(self.g_l_mm.value) + abs(self.g_r_mm.value)
//
//    if self.g_l_mm.value < 0 and self.g_r_mm.value > 0:
//    self.extremum_sign = ExtremumSign.positive
//    else:
//    self.extremum_sign = ExtremumSign.negative
//
//    self.l_l_mm = self.l_mms[0] if len(self.l_mms) > 0 else None
//        self.l_r_mm = self.r_mms[0] if len(self.r_mms) > 0 else None
//        if self.l_l_mm is not None and self.l_r_mm is not None:
//    self.l_ampl = abs(self.l_l_mm.value) + abs(self.l_r_mm.value)
}


std::vector<ZeroCrossing> get_zcs_in_window(const std::vector<double>& wdc,
                                            const std::vector<ZeroCrossing>& zcs, const std::vector<int>& ids_zcs,
                                            size_t begin_index, size_t end_index)
{
    int begin_index_for_zc = static_cast<int>(begin_index) + 1;
    int end_index_for_zc = static_cast<int>(end_index) - 1;

    int begin_index_for_mm = static_cast<int>(begin_index);
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

    if (target_zcs.size() > 0)
    {

        ZeroCrossing& left_zc = target_zcs[0];
        if (left_zc.l_mms.size() > 0)
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
                ModulusMaxima first_mm(begin_index_for_mm, left_zc.l_mms[0].id, wdc);
                left_zc.l_mms = { first_mm };
                left_zc.zc_proc();
            }
        }

        ZeroCrossing& right_zc = target_zcs.back();
        if (right_zc.r_mms.size() > 0)
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
                ModulusMaxima last_mm(end_index_for_mm, right_zc.r_mms.back().id, wdc);
                right_zc.r_mms = { last_mm };
                right_zc.zc_proc();
            }
        }
    }

    return target_zcs;
}


int get_closest_zc_id(const std::vector<ZeroCrossing>& zcs,
                      const std::vector<int>& ids_zcs, size_t index)
{
    int id = ids_zcs[index];
    if (id == -1)
    {
        return 0;
    }
    if (id == zcs.size() - 1)
    {
        return static_cast<int>(zcs.size()) - 1;
    }
    else
    {
        int first = static_cast<int>(zcs[id].index - index);
        int second = static_cast<int>(zcs[id + 1].index - index);
        if (std::abs(first) < std::abs(second))
        {
            return id;
        }
        else
        {
            return id + 1;
        }
    }
}
