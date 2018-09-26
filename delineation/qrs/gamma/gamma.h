#pragma once
#include "data.h"
#include "left.h"
#include "right.h"
#include "branches.h"


void get_qrs_morphology(const ECGLead& ecg_lead, int del_id,
    WaveDelineation* delineation)
{
    int scale_id = 0;

    QRSMorphologyData qrs_morphology_data(ecg_lead, *delineation, scale_id);
    int r_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];

    bool is_left_complex;
    int first_zc_id; 
    std::vector<Point> left_points;
    std::tie(is_left_complex, first_zc_id, left_points) = 
        left_qrs_morphology(ecg_lead, *delineation, qrs_morphology_data);

    bool is_right_complex;
    int last_zc_id;
    std::vector<Point> right_points;
    std::tie(is_right_complex, last_zc_id, right_points) =
        right_qrs_morphology(ecg_lead, *delineation, qrs_morphology_data);

    int q_zc_id_diff = is_left_complex ? first_zc_id - r_zc_id : -1;

    int s_zc_id_diff = is_right_complex ? last_zc_id - r_zc_id : 1;

    std::vector<int> branch_id;

    if (is_left_complex && is_right_complex)
    {
        branch_id.push_back(1);
        borders_processing(ecg_lead, delineation, qrs_morphology_data,
            q_zc_id_diff, left_points,
            s_zc_id_diff, right_points,
            &branch_id);
    }
}
