//
// Created by Blintsov Sergey on 22/10/2018.
//

#pragma once

#include "../../zero_crossing/zero_crossing.h"
#include "routines.h"

std::vector<ZeroCrossing> get_p_zcs(const ECGLead& ecg_lead, size_t qrs_id, int window) {

    int wdc_scale_id = get_p_wdc_scale_id(ecg_lead);
    std::vector<double> wdc = ecg_lead.wdc[wdc_scale_id];

    size_t begin_index = get_p_begin_index(ecg_lead, qrs_id);
    size_t end_index = get_p_end_index(ecg_lead, qrs_id);

    std::vector<ZeroCrossing> zcs = get_zcs_with_special_mms(wdc, begin_index, end_index, window);

    return zcs;
}


