#pragma once
#include "orders.h"
#include "../../../params/params.h"


void offset_processing(int last_zc_id, const ECGLead& ecg_lead, WaveDelineation* delineation,
    const QRSMorphologyData& morph_data, std::vector<Point>* points, int direction)
{
    double rate = ecg_lead.rate;

    // Init necessary data
    int scale_id = morph_data.scale_id;
    int peak_zc_id = morph_data.peak_zcs_ids[scale_id];
    const std::vector<double>& wdc = morph_data.wdc[scale_id];
    const std::vector<ZeroCrossing>& zcs = morph_data.zcs[scale_id];
    size_t end_index = morph_data.end_index;
    size_t offset_index_beta = delineation->offset_index;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];

    // Init offset index
    size_t qrs_offset_index = offset_index_beta;

    if (last_zc_id < zcs.size())
    {

        const ZeroCrossing& last_zc = zcs[last_zc_id];

        // Begin of allowed interval
        int right_lim = static_cast<int>(last_zc.index) + static_cast<int>(GAMMA_RIGHT_WINDOW * rate);

        // Offset searching
        bool is_offset_found = false;

        // Threshold for M-morphology
        double mm_ampl = zcs[peak_zc_id].g_ampl * GAMMA_RIGHT_XTD_ZCS_MM_PART;

        // 1.  First check:
        //     If exist zc before offset, defined at the step beta,
        //     index of that zc defined as new offset.
        //     If count of that zcs is more than 1, choose first

        if (!is_offset_found)
        {

            std::vector<int> offset_zcs_ids;
            for (int zc_id = 0; zc_id < zcs.size(); ++zc_id)
            {
                if ((last_zc.index < zcs[zc_id].index) && (zcs[zc_id].index <= offset_index_beta))
                {
                    offset_zcs_ids.push_back(zc_id);
                }
            }

            // We check 2 options:
            // * zc in small window
            // * zc left mm is big
            // If at least one is passed, onset defines here

            if (offset_zcs_ids.size() > 0)
            {
                int offset_zc_id = offset_zcs_ids[0];
                if (zcs[offset_zc_id].index <= right_lim)
                {
                    qrs_offset_index = zcs[offset_zc_id].index;
                    is_offset_found = true;
                }
                if (std::abs(zcs[offset_zc_id].g_l_mm.value) > mm_ampl)
                {
                    qrs_offset_index = zcs[offset_zc_id].index;
                    is_offset_found = true;
                }
            }
        }

        // 2.  Second check:
        //     Form mms list and search incorrect mm,
        //     which index defines as new offset
        if (!is_offset_found)
        {
            std::vector<ModulusMaxima> mms;
            ModulusMaxima mm_curr = all_mms[last_zc.r_mms[0].id];
            ModulusMaxima mm_next = mm_curr;
            while (mm_next.index < end_index)
            {
                mm_curr = mm_next;
                mms.push_back(mm_curr);
                mm_next = all_mms[mm_curr.id + 1];
            }

            std::vector<int> mms_ids_incorrect;
            for (int mm_id = 0; mm_id < mms.size(); ++mm_id)
            {
                if (!mms[mm_id].correctness)
                {
                    mms_ids_incorrect.push_back(mm_id);
                }
            }

            if (mms.size() > 0)
            {
                if (mms_ids_incorrect.size() > 0)
                {
                    if (std::abs(zcs[last_zc_id].g_r_mm.value) > mm_ampl)
                    {
                        int mm_id_incorrect = mms_ids_incorrect.back();
                        size_t qrs_offset_index = mms[mm_id_incorrect].index;
                        is_offset_found = true;
                    }
                    else
                    {
                        for (int mm_id : mms_ids_incorrect)
                        {
                            if (mms[mm_id].index <= right_lim)
                            {
                                qrs_offset_index = mms[mm_id].index;
                                is_offset_found = true;
                            }
                        }
                    }
                }
            }
        }

        // 3.  Third check:
        //     In allowed window exist only correct mms,
        //     then we looking for correct mm on special scale in allowed interval
        //     and define its index as onset
        if (!is_offset_found)
        {
            size_t scale_id_bord = GAMMA_BORD_SCALE;
            ModulusMaxima mm_bord = mm_bord = get_right_mm(ecg_lead.mms[scale_id_bord],
                ecg_lead.ids_mms[scale_id_bord], last_zc.index);
            qrs_offset_index = mm_bord.index;
            is_offset_found = true;
        }

        // 4.  Last scenario:
        //     Init onset with onset from beta step
        if (!is_offset_found)
        {
            qrs_offset_index = offset_index_beta;
        }
    }

    // Including onset to morphology
    double qrs_offset_value = ecg_lead.filter_[qrs_offset_index];
    WaveSign qrs_offset_sign = WaveSign::NONE;
    Point qrs_offset_point = Point(PointName::QRS_OFFSET, qrs_offset_index, qrs_offset_value, qrs_offset_sign);
    if (direction < 0)
    {
        points->insert(points->begin(), qrs_offset_point);
    }
    else
    {
        points->push_back(qrs_offset_point);
    }
    delineation->offset_index = qrs_offset_index;
}
