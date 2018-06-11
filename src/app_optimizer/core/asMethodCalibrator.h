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

#ifndef ASMETHODCALIBRATOR_H
#define ASMETHODCALIBRATOR_H

#include <asIncludes.h>
#include <asMethodStandard.h>
#include <asParametersCalibration.h>
#include <asResultsDates.h>
#include <asPredictorArch.h>
#include <asResultsParametersArray.h>
#include <asResultsDates.h>
#include <asResultsValues.h>
#include <asResultsScores.h>
#include <asResultsTotalScore.h>
#include <asResultsScoresMap.h>
#include <asParametersCalibration.h>
#include <asParametersOptimization.h>
#include <asCriteria.h>
#include <asAreaCompGrid.h>
#include <asTimeArray.h>
#include <asProcessor.h>
#include <asProcessorScore.h>
#include <asPreprocessor.h>
#include <asScore.h>


class asMethodCalibrator
        : public asMethodStandard
{
public:
    asMethodCalibrator();

    virtual ~asMethodCalibrator();

    bool GetAnalogsDates(asResultsDates &results, asParametersScoring *params, int iStep, bool &containsNaNs);

    bool GetAnalogsSubDates(asResultsDates &results, asParametersScoring *params, asResultsDates &anaDates, int iStep,
                            bool &containsNaNs);

    bool GetAnalogsValues(asResultsValues &results, asParametersScoring *params, asResultsDates &anaDates, int iStep);

    bool GetAnalogsScores(asResultsScores &results, asParametersScoring *params, asResultsValues &anaValues, int iStep);

    bool GetAnalogsTotalScore(asResultsTotalScore &results, asParametersScoring *params, asResultsScores &anaScores,
                              int iStep);

    bool SubProcessAnalogsNumber(asParametersCalibration &params, asResultsDates &anaDatesPrevious, int iStep = 0);

    void ClearAll();

    void ClearTemp();

    bool PushBackBestTemp();

    void RemoveNaNsInTemp();

    void KeepBestTemp();

    void PushBackFirstTemp();

    void KeepFirstTemp();

    bool SortScoresAndParametersTemp();

    bool PushBackInTempIfBetter(asParametersCalibration &params, asResultsTotalScore &scoreFinal);

    bool KeepIfBetter(asParametersCalibration &params, asResultsTotalScore &scoreFinal);

    bool SetSelectedParameters(asResultsParametersArray &results);

    bool SetBestParameters(asResultsParametersArray &results);

    wxString GetPredictandStationIdsList(vi &stationIds) const;

    bool Manager();

    bool SaveDetails(asParametersCalibration *params);

    virtual bool Validate(asParametersCalibration *params);

    void SetScoreOrder(Order val)
    {
        m_scoreOrder = val;
    }

    vf GetScoreClimatology() const
    {
        return m_scoreClimatology;
    }

    void SetScoreClimatology(vf val)
    {
        m_scoreClimatology = val;
    }

    void SetPredictandStationIds(vi val)
    {
        m_predictandStationIds = val;
    }

protected:
    struct ParamExploration
    {
        double xMinStart;
        double xMinEnd;
        int xPtsNbStart;
        int xPtsNbEnd;
        int xPtsNbIter;
        double yMinStart;
        double yMinEnd;
        int yPtsNbIter;
        int yPtsNbStart;
        int yPtsNbEnd;
    };
    vi m_predictandStationIds;
    vf m_scoresCalib;
    vf m_scoresCalibTemp;
    Order m_scoreOrder;
    float m_scoreValid;
    vf m_scoreClimatology;
    std::vector<asParametersCalibration> m_parameters;
    std::vector<asParametersCalibration> m_parametersTemp;
    asParametersCalibration m_originalParams;
    bool m_validationMode;

    virtual bool Calibrate(asParametersCalibration &params) = 0;

    va1f GetClimatologyData(asParametersScoring *params);

    double GetEffectiveArchiveDataStart(asParameters *params) const;

    double GetEffectiveArchiveDataEnd(asParameters *params) const;

private:

    double GetTimeStartCalibration(asParametersScoring *params) const;

    double GetTimeEndCalibration(asParametersScoring *params) const;

    void LoadScoreOrder(asParametersCalibration &params);
};

#endif // ASMETHODCALIBRATOR_H
