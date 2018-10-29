#pragma once
#include "../../morfology_point.h"
#include "../../../ecg_lead/ecg_lead.h"
#include "gamma.h"


void offset_processing(const ECGLead& ecg_lead, WaveDelineation& delineation, const Morphology morphology_data, std::vector<Point> points) {
    size_t offset_index = delineation.offset_index;
    auto offset_value = ecg_lead.filter[offset_index];
    auto offset_sign = WaveSign::NONE;
    auto offset_point = Point(PointName::P_OFFSET, offset_index, offset_value, offset_sign);
    points.push_back(offset_point);

    delineation.offset_index = offset_index;
}