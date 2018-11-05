#pragma once
#include "../../../ecg_lead/ecg_lead.h"
#include "../delineation.h"
#include "../../../zero_crossing/zero_crossing.h"
#include "first.h"
#include "second.h"
#include "third.h"
#include "../../../zero_crossing/routines.h"


std::vector<ZeroCrossing> alpha_processing(const ECGLead& ecg_lead, size_t begin_index, size_t end_index)
{
    size_t wdc_scale_id = get_qrs_wdc_scale_id(ecg_lead);
    size_t wdc_scale_id_aux = get_qrs_aux_wdc_scale_id(ecg_lead);
    auto wdc = ecg_lead.wdc[wdc_scale_id];
    auto wdc_aux = ecg_lead.wdc[wdc_scale_id_aux];

    // Main scale
    auto zcs = get_zcs_in_window(ecg_lead.wdc[wdc_scale_id], ecg_lead.zcs[wdc_scale_id],
        ecg_lead.ids_zcs[wdc_scale_id], begin_index, end_index);
    std::vector<int> zcs_ids = get_candidates_qrs_zcs_ids(ecg_lead, zcs);
    zcs_ids = get_confirmed_qrs_zcs_ids(ecg_lead, zcs, zcs_ids, wdc);

    // Aux scale
    zcs_ids = get_confirmed_qrs_zcs_ids(ecg_lead, zcs, zcs_ids, wdc_aux);

    // Restore missed qrs
    zcs_ids = get_additional_qrs_zcs_ids(ecg_lead, zcs, zcs_ids, wdc, wdc_aux);

    // Exclude borders
    zcs_ids = std::vector<int>(zcs_ids.begin() + 1, zcs_ids.end() - 1);

    std::vector<ZeroCrossing> qrs_zcs;
    for (int zc_id : zcs_ids)
    {
        qrs_zcs.push_back(zcs[zc_id]);
    }

    return qrs_zcs;
}
