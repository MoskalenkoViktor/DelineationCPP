#pragma once
#include "../../../ecg_lead/ecg_lead.h"
#include "data.h"
#include "../../morfology_point.h"


std::vector<Point> processing_default_morphology(const ECGLead& ecg_lead,
    WaveDelineation* delineation, const QRSMorphologyData& morph_data)
{
    std::vector<Point> points;

    int scale_id = morph_data.scale_id;
    const std::vector<ZeroCrossing> zcs = morph_data.zcs[scale_id];
    int peak_zc_id = morph_data.peak_zcs_ids[scale_id];

    size_t onset_index = delineation->onset_index;
    size_t offset_index = delineation->offset_index;

    size_t qrs_onset_index = onset_index;
    double qrs_onset_value = ecg_lead.filter_[qrs_onset_index];
    WaveSign qrs_onset_sign = WaveSign::NONE;
    Point qrs_onset_point(PointName::QRS_ONSET, qrs_onset_index, qrs_onset_value, qrs_onset_sign);
    points.insert(points.begin(), qrs_onset_point);

    size_t r_index = zcs[peak_zc_id].index;
    double r_value = ecg_lead.filter_[r_index];
    WaveSign r_sign = (zcs[peak_zc_id].extremum_sign == ExtremumSign::NEGATIVE) ?
        WaveSign::NEGATIVE : WaveSign::POSITIVE;
    Point r_point(PointName::R, r_index, r_value, r_sign);
    points.push_back(r_point);

    size_t qrs_offset_index = offset_index;
    double qrs_offset_value = ecg_lead.filter_[qrs_offset_index];
    WaveSign qrs_offset_sign = WaveSign::NONE;
    Point qrs_offset_point(PointName::QRS_OFFSET, qrs_offset_index, qrs_offset_value, qrs_offset_sign);
    points.push_back(qrs_offset_point);

    return points;
}