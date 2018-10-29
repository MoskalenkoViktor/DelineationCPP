#pragma once

#include "../../wave_delineation.h"
#include "../../../zero_crossing/zero_crossing.h"
#include "../../../zero_crossing/routines.h"
#include "../delineation.h"
#include "../routines.h"
#include "../../../params/params.h"
#include "data.h"


PMorphologyData::PMorphologyData(const ECGLead& ecg_lead,
                                     const WaveDelineation& delineation, int target_scale_id) : wdc(ecg_lead.wdc) {
    double rate = ecg_lead.rate;

    size_t onset_index = delineation.onset_index;
    size_t peak_index = delineation.peak_index;
    size_t offset_index = delineation.offset_index;

    size_t current_length = offset_index - onset_index;

    size_t begin_index = onset_index;
    size_t end_index = offset_index;

    WaveDelineation wdc = wdc_all_scales[target_scale_id];
    ZeroCrossing zcs = get_zcs_with_global_mms(wdc, begin_index, end_index);

    if (zcs.size() > 0) {
        dels_zcs_ids = [];
        peak_zc_id = 0;
        min_dist = wdc.size();
        for(size_t zc_id = 0; zc_id<zcs.size()-1; zc_id++) {
            dels_zcs_ids.push_back(zc_id);
            current_dist = abs(zcs[zc_id].index - peak_index);
            if (current_dist < min_dist) {
                min_dist = current_dist;
                peak_zc_id = zc_id;
            }
        }
        if (zcs[peak_zc_id].extremum_sign == 1)
            t_sign = 1;
        else
            t_sign = -1;

        this->begin_index = begin_index;
        this->end_index = end_index;
        this->length = current_length;
        this->scale_id = target_scale_id;
        this->wdc = wdc;
        this->zcs = zcs;
        this->dels_zcs_ids = dels_zcs_ids;
        this->peak_zc_id = peak_zc_id;
        this->t_sign = t_sign;
        this->correct = 1;
    }
    else
        this->correct = 0;
}
