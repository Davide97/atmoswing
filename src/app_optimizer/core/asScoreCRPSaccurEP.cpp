/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can read the License at http://opensource.org/licenses/CDDL-1.0
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * 
 * When distributing Covered Code, include this CDDL Header Notice in 
 * each file and include the License file (licence.txt). If applicable, 
 * add the following below this CDDL Header, with the fields enclosed
 * by brackets [] replaced by your own identifying information:
 * "Portions Copyright [year] [name of copyright owner]"
 * 
 * The Original Software is AtmoSwing.
 * The Original Software was developed at the University of Lausanne.
 * All Rights Reserved.
 * 
 */

/*
 * Portions Copyright 2008-2013 Pascal Horton, University of Lausanne.
 * Portions Copyright 2013-2015 Pascal Horton, Terranum.
 */

#include "asScoreCRPSaccurEP.h"
#include "asScoreCRPSEP.h"
#include "asScoreCRPSsharpEP.h"

asScoreCRPSaccurEP::asScoreCRPSaccurEP()
        : asScore()
{
    m_score = asScore::CRPSaccuracyEP;
    m_name = _("CRPS Accuracy Exact Primitive");
    m_fullName = _("Continuous Ranked Probability Score Accuracy exact solution");
    m_order = Asc;
    m_scaleBest = 0;
    m_scaleWorst = NaNf;
}

asScoreCRPSaccurEP::~asScoreCRPSaccurEP()
{
    //dtor
}

float asScoreCRPSaccurEP::Assess(float ObservedVal, const a1f &ForcastVals, int nbElements) const
{
    wxASSERT(ForcastVals.size() > 1);
    wxASSERT(nbElements > 0);

    // Check inputs
    if (!CheckObservedValue(ObservedVal)) {
        return NaNf;
    }
    if (!CheckVectorLength( ForcastVals, nbElements)) {
        wxLogWarning(_("Problems in a vector length."));
        return NaNf;
    }

    asScoreCRPSEP scoreCRPSEP = asScoreCRPSEP();
    float CRPS = scoreCRPSEP.Assess(ObservedVal, ForcastVals, nbElements);
    asScoreCRPSsharpEP scoreCRPSsharpnessEP = asScoreCRPSsharpEP();
    float CRPSsharpness = scoreCRPSsharpnessEP.Assess(ObservedVal, ForcastVals, nbElements);

    return CRPS - CRPSsharpness;
}

bool asScoreCRPSaccurEP::ProcessScoreClimatology(const a1f &refVals, const a1f &climatologyData)
{
    return true;
}
