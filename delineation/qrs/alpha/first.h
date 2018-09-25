#pragma once
#include <vector>
#include "../../../ecg_lead/ecg_lead.h"

std::vector<int> get_candidates_qrs_zcs_ids(const ECGLead& ecg_lead, const std::vector<ZeroCrossing>& zcs)
{
    double rate = ecg_lead.rate;
    // DUMMY_CONST
    const int window = static_cast<int>(rate * 0.28);

    std::vector<int> candidates_zcs_ids;
    int current_zc_id = 0;
    int next_zc_id = current_zc_id + 1;
    while (next_zc_id < zcs.size())
    {
        while ((zcs[next_zc_id].index - zcs[current_zc_id].index < window) && (next_zc_id < zcs.size() - 1))
        {
            if (zcs[next_zc_id].g_ampl > zcs[current_zc_id].g_ampl)
            {
                current_zc_id = next_zc_id;
                next_zc_id = current_zc_id + 1;
            }
            else
            {
                ++next_zc_id;
            }
        }

        candidates_zcs_ids.push_back(current_zc_id);
        current_zc_id = next_zc_id;
        next_zc_id = current_zc_id + 1;
    }

    return candidates_zcs_ids;
}
