#pragma once

#include "../../wave_delineation.h"
#include "../../../zero_crossing/zero_crossing.h"
#include "../../../zero_crossing/routines.h"
#include "../delineation.h"
#include "../routines.h"
#include "../../../params/params.h"


class PMorphologyData {
public:
    PMorphologyData(const ECGLead& ecg_lead,
        const WaveDelineation& delineation, int target_scale_id);

    int correct = -1;
    ExtremumSign t_sign;
    size_t begin_index = 0;
    size_t end_index = 0;
    int min_dist = 0;
    size_t length = 0;
    int current_dist = 0;
    int scale_id;
    const std::vector<double>* wdc;
    std::vector<std::vector<ZeroCrossing>> zcs;
    std::vector<size_t> dels_zcs_ids;
    std::vector<int> peak_zcs_ids;
    size_t peak_zc_id = 0;
};
