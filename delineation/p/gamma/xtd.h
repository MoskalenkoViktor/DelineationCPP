#pragma once

#include "../../morfology_point.h"
#include "../../../zero_crossing/zero_crossing.h"


void xtd_processing(const ECGLead& ecg_lead, WaveDelineation& delineation, PMorphologyData& morphology_data, std::vector<Point> points) {

    WaveSign p_sign;

    ZeroCrossing zcs = morphology_data.zcs;
    size_t peak_zc_id = morphology_data.peak_zc_id;

    if (peak_zc_id > 0)
        for (size_t zc_id = peak_zc_id-1; zc_id>= 0; zc_id--) {
            size_t p_index = zcs[zc_id].index;
            auto p_value = ecg_lead.filter[p_index];
            if (zcs[zc_id].extremum_sign == ExtremumSign::NEGATIVE)
                p_sign = WaveSign::NEGATIVE;
            else
                p_sign = WaveSign::POSITIVE;
            Point p = Point(PointName::XTD_POINT, p_index, p_value, p_sign);
            points.insert(points.begin(), p);
        }

    if (peak_zc_id < zcs.size() - 1)
        for (size_t zc_id = peak_zc_id + 1; zc_id<zcs.size(); zc_id++) {
            size_t p_index = zcs[zc_id].index;
            auto p_value = ecg_lead.filter[p_index];
            if (zcs[zc_id].extremum_sign == ExtremumSign::NEGATIVE)
                p_sign = WaveSign::NEGATIVE;
            else
                p_sign = WaveSign::POSITIVE;
            Point p = Point(PointName::XTD_POINT, p_index, p_value, p_sign);
            points.push_back(p);
        }
}