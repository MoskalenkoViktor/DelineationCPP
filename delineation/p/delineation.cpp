//
// Created by Blintsov Sergey on 15/10/2018.
//
#include "delineation.h"
#include "../wave_delineation.h"
#include "beta/beta.h"
#include "gamma/gamma.h"
#include "fibrillation/fibrillation.h"

std::pair<std::vector<WaveDelineation>, std::vector<Morphology>> get_p_dels(const ECGLead& ecg_lead)
{
    std::vector<WaveDelineation> dels;
    std::vector<Morphology> morphs;
    int num_dels = 0;

    for (size_t qrs_id = 1; qrs_id <= ecg_lead.qrs_dels.size(); qrs_id++) {
        WaveDelineation delineation = get_p_del(ecg_lead, qrs_id);

        if (delineation.specification != WaveSpecification::ABSENCE) {
            Morphology morphology = get_p_morph(ecg_lead, num_dels, delineation);
            dels.push_back(delineation);
            morphs.push_back(morphology);
            num_dels += 1;
        }
    }
    return{ dels, morphs };
}
