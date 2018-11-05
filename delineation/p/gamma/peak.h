#pragma once

#include "../../morfology_point.h"
#include "../../../zero_crossing/zero_crossing.h"
#include "data.h"


void peak_processing(const ECGLead& ecg_lead, WaveDelineation& delineation, PMorphologyData& morphology_data, std::vector<Point> points) {
    int direction = 0;
    WaveSign p_sign;
    std::vector<ZeroCrossing> zcs = morphology_data.zcs;
    size_t peak_zc_id = morphology_data.peak_zc_id;

    ExtremumSign p_zc_sign = morphology_data.t_sign;
    size_t p_index = zcs[peak_zc_id].index;
    auto p_value = ecg_lead.filter_
            [p_index];
    if (p_zc_sign == ExtremumSign::POSITIVE)
        p_sign = WaveSign::POSITIVE;
    else
        p_sign = WaveSign::NEGATIVE;
    Point p_point = Point(PointName::P_PEAK, p_index, p_value, p_sign);
    if (direction < 0)
        points.insert(points.begin(), p_point);
    else
        points.push_back(p_point);

    delineation.peak_index = p_index;
}