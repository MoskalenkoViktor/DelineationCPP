#include "ecg_lead.h"
#include "../filtration/adaptive_filtration.h"
#include "../filtration/common_filtration.h"
#include "../filtration/cwt_filtration.h"
#include "../discrete_wavelet_transform/wdc.h"
#include "../zero_crossing/zero_crossing.h"
#include "../delineation/qrs/delineation.h"
#include "../delineation/p/delineation.h"
#include "../delineation/wave_delineation.h"
#include <tuple>
#include <vector>
#include "../params/params.h"


ECGLead::ECGLead(const std::string& lead_name, const std::vector<double>& data, double sample_rate) :
    name_(lead_name), signal_(data), sampling_rate(sample_rate)
{ }

void ECGLead::cwt_filtration()
{
    filter = filtration::cwt_filtration(origin_);
}

void ECGLead::common_filtration()
{
    filter = filtration::common_filtration(origin_);
}

void ECGLead::adaptive_filtration()
{
    filter = filtration::adaptive_filtration(origin_);
}

void ECGLead::dwt()
{
    wdc = get_wdc(filter);
}

void ECGLead::calc_mms()
{
    mms.clear();
    ids_mms.clear();
    for (size_t id = 0; id < wdc.size(); ++id)
    {
        mms.push_back(get_mms(wdc[id]));
        auto curr_mms = mms.back();

        std::vector<int> curr_ids_mms(wdc[id].size(), 0);
        int curr_id = -1;
        size_t curr_index = 0;
        for (auto mm : curr_mms)
        {
            std::fill(curr_ids_mms.begin() + curr_index, curr_ids_mms.begin() + mm.index, curr_id);
            curr_index = mm.index;
            curr_id = mm.id;
        }
        std::fill(curr_ids_mms.begin() + curr_index, curr_ids_mms.end(), curr_id);
        ids_mms.push_back(curr_ids_mms);
    }
}

void ECGLead::calc_zcs()
{
    zcs.clear();
    ids_zcs.clear();
    for (size_t id = 0; id < wdc.size(); ++id)
    {
        zcs.push_back(get_zcs(wdc[id], mms[id]));
        std::vector<ZeroCrossing>& curr_zcs = zcs.back();

        std::vector<int> curr_ids_zcs(wdc[id].size(), 0);
        int curr_id = -1;
        size_t curr_index = 0;
        for (const ZeroCrossing& zc : curr_zcs)
        {
            std::fill(curr_ids_zcs.begin() + curr_index, curr_ids_zcs.begin() + zc.index, curr_id);
            curr_index = zc.index;
            curr_id = static_cast<int>(zc.id);
        }
        std::fill(curr_ids_zcs.begin() + curr_index, curr_ids_zcs.end(), curr_id);
        ids_zcs.push_back(curr_ids_zcs);
    }
}

void ECGLead::qrs_del()
{
    std::vector<WaveDelineation> cur_qrs_dels;
    std::vector<Morphology> cur_qrs_morph;
    std::tie(cur_qrs_dels, cur_qrs_morph) = get_qrs_dels(*this, 0, wdc[0].size());

    qrs_dels = cur_qrs_dels;
    qrs_morphs = cur_qrs_morph;

    if (cur_qrs_morph.size() > 0)
    {
        size_t next_start = cur_qrs_dels.back().offset_index;
        while (next_start < static_cast<size_t>(wdc[0].size() * ALPHA_HUGE_PART))
        {
            std::tie(cur_qrs_dels, cur_qrs_morph) = get_qrs_dels(*this, next_start, wdc[0].size());
            if (cur_qrs_dels.size() > 0)
            {
                qrs_dels.insert(qrs_dels.end(), cur_qrs_dels.begin(), cur_qrs_dels.end());
                qrs_morphs.insert(qrs_morphs.end(), cur_qrs_morph.begin(), cur_qrs_morph.end());

                next_start = cur_qrs_dels.back().offset_index;
            }
            else
            {
                next_start += static_cast<size_t>((wdc[0].size() - next_start) * ALPHA_INC);
            }
            
        }
    }
}

void ECGLead::t_del()
{
}

void ECGLead::p_del()
{
//    std::vector<WaveDelineation> cur_p_dels_seq;
//    std::vector<Morphology> cur_p_morph_seq;
//    std::tie(cur_p_dels_seq, cur_p_morph_seq) = get_p_dels(*this);
//
//    p_dels = cur_p_dels_seq;
//    p_morphs = cur_p_morph_seq;
//
//    fib_analysis_imbalance();
//    fib_analysis_shortage();
}

void ECGLead::del_correction()
{
}

void ECGLead::calc_characteristics()
{
}

void ECGLead::init_plot_data()
{
}

void ECGLead::print_del_info()
{
}
