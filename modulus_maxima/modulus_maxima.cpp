#include "modulus_maxima.h"
#include <cmath>
#include <algorithm>


ModulusMaxima::ModulusMaxima(size_t index, int id, const std::vector<double>& wdc)
{
    this->index = index;
    this->id = id;
    this->value = wdc[index];

    if (index == 0)
    {
        correctness = ((wdc[index] > wdc[index + 1]) && (wdc[index] > 0)) ||
            ((wdc[index] < wdc[index + 1]) && (wdc[index] < 0));
    }
    else if (index == wdc.size() - 1)
    {
        correctness = ((wdc[index] > wdc[index - 1]) && (wdc[index] > 0))
            || ((wdc[index] < wdc[index - 1]) && (wdc[index] < 0));
    }
    else
    {
        correctness = ((wdc[index] > wdc[index + 1]) && (wdc[index] > wdc[index - 1]) && (wdc[index] > 0)) ||
            ((wdc[index] < wdc[index + 1]) && (wdc[index] < wdc[index - 1]) && (wdc[index] < 0));
    }
}


std::vector<ModulusMaxima> get_mms(const std::vector<double>& wdc)
{
    std::vector<ModulusMaxima> mms;
    int id = 0;
    for (size_t i = 1; i < wdc.size() - 1; ++i)
    {
        if (((wdc[i] < wdc[i - 1]) && (wdc[i] < wdc[i + 1])) ||
            ((wdc[i] > wdc[i - 1]) && (wdc[i] > wdc[i + 1])))
        {
            mms.emplace_back(i, id++, wdc);
        }
    }
    return mms;
}


ModulusMaxima get_left_mm(const std::vector<ModulusMaxima>& mms,
    const std::vector<int>& ids_mms, size_t index)
{
    return mms[get_closest_mm_id_left(mms, ids_mms, index)];
}


ModulusMaxima get_right_mm(const std::vector<ModulusMaxima>& mms,
    const std::vector<int>& ids_mms, size_t index)
{
    return mms[get_closest_mm_id_right(mms, ids_mms, index)];
}


std::vector<ModulusMaxima> get_lr_mms_in(const ECGLead& ecg_lead, 
    size_t scale_id, int left, int right)
{
    std::vector<ModulusMaxima> mms;
    for (const ModulusMaxima& mm : ecg_lead.mms[scale_id])
    {
        if ((left <= mm.index) && (mm.index < right))
        {
            mms.push_back(mm);
        }
    }
    return mms;
}


std::vector<ModulusMaxima> get_rl_mms_in(const ECGLead& ecg_lead, 
    size_t scale_id, int left, int right)
{
    std::vector<ModulusMaxima> mms = get_lr_mms_in(ecg_lead, scale_id, left, right);
    std::reverse(mms.begin(), mms.end());
    return mms;
}


std::vector<int> get_correct_mms_ids(const std::vector<ModulusMaxima>& mms)
{
    std::vector<int> correct_mms_ids;

    for (int mm_id = 0; mm_id < mms.size(); ++mm_id)
    {
        if (mms[mm_id].correctness)
        {
            correct_mms_ids.push_back(mm_id);
        }
    }

    return correct_mms_ids;
}


std::vector<int> get_incorrect_mms_ids(const std::vector<ModulusMaxima>& mms)
{
    std::vector<int> incorrect_mms_ids;

    for (int mm_id = 0; mm_id < mms.size(); ++mm_id)
    {
        if (!mms[mm_id].correctness)
        {
            incorrect_mms_ids.push_back(mm_id);
        }
    }

    return incorrect_mms_ids;
}


size_t get_closest_mm_id_left(const std::vector<ModulusMaxima>& mms,
    const std::vector<int>& ids_mms, size_t index)
{
    size_t id = ids_mms[index];
    if (id == -1)
    {
        return 0;
    }
    return id;
}


size_t get_closest_mm_id_right(const std::vector<ModulusMaxima>& mms,
    const std::vector<int>& ids_mms, size_t index)
{
    size_t id = ids_mms[index];
    if (id == -1)
    {
        return 0;
    }
    if (id == mms.size() - 1)
    {
        return mms.size() - 1;
    }
    else
    {
        return id + 1;
    }
}
