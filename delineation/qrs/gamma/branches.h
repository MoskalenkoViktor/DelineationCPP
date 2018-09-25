#pragma once
#include "../../../ecg_lead/ecg_lead.h"
#include "data.h"
#include "../../morfology_point.h"
#include "orders.h"
#include "default.h"


std::vector<Point> borders_processing(const ECGLead& ecg_lead, const WaveDelineation& delineation,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, const std::vector<Point>& left_points,
    int s_zc_id_diff, const std::vector<Point>& right_points, std::vector<int>* branch_id)
{
    std::vector<Point> points;

    if (is_r_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(1);
        points = q_r_s_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_q_r_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(2);
        points = q_r_s_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_r_s_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(3);
        points = s_r_q_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_q_r_s_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(4);
        points = q_r_s_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_extra_zcs_q_r_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(5);
        points = q_r_s_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_p_zcs_q_r_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(6);
        points = q_r_s_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_extra_zcs_q_r_s_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(7);
        points = q_r_s_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_p_zcs_q_r_s_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(8);
        points = q_r_s_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_r_s_extra_zcs_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(9);
        points = s_r_q_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_r_s_t_zcs_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(10);
        points = s_r_q_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_q_r_s_extra_zcs_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(11);
        points = s_r_q_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_q_r_s_t_zcs_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(12);
        points = s_r_q_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_extra_zcs_q_r_s_extra_zcs_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(13);
        points = q_r_s_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_p_zcs_q_r_s_extra_zcs_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(14);
        points = s_r_q_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_extra_zcs_q_r_s_t_zcs_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(15);
        points = q_r_s_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else if (is_p_zcs_q_r_s_t_zcs_in_del(ecg_lead, qrs_morphology_data, q_zc_id_diff, s_zc_id_diff))
    {
        branch_id->push_back(16);
        points = q_r_s_processing(ecg_lead, delineation, qrs_morphology_data, q_zc_id_diff, 
            left_points, s_zc_id_diff, right_points);
    }

    else
    {
        // Default :
        branch_id->push_back(0);
        points = processing_default_morphology(ecg_lead, delineation, qrs_morphology_data);
    }
    
    return points;
}


bool is_r_in_del(const ECGLead& ecg_lead, 
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    return (real_dels_zcs_ids.size() == 1) && (real_r_zc_id_index == 0) &&
        (zcs[r_zc_id].extremum_sign == r_sign);
}


bool is_q_r_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    return (real_dels_zcs_ids.size() == 2) && (real_r_zc_id_index == 1) &&
        (zcs[q_zc_id].extremum_sign == q_sign) && (zcs[r_zc_id].extremum_sign == r_sign);
}


bool is_r_s_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    return (real_dels_zcs_ids.size() == 2) && (real_r_zc_id_index == 0) &&
        (zcs[r_zc_id].extremum_sign == r_sign) && (zcs[s_zc_id].extremum_sign == s_sign);
}


bool is_q_r_s_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    return (real_dels_zcs_ids.size() == 3) && (real_r_zc_id_index == 1) &&
        (zcs[q_zc_id].extremum_sign == q_sign) && (zcs[r_zc_id].extremum_sign == r_sign) &&
        (zcs[s_zc_id].extremum_sign == s_sign);
}


bool is_extra_zcs_q_r_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    double mm_small_left = zcs[peak_zc_id].g_ampl * GAMMA_MM_SMALL_PART_LEFT;
    
    if ((real_dels_zcs_ids.size() > 2) && (real_r_zc_id_index == real_dels_zcs_ids.size() - 1) &&
        (zcs[q_zc_id].extremum_sign == q_sign) && (zcs[r_zc_id].extremum_sign == r_sign))
    {
        if (q_zc_id - 1 >= dels_zcs_ids[0])
        {
            auto mm_left = all_mms[zcs[q_zc_id - 1].l_mms[0].id];
            return std::abs(mm_left.value) < mm_small_left;
        }
    }
    return false;
}


bool is_p_zcs_q_r_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    double mm_small_left = zcs[peak_zc_id].g_ampl * GAMMA_MM_SMALL_PART_LEFT;

    if ((real_dels_zcs_ids.size() > 2) && (real_r_zc_id_index == real_dels_zcs_ids.size() - 1) &&
        (zcs[q_zc_id].extremum_sign == q_sign) && (zcs[r_zc_id].extremum_sign == r_sign))
    {
        if (q_zc_id - 1 >= dels_zcs_ids[0])
        {
            auto mm_left = all_mms[zcs[q_zc_id - 1].l_mms[0].id];
            return std::abs(mm_left.value) > mm_small_left;
        }
    }
    return false;
}


bool is_extra_zcs_q_r_s_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    double mm_small_left = zcs[peak_zc_id].g_ampl * GAMMA_MM_SMALL_PART_LEFT;

    if ((real_dels_zcs_ids.size() > 3) && (real_r_zc_id_index == real_dels_zcs_ids.size() - 2) &&
        (zcs[q_zc_id].extremum_sign == q_sign) && (zcs[r_zc_id].extremum_sign == r_sign) && 
        (zcs[s_zc_id].extremum_sign == s_sign))
    {
        if (q_zc_id - 1 >= dels_zcs_ids[0])
        {
            auto mm_left = all_mms[zcs[q_zc_id - 1].l_mms[0].id];
            return std::abs(mm_left.value) < mm_small_left;
        }
    }
    return false;
}


bool is_p_zcs_q_r_s_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    double mm_small_left = zcs[peak_zc_id].g_ampl * GAMMA_MM_SMALL_PART_LEFT;

    if ((real_dels_zcs_ids.size() > 3) && (real_r_zc_id_index == real_dels_zcs_ids.size() - 2) &&
        (zcs[q_zc_id].extremum_sign == q_sign) && (zcs[r_zc_id].extremum_sign == r_sign) &&
        (zcs[s_zc_id].extremum_sign == s_sign))
    {
        if (q_zc_id - 1 >= dels_zcs_ids[0])
        {
            auto mm_left = all_mms[zcs[q_zc_id - 1].l_mms[0].id];
            return std::abs(mm_left.value) > mm_small_left;
        }
    }
    return false;
}


bool is_r_s_extra_zcs_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    double mm_small_right = zcs[peak_zc_id].g_ampl * GAMMA_MM_SMALL_PART_RIGHT;

    if ((real_dels_zcs_ids.size() > 2) && (real_r_zc_id_index == 0) &&
        (zcs[r_zc_id].extremum_sign == r_sign) && (zcs[s_zc_id].extremum_sign == s_sign))
    {
        if (s_zc_id + 1 < dels_zcs_ids[0] + dels_zcs_ids.size())
        {
            auto mm_right = all_mms[zcs[s_zc_id + 1].r_mms[0].id];
            return std::abs(mm_right.value) < mm_small_right;
        }
    }
    return false;
}


bool is_r_s_t_zcs_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    double mm_small_right = zcs[peak_zc_id].g_ampl * GAMMA_MM_SMALL_PART_RIGHT;

    if ((real_dels_zcs_ids.size() > 2) && (real_r_zc_id_index == 0) &&
        (zcs[r_zc_id].extremum_sign == r_sign) && (zcs[s_zc_id].extremum_sign == s_sign))
    {
        if (s_zc_id + 1 < dels_zcs_ids[0] + dels_zcs_ids.size())
        {
            auto mm_right = all_mms[zcs[s_zc_id + 1].r_mms[0].id];
            return std::abs(mm_right.value) > mm_small_right;
        }
    }
    return false;
}


bool is_q_r_s_extra_zcs_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    return true;
}


bool is_q_r_s_t_zcs_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    return true;
}


bool is_extra_zcs_q_r_s_extra_zcs_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    return true;
}


bool is_p_zcs_q_r_s_extra_zcs_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    return true;
}



bool is_extra_zcs_q_r_s_t_zcs_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    return true;
}


bool is_p_zcs_q_r_s_t_zcs_in_del(const ECGLead& ecg_lead,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, int s_zc_id_diff)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];
    const std::vector<int>& dels_zcs_ids = qrs_morphology_data.dels_zcs_ids[scale_id];
    int peak_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    ExtremumSign q_sign = qrs_morphology_data.q_signs[scale_id];
    ExtremumSign r_sign = qrs_morphology_data.r_signs[scale_id];
    ExtremumSign s_sign = qrs_morphology_data.s_signs[scale_id];

    int r_zc_id = peak_zc_id;
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    auto it = std::find(dels_zcs_ids.begin(), dels_zcs_ids.end(), r_zc_id);
    if (it == dels_zcs_ids.end())
    {
        return false;
    }

    int r_zc_id_index = std::distance(dels_zcs_ids.begin(), it);
    int q_zc_id_index = r_zc_id_index + q_zc_id_diff;
    int s_zc_id_index = r_zc_id_index + s_zc_id_diff;

    std::vector<int> real_dels_zcs_ids(dels_zcs_ids.begin(), dels_zcs_ids.begin() + q_zc_id_index + 1);
    real_dels_zcs_ids.push_back(dels_zcs_ids[r_zc_id_index]);
    real_dels_zcs_ids.insert(real_dels_zcs_ids.end(), dels_zcs_ids.begin() + s_zc_id_index, dels_zcs_ids.end());

    it = std::find(real_dels_zcs_ids.begin(), real_dels_zcs_ids.end(), r_zc_id);
    int real_r_zc_id_index = std::distance(real_dels_zcs_ids.cbegin(), it);
    int real_q_zc_id_index = real_r_zc_id_index - 1;
    int real_s_zc_id_index = real_r_zc_id_index + 1;

    return true;
}
