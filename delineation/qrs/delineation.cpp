#pragma once
#include "../../ecg_lead/ecg_lead.h"
#include "alpha/alpha.h"
#include "beta/beta.h"


std::vector<WaveDelineation> get_qrs_dels(const ECGLead& ecg_lead, size_t begin_index, size_t end_index)
{
    auto qrs_zcs = alpha_processing(ecg_lead, begin_index, end_index);

    std::vector<WaveDelineation> delineations;
    for (const ZeroCrossing& qrs_zc : qrs_zcs)
    {
        delineations.push_back(beta_processing(ecg_lead, qrs_zc));
    }

    return delineations;
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
