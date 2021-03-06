#pragma once
#include "beta_legacy.h"
#include "../../../threshold_crossings/routines.h"
#include "../../morfology_point.h"
#include <tuple>


std::tuple<bool, int, std::vector<Point>> left_qrs_morphology(const ECGLead& ecg_lead,
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

    size_t onset_index_beta = delineation.onset_index;

    // Analysis of zcs count in original wdc
    const std::vector<int> dels_zcs_ids_origin = origin_scale_analysis(ecg_lead, morph_data);

    // Init result data
    bool is_q_exist = false;
    int q_zc_id = 0;
    std::vector<Point> points;
    int first_zc_id = 0;

    int left_zc_id_origin = dels_zcs_ids_origin[0];
    const ZeroCrossing& left_zc_origin = zcs_origin[left_zc_id_origin];
    std::vector<int> xtd_zcs_ids;

    // If there is no zc on original scale,
    // which explicit corresponds to Q
    if (left_zc_id_origin == peak_zc_id_origin)
    {

        // Init right border
        size_t right_index = left_zc_origin.index;

        // Init left border
        size_t left_index = find_left_thc_index(wdc_origin, right_index - 1, 
            static_cast<int>(morph_data.begin_index), 0.0);
        left_index = std::max(left_index, onset_index_beta);

        // Form mms array in searching interval
        const std::vector<ModulusMaxima> mms = get_rl_mms_in(ecg_lead, scale_id, 
            static_cast<int>(right_index), static_cast<int>(left_index));

        // Define list, which contains correct mms ids
        const std::vector<int> correct_mms_ids = get_correct_mms_ids(mms);

        // Choose last correct mm index (if exist)
        // as new right border
        if (mms.size() > 0)
        {
            if (correct_mms_ids.size() > 0)
            {
                int last_correct_mm_id = correct_mms_ids.back();
                left_index = mms[last_correct_mm_id].index;
            }
        }

        // Creating list of additional morphology points
        int xtd_zc_id = peak_zc_id - 1;

        while (xtd_zc_id >= 0 && zcs[xtd_zc_id].index >= left_index)
        {
            xtd_zcs_ids.push_back(xtd_zc_id);
            --xtd_zc_id;
        }

        // If some additional points is found
        if (xtd_zcs_ids.size() > 0)
        {

            // If that list contains even count
            // (more than 0) of zcs, then there is no Q
            if (xtd_zcs_ids.size() % 2 == 0)
            {
                is_q_exist = false;
            }
            else
            {
                is_q_exist = true;
            }

            // If Q exist, it corresponds to odd zc with bigger amplitude
            if (is_q_exist)
            {
                q_zc_id = xtd_zcs_ids[0];
                double q_zc_ampl = zcs[q_zc_id].g_ampl;
                for (int zc_id = xtd_zcs_ids[0]; zc_id > xtd_zcs_ids[0] - 
                    static_cast<int>(xtd_zcs_ids.size()); zc_id -= 2)
                {
                    if (zcs[zc_id].g_ampl > q_zc_ampl)
                    {
                        q_zc_id = zc_id;
                        q_zc_ampl = zcs[q_zc_id].g_ampl;
                    }
                }
            }
        }
    }
    // There is zc on original scale,
    // which explicit corresponds to Q
    else
    {
        // Init left (aux) and right borders
        size_t right_index = zcs_origin[peak_zc_id_origin].index;
        size_t left_index = onset_index_beta;

        // Creating list of additional morphology points
        int xtd_zc_id = peak_zc_id - 1;

        while (xtd_zc_id >= 0 && zcs[xtd_zc_id].index >= left_index)
        {
            xtd_zcs_ids.push_back(xtd_zc_id);
            --xtd_zc_id;
        }

        // Some additional checking, that we really
        // found at least one zc, corresponding to Q
        if (xtd_zcs_ids.size() > 0)
        {
            is_q_exist = true;

            // If that list contains even count,
            // then we must consider deletion
            // of last or insertion of next
            if (xtd_zcs_ids.size() % 2 == 0)
            {
                int first_xtd_zc_id = xtd_zcs_ids.back();
                if (first_xtd_zc_id > 0)
                {
                    int dist_1 = std::abs(
                        static_cast<int>(zcs[first_xtd_zc_id].index) - static_cast<int>(right_index));
                    int dist_2 = std::abs(
                        static_cast<int>(zcs[first_xtd_zc_id - 1].index) - static_cast<int>(right_index));
                    if (dist_2 < GAMMA_SCALES_DIFF * dist_1)
                    {
                        xtd_zcs_ids.push_back(first_xtd_zc_id - 1);
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

            // If Q exist, it corresponds to odd zc with some big amplitude
            // If there is zcs after Q-zc we should check them
            if (is_q_exist)
            {
                int max_zc_id = xtd_zcs_ids[0];
                double max_zc_ampl = zcs[max_zc_id].g_ampl;
                for (int zc_id = xtd_zcs_ids[0]; 
                    zc_id > xtd_zcs_ids[0] - static_cast<int>(xtd_zcs_ids.size()); zc_id -= 2)
                {
                    if (zcs[zc_id].g_ampl > max_zc_ampl)
                    {
                        max_zc_id = zc_id;
                        max_zc_ampl = zcs[q_zc_id].g_ampl;
                    }
                }

                q_zc_id = xtd_zcs_ids[0];
                double q_zc_ampl = zcs[q_zc_id].g_ampl;
                for (int zc_id = xtd_zcs_ids[0]; 
                    zc_id > xtd_zcs_ids[0] - static_cast<int>(xtd_zcs_ids.size()); zc_id -= 2)
                {
                    if (zcs[zc_id].g_ampl > max_zc_ampl * GAMMA_LEFT_Q_PART)
                    {
                        q_zc_id = zc_id;
                        q_zc_ampl = zcs[q_zc_id].g_ampl;
                    }
                }

                // Check zcs after Q-zc
                if (xtd_zcs_ids.back() < q_zc_id)
                {
                    auto it = std::find(xtd_zcs_ids.begin(), xtd_zcs_ids.end(), q_zc_id);
                    std::vector<int> after_q_zcs_ids(it + 1, xtd_zcs_ids.end());
                    if (after_q_zcs_ids.size() % 2 == 1)
                    {
                        after_q_zcs_ids.pop_back();
                    }

                    // We check :
                    // * Difference between Q-zc and previous zc must be in allowed window - odd shift
                    // * Difference between zcs in M-morphology before Q must be small - even shift
                    // * Amplitude of mm in M-morphology must be larger than some threshold
                    // If one of the conditions is passed then we should exclude xtd zcs
                    bool is_zcs_valid = true;
                    int odd_shift = static_cast<int>(GAMMA_LEFT_ODD_XTD_ZCS_SHIFT * rate);
                    int even_shift = static_cast<int>(GAMMA_LEFT_EVEN_XTD_ZCS_SHIFT * rate);
                    double mm_ampl = zcs[peak_zc_id].g_ampl * GAMMA_LEFT_XTD_ZCS_MM_PART;

                    for (int i = 0; i < after_q_zcs_ids.size() - 1; i += 2)
                    {
                        int zc_id = after_q_zcs_ids[i];
                        if (std::abs(static_cast<int>(zcs[zc_id].index) - 
                            static_cast<int>(zcs[zc_id + 1].index)) > odd_shift)
                        {
                            is_zcs_valid = false;
                        }
                        if (std::abs(static_cast<int>(zcs[zc_id - 1].index) -
                            static_cast<int>(zcs[zc_id].index)) > even_shift)
                        {
                            is_zcs_valid = false;
                        }
                        if (std::abs(zcs[zc_id - 1].g_l_mm->value) < mm_ampl)
                        {
                            is_zcs_valid = false;
                        }
                    }

                    if (is_zcs_valid)
                    {
                        if (after_q_zcs_ids.size() > 0)
                        {
                            auto it = std::find(xtd_zcs_ids.begin(), xtd_zcs_ids.end(), q_zc_id);
                            xtd_zcs_ids = std::vector<int>(xtd_zcs_ids.begin(), it + 1);
                            xtd_zcs_ids.insert(xtd_zcs_ids.end(), after_q_zcs_ids.begin(), after_q_zcs_ids.end());
                        }
                        else
                        {
                            auto it = std::find(xtd_zcs_ids.begin(), xtd_zcs_ids.end(), q_zc_id);
                            xtd_zcs_ids = std::vector<int>(xtd_zcs_ids.begin(), it + 1);
                        }
                    }
                }
            }
        }
    }

    // Adding xtd points(including Q)
    for (int xtd_point_zc_id : xtd_zcs_ids)
    {

        if (xtd_point_zc_id == q_zc_id && is_q_exist)
        {
            ExtremumSign q_zc_sign = morph_data.q_signs[scale_id];
            size_t q_index = zcs[q_zc_id].index;
            double q_value = ecg_lead.filter[q_index];
            WaveSign q_sign;
            if (q_zc_sign == ExtremumSign::POSITIVE)
            {
                q_sign = WaveSign::POSITIVE;
            }
            else
            {
                q_sign = WaveSign::NEGATIVE;
            }
            points.emplace_back(PointName::Q, q_index, q_value, q_sign);
        }
        else
        {
            size_t p_index = zcs[xtd_point_zc_id].index;
            double p_value = ecg_lead.filter[p_index];
            WaveSign p_sign;
            if (zcs[xtd_point_zc_id].extremum_sign == ExtremumSign::NEGATIVE)
            {
                p_sign = WaveSign::NEGATIVE;
            }
            else
            {
                p_sign = WaveSign::POSITIVE;
            }
            points.emplace_back(PointName::XTD_POINT, p_index, p_value, p_sign);
        }
    }
    std::reverse(points.begin(), points.end());

    if (xtd_zcs_ids.size() > 0)
    {
        first_zc_id = xtd_zcs_ids.back();
    }

    if (points.size() <= 1)
    {
        return std::make_tuple(false, first_zc_id, points);
    }
    else
    {
        return std::make_tuple(true, first_zc_id, points);
    }
}
