#include "data.h"
#include "../delineation.h"
#include "../../../zero_crossing/zero_crossing.h"


// DUMMY_CONST
static const double GAMMA_NORMAL_LENGTH = 0.155;
// DUMMY_CONST
static const double GAMMA_ALLOWED_DIFF_PART_LEFT = 0.3;
// DUMMY_CONST
static const double GAMMA_ALLOWED_DIFF_PART_RIGHT = 0.7;
// DUMMY_CONST
static const double GAMMA_R_NEG_PART = 0.75;


QRSMorphologyData::QRSMorphologyData(const ECGLead& ecg_lead, 
    const WaveDelineation& delineation, int target_scale_id) :
    wdc(ecg_lead.wdc)
{

    size_t aux_wdc_scale_id = get_qrs_aux_wdc_scale_id(ecg_lead);
    size_t num_scales = aux_wdc_scale_id + 1;
    double rate = ecg_lead.rate;

    size_t onset_index = delineation.onset_index;
    size_t peak_index = delineation.peak_index;
    size_t offset_index = delineation.offset_index;


    int normal_length = static_cast<int>(GAMMA_NORMAL_LENGTH * rate);
    int current_length = static_cast<int>(offset_index) - static_cast<int>(onset_index);
    int allowed_length_diff = normal_length - current_length;

    size_t begin_index = 0;
    size_t end_index = 0;
    int window_left = 0;
    int window_right = 0;

    if (allowed_length_diff > 0)
    {
        window_left = static_cast<int>(allowed_length_diff * GAMMA_ALLOWED_DIFF_PART_LEFT);
        window_right = static_cast<int>(allowed_length_diff * GAMMA_ALLOWED_DIFF_PART_RIGHT);
        if ((onset_index - window_left) >= 0)
        {
            begin_index = onset_index - window_left;
        }
        else
        {
            begin_index = 0;
        }
        end_index = offset_index + window_right;
    }
    else
    {
        window_left = 0;
        window_right = 0;
        begin_index = onset_index;
        end_index = offset_index;
    }

    std::vector<std::vector<ZeroCrossing>> zcs;
    std::vector<std::vector<int>> dels_zcs_ids;
    std::vector<int> peak_zcs_ids;

    std::vector<ExtremumSign> q_signs;
    std::vector<ExtremumSign> r_signs;
    std::vector<ExtremumSign> s_signs;

    for (size_t scale_id = 0; scale_id < aux_wdc_scale_id + 1; ++scale_id)
    {
        const std::vector<double>& curr_wdc = wdc[scale_id];
        std::vector<ZeroCrossing> curr_zcs = get_zcs_in_window(ecg_lead.wdc[scale_id],
            ecg_lead.zcs[scale_id], ecg_lead.ids_zcs[scale_id], begin_index, end_index);
        zcs.push_back(curr_zcs);

        std::vector<int> curr_dels_zcs_ids;
        int peak_zc_id = 0;
        size_t min_dist = curr_wdc.size();
        for (int zc_id = 0; zc_id < curr_zcs.size(); ++zc_id)
        {
            if ((onset_index <= curr_zcs[zc_id].index) && (curr_zcs[zc_id].index <= offset_index))
            {
                curr_dels_zcs_ids.push_back(zc_id);
                int current_dist = std::abs(
                    static_cast<int>(curr_zcs[zc_id].index) - static_cast<int>(peak_index));
                if (current_dist < min_dist)
                {
                    min_dist = current_dist;
                    peak_zc_id = zc_id;
                }
            }
        }

        if ((curr_dels_zcs_ids.size() > 0) && (peak_zc_id > curr_dels_zcs_ids[0]) &&
            (curr_zcs[peak_zc_id - 1].g_ampl >= curr_zcs[peak_zc_id].g_ampl * GAMMA_R_NEG_PART) &&
            (curr_zcs[peak_zc_id].extremum_sign == ExtremumSign::NEGATIVE))
        {
            --peak_zc_id;
        }

        dels_zcs_ids.push_back(curr_dels_zcs_ids);
        peak_zcs_ids.push_back(peak_zc_id);

        if (curr_zcs.size() > 0)
        {
            if (curr_zcs[peak_zc_id].extremum_sign == ExtremumSign::POSITIVE)
            {
                q_signs.push_back(ExtremumSign::NEGATIVE);
                r_signs.push_back(ExtremumSign::POSITIVE);
                s_signs.push_back(ExtremumSign::NEGATIVE);
            }
            else
            {
                q_signs.push_back(ExtremumSign::POSITIVE);
                r_signs.push_back(ExtremumSign::NEGATIVE);
                s_signs.push_back(ExtremumSign::POSITIVE);
            }
        }
    }

    this->window_left = window_left;
    this->window_right = window_right;
    this->begin_index = begin_index;
    this->end_index = end_index;
    this->num_scales = num_scales;
    this->scale_id = target_scale_id;
    this->zcs = zcs;
    this->dels_zcs_ids = dels_zcs_ids;
    this->peak_zcs_ids = peak_zcs_ids;
    this->allowed_length_diff = allowed_length_diff;
    this->q_signs = q_signs;
    this->r_signs = r_signs;
    this->s_signs = s_signs;
}
