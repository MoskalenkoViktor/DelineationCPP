#pragma once
#include "../../../ecg_lead/ecg_lead.h"
#include "data.h"
#include "../../morfology_point.h"


std::vector<Point> q_r_s_processing(const ECGLead& ecg_lead, const WaveDelineation& delineation,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, const std::vector<Point>& left_points,
    int s_zc_id_diff, const std::vector<Point>& right_points)
{
    return {};
}



std::vector<Point> s_r_q_processing(const ECGLead& ecg_lead, const WaveDelineation& delineation,
    const QRSMorphologyData& qrs_morphology_data, int q_zc_id_diff, const std::vector<Point>& left_points,
    int s_zc_id_diff, const std::vector<Point>& right_points)
{
    return {};
}
