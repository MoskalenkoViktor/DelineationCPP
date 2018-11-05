//
// Created by Blintsov Sergey on 22/10/2018.
//

#pragma once

#include "../../params/params.h"
#include "../../ecg_lead/ecg_lead.h"
#include "../../modulus_maxima/routines.h"
#include "../../zero_crossing/routines.h"
#include "routines.h"

std::vector<ZeroCrossing> get_p_zcs(const ECGLead& ecg_lead, size_t qrs_id, int window) {

    int wdc_scale_id = get_p_wdc_scale_id(ecg_lead);

    size_t begin_index = get_p_begin_index(ecg_lead, qrs_id);
    size_t end_index = get_p_end_index(ecg_lead, qrs_id);

    std::vector<ZeroCrossing> zcs = get_zcs_in_window(ecg_lead.wdc[wdc_scale_id], ecg_lead.zcs[wdc_scale_id], ecg_lead.ids_zcs[wdc_scale_id], begin_index, end_index);
    if len(zcs) > 0:
    if len(zcs) == 1:
    zcs[0].special(ecg_lead.wdc[wdc_scale_id], max(begin_index, zcs[0].index - window), min(end_index, zcs[0].index + window))
    elif len(zcs) == 2:
    zcs[0].special(ecg_lead.wdc[wdc_scale_id], max(begin_index, zcs[0].index - window), min(zcs[1].index, zcs[0].index + window))
    zcs[-1].special(ecg_lead.wdc[wdc_scale_id], max(zcs[-2].index, zcs[-1].index - window), min(end_index, zcs[-1].index + window))
    else:
    zcs[0].special(ecg_lead.wdc[wdc_scale_id], max(begin_index, zcs[0].index - window), min(zcs[1].index, zcs[0].index + window))

    for zc_id in range(1, len(zcs) - 1):
    zcs[zc_id].special(ecg_lead.wdc[wdc_scale_id], max(zcs[zc_id - 1].index, zcs[zc_id].index - window), min(zcs[zc_id + 1].index, zcs[zc_id].index + window))

    zcs[-1].special(ecg_lead.wdc[wdc_scale_id], max(zcs[-2].index, zcs[-1].index - window), min(end_index, zcs[-1].index + window))

    return zcs
}


