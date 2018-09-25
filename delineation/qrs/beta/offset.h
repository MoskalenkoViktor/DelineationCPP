#pragma once
#include "../../wave_delineation.h"
#include "../../../ecg_lead/ecg_lead.h"
#include "../delineation.h"


// DUMMY_CONST
static const double BETA_OFFSET_WINDOW = 0.15;
// DUMMY_CONST
static const double BETA_OFFSET_MM_LOW_LIM = 0.075;
// DUMMY_CONST
static const double BETA_OFFSET_THRESHOLD = 0.05;

std::vector<ModulusMaxima> get_qrs_offset_mms(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc);
int get_qrs_offset_mm_id(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc,
    const std::vector<ModulusMaxima>& mms, int offset_mm_id);
int get_complex_offset_mm_id(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc,
    const std::vector<ModulusMaxima>& mms, int offset_mm_id);


void define_qrs_offset_index(const ECGLead& ecg_lead,
    WaveDelineation* delineation, const ZeroCrossing& qrs_zc)
{
    size_t wdc_scale_id = get_qrs_wdc_scale_id(ecg_lead);
    const std::vector<double>& wdc = ecg_lead.wdc[wdc_scale_id];
    double rate = ecg_lead.rate;
    size_t window = static_cast<size_t>(BETA_OFFSET_WINDOW * rate);
    const std::vector<ModulusMaxima>& mms = ecg_lead.mms[wdc_scale_id];

    ZeroCrossing zc = qrs_zc;
    std::vector<ModulusMaxima> offset_mms = get_qrs_offset_mms(ecg_lead, zc);

    int offset_mm_id = get_qrs_offset_mm_id(ecg_lead, zc, offset_mms, 0);
    int offset_mm_id_wide_morphology = get_complex_offset_mm_id(
        ecg_lead, zc, offset_mms, offset_mm_id);

    if (offset_mm_id != offset_mm_id_wide_morphology)
    {
        offset_mm_id = get_qrs_offset_mm_id(ecg_lead, zc, offset_mms, offset_mm_id_wide_morphology);
    }

    double threshold = offset_mms[offset_mm_id].value * BETA_OFFSET_THRESHOLD;

    const ModulusMaxima& first_mm = offset_mms[offset_mm_id];
    const ModulusMaxima& next_mm = mms[first_mm.id + 1];

    size_t offset_index = 0;
    if (!next_mm.correctness)
    {
        if (next_mm.index < zc.g_r_mm->index + window)
        {
            size_t offset_index_candidate_1 = next_mm.index;
            size_t offset_index_candidate_2 = 
                find_right_thc_index(wdc, first_mm.index, 
                    static_cast<int>(zc.g_r_mm->index) + static_cast<int>(window), threshold);
            offset_index = std::min(offset_index_candidate_1, offset_index_candidate_2);
        }
        else
        {
            offset_index = find_right_thc_index(wdc, first_mm.index, 
                static_cast<int>(zc.g_r_mm->index) + static_cast<int>(window), threshold);
        }
    }
    else
    {
        offset_index = find_right_thc_index(wdc, first_mm.index, 
            static_cast<int>(next_mm.index), threshold);
    }

    delineation->offset_index = offset_index;
}


int get_qrs_offset_mm_id(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc,
    const std::vector<ModulusMaxima>& mms, int offset_mm_id)
{
    double threshold = std::max(std::abs(qrs_zc.g_l_mm->value), std::abs(qrs_zc.g_r_mm->value)) *
        BETA_OFFSET_MM_LOW_LIM;

    int qrs_offset_mm_id = offset_mm_id;
    if (offset_mm_id + 1 < mms.size())
    {
        for (int mm_id = offset_mm_id + 1; mm_id < mms.size(); ++mm_id)
        {
            if (mms[mm_id].correctness)
            {
                if (std::abs(mms[mm_id].value) > threshold)
                {
                    qrs_offset_mm_id = mm_id;
                }
                else
                {
                    break;
                }
            }
        }
    }

    return qrs_offset_mm_id;
}


std::vector<ModulusMaxima> get_qrs_offset_mms(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc)
{
    size_t wdc_scale_id = get_qrs_wdc_scale_id(ecg_lead);
    double rate = ecg_lead.rate;
    size_t window = static_cast<size_t>(BETA_OFFSET_WINDOW * rate);
    const std::vector<ModulusMaxima>& mms = ecg_lead.mms[wdc_scale_id];

    int mm_id = qrs_zc.g_r_mm->id + 1;
    std::vector<ModulusMaxima> offset_mms = { *qrs_zc.g_r_mm };
    while ((mm_id < mms.size()) && (mms[mm_id].index - qrs_zc.g_r_mm->index <= window))
    {
        offset_mms.push_back(mms[mm_id]);
        ++mm_id;
    }

    return offset_mms;
}


int get_complex_offset_mm_id(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc,
    const std::vector<ModulusMaxima>& mms, int offset_mm_id)
{
    double threshold = std::max(std::abs(qrs_zc.g_l_mm->value), std::abs(qrs_zc.g_r_mm->value)) *
        BETA_OFFSET_MM_LOW_LIM;

    if (offset_mm_id != mms.size() - 1)
    {
        int begin_mm_id = offset_mm_id + 1;
        int candidate_mm_id = offset_mm_id;
        for (int mm_id = begin_mm_id; mm_id < mms.size(); ++mm_id)
        {
            if ((std::abs(mms[mm_id].value) > BETA_COMPLEX_ZC_AMPL * std::abs(qrs_zc.g_ampl)) ||
                (std::abs(mms[mm_id].value) > BETA_COMPLEX_MM_VAL * abs(qrs_zc.g_l_mm->value)) ||
                (std::abs(mms[mm_id].value) > BETA_COMPLEX_MM_VAL * abs(qrs_zc.g_r_mm->value)))
            {
                candidate_mm_id = mm_id;
                break;
            }
        }

        if (candidate_mm_id > offset_mm_id)
        {

            bool is_new_candidate_correct = true;

            if (candidate_mm_id > begin_mm_id)
            {
                for (int temp_mm_id = begin_mm_id; temp_mm_id < candidate_mm_id; ++temp_mm_id)
                {
                    if (std::abs(mms[temp_mm_id].value) > threshold)
                    {
                        is_new_candidate_correct = false;
                        break;
                    }
                }
            }

            if (is_new_candidate_correct)
            {
                offset_mm_id = candidate_mm_id;
            }
        }
    }

    return offset_mm_id;
}
