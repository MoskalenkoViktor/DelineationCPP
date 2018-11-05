#pragma once
#include "orders.h"
#include "../../../params/params.h"


void onset_processing(int first_zc_id, const ECGLead& ecg_lead, WaveDelineation* delineation,
    const QRSMorphologyData& morph_data, std::vector<Point>* points, int direction)
{
    double rate = ecg_lead.rate;

    // Init necessary data
    int scale_id = morph_data.scale_id;
    int peak_zc_id = morph_data.peak_zcs_ids[scale_id];
    const std::vector<double>& wdc = morph_data.wdc[scale_id];
    const std::vector<ZeroCrossing>& zcs = morph_data.zcs[scale_id];
    size_t begin_index = morph_data.begin_index;
    size_t onset_index_beta = delineation->onset_index;
    const std::vector<ModulusMaxima>& all_mms = ecg_lead.mms[scale_id];

    // Init onset index
    size_t qrs_onset_index = onset_index_beta;

    if (first_zc_id >= 0)
    {

        const ZeroCrossing& first_zc = zcs[first_zc_id];

        // Begin of allowed interval
        int left_lim = static_cast<int>(first_zc.index) - static_cast<int>(GAMMA_LEFT_WINDOW * rate);

        // Onset searching
        bool is_onset_found = false;

        // Threshold for M-morphology
        double mm_ampl = zcs[peak_zc_id].g_ampl * GAMMA_LEFT_XTD_ZCS_MM_PART;

        // 1.  First check:
        //     If exist zc before onset (move from right to left), defined at the step beta,
        //     index of that zc defined as new onset.
        //     If count of that zcs is more than 1, choose last

        if (!is_onset_found)
        {

            std::vector<int> onset_zcs_ids;
            for (int zc_id = 0; zc_id < zcs.size(); ++zc_id)
            {
                if ((onset_index_beta <= zcs[zc_id].index) && (zcs[zc_id].index < first_zc.index))
                {
                    onset_zcs_ids.push_back(zc_id);
                }
            }

            // We check 2 options:
            // * zc in small window
            // * zc right mm is big
            // If at least one is passed, onset defines here

            if (onset_zcs_ids.size() > 0)
            {
                int onset_zc_id = onset_zcs_ids.back();
                if (zcs[onset_zc_id].index >= left_lim)
                {
                    qrs_onset_index = zcs[onset_zc_id].index;
                    is_onset_found = true;
                }
                if (std::abs(zcs[onset_zc_id].g_r_mm.value) > mm_ampl)
                {
                    qrs_onset_index = zcs[onset_zc_id].index;
                    is_onset_found = true;
                }
            }
        }

        // 2.  Second check:
        //     Form mms list and search incorrect mm,
        //     which index defines as new onset
        if (!is_onset_found)
        {
            std::vector<ModulusMaxima> mms;
            ModulusMaxima mm_curr = all_mms[first_zc.l_mms[0].id];
            ModulusMaxima mm_next = mm_curr;
            while (mm_next.index > begin_index)
            {
                mm_curr = mm_next;
                mms.push_back(mm_curr);
                mm_next = all_mms[mm_curr.id - 1];
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
                    if (std::abs(zcs[first_zc_id].g_l_mm.value) > mm_ampl)
                    {
                        int mm_id_incorrect = mms_ids_incorrect[0];
                        size_t qrs_onset_index = mms[mm_id_incorrect].index;
                        is_onset_found = true;
                    }
                    else
                    {
                        for (int mm_id: mms_ids_incorrect)
                        {
                            if (mms[mm_id].index >= left_lim)
                            {
                                qrs_onset_index = mms[mm_id].index;
                                is_onset_found = true;
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
        if (!is_onset_found)
        {
            size_t scale_id_bord = GAMMA_BORD_SCALE;
            ModulusMaxima mm_bord = get_left_mm(
                ecg_lead.mms[scale_id_bord], ecg_lead.ids_mms[scale_id_bord], first_zc.index);
            qrs_onset_index = mm_bord.index;
            is_onset_found = true;
        }

        // 4.  Last scenario:
        //     Init onset with onset from beta step
        if (!is_onset_found)
        {
            qrs_onset_index = onset_index_beta;
        }
    }

    // Including onset to morphology
    double qrs_onset_value = ecg_lead.filter_[qrs_onset_index];
    WaveSign qrs_onset_sign = WaveSign::NONE;
    Point qrs_onset_point = Point(PointName::QRS_ONSET, qrs_onset_index, qrs_onset_value, qrs_onset_sign);
    if (direction < 0)
    {
        points->insert(points->begin(), qrs_onset_point);
    }
    else
    {
        points->push_back(qrs_onset_point);
    }
    delineation->onset_index = qrs_onset_index;
}
