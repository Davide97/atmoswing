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

#ifndef ASPROCESSOR_H
#define ASPROCESSOR_H

#include <asIncludes.h>

class asTimeArray;

class asParameters;

class asResultsDates;

class asCriteria;

class asResultsValues;

class asPredictor;

class asPredictand;


class asProcessor
        : public wxObject
{
public:
    static bool GetAnalogsDates(std::vector<asPredictor *> predictorsArchive,
                                std::vector<asPredictor *> predictorsTarget, asTimeArray &timeArrayArchiveData,
                                asTimeArray &timeArrayArchiveSelection, asTimeArray &timeArrayTargetData,
                                asTimeArray &timeArrayTargetSelection, std::vector<asCriteria *> criteria,
                                asParameters &params, int step, asResultsDates &results, bool &containsNaNs);

    static bool GetAnalogsSubDates(std::vector<asPredictor *> predictorsArchive,
                                   std::vector<asPredictor *> predictorsTarget, asTimeArray &timeArrayArchiveData,
                                   asTimeArray &timeArrayTargetData, asResultsDates &anaDates,
                                   std::vector<asCriteria *> criteria, asParameters &params, int step,
                                   asResultsDates &results, bool &containsNaNs);

    static bool GetAnalogsValues(asPredictand &predictand, asResultsDates &anaDates, asParameters &params,
                                 asResultsValues &results);

protected:

private:
    static void
    InsertInArrays(bool isAsc, int analogsNb, const a1d &timeArchiveData, float thisScore, int counter, int iTimeArch,
                   a1f &scoreArrayOneDay, a1f &dateArrayOneDay);
};

#endif
