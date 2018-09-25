#pragma once
#include <vector>
#include <algorithm>
#include <numeric>
#include "../../../ecg_lead/ecg_lead.h"


double get_delta(const std::vector<double>& wdc, const ZeroCrossing& qrs_zc, size_t window);


std::vector<int> get_confirmed_qrs_zcs_ids(const ECGLead& ecg_lead, const std::vector<ZeroCrossing>& qrs_zcs, 
    const std::vector<int>& candidates_zcs_ids, const std::vector<double>& wdc)
{
    double rate = ecg_lead.rate;

    // DUMMY_CONST
    const double ALPHA_TRAINING_WINDOW = 8.0;
    // DUMMY_CONST
    const size_t ALPHA_BEATS_IN_TRAINING_WINDOW = 4;
    // DUMMY_CONST
    const double ALPHA_QRS_WINDOW = 0.1;
    // DUMMY_CONST
    const double ALPHA_THRESHOLD = 0.5;


    std::vector<int> confirmed_zcs_ids;
    if (candidates_zcs_ids.size() > 1)
    {

        size_t qrs_len = qrs_zcs[candidates_zcs_ids.back()].index - qrs_zcs[candidates_zcs_ids.front()].index;
        double training_window = 0;
        size_t training_deltas_count = 0;
        if (qrs_len <= static_cast<int>(2.0 * ALPHA_TRAINING_WINDOW * rate))
        {
            training_window = qrs_len / rate * 0.5;
            training_deltas_count = std::max(static_cast<int>(training_window * 0.5), 1);
        }
        else
        {
            training_window = ALPHA_TRAINING_WINDOW;
            training_deltas_count = ALPHA_BEATS_IN_TRAINING_WINDOW;
        }

        size_t window = static_cast<size_t>(rate * ALPHA_QRS_WINDOW);

        std::vector<double> deltas;
        std::vector<double> training_deltas;
        for (int candidate_zc_id : candidates_zcs_ids)
        {
            double delta = get_delta(wdc, qrs_zcs[candidate_zc_id], window);
            deltas.push_back(delta);
            if (qrs_zcs[candidate_zc_id].index <= rate * training_window + qrs_zcs[candidates_zcs_ids[0]].index)
            {
                training_deltas.push_back(delta);
            }
        }

        std::vector<double> correct_training_deltas = training_deltas;
        std::sort(correct_training_deltas.begin(), correct_training_deltas.end());
        correct_training_deltas = std::vector<double>(
            correct_training_deltas.rbegin() + training_deltas_count + 1,
            correct_training_deltas.rend());

        double epsilon = 0.0;
        if (training_deltas_count > 1)
        {
            epsilon = ALPHA_THRESHOLD *
                std::accumulate(correct_training_deltas.begin() + 1, correct_training_deltas.end(), 0.0) /
                (training_deltas_count - 1);
        }
        else
        {
            epsilon = ALPHA_THRESHOLD *
                std::accumulate(correct_training_deltas.begin(), correct_training_deltas.end(), 0.0) /
                training_deltas_count;
        }

        for (size_t i = training_deltas.size(); i < deltas.size(); ++i)
        {
            double current_delta = deltas[i];
            if (current_delta > epsilon)
            {
                confirmed_zcs_ids.push_back(candidates_zcs_ids[i]);
                correct_training_deltas = std::vector<double>(
                    correct_training_deltas.begin() + 1,
                    correct_training_deltas.begin() + training_deltas_count);
                correct_training_deltas.push_back(current_delta);
                epsilon = ALPHA_THRESHOLD *
                    std::accumulate(correct_training_deltas.begin(), correct_training_deltas.end(), 0.0) /
                    training_deltas_count;
            }
        }

        std::vector<double> reversed_correct_training_deltas;
        for (size_t i = 0; i < std::min(training_deltas_count, confirmed_zcs_ids.size()); ++i)
        {
            reversed_correct_training_deltas.push_back(
                get_delta(wdc, qrs_zcs[confirmed_zcs_ids[i]], window));
        }

        epsilon = ALPHA_THRESHOLD * std::accumulate(reversed_correct_training_deltas.begin(), 
            reversed_correct_training_deltas.end(), 0.0) / 
            training_deltas_count;

        for (int i = static_cast<int>(training_deltas.size()) - 1; i > -1; --i)
        {
            double current_delta = deltas[i];
            if (current_delta > epsilon)
            {
                confirmed_zcs_ids.insert(confirmed_zcs_ids.begin(), candidates_zcs_ids[i]);
                reversed_correct_training_deltas.resize(training_deltas_count - 1);
                reversed_correct_training_deltas.insert(
                    reversed_correct_training_deltas.begin(), current_delta);
                epsilon = ALPHA_THRESHOLD * std::accumulate(reversed_correct_training_deltas.begin(),
                    reversed_correct_training_deltas.end(), 0.0) /
                    training_deltas_count;
            }
        }
    }

    return confirmed_zcs_ids;
}


double get_delta(const std::vector<double>& wdc, const ZeroCrossing& qrs_zc, size_t window)
{
    size_t min_begin = std::min(window, qrs_zc.index);
    size_t min_end = std::min(window, wdc.size() - qrs_zc.index);
    double max_wdc = *std::max_element(
        wdc.begin() + qrs_zc.index - min_begin, 
        wdc.begin() + qrs_zc.index + min_end);
    double min_wdc = *std::min_element(
        wdc.begin() + qrs_zc.index - min_begin,
        wdc.begin() + qrs_zc.index + min_end);
    return max_wdc - min_wdc;
}
