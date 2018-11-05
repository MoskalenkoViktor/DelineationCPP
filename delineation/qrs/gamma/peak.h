#pragma once
#include "orders.h"


void r_processing(int r_zc_id, const ECGLead& ecg_lead, WaveDelineation* delineation,
    const QRSMorphologyData& qrs_morphology_data, std::vector<Point>* points, int direction)
{
    int scale_id = qrs_morphology_data.scale_id;
    const std::vector<ZeroCrossing>& zcs = qrs_morphology_data.zcs[scale_id];

    ExtremumSign r_zc_sign = qrs_morphology_data.r_signs[scale_id];

    size_t r_index = zcs[r_zc_id].index;
    double r_value = ecg_lead.filter_[r_index];
    WaveSign r_sign = (r_zc_sign == ExtremumSign::POSITIVE) ?
        WaveSign::POSITIVE : WaveSign::NEGATIVE;

    Point r_point(PointName::R, r_index, r_value, r_sign);
    if (direction < 0)
    {
        points->insert(points->begin(), r_point);
    }
    else
    {
        points->push_back(r_point);
    }

    delineation->peak_index = r_index;
}
