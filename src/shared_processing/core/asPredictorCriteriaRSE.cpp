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

#include "asPredictorCriteriaRSE.h"

asPredictorCriteriaRSE::asPredictorCriteriaRSE(int linAlgebraMethod)
        : asPredictorCriteria(linAlgebraMethod)
{
    m_criteria = asPredictorCriteria::RSE;
    m_name = "RSE";
    m_fullName = _("Root Squared Error");
    m_order = Asc;
    m_scaleBest = 0;
    m_scaleWorst = NaNf;
    m_canUseInline = true;
}

asPredictorCriteriaRSE::~asPredictorCriteriaRSE()
{
    //dtor
}

float asPredictorCriteriaRSE::Assess(const a2f &refData, const a2f &evalData, int rowsNb, int colsNb) const
{
    wxASSERT_MSG(refData.rows() == evalData.rows(),
                 wxString::Format("refData.rows()=%d, evalData.rows()=%d", (int) refData.rows(),
                                  (int) evalData.rows()));
    wxASSERT_MSG(refData.cols() == evalData.cols(),
                 wxString::Format("refData.cols()=%d, evalData.cols()=%d", (int) refData.cols(),
                                  (int) evalData.cols()));

    float se = 0;

    switch (m_linAlgebraMethod) {
        case (asLIN_ALGEBRA_NOVAR):
        case (asLIN_ALGEBRA): {
            se += (evalData - refData).pow(2).sum();
            break;
        }

        case (asCOEFF_NOVAR):
        case (asCOEFF): {
            for (int i = 0; i < rowsNb; i++) {
                for (int j = 0; j < colsNb; j++) {
                    se += (evalData(i, j) - refData(i, j)) * (evalData(i, j) - refData(i, j));
                }
            }

            break;
        }

        default: {
            wxLogError(_("The calculation method was not correcty set"));
            return NaNf;
        }
    }

    return sqrt(se); // Can be NaN

}
