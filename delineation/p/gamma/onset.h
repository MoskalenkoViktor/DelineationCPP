#pragma once

#include "../../morfology_point.h"
#include "../../../ecg_lead/ecg_lead.h"


void onset_processing(const ECGLead& ecg_lead, WaveDelineation& delineation, const Morphology morphology_data, std::vector<Point> points) {
    size_t onset_index = delineation.onset_index;
    auto onset_value = ecg_lead.filter[onset_index];
    auto onset_sign = WaveSign::NONE;
    Point onset_point = Point(PointName::P_ONSET, onset_index, onset_value, onset_sign);
    points.insert(points.begin(), onset_point);

    delineation.onset_index = onset_index;
}