#pragma once
#include "../../../ecg_lead/ecg_lead.h"
#include "data.h"
#include "../../morfology_point.h"
#include "onset.h"
#include "peak.h"
#include "offset.h"


std::vector<Point> q_r_s_processing(const ECGLead& ecg_lead, WaveDelineation* delineation,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, const std::vector<Point>& left_points,
    int s_zc_id_diff, const std::vector<Point>& right_points)
{

    std::vector<Point> points;

    int scale_id = qrs_morphology_data.scale_id;

    int r_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    if (left_points.size() > 0)
    {
        points.insert(points.begin(), left_points.begin(), left_points.end());
    }
    onset_processing(q_zc_id, ecg_lead, delineation, qrs_morphology_data, &points, -1);
    QRSMorphologyData qrs_morph_data(ecg_lead, *delineation, scale_id);

    r_zc_id = qrs_morph_data.peak_zcs_ids[scale_id];
    q_zc_id = r_zc_id + q_zc_id_diff;
    s_zc_id = r_zc_id + s_zc_id_diff;

    r_processing(r_zc_id, ecg_lead, delineation, qrs_morph_data, &points, 1);
    QRSMorphologyData qrs_morph_data(ecg_lead, *delineation, scale_id);

    r_zc_id = qrs_morph_data.peak_zcs_ids[scale_id];
    q_zc_id = r_zc_id + q_zc_id_diff;
    s_zc_id = r_zc_id + s_zc_id_diff;

    if (right_points.size() > 0)
    {
        points.insert(points.end(), right_points.begin(), right_points.end());
    }
    offset_processing(s_zc_id, ecg_lead, delineation, qrs_morph_data, &points, 1);
    QRSMorphologyData qrs_morph_data(ecg_lead, *delineation, scale_id);

    return points;
}



std::vector<Point> s_r_q_processing(const ECGLead& ecg_lead, WaveDelineation* delineation,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, const std::vector<Point>& left_points,
    int s_zc_id_diff, const std::vector<Point>& right_points)
{
    std::vector<Point> points;

    int scale_id = qrs_morphology_data.scale_id;

    int r_zc_id = qrs_morphology_data.peak_zcs_ids[scale_id];
    int q_zc_id = r_zc_id + q_zc_id_diff;
    int s_zc_id = r_zc_id + s_zc_id_diff;

    if (right_points.size() > 0)
    {
        points.insert(points.end(), right_points.begin(), right_points.end());
    }
    offset_processing(s_zc_id, ecg_lead, delineation, qrs_morphology_data, &points, 1);
    QRSMorphologyData qrs_morph_data(ecg_lead, *delineation, scale_id);

    r_zc_id = qrs_morph_data.peak_zcs_ids[scale_id];
    q_zc_id = r_zc_id + q_zc_id_diff;
    s_zc_id = r_zc_id + s_zc_id_diff;

    r_processing(r_zc_id, ecg_lead, delineation, qrs_morph_data, &points, -1);
    QRSMorphologyData qrs_morph_data(ecg_lead, *delineation, scale_id);

    r_zc_id = qrs_morph_data.peak_zcs_ids[scale_id];
    q_zc_id = r_zc_id + q_zc_id_diff;
    s_zc_id = r_zc_id + s_zc_id_diff;

    if (left_points.size() > 0)
    {
        points.insert(points.begin(), left_points.begin(), left_points.end());
    }
    onset_processing(q_zc_id, ecg_lead, delineation, qrs_morph_data, &points, -1);
    QRSMorphologyData qrs_morph_data(ecg_lead, *delineation, scale_id);

    return points;
}
