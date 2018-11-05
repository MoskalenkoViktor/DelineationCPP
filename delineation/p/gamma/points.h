#pragma once

#include "vector"


#include "../../morfology_point.h"
#include "../../../ecg_lead/ecg_lead.h"
#include "data.h"

std::vector<Point> points_processing(const ECGLead& ecg_lead, const WaveDelineation& delineation, const PMorphologyData& morphology_data) {

    std::vector<Point> points;

//    // Firstly get P-peak
//    peak_processing(ecg_lead, delineation, morphology_data, points);
//
//    // Get all xtd peaks
//    xtd_processing(ecg_lead, delineation, morphology_data, points);
//
//    // Get onset
//    onset_processing(ecg_lead, delineation, morphology_data, points);
//
//    // Get offset
//    offset_processing(ecg_lead, delineation, morphology_data, points);

    return points;
}


std::vector<Point> points_processing_trivial(const ECGLead& ecg_lead, const WaveDelineation& delineation) {
    WaveSign peak_sign;

    PointName onset_name = PointName::T_ONSET;
    size_t onset_index = delineation.onset_index;
    auto onset_value = ecg_lead.filter[onset_index];
    WaveSign onset_sign = WaveSign::NONE;
    Point onset = Point(onset_name, onset_index, onset_value, onset_sign);

    PointName offset_name = PointName::T_OFFSET;
    size_t offset_index = delineation.offset_index;
    auto offset_value = ecg_lead.filter[offset_index];
    WaveSign offset_sign = WaveSign::NONE;
    Point offset = Point(offset_name, offset_index, offset_value, offset_sign);

    PointName peak_name = PointName::T_PEAK;
    size_t peak_index = delineation.peak_index;
    auto peak_value = ecg_lead.filter[peak_index];

// if we have Line[{x1, y1}, {x2, y2}] and Point {xA, yA}
// first vector v1 = {x2 - x1, y2 - y1}
// second vector v2 = {x2 - xA, y2 - yA}
// cross product xp = v1.x * v2.y - v1.y * v2.x

    auto cross_prod = (offset_index - onset_index) * (offset_value - peak_value) - (offset_value - onset_value) * (offset_index - peak_index);

    if (cross_prod > 0)
        peak_sign = WaveSign::POSITIVE;
    else if (cross_prod < 0)
        peak_sign = WaveSign::NEGATIVE;
    else
        peak_sign = WaveSign::NONE;

    Point peak = Point(peak_name, peak_index, peak_value, peak_sign);

    std::vector<Point> points = {onset, peak, offset};

    return points;
}