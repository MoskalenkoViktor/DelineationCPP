#pragma once
#include "beta_legacy.h"
#include "../../../threshold_crossings/routines.h"
#include "../../morfology_point.h"
#include <tuple>


std::tuple<bool, int, std::vector<Point>> right_qrs_morphology(const ECGLead& ecg_lead,
    const WaveDelineation& delineation, const QRSMorphologyData& morph_data)
{
    double rate = ecg_lead.rate;

    // Init data for target wdc scale
    int scale_id = morph_data.scale_id;
    const std::vector<double>& wdc = morph_data.wdc[scale_id];
    const std::vector<ZeroCrossing>& zcs = morph_data.zcs[scale_id];
    int peak_zc_id = morph_data.peak_zcs_ids[scale_id];
    const std::vector<ZeroCrossing>& zcs_all = ecg_lead.zcs[scale_id];

    // Init data for original wdc scale
    size_t scale_id_origin = WDC_SCALE_ID;
    const std::vector<double>& wdc_origin = morph_data.wdc[scale_id_origin];
    const std::vector<ZeroCrossing>& zcs_origin = morph_data.zcs[scale_id_origin];
    int peak_zc_id_origin = morph_data.peak_zcs_ids[scale_id_origin];
    const std::vector<ZeroCrossing>& zcs_all_origin = ecg_lead.zcs[scale_id_origin];

    size_t offset_index_beta = delineation.offset_index;

    // Analysis of zcs count in original wdc
    const std::vector<int> dels_zcs_ids_origin = origin_scale_analysis(ecg_lead, morph_data);

    // Init result data
    bool is_s_exist = false;
    int s_zc_id = 0;
    std::vector<Point> points;
    int last_zc_id = 0;

    int right_zc_id_origin = dels_zcs_ids_origin.back();
    const ZeroCrossing& right_zc_origin = zcs_origin[right_zc_id_origin];
    std::vector<int> xtd_zcs_ids;

    // If there is no zc on original scale,
    // which explicit corresponds to S
    if (right_zc_id_origin == peak_zc_id_origin)
    {

        // Init left border
        size_t left_index = right_zc_origin.index;

        // Init right border
        size_t r_ind_1 = std::min(zcs_all_origin[right_zc_origin.id + 1].index, morph_data.end_index);
        const std::vector<ModulusMaxima> mms_origin =
            get_lr_mms_in(ecg_lead, scale_id_origin, left_index, offset_index_beta);
        const std::vector<int> incorrect_mms_ids_origin = get_incorrect_mms_ids(mms_origin);
        double incorrect_mm_limit = zcs_origin[peak_zc_id_origin].g_ampl * GAMMA_RIGHT_ORIGIN_INCORRECT;
        size_t r_ind_2 = r_ind_1;
        for (int mm_id : incorrect_mms_ids_origin)
        {
            if (mms_origin[mm_id].value < incorrect_mm_limit)
            {
                r_ind_2 = mms_origin[mm_id].index;
                break;
            }
        }
        size_t right_index = std::min(r_ind_1, r_ind_2);
        right_index = std::min(right_index, offset_index_beta);

        // Form mms array in searching interval
        const std::vector<ModulusMaxima> mms = get_lr_mms_in(ecg_lead, scale_id, left_index, right_index);

        // Define list, which contains correct mms ids
        const std::vector<int> correct_mms_ids = get_correct_mms_ids(mms);

        // Choose last correct mm index (if exist)
        // as new right border
        if (mms.size() > 0)
        {
            if (correct_mms_ids.size() > 0)
            {
                int last_correct_mm_id = correct_mms_ids.back();
                right_index = mms[last_correct_mm_id].index;
            }
        }

        // Creating list of additional morphology points
        int xtd_zc_id = peak_zc_id + 1;

        while (xtd_zc_id < zcs.size() && zcs[xtd_zc_id].index <= right_index)
        {
            xtd_zcs_ids.push_back(xtd_zc_id);
            ++xtd_zc_id;
        }

        // If some additional points is found
        if (xtd_zcs_ids.size() > 0)
        {

            is_s_exist = true;

            // If that list contains even count
            // (more than 0) of zcs, then delete one of them
            if (xtd_zcs_ids.size() % 2 == 0)
            {
                xtd_zcs_ids.pop_back();
            }

            s_zc_id = xtd_zcs_ids[0];
            double s_zc_amplitude = zcs[s_zc_id].g_ampl;
            for (int zc_id = xtd_zcs_ids[0]; zc_id < xtd_zcs_ids[0] + xtd_zcs_ids.size(); zc_id += 2)
            {
                if (zcs[zc_id].g_ampl > s_zc_amplitude)
                {
                    s_zc_id = zc_id;
                    s_zc_amplitude = zcs[s_zc_id].g_ampl;
                }
            }
        }
    }
    // There is zc on original scale,
    // which explicit corresponds to S
    else
    {
        // Init left (aux) and right borders
        size_t left_index = zcs_origin[peak_zc_id_origin].index;
        size_t right_index = offset_index_beta;

        // Creating list of additional morphology points
        int xtd_zc_id = peak_zc_id - 1;

        while ((xtd_zc_id < zcs.size()) && (zcs[xtd_zc_id].index <= right_index))
        {
            xtd_zcs_ids.push_back(xtd_zc_id);
            ++xtd_zc_id;
        }

        // Some additional checking, that we really
        // found at least one zc, corresponding to S
        if (xtd_zcs_ids.size() > 0)
        {
            is_s_exist = true;

            // If that list contains even count,
            // then we must consider deletion
            // of last or insertion of next
            if (xtd_zcs_ids.size() % 2 == 0)
            {
                int last_xtd_zc_id = xtd_zcs_ids.back();
                if (last_xtd_zc_id < zcs.size() - 1)
                {
                    int dist_1 = std::abs(
                        static_cast<int>(zcs[last_xtd_zc_id].index) - static_cast<int>(right_index));
                    int dist_2 = std::abs(
                        static_cast<int>(zcs[last_xtd_zc_id + 1].index) - static_cast<int>(right_index));
                    if (dist_2 < GAMMA_SCALES_DIFF * dist_1)
                    {
                        xtd_zcs_ids.push_back(last_xtd_zc_id - 1);
                    }
                    else
                    {
                        xtd_zcs_ids.pop_back();
                    }
                }
                else
                {
                    xtd_zcs_ids.pop_back();
                }
            }

            // If S exist, it corresponds to odd zc with some big amplitude
            // If there is zcs after S-zc we should check them
            if (is_s_exist)
            {
                int max_zc_id = xtd_zcs_ids[0];
                double max_zc_amplitude = zcs[max_zc_id].g_ampl;
                for (int zc_id = xtd_zcs_ids[0]; zc_id < xtd_zcs_ids[0] + xtd_zcs_ids.size(); zc_id += 2)
                {
                    if (zcs[zc_id].g_ampl > max_zc_amplitude)
                    {
                        max_zc_id = zc_id;
                        max_zc_amplitude = zcs[s_zc_id].g_ampl;
                    }
                }

                s_zc_id = xtd_zcs_ids[0];
                double s_zc_amplitude = zcs[s_zc_id].g_ampl;
                for (int zc_id = xtd_zcs_ids[0]; zc_id < xtd_zcs_ids[0] + xtd_zcs_ids.size(); zc_id += 2)
                {
                    if (zcs[zc_id].g_ampl > max_zc_amplitude * GAMMA_RIGHT_S_PART)
                    {
                        s_zc_id = zc_id;
                        s_zc_amplitude = zcs[s_zc_id].g_ampl;
                    }
                }

                // Check zcs after S-zc
                if (xtd_zcs_ids.back() > s_zc_id)
                {
                    auto it = std::find(xtd_zcs_ids.begin(), xtd_zcs_ids.end(), s_zc_id);
                    std::vector<int> after_s_zcs_ids(it + 1, xtd_zcs_ids.end());
                    if (after_s_zcs_ids.size() % 2 == 1)
                    {
                        after_s_zcs_ids.pop_back();
                    }

                    // We check:
                    // * Difference between S-zc and next zc must be in allowed window - odd shift
                    // * Difference between zcs in M-morphology after S must be small - even shift
                    // * Amplitude of mm in M-morphology must be larger than some threshold
                    // If one of the conditions is passed then we should exclude xtd zcs
                    bool is_zcs_valid = true;
                    int odd_shift = static_cast<int>(GAMMA_RIGHT_ODD_XTD_ZCS_SHIFT * rate);
                    int even_shift = static_cast<int>(GAMMA_RIGHT_EVEN_XTD_ZCS_SHIFT * rate);
                    double mm_ampl = zcs[peak_zc_id].g_ampl * GAMMA_RIGHT_XTD_ZCS_MM_PART;

                    for (int i = 0; i < after_s_zcs_ids.size() - 1; i += 2)
                    {
                        int zc_id = after_s_zcs_ids[i];
                        if (std::abs(static_cast<int>(zcs[zc_id].index) -
                            static_cast<int>(zcs[zc_id - 1].index)) > odd_shift)
                        {
                            is_zcs_valid = false;
                        }
                        if (std::abs(static_cast<int>(zcs[zc_id + 1].index) -
                            static_cast<int>(zcs[zc_id].index)) > even_shift)
                        {
                            is_zcs_valid = false;
                        }
                        if (std::abs(zcs[zc_id + 1].g_r_mm->value) < mm_ampl)
                        {
                            is_zcs_valid = false;
                        }
                    }

                    if (is_zcs_valid)
                    {
                        if (after_s_zcs_ids.size() > 0)
                        {
                            auto it = std::find(xtd_zcs_ids.begin(), xtd_zcs_ids.end(), s_zc_id);
                            xtd_zcs_ids = std::vector<int>(xtd_zcs_ids.begin(), it + 1);
                            xtd_zcs_ids.insert(xtd_zcs_ids.end(), after_s_zcs_ids.begin(), after_s_zcs_ids.end());
                        }
                        else
                        {
                            auto it = std::find(xtd_zcs_ids.begin(), xtd_zcs_ids.end(), s_zc_id);
                            xtd_zcs_ids = std::vector<int>(xtd_zcs_ids.begin(), it + 1);
                        }
                    }
                }
            }
        }
    }

    // Adding xtd points (including S)
    for (int xtd_point_zc_id : xtd_zcs_ids)
    {

        if (xtd_point_zc_id == s_zc_id && is_s_exist)
        {
            ExtremumSign s_zc_sign = morph_data.s_signs[scale_id];
            size_t s_index = zcs[s_zc_id].index;
            double s_value = ecg_lead.filter[s_index];
            WaveSign s_sign;
            if (s_zc_sign == ExtremumSign::POSITIVE)
            {
                s_sign = WaveSign::POSITIVE;
            }
            else
            {
                s_sign = WaveSign::NEGATIVE;
            }
            points.emplace_back(PointName::S, s_index, s_value, s_sign);
        }
        else
        {
            size_t p_index = zcs[xtd_point_zc_id].index;
            double p_value = ecg_lead.filter[p_index];
            ExtremumSign p_sign;
            if (zcs[xtd_point_zc_id].extremum_sign == ExtremumSign::NEGATIVE)
            {
                p_sign = ExtremumSign::NEGATIVE;
            }
            else
            {
                p_sign = ExtremumSign::POSITIVE;
            }
            points.emplace_back(PointName::XTD_POINT, p_index, p_value, p_sign);
        }
    }
    std::reverse(points.begin(), points.end());

    if (xtd_zcs_ids.size() > 0)
    {
        last_zc_id = xtd_zcs_ids.back();
    }

    if (points.size() <= 1)
    {
        return std::make_tuple(false, last_zc_id, points);
    }
    else
    {
        return std::make_tuple(true, last_zc_id, points);
    }
}
