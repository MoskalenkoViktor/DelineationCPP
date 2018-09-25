#pragma once
#include "../../../ecg_lead/ecg_lead.h"
#include "../../wave_delineation.h"
#include "../../../zero_crossing/zero_crossing.h"
#include "data.h"


class QRSMorphologyData
{
public:
    QRSMorphologyData(const ECGLead& ecg_lead, 
        const WaveDelineation& delineation, int target_scale_id);

    int window_left = 0;
    int window_right = 0;
    size_t begin_index = 0;
    size_t end_index = 0;
    size_t num_scales = 0;
    int scale_id;
    const std::vector<std::vector<double>>& wdc;
    std::vector<std::vector<ZeroCrossing>> zcs;
    std::vector<std::vector<int>> dels_zcs_ids;
    std::vector<int> peak_zcs_ids;
    int allowed_length_diff = 0;
    std::vector<ExtremumSign> q_signs;
    std::vector<ExtremumSign> r_signs;
    std::vector<ExtremumSign> s_signs;
};
