#pragma once
#include "../../../ecg_lead/ecg_lead.h"


std::vector<int> get_additional_qrs_zcs_ids(const ECGLead& ecg_lead, const std::vector<ZeroCrossing>& zcs,
    std::vector<int> zcs_ids, const std::vector<double>& wdc, const std::vector<double>& wdc_aux)
{
    // 





    const double ALPHA_SKIP = 3.0;

    std::vector<int> zcs_diffs;
    for (size_t zc_id = 0; zc_id < zcs_ids.size() - 1; ++zc_id)
    {
        zcs_diffs.push_back(static_cast<int>(zcs[zcs_ids[zc_id + 1]].index - zcs[zcs_ids[zc_id]].index));
    }
    double mean_zcs_diff = std::accumulate(zcs_diffs.begin(), zcs_diffs.end(), 0.0) / zcs_diffs.size();

    for (size_t zc_id = 0; zc_id < zcs_ids.size() - 1; ++zc_id)
    {
        if ((zcs[zcs_ids[zc_id + 1]].index - zcs[zcs_ids[zc_id]].index) > (ALPHA_SKIP * mean_zcs_diff))
        {
            std::vector<ZeroCrossing> sub_zcs(zcs.begin() + zcs_ids[zc_id], zcs.begin() + zcs_ids[zc_id + 1]);
            auto zcs_ids_add = get_candidates_qrs_zcs_ids(ecg_lead, sub_zcs);

            for (size_t zc_id_add = 0; zc_id_add < zcs_ids_add.size(); ++zc_id_add)
            {
                zcs_ids_add[zc_id_add] = zcs_ids_add[zc_id_add] + zcs_ids[zc_id];
            }

            if (zcs_ids_add.size() > 0)
            {
                zcs_ids_add = get_confirmed_qrs_zcs_ids(ecg_lead, zcs, zcs_ids_add, wdc);
                zcs_ids_add = get_confirmed_qrs_zcs_ids(ecg_lead, zcs, zcs_ids_add, wdc_aux);
                if (zcs_ids_add.size() > 0)
                {
                    zcs_ids.insert(zcs_ids.end(), zcs_ids_add.begin(), zcs_ids_add.end());
                }
            }
        }
    }

    std::sort(zcs_ids.begin(), zcs_ids.end());

    return zcs_ids;
}
