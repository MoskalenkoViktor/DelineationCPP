//
// Created by Blintsov Sergey on 22/10/2018.
//

#pragma once

#include <cmath>
#include "../../../params/params.h"
#include "../../wave_delineation.h"
#include "../../../ecg_lead/ecg_lead.h"


void check_flexure_p(const Peak_ZCs_Ids* triplet, const ECGLead& ecg_lead, size_t qrs_id, const std::vector<ZeroCrossing>* zcs, WaveDelineation delineation) {

    int zc_flexure_id = get_p_flexure_zc_id(ecg_lead, qrs_id, zcs, triplet->center_zc_id);

    if (zc_flexure_id != -1) {
        int peak_zc_id = zc_flexure_id;
        double left_peak_zc_id = zc_flexure_id - 1;
        double right_peak_zc_id = zc_flexure_id + 1;

        triplet->left_zc_id = left_peak_zc_id;
        triplet->center_zc_id = peak_zc_id;
        triplet->right_zc_id = right_peak_zc_id;

        delineation.specification = WaveSpecification::FLEXURE;
        delineation.peak_index = zcs[peak_zc_id].index;
    }
}
int get_p_flexure_zc_id(const ECGLead& ecg_lead, size_t qrs_id, std::vector<ZeroCrossing>& zcs, size_t peak_zc_id) {

    std::vector<WaveDelineation> qrs_dels = ecg_lead.qrs_dels;

    int flexure_zc_id = 0;

    size_t rr = qrs_dels[qrs_id].peak_index - qrs_dels[qrs_id - 1].peak_index;

    for (int zc_id = 1; zc_id<zcs.size()-1; zc_id++) {
        if (std::abs(zcs[zc_id - 1].index - zcs[zc_id].index) < float(ALPHA_FLEX_SHIFT) * rr \
 && abs(zcs[zc_id + 1].index - zcs[zc_id].index) < float(ALPHA_FLEX_SHIFT) * rr \
 && zcs[zc_id].mm_amplitude < float(ALPHA_FLEX_AMPL_NGBR) * zcs[zc_id - 1].mm_amplitude \
 && zcs[zc_id].mm_amplitude < float(ALPHA_FLEX_AMPL_NGBR) * zcs[zc_id + 1].mm_amplitude\
 && zcs[zc_id - 1].mm_amplitude > float(ALPHA_FLEX_AMPL_OLD_ZC) * zcs[peak_zc_id].mm_amplitude\
 && zcs[zc_id + 1].mm_amplitude > float(ALPHA_FLEX_AMPL_OLD_ZC) * zcs[peak_zc_id].mm_amplitude)
            flexure_zc_id = zc_id;
    }

    return flexure_zc_id;
}