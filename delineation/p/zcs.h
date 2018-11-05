//
// Created by Blintsov Sergey on 22/10/2018.
//

#pragma once

#include "../../params/params.h"
#include "../../ecg_lead/ecg_lead.h"
#include "../../modulus_maxima/routines.h"
#include "../../zero_crossing/routines.h"
#include "../../zero_crossing/zero_crossing.h"
#include "routines.h"

std::vector<ZeroCrossing> get_p_zcs(ECGLead& ecg_lead, size_t qrs_id, int window) {

    int wdc_scale_id = get_p_wdc_scale_id(ecg_lead);

    size_t begin_index = get_p_begin_index(ecg_lead, qrs_id);
    size_t end_index = get_p_end_index(ecg_lead, qrs_id);

    std::vector<ZeroCrossing> zcs = get_zcs_in_window(ecg_lead.wdc[wdc_scale_id], ecg_lead.zcs[wdc_scale_id], ecg_lead.ids_zcs[wdc_scale_id], begin_index, end_index);

    if (zcs.size() > 0)
        if (zcs.size() == 1)
            zcs[0].special(ecg_lead.wdc[wdc_scale_id], static_cast<int>(std::max(begin_index, zcs[0].index - window)), static_cast<int>(std::min(end_index, zcs[0].index + window)));
        else
            if (zcs.size() == 2) {
                zcs[0].special(ecg_lead.wdc[wdc_scale_id], static_cast<int>(std::max(begin_index, zcs[0].index - window)),
                               static_cast<int>(std::min(zcs[1].index, zcs[0].index + window)));
                zcs[-1].special(ecg_lead.wdc[wdc_scale_id], static_cast<int>(std::max(zcs[-2].index, zcs[-1].index - window)),
                                static_cast<int>(std::min(end_index, zcs[-1].index + window)));
            }
            else {
                zcs[0].special(ecg_lead.wdc[wdc_scale_id],
                               static_cast<int>(std::max(begin_index, zcs[0].index - window)),
                               static_cast<int>(std::min(zcs[1].index, zcs[0].index + window)));

                for (size_t zc_id = 1; zc_id < zcs.size()-1; ++zc_id)
                    zcs[zc_id].special(ecg_lead.wdc[wdc_scale_id], static_cast<int>(std::max(zcs[zc_id - 1].index, zcs[zc_id].index - window)),
                                       static_cast<int>(std::min(zcs[zc_id + 1].index, zcs[zc_id].index + window)));

                zcs[-1].special(ecg_lead.wdc[wdc_scale_id], static_cast<int>(std::max(zcs[-2].index, zcs[-1].index - window)),
                                static_cast<int>(std::min(end_index, zcs[-1].index + window)));
            }
    return zcs;
}


