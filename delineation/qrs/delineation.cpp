#pragma once
#include "../../ecg_lead/ecg_lead.h"
#include "alpha/alpha.h"
#include "beta/beta.h"
#include "gamma/gamma.h"


std::pair<std::vector<WaveDelineation>, std::vector<Morphology>> get_qrs_dels(
    const ECGLead& ecg_lead, size_t begin_index, size_t end_index)
{
    auto qrs_zcs = alpha_processing(ecg_lead, begin_index, end_index);

    std::vector<WaveDelineation> delineations;
    std::vector<Morphology> morphologies;
    int num_dels = 0;
    for (const ZeroCrossing& qrs_zc : qrs_zcs)
    {
        WaveDelineation delineation = beta_processing(ecg_lead, qrs_zc);
        if (qrs_zc.id == 90)
        {
            int a = 5;
        }
        Morphology morphology = get_qrs_morphology(ecg_lead, num_dels, &delineation);
        delineations.push_back(delineation);
        morphologies.push_back(morphology);
        ++num_dels;
    }

    if (delineations.size() > 1)
    {
        for (int del_id = 1; del_id < delineations.size(); ++del_id)
        {
            if (delineations[del_id].onset_index <= delineations[del_id - 1].offset_index)
            {
                delineations[del_id].onset_index = delineations[del_id - 1].offset_index + 1;
                morphologies[del_id].points[0].index = delineations[del_id - 1].offset_index + 1;
                morphologies[del_id].points[0].value = ecg_lead.filter[delineations[del_id - 1].offset_index + 1];
            }
        }
    }

    double mean_rr = 0.0;
    for (int qrs_delineation_id = 0; qrs_delineation_id < delineations.size() - 1; ++qrs_delineation_id)
    {
        mean_rr += (delineations[qrs_delineation_id + 1].peak_index - delineations[qrs_delineation_id].peak_index);
    }

    if (delineations.size() > 1)
    {
        mean_rr /= delineations.size() - 1;
    }

    for (int qrs_delineation_id = 1; qrs_delineation_id < delineations.size() - 1; ++qrs_delineation_id)
    {
        double rr = static_cast<double>(
            (delineations[qrs_delineation_id].peak_index - delineations[qrs_delineation_id - 1].peak_index));
        if (rr < EXTRA_BEAT_PART * mean_rr)
        {
            delineations[qrs_delineation_id].specification = WaveSpecification::EXTRA;
        }
    }

    return{ delineations, morphologies };
}


size_t get_qrs_wdc_scale_id(const ECGLead& ecg_lead)
{
    size_t num_wdc_scales = ecg_lead.wdc.size();
    // DUMMY_CONST
    const size_t wdc_scale_id = 3;

    if (wdc_scale_id > num_wdc_scales - 1)
    {
        throw std::exception("Wrong wdc scale id for qrs");
    }

    return wdc_scale_id;
}


size_t get_qrs_aux_wdc_scale_id(const ECGLead& ecg_lead)
{
    size_t num_wdc_scales = ecg_lead.wdc.size();
    // DUMMY_CONST
    const size_t wdc_scale_id_aux = 4;

    if (wdc_scale_id_aux > num_wdc_scales - 1)
    {
        throw std::exception("Wrong wdc scale id aux for qrs");
    }

    return wdc_scale_id_aux;
}
