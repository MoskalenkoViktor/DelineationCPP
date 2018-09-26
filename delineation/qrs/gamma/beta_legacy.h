#pragma once
#include "data.h"


std::vector<int> origin_scale_analysis(const ECGLead& ecg_lead, const QRSMorphologyData& morph_data)
{
    // DUMMY_CONST
    const size_t WDC_SCALE_ID = 3;

    // Init data for original wdc scale
    int scale_id_origin = WDC_SCALE_ID;
    const std::vector<double>& wdc_origin = morph_data.wdc[scale_id_origin];
    const std::vector<ZeroCrossing>& zcs_origin = morph_data.zcs[scale_id_origin];
    int peak_zc_id_origin = morph_data.peak_zcs_ids[scale_id_origin];
    std::vector<int> dels_zcs_ids_origin = morph_data.dels_zcs_ids[scale_id_origin];

    // Init zcs, which correspond to begin and end
    int del_begin_zc_id_origin = dels_zcs_ids_origin[0];
    int del_end_zc_id_origin = dels_zcs_ids_origin.back();

    // Defining zcs, which correspond to begin and end:
    // Leave only one first zc on each side
    if ((del_begin_zc_id_origin <= peak_zc_id_origin) && (peak_zc_id_origin < del_end_zc_id_origin))
    {

        int begin_zc_id = peak_zc_id_origin;
        if (begin_zc_id > del_begin_zc_id_origin)
        {
            --begin_zc_id;
        }

        int end_zc_id = peak_zc_id_origin;
        if (end_zc_id < del_end_zc_id_origin)
        {
            ++end_zc_id;
        }

        // Don't forget about shift:
        // del_begin_zc_id_origin is not necessary to be 0
        begin_zc_id -= del_begin_zc_id_origin;
        end_zc_id -= del_begin_zc_id_origin;

        dels_zcs_ids_origin = std::vector<int>(dels_zcs_ids_origin.begin() + begin_zc_id,
            dels_zcs_ids_origin.begin() + end_zc_id + 1);
    }

    return dels_zcs_ids_origin;
}
