#pragma once
#include "../../wave_delineation.h"
#include "../../../ecg_lead/ecg_lead.h"
#include "../delineation.h"
#include "../../../threshold_crossings/routines.h"
#include "../../../modulus_maxima/modulus_maxima.h"
#include "../../../params/params.h"
#include <cmath>

/*
������� ������ ������ QRS.

������� ���������:
    ecg_lead - ��������� ������ ��������� ������� ���.
    delineation - ��������� ������, ����������� ���������� � ������� ���������������� ���������.
    qrs_zc_id - ������ ����������� ���� ��������������� �������-�������������� ��� �������� ��������� QRS.
    qrs_zcs - ������ ����������� ���� ��������������� �������-��������������.
    qrs_zc - ����������� ���� ��������������� �������-�������������� ��� �������� ��������� QRS.
    mms - ������ ����������� �������������� �������-�������������.
    onset_mm_id - ������ ����������, � �������� ���������� ����� ������ QRS.
*/


std::vector<ModulusMaxima> get_qrs_onset_mms(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc);
int get_qrs_onset_mm_id(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc,
    const std::vector<ModulusMaxima>& mms, int onset_mm_id);
int get_complex_mm_id(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc,
    const std::vector<ModulusMaxima>& mms, int onset_mm_id);


void define_qrs_onset_index(const ECGLead& ecg_lead,
    WaveDelineation* delineation, const ZeroCrossing& qrs_zc)
{
    size_t wdc_scale_id = get_qrs_wdc_scale_id(ecg_lead);
    const std::vector<double>& wdc = ecg_lead.wdc[wdc_scale_id];
    double rate = ecg_lead.rate;
    size_t window = static_cast<size_t>(BETA_ONSET_WINDOW * rate);
    const std::vector<ModulusMaxima>& mms = ecg_lead.mms[wdc_scale_id];

    ZeroCrossing zc = qrs_zc;
    auto onset_mms = get_qrs_onset_mms(ecg_lead, zc);

    int onset_mm_id = get_qrs_onset_mm_id(ecg_lead, zc, onset_mms, 0);
    int complex_mm_id = get_complex_mm_id(ecg_lead, zc, onset_mms, onset_mm_id);

    if (onset_mm_id != complex_mm_id)
    {
        onset_mm_id = get_qrs_onset_mm_id(ecg_lead, zc, onset_mms, complex_mm_id);
    }

    double threshold = onset_mms[onset_mm_id].value * BETA_ONSET_THRESHOLD;

    ModulusMaxima first_mm = onset_mms[onset_mm_id];
    ModulusMaxima next_mm = mms[first_mm.id - 1];

    if (!next_mm.correctness)
    {
        ModulusMaxima candidate_mm = mms[next_mm.id - 1];

        if ((std::abs(candidate_mm.value) > BETA_ONSET_MM_HIGH_LIM * abs(first_mm.value)) &&
            (std::abs(static_cast<int>(first_mm.index) - static_cast<int>(candidate_mm.index)) <
                static_cast<int>(BETA_ONSET_MM_WINDOW * rate)))
        {
            first_mm = candidate_mm;
            next_mm = mms[first_mm.id - 1];
        }
        else
        {
            size_t onset_index_candidate_1 = next_mm.index;
            size_t onset_index_candidate_2 = find_left_thc_index(
                wdc, first_mm.index, static_cast<size_t>(static_cast<int>(zc.g_l_mm->index) - window), threshold);
            find_left_thc_index(wdc, first_mm.index, zc.g_l_mm->index - window, threshold);
            size_t onset_index = std::max(onset_index_candidate_1, onset_index_candidate_2);
            delineation->onset_index = onset_index;
            return;
        }
    }

    size_t right_zc_index = find_right_thc_index(wdc, first_mm.index, static_cast<int>(zc.index), 0.0);
    size_t left_zc_index = find_left_thc_index(wdc, first_mm.index, static_cast<int>(next_mm.index), 0.0);

    // Compromise
    size_t compromise_window = static_cast<size_t>(BETA_ONSET_COMPROMISE_WINDOW * rate);
    double compromise_mm_lim = BETA_ONSET_COMPROMISE_MM_LIM *
        std::min(std::abs(zc.g_l_mm->value), std::abs(zc.g_r_mm.value));

    size_t onset_index = 0;
    if (((right_zc_index - left_zc_index) > compromise_window) &&
        (std::abs(first_mm.value) < compromise_mm_lim))
    {
        onset_index = first_mm.index;
    }
    else
    {
        onset_index = find_left_thc_index(wdc, first_mm.index, static_cast<int>(next_mm.index), threshold);
    }

    delineation->onset_index = onset_index;
}


std::vector<ModulusMaxima> get_qrs_onset_mms(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc)
{
    size_t wdc_scale_id = get_qrs_wdc_scale_id(ecg_lead);
    double rate = ecg_lead.rate;
    size_t window = static_cast<size_t>(BETA_ONSET_WINDOW * rate);
    const std::vector<ModulusMaxima>& mms = ecg_lead.mms[wdc_scale_id];

    std::vector<ModulusMaxima> onset_mms = { mms[qrs_zc.g_l_mm->id] };
    size_t mm_id = qrs_zc.g_l_mm->id - 1;
    while ((mm_id > 0) && (qrs_zc.g_l_mm->index - mms[mm_id].index <= window))
    {
        onset_mms.push_back(mms[mm_id]);
        --mm_id;
    }

    return onset_mms;
}


int get_qrs_onset_mm_id(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc,
    const std::vector<ModulusMaxima>& mms, int onset_mm_id)
{
    double rate = ecg_lead.rate;
    size_t window = static_cast<size_t>(BETA_ONSET_WINDOW * rate);

    double mm_val = std::max(std::abs(qrs_zc.g_l_mm->value), std::abs(qrs_zc.g_r_mm.value)) *
        BETA_ONSET_MM_LOW_LIM;

    size_t start_index = qrs_zc.g_l_mm->index;
    int qrs_onset_mm_id = onset_mm_id;
    if (onset_mm_id + 1 < mms.size())
    {
        for (int mm_id = onset_mm_id + 1; mm_id < mms.size(); ++mm_id)
        {
            if (mms[mm_id].correctness)
            {
                double shift_percentage = (static_cast<double>(start_index) - mms[mm_id].index) / window;
                double amplitude_part = 1.0 - 
                    std::pow(shift_percentage, BETA_ONSET_AMPL_DECR_POW) * BETA_ONSET_AMPL_DECR_VAL;

                if (mm_val < abs(mms[mm_id].value) * amplitude_part)
                {
                    qrs_onset_mm_id = mm_id;
                }
                else
                {
                    break;
                }
            }
        }
    }

    return qrs_onset_mm_id;
}


int get_complex_mm_id(const ECGLead& ecg_lead, const ZeroCrossing& qrs_zc,
    const std::vector<ModulusMaxima>& mms, int onset_mm_id)
{
    double mm_val = std::max(std::abs(qrs_zc.g_l_mm->value), std::abs(qrs_zc.g_r_mm.value)) *
        BETA_ONSET_MM_LOW_LIM;

    if (onset_mm_id != mms.size() - 1)
    {
        int begin_mm_id = onset_mm_id + 1;
        int candidate_mm_id = onset_mm_id;
        for (int mm_id = begin_mm_id; mm_id < mms.size(); ++mm_id)
        {
            if ((std::abs(mms[mm_id].value) > BETA_COMPLEX_ZC_AMPL * std::abs(qrs_zc.g_ampl)) ||
                (std::abs(mms[mm_id].value) > BETA_COMPLEX_MM_VAL * abs(qrs_zc.g_l_mm->value)) ||
                (std::abs(mms[mm_id].value) > BETA_COMPLEX_MM_VAL * abs(qrs_zc.g_r_mm.value)))
            {
                candidate_mm_id = mm_id;
                break;
            }
        }

        if (candidate_mm_id > onset_mm_id)
        {
            bool is_new_candidate_correct = true;
            if (candidate_mm_id > begin_mm_id)
            {
                for (int temp_mm_id = begin_mm_id; temp_mm_id < candidate_mm_id; ++temp_mm_id)
                {
                    if (std::abs(mms[temp_mm_id].value) > mm_val)
                    {
                        is_new_candidate_correct = false;
                        break;
                    }
                }
            }

            if (is_new_candidate_correct)
            {
                onset_mm_id = candidate_mm_id;
            }
        }
    }

    return onset_mm_id;
}
