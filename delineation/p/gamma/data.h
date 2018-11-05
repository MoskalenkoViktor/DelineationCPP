#pragma once

#include "../../../ecg_lead/ecg_lead.h"

class PMorphologyData {
public:
    PMorphologyData(const ECGLead& ecg_lead,
                    const WaveDelineation& delineation, int target_scale_id);

    int correct = 0;

    ExtremumSign t_sign;

    size_t begin_index0;
    size_t end_index;
    size_t length;
    size_t peak_zc_id = 0;

    int min_dist = 0;
    int current_dist = 0;
    int scale_id;

    std::vector<double> wdc;
    std::vector<ZeroCrossing> zcs;
    std::vector<size_t> dels_zcs_ids;
};