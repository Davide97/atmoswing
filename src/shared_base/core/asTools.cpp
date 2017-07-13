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

#include "asTools.h"

void asTools::InitRandom()
{
    srand(time(NULL));
}

int asTools::Random(const int min, const int max, const int step)
{
    // Initialize random seed
    double norm = ((double) rand() / (double) (RAND_MAX));

    double dmin = (double) min;
    double dmax = (double) max;
    double dstep = (double) step;

    double range = 0;
    if (step < 1) {
        dstep = 1;
    }

    range = norm * (dmax + dstep -
                    dmin); // Add the step to account for cases at the limits, so they have the same probability.
    range = floor(range / dstep) * dstep;

    double val = range + dmin;
    int intval = asTools::Round(val);

    if (intval > max)
        intval -= step;
    if (intval < min)
        intval += step;

    return intval;
}

float asTools::Random(const float min, const float max, const float step)
{
    return (float) asTools::Random((double) min, (double) max, (double) step);
}

double asTools::Random(const double min, const double max, const double step)
{
    // Initialize random seed
    double norm = ((double) rand() / (double) (RAND_MAX));

    double range = 0;
    if (step == 0) {
        range = norm * (max - min);
    } else {
        range = norm * (max + step -
                        min); // Add the step to account for cases at the limits, so they have the same probability.
        range = floor(range / step) * step; // -step/2.0 for cases at the limits.
    }

    double val = range + min;

    if (val > max)
        val -= step;
    if (val < min)
        val += step;

    return val;
}

int asTools::RandomNormalDistribution(const int mean, const int stDev, const int step)
{
    return (int) asTools::RandomNormalDistribution((double) mean, (double) stDev, (double) step);
}

float asTools::RandomNormalDistribution(const float mean, const float stDev, const float step)
{
    return (float) asTools::RandomNormalDistribution((double) mean, (double) stDev, (double) step);
}

double asTools::RandomNormalDistribution(const double mean, const double stDev, const double step)
{
    // Initialize random seed
    double u1 = ((double) rand() / (double) (RAND_MAX));
    double u2 = ((double) rand() / (double) (RAND_MAX));

    // Exclude 0
    while (u1 == 0) {
        u1 = ((double) rand() / (double) (RAND_MAX));
    }
    while (u2 == 0) {
        u2 = ((double) rand() / (double) (RAND_MAX));
    }

    // Box-Muller transform
    double z0 = sqrt(-2 * log(u1)) * cos(2 * M_PI * u2);
    //double z1 = sqrt(-2*log(u1))*sin(2*M_PI*u2);

    z0 *= stDev;

    if (step != 0) {
        z0 = step * asTools::Round(z0 / step);
    }

    z0 += mean;

    return z0;
}

bool asTools::IsRound(const float value)
{
    float valueround = Round(value);

    return std::abs(value - valueround) < 0.000001;
}

bool asTools::IsRound(const double value)
{
    double valueround = Round(value);

    return std::abs(value - valueround) < 0.000000000001;
}

float asTools::Round(const float value)
{
    if (value > 0) {
        return (float) floor(value + 0.5);
    } else {
        return (float) ceil(value - 0.5);
    }
}

double asTools::Round(const double value)
{
    if (value > 0) {
        return floor(value + 0.5);
    } else {
        return ceil(value - 0.5);
    }
}

float asTools::Mean(const int *pArrStart, const int *pArrEnd)
{
    float sum = 0, nb = 0;
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        // Does not check for NaNs, as there are no NaN for integers
        sum += (float) *(pArrStart + i);
        nb++;
    }
    return sum / nb;
}

float asTools::Mean(const float *pArrStart, const float *pArrEnd)
{
    float sum = 0, nb = 0;
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            sum += *(pArrStart + i);
            nb++;
        }
    }
    return sum / nb;
}

double asTools::Mean(const double *pArrStart, const double *pArrEnd)
{
    double sum = 0, nb = 0;
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            sum += *(pArrStart + i);
            nb++;
        }
    }
    return sum / nb;
}

float asTools::StDev(const int *pArrStart, const int *pArrEnd, const int sample)
{
    float sum = 0, sumsquares = 0, nb = 0;
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        // Dones't check for NaNs, as there are no NaN for intergers
        sum += *(pArrStart + i);
        sumsquares += (*(pArrStart + i)) * (*(pArrStart + i));
        nb++;
    }

    if (sample == asSAMPLE) {
        return (float) sqrt((sumsquares - (sum * sum / nb)) / (nb - 1));
    } else if (sample == asENTIRE_POPULATION) {
        return (float) sqrt((sumsquares - (sum * sum / nb)) / (nb));
    } else {
        return NaNf;
    }
}

float asTools::StDev(const float *pArrStart, const float *pArrEnd, const int sample)
{
    float sum = 0, sumsquares = 0, nb = 0;
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            sum += *(pArrStart + i);
            sumsquares += (*(pArrStart + i)) * (*(pArrStart + i));
            nb++;
        }
    }

    if (sample == asSAMPLE) {
        return (float) sqrt((sumsquares - (sum * sum / nb)) / (nb - 1));
    } else if (sample == asENTIRE_POPULATION) {
        return (float) sqrt((sumsquares - (sum * sum / nb)) / (nb));
    } else {
        return NaNf;
    }
}

double asTools::StDev(const double *pArrStart, const double *pArrEnd, const int sample)
{
    double sum = 0, sumsquares = 0, nb = 0;
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            sum += *(pArrStart + i);
            sumsquares += (*(pArrStart + i)) * (*(pArrStart + i));
            nb++;
        }
    }

    if (sample == asSAMPLE) {
        return sqrt((sumsquares - (sum * sum / nb)) / (nb - 1));
    } else if (sample == asENTIRE_POPULATION) {
        return sqrt((sumsquares - (sum * sum / nb)) / (nb));
    } else {
        return NaNd;
    }
}

a1f asTools::GetCumulativeFrequency(const int size)
{
    a1f F(size);

    // Parameters for the estimated distribution from Gringorten (a=0.44, b=0.12).
    // Choice based on [Cunnane, C., 1978, Unbiased plotting positions—A review: Journal of Hydrology, v. 37, p. 205–222.]
    // Bontron used a=0.375, b=0.25, that are optimal for a normal distribution
    float irep = 0.44f;
    float nrep = 0.12f;

    // Change the values for unit testing to compare to the results from Grenoble
    if (g_unitTesting) {
        irep = 0.375;
        nrep = 0.25;
    }

    float divisor = 1.0f / (size + nrep);
    for (float i = 0; i < size; i++) {
        F(i) = (i + 1.0f - irep) * divisor; // i+1 as i starts from 0
    }

    return F;
}

float asTools::GetValueForQuantile(const a1f &values, const float quantile)
{
    float value = NaNf;
    int size = values.size();

    a1f valuesCopy = values;

    // Sort the forcast array
    asTools::SortArray(&valuesCopy[0], &valuesCopy[size - 1], Asc);

    // Cumulative frequency
    a1f F = asTools::GetCumulativeFrequency(size);

    // Check limits
    if (quantile <= F[0])
        return valuesCopy[0];
    if (quantile >= F[size - 1])
        return valuesCopy[size - 1];

    // Indices for the left and right part (according to xObs)
    int indLeft = asTools::SortedArraySearchFloor(&F[0], &F[size - 1], quantile);
    int indRight = asTools::SortedArraySearchCeil(&F[0], &F[size - 1], quantile);
    wxASSERT(indLeft >= 0);
    wxASSERT(indRight >= 0);
    wxASSERT(indLeft <= indRight);

    if (indLeft == indRight) {
        value = valuesCopy[indLeft];
    } else {
        value = valuesCopy(indLeft) +
                (valuesCopy(indRight) - valuesCopy(indLeft)) * (quantile - F(indLeft)) / (F(indRight) - F(indLeft));
    }

    return value;
}

bool asTools::IsNaN(const int value)
{
    return value == NaNi;
}

bool asTools::IsNaN(const float value)
{
    return value != value;
}

bool asTools::IsNaN(const double value)
{
    return value != value;
}

bool asTools::IsInf(const float value)
{
    return value == Inff;
}

bool asTools::IsInf(const double value)
{
    return value == Infd;
}

bool asTools::IsInf(const long double value)
{
    return value == Infld;
}

int asTools::CountNotNaN(const float *pArrStart, const float *pArrEnd)
{
    int counter = 0;

    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            counter++;
        }
    }

    return counter;
}

int asTools::CountNotNaN(const double *pArrStart, const double *pArrEnd)
{
    int counter = 0;

    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            counter++;
        }
    }

    return counter;
}

bool asTools::HasNaN(const a2f &data)
{
    return !((data == data)).all();
}

bool asTools::HasNaN(const float *pArrStart, const float *pArrEnd)
{
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (asTools::IsNaN(*(pArrStart + i))) {
            return true;
        }
    }

    return false;
}

bool asTools::HasNaN(const double *pArrStart, const double *pArrEnd)
{
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (asTools::IsNaN(*(pArrStart + i))) {
            return true;
        }
    }

    return false;
}

int asTools::MinArray(const int *pArrStart, const int *pArrEnd)
{
    int min;

    min = *(pArrStart);

    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (*(pArrStart + i) < min) {
            min = *(pArrStart + i);
        }
    }

    return min;
}

float asTools::MinArray(const float *pArrStart, const float *pArrEnd)
{
    float min;
    int i = 0;

    // Manage the case where the first elements are NaNs
    while (asTools::IsNaN(*(pArrStart + i))) {
        i++;
    }
    min = *(pArrStart + i);

    for (; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            if (*(pArrStart + i) < min) {
                min = *(pArrStart + i);
            }
        }
    }

    return min;
}

double asTools::MinArray(const double *pArrStart, const double *pArrEnd)
{
    double min;
    int i = 0;

    // Manage the case where the first elements are NaNs
    while (asTools::IsNaN(*(pArrStart + i))) {
        i++;
    }
    min = *(pArrStart + i);

    for (; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            if (*(pArrStart + i) < min) {
                min = *(pArrStart + i);
            }
        }
    }

    return min;
}

int asTools::MinArrayIndex(const int *pArrStart, const int *pArrEnd)
{
    int min;
    int index;
    int i = 0;

    min = *(pArrStart + i);
    index = 0;

    for (; i <= pArrEnd - pArrStart; i++) {
        if (*(pArrStart + i) < min) {
            min = *(pArrStart + i);
            index = i;
        }
    }

    return index;
}

int asTools::MinArrayIndex(const float *pArrStart, const float *pArrEnd)
{
    float min;
    int index;
    int i = 0;

    min = *(pArrStart + i);
    index = 0;

    for (; i <= pArrEnd - pArrStart; i++) {
        if (*(pArrStart + i) < min) {
            min = *(pArrStart + i);
            index = i;
        }
    }

    return index;
}

int asTools::MinArrayIndex(const double *pArrStart, const double *pArrEnd)
{
    double min;
    int index;
    int i = 0;

    min = *(pArrStart + i);
    index = 0;

    for (; i <= pArrEnd - pArrStart; i++) {
        if (*(pArrStart + i) < min) {
            min = *(pArrStart + i);
            index = i;
        }
    }

    return index;
}

int asTools::MaxArray(const int *pArrStart, const int *pArrEnd)
{
    int max;
    int i = 0;

    max = *(pArrStart + i);

    for (; i <= pArrEnd - pArrStart; i++) {
        if (*(pArrStart + i) > max) {
            max = *(pArrStart + i);
        }
    }

    return max;
}

float asTools::MaxArray(const float *pArrStart, const float *pArrEnd)
{
    float max;
    int i = 0;

    // Manage the case where the first elements are NaNs
    while (asTools::IsNaN(*(pArrStart + i))) {
        i++;
    }
    max = *(pArrStart + i);

    for (; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            if (*(pArrStart + i) > max) {
                max = *(pArrStart + i);
            }
        }
    }

    return max;
}

double asTools::MaxArray(const double *pArrStart, const double *pArrEnd)
{
    double max;
    int i = 0;

    // Manage the case where the first elements are NaNs
    while (asTools::IsNaN(*(pArrStart + i))) {
        i++;
    }
    max = *(pArrStart + i);

    for (; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            if (*(pArrStart + i) > max) {
                max = *(pArrStart + i);
            }
        }
    }

    return max;
}

int asTools::MaxArrayIndex(const int *pArrStart, const int *pArrEnd)
{
    int max;
    int index;
    int i = 0;

    max = *(pArrStart + i);
    index = 0;

    for (; i <= pArrEnd - pArrStart; i++) {
        if (*(pArrStart + i) > max) {
            max = *(pArrStart + i);
            index = i;
        }
    }

    return index;
}

int asTools::MaxArrayIndex(const float *pArrStart, const float *pArrEnd)
{
    float max;
    int index;
    int i = 0;

    max = *(pArrStart + i);
    index = 0;

    for (; i <= pArrEnd - pArrStart; i++) {
        if (*(pArrStart + i) > max) {
            max = *(pArrStart + i);
            index = i;
        }
    }

    return index;
}

int asTools::MaxArrayIndex(const double *pArrStart, const double *pArrEnd)
{
    double max;
    int index;
    int i = 0;

    max = *(pArrStart + i);
    index = 0;

    for (; i <= pArrEnd - pArrStart; i++) {
        if (*(pArrStart + i) > max) {
            max = *(pArrStart + i);
            index = i;
        }
    }

    return index;
}

int asTools::MinArrayStep(const int *pArrStart, const int *pArrEnd, const int tolerance)
{
    // Copy data to not alter original array
    a1i copyData(pArrEnd - pArrStart + 1);

    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        copyData[i] = *(pArrStart + i);
    }

    // Sort the array
    asTools::SortArray(&copyData[0], &copyData[copyData.size() - 1], Asc);

    // Find min step
    int i = 1;

    while (copyData[i] - copyData[i - 1] <= tolerance) {
        i++;
        if (i == copyData.size()) {
            return asNOT_FOUND;
        }
    }

    int minstep = copyData[i] - copyData[i - 1];

    for (; i < copyData.size(); i++) {
        int currentval = std::abs(copyData[i] - copyData[i - 1]);
        if ((currentval < minstep) & (currentval > tolerance)) {
            minstep = currentval;
        }
    }

    return minstep;
}

float asTools::MinArrayStep(const float *pArrStart, const float *pArrEnd, const float tolerance)
{
    int nbNotNans = asTools::CountNotNaN(pArrStart, pArrEnd);
    int j = 0;

    // Copy data to not alter original array
    a1f copyData(nbNotNans);

    // Remove Nans
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            copyData[j] = *(pArrStart + i);
            j++;
        }
    }

    // Sort the array
    asTools::SortArray(&copyData[0], &copyData[copyData.size() - 1], Asc);

    // Find min step
    int i = 1;

    while (copyData[i] - copyData[i - 1] <= tolerance) {
        i++;
        if (i == copyData.size()) {
            return asNOT_FOUND;
        }
    }

    float minstep = copyData[i] - copyData[i - 1];

    for (; i < copyData.size(); i++) {
        float currentval = std::abs(copyData[i] - copyData[i - 1]);
        if ((currentval < minstep) & (currentval > tolerance)) {
            minstep = currentval;
        }
    }

    return minstep;
}

double asTools::MinArrayStep(const double *pArrStart, const double *pArrEnd, const double tolerance)
{
    int nbNotNans = asTools::CountNotNaN(pArrStart, pArrEnd);
    int j = 0;

    // Copy data to not alter original array
    a1d copyData(nbNotNans);

    // Remove Nans
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            copyData[j] = *(pArrStart + i);
            j++;
        }
    }

    // Sort the array
    asTools::SortArray(&copyData[0], &copyData[copyData.size() - 1], Asc);

    // Find min step
    int i = 1;

    while (copyData[i] - copyData[i - 1] <= tolerance) {
        i++;
        if (i == copyData.size()) {
            return asNOT_FOUND;
        }
    }

    double minstep = copyData[i] - copyData[i - 1];

    for (; i < copyData.size(); i++) {
        double currentval = std::abs(copyData[i] - copyData[i - 1]);
        if ((currentval < minstep) & (currentval > tolerance)) {
            minstep = currentval;
        }
    }

    return minstep;
}

a1i asTools::ExtractUniqueValues(const int *pArrStart, const int *pArrEnd, const int tolerance)
{
    int j = 0;

    // Copy data to not alter original array
    vi copyData(pArrEnd - pArrStart + 1);

    // Remove Nans
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        copyData[j] = *(pArrStart + i);
        j++;
    }

    // Sort the array
    asTools::SortArray(&copyData[0], &copyData[copyData.size() - 1], Asc);

    // Extract unique values
    vi copyDataUniques;
    copyDataUniques.reserve(pArrEnd - pArrStart + 1);
    copyDataUniques.push_back(copyData[0]); // Add first value

    for (unsigned int i = 1; i < copyData.size(); i++) {
        if ((std::abs(copyData[i] - copyData[i - 1]) > tolerance)) {
            copyDataUniques.push_back(copyData[i]);
        }
    }

    // Copy data to the final container
    a1i resultArray(copyDataUniques.size());

    for (unsigned int i = 0; i < copyDataUniques.size(); i++) {
        resultArray[i] = copyDataUniques[i];
    }

    return resultArray;
}

a1f asTools::ExtractUniqueValues(const float *pArrStart, const float *pArrEnd, const float tolerance)
{
    unsigned int nbNotNans = (unsigned int) asTools::CountNotNaN(pArrStart, pArrEnd);
    int j = 0;

    // Copy data to not alter original array
    vf copyData(nbNotNans);

    // Remove Nans
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            copyData[j] = *(pArrStart + i);
            j++;
        }
    }

    // Sort the array
    asTools::SortArray(&copyData[0], &copyData[copyData.size() - 1], Asc);

    // Extract unique values
    vf copyDataUniques;
    copyDataUniques.reserve(nbNotNans);
    copyDataUniques.push_back(copyData[0]); // Add first value

    for (unsigned int i = 1; i < copyData.size(); i++) {
        if ((std::abs(copyData[i] - copyData[i - 1]) > tolerance)) {
            copyDataUniques.push_back(copyData[i]);
        }
    }

    // Copy data to the final container
    a1f resultArray(copyDataUniques.size());

    for (unsigned int i = 0; i < copyDataUniques.size(); i++) {
        resultArray[i] = copyDataUniques[i];
    }

    return resultArray;
}

a1d asTools::ExtractUniqueValues(const double *pArrStart, const double *pArrEnd, const double tolerance)
{
    unsigned int nbNotNans = (unsigned int) asTools::CountNotNaN(pArrStart, pArrEnd);
    int j = 0;

    // Copy data to not alter original array
    vd copyData = vd(nbNotNans);

    // Remove Nans
    for (int i = 0; i <= pArrEnd - pArrStart; i++) {
        if (!asTools::IsNaN(*(pArrStart + i))) {
            copyData[j] = *(pArrStart + i);
            j++;
        }
    }

    // Sort the array
    asTools::SortArray(&copyData[0], &copyData[copyData.size() - 1], Asc);

    // Extract unique values
    vd copyDataUniques;
    copyDataUniques.reserve(nbNotNans);
    copyDataUniques.push_back(copyData[0]); // Add first value

    for (unsigned int i = 1; i < copyData.size(); i++) {
        if ((std::abs(copyData[i] - copyData[i - 1]) > tolerance)) {
            copyDataUniques.push_back(copyData[i]);
        }
    }

    // Copy data to the final container
    a1d resultArray(copyDataUniques.size());

    for (unsigned int i = 0; i < copyDataUniques.size(); i++) {
        resultArray[i] = copyDataUniques[i];
    }

    return resultArray;
}

int asTools::SortedArraySearch(const int *pArrStart, const int *pArrEnd, const int targetValue, const int tolerance, const int showWarning)
{
    return SortedArraySearchT<int>(pArrStart, pArrEnd, targetValue, tolerance, showWarning);
}

int asTools::SortedArraySearch(const float *pArrStart, const float *pArrEnd, const float targetValue, const float tolerance, const int showWarning)
{
    return SortedArraySearchT<float>(pArrStart, pArrEnd, targetValue, tolerance, showWarning);
}

int asTools::SortedArraySearch(const double *pArrStart, const double *pArrEnd, const double targetValue, const double tolerance,
                               int showWarning)
{
    return SortedArraySearchT<double>(pArrStart, pArrEnd, targetValue, tolerance, showWarning);
}

template<class T>
int asTools::SortedArraySearchT(const T *pArrStart, const T *pArrEnd, const T targetValue, const T tolerance, const int showWarning)
{
    wxASSERT(pArrStart);
    wxASSERT(pArrEnd);

    T *pFirst = NULL, *pMid = NULL, *pLast = NULL;
    int vlength;

    // Initialize first and last variables.
    pFirst = (T *) pArrStart;
    pLast = (T *) pArrEnd;

    // Check array order
    if (*pLast > *pFirst) {
        // Binary search
        while (pFirst <= pLast) {
            vlength = (int) (pLast - pFirst);
            pMid = pFirst + vlength / 2;
            if (targetValue - tolerance > *pMid) {
                pFirst = pMid + 1;
            } else if (targetValue + tolerance < *pMid) {
                pLast = pMid - 1;
            } else {
                // Return found index
                return int(pMid - pArrStart);
            }
        }

        // Check the pointers
        if (pLast - pArrStart < 0) {
            pLast = (T *) pArrStart;
        } else if (pLast - pArrEnd > 0) {
            pLast = (T *) pArrEnd - 1;
        } else if (pLast - pArrEnd == 0) {
            pLast -= 1;
        }

        // If the value was not found, return closest value inside tolerance
        if (std::abs(targetValue - *pLast) <= std::abs(targetValue - *(pLast + 1))) {
            if (std::abs(targetValue - *pLast) <= tolerance) {
                return int(pLast - pArrStart);
            } else {
                // Check that the value is within the array. Do it here to allow a margin for the tolerance
                if (targetValue > *pArrEnd || targetValue < *pArrStart) {
                    if (showWarning == asSHOW_WARNINGS) {
                        wxLogWarning(_("The value (%f) is out of the array range."), float(targetValue));
                    }
                    return asOUT_OF_RANGE;
                }
                if (showWarning == asSHOW_WARNINGS) {
                    wxLogWarning(_("The value was not found in the array."));
                }
                return asNOT_FOUND;
            }
        } else {
            if (std::abs(targetValue - *(pLast + 1)) <= tolerance) {
                return int(pLast - pArrStart + 1);
            } else {
                // Check that the value is whithin the array. Do it here to allow a margin for the tolerance
                if (targetValue > *pArrEnd || targetValue < *pArrStart) {
                    if (showWarning == asSHOW_WARNINGS) {
                        wxLogWarning(_("The value (%f) is out of the array range."), float(targetValue));
                    }
                    return asOUT_OF_RANGE;
                }
                if (showWarning == asSHOW_WARNINGS) {
                    wxLogWarning(_("The value was not found in the array."));
                }
                return asNOT_FOUND;
            }
        }
    } else if (*pLast < *pFirst) {
        // Binary search
        while (pFirst <= pLast) {
            vlength = (int) (pLast - pFirst);
            pMid = pFirst + vlength / 2;
            if (targetValue - tolerance > *pMid) {
                pLast = pMid - 1;
            } else if (targetValue + tolerance < *pMid) {
                pFirst = pMid + 1;
            } else {
                // Return found index
                return int(pMid - pArrStart);
            }
        }

        // Check the pointers
        if (pFirst - pArrStart < 0) {
            pFirst = (T *) pArrStart + 1;
        } else if (pFirst - pArrEnd > 0) {
            pFirst = (T *) pArrEnd;
        } else if (pFirst - pArrStart == 0) {
            pFirst += 1;
        }


        // If the value was not found, return closest value inside tolerance
        if (std::abs(targetValue - *pFirst) <= std::abs(targetValue - *(pFirst - 1))) {
            if (std::abs(targetValue - *pFirst) <= tolerance) {
                return int(pFirst - pArrStart);
            } else {
                // Check that the value is whithin the array. Do it here to allow a margin for the tolerance.
                if (targetValue < *pArrEnd || targetValue > *pArrStart) {
                    if (showWarning == asSHOW_WARNINGS) {
                        wxLogWarning(_("The value (%f) is out of the array range."), float(targetValue));
                    }
                    return asOUT_OF_RANGE;
                }
                if (showWarning == asSHOW_WARNINGS) {
                    wxLogWarning(_("The value was not found in the array."));
                }
                return asNOT_FOUND;
            }
        } else {
            if (std::abs(targetValue - *(pFirst + 1)) <= tolerance) {
                return int(pFirst - pArrStart - 1);
            } else {
                // Check that the value is whithin the array. Do it here to allow a margin for the tolerance.
                if (targetValue < *pArrEnd || targetValue > *pArrStart) {
                    if (showWarning == asSHOW_WARNINGS) {
                        wxLogWarning(_("The value (%f) is out of the array range."), float(targetValue));
                    }
                    return asOUT_OF_RANGE;
                }
                if (showWarning == asSHOW_WARNINGS) {
                    wxLogWarning(_("The value was not found in the array."));
                }
                return asNOT_FOUND;
            }
        }
    } else {
        if (pLast - pFirst == 0) {
            if (*pFirst >= targetValue - tolerance && *pFirst <= targetValue + tolerance) {
                return 0; // Value corresponds
            } else {
                return asOUT_OF_RANGE;
            }
        }

        if (*pFirst >= targetValue - tolerance && *pFirst <= targetValue + tolerance) {
            return 0; // Value corresponds
        } else {
            return asOUT_OF_RANGE;
        }
    }
}

int asTools::SortedArraySearchClosest(const int *pArrStart, const int *pArrEnd, const int targetValue, const int showWarning)
{
    return SortedArraySearchClosestT<int>(pArrStart, pArrEnd, targetValue, showWarning);
}

int asTools::SortedArraySearchClosest(const float *pArrStart, const float *pArrEnd, const float targetValue, const int showWarning)
{
    return SortedArraySearchClosestT<float>(pArrStart, pArrEnd, targetValue, showWarning);
}

int asTools::SortedArraySearchClosest(const double *pArrStart, const double *pArrEnd, const double targetValue, const int showWarning)
{
    return SortedArraySearchClosestT<double>(pArrStart, pArrEnd, targetValue, showWarning);
}

template<class T>
int asTools::SortedArraySearchClosestT(const T *pArrStart, const T *pArrEnd, const T targetValue, const int showWarning)
{
    wxASSERT(pArrStart);
    wxASSERT(pArrEnd);

    T *pFirst = NULL, *pMid = NULL, *pLast = NULL;
    int vlength, IndexMid;

    // Initialize first and last variables.
    pFirst = (T *) pArrStart;
    pLast = (T *) pArrEnd;

    // Check array order
    if (*pLast > *pFirst) {
        // Check that the value is whithin the array
        if (targetValue > *pLast || targetValue < *pFirst) {
            if (showWarning == asSHOW_WARNINGS) {
                wxLogWarning(_("The value (%f) is out of the array range."), float(targetValue));
            }
            return asOUT_OF_RANGE;
        }

        // Binary search
        while (pFirst <= pLast) {
            vlength = (int) (pLast - pFirst);
            pMid = pFirst + vlength / 2;
            if (targetValue > *pMid) {
                pFirst = pMid + 1;
            } else if (targetValue < *pMid) {
                pLast = pMid - 1;
            } else {
                // Return found index
                IndexMid = (int) (pMid - pArrStart);
                return IndexMid;
            }
        }

        // Check the pointers
        if (pLast - pArrStart < 0) {
            pLast = (T *) pArrStart;
        } else if (pLast - pArrEnd > 0) {
            pLast = (T *) pArrEnd - 1;
        } else if (pLast - pArrEnd == 0) {
            pLast -= 1;
        }

        // If the value was not found, return closest value
        if (std::abs(targetValue - *pLast) <= std::abs(targetValue - *(pLast + 1))) {
            return int(pLast - pArrStart);
        } else {
            return int(pLast - pArrStart + 1);
        }
    } else if (*pLast < *pFirst) {
        // Check that the value is whithin the array
        if (targetValue < *pLast || targetValue > *pFirst) {
            if (showWarning == asSHOW_WARNINGS) {
                wxLogWarning(_("The value (%f) is out of the array range."), float(targetValue));
            }
            return asOUT_OF_RANGE;
        }

        // Binary search
        while (pFirst <= pLast) {
            vlength = (int) (pLast - pFirst);
            pMid = pFirst + vlength / 2;
            if (targetValue > *pMid) {
                pLast = pMid - 1;
            } else if (targetValue < *pMid) {
                pFirst = pMid + 1;
            } else {
                // Return found index
                IndexMid = (int) (pMid - pArrStart);
                return IndexMid;
            }
        }

        // Check the pointers
        if (pFirst - pArrStart < 0) {
            pFirst = (T *) pArrStart + 1;
        } else if (pFirst - pArrEnd > 0) {
            pFirst = (T *) pArrEnd;
        } else if (pFirst - pArrStart == 0) {
            pFirst += 1;
        }

        // If the value was not found, return closest value
        if (std::abs(targetValue - *pFirst) <= std::abs(targetValue - *(pFirst - 1))) {
            return int(pFirst - pArrStart);
        } else {
            return int(pFirst - pArrStart - 1);
        }
    } else {
        if (pLast - pFirst == 0) {
            if (*pFirst == targetValue) {
                return 0; // Value corresponds
            } else {
                return asOUT_OF_RANGE;
            }
        }

        if (*pFirst == targetValue) {
            return 0; // Value corresponds
        } else {
            return asOUT_OF_RANGE;
        }
    }
}

int asTools::SortedArraySearchFloor(const int *pArrStart, const int *pArrEnd, const int targetValue, const int tolerance, const int showWarning)
{
    return SortedArraySearchFloorT<int>(pArrStart, pArrEnd, targetValue, tolerance, showWarning);
}

int asTools::SortedArraySearchFloor(const float *pArrStart, const float *pArrEnd, const float targetValue, const float tolerance, const int showWarning)
{
    return SortedArraySearchFloorT<float>(pArrStart, pArrEnd, targetValue, tolerance, showWarning);
}

int asTools::SortedArraySearchFloor(const double *pArrStart, const double *pArrEnd, const double targetValue, const double tolerance, const int showWarning)
{
    return SortedArraySearchFloorT<double>(pArrStart, pArrEnd, targetValue, tolerance, showWarning);
}

template<class T>
int asTools::SortedArraySearchFloorT(const T *pArrStart, const T *pArrEnd, const T targetValue, const T tolerance, const int showWarning)
{
    wxASSERT(pArrStart);
    wxASSERT(pArrEnd);

    T *pFirst = NULL, *pMid = NULL, *pLast = NULL;
    int vlength;

    // Initialize first and last variables.
    pFirst = (T *) pArrStart;
    pLast = (T *) pArrEnd;

    // Check array order
    if (*pLast > *pFirst) {
        // Check that the value is within the array
        if (targetValue - tolerance > *pLast || targetValue + tolerance < *pFirst) {
            if (showWarning == asSHOW_WARNINGS) {
                wxLogWarning(_("The value (%f) is out of the array range."), float(targetValue));
            }
            return asOUT_OF_RANGE;
        }

        // Binary search
        while (pFirst <= pLast) {
            vlength = (int) (pLast - pFirst);
            pMid = pFirst + vlength / 2;
            if (targetValue - tolerance > *pMid) {
                pFirst = pMid + 1;
            } else if (targetValue + tolerance < *pMid) {
                pLast = pMid - 1;
            } else {
                // Return found index
                return int(pMid - pArrStart);
            }
        }

        // Check the pointers
        if (pLast - pArrStart < 0) {
            pLast = (T *) pArrStart;
        } else if (pLast - pArrEnd > 0) {
            pLast = (T *) pArrEnd;
        }

        // If the value was not found, return floor value
        return int(pLast - pArrStart);
    } else if (*pLast < *pFirst) {
        // Check that the value is within the array
        if (targetValue + tolerance < *pLast || targetValue - tolerance > *pFirst) {
            if (showWarning == asSHOW_WARNINGS) {
                wxLogWarning(_("The value (%f) is out of the array range."), float(targetValue));
            }
            return asOUT_OF_RANGE;
        }

        // Binary search
        while (pFirst <= pLast) {
            vlength = (int) (pLast - pFirst);
            pMid = pFirst + vlength / 2;
            if (targetValue - tolerance > *pMid) {
                pLast = pMid - 1;
            } else if (targetValue + tolerance < *pMid) {
                pFirst = pMid + 1;
            } else {
                // Return found index
                return int(pMid - pArrStart);
            }
        }

        // Check the pointers
        if (pFirst - pArrStart < 0) {
            pFirst = (T *) pArrStart;
        } else if (pFirst - pArrEnd > 0) {
            pFirst = (T *) pArrEnd;
        }

        // If the value was not found, return floor value
        return int(pFirst - pArrStart);
    } else {
        if (pLast - pFirst == 0) {
            if (abs(*pFirst - targetValue) <= tolerance) {
                return 0; // Value corresponds
            } else {
                return asOUT_OF_RANGE;
            }
        }

        if (abs(*pFirst - targetValue) <= tolerance) {
            return 0; // Value corresponds
        } else {
            return asOUT_OF_RANGE;
        }
    }
}


int asTools::SortedArraySearchCeil(const int *pArrStart, const int *pArrEnd, const int targetValue, const int tolerance, const int showWarning)
{
    return SortedArraySearchCeilT<int>(pArrStart, pArrEnd, targetValue, tolerance, showWarning);
}

int asTools::SortedArraySearchCeil(const float *pArrStart, const float *pArrEnd, const float targetValue, const float tolerance, const int showWarning)
{
    return SortedArraySearchCeilT<float>(pArrStart, pArrEnd, targetValue, tolerance, showWarning);
}

int asTools::SortedArraySearchCeil(const double *pArrStart, const double *pArrEnd, const double targetValue, const double tolerance, const int showWarning)
{
    return SortedArraySearchCeilT<double>(pArrStart, pArrEnd, targetValue, tolerance, showWarning);
}

template<class T>
int asTools::SortedArraySearchCeilT(const T *pArrStart, const T *pArrEnd, const T targetValue, const T tolerance, const int showWarning)
{
    wxASSERT(pArrStart);
    wxASSERT(pArrEnd);

    T *pFirst = NULL, *pMid = NULL, *pLast = NULL;
    int vlength;

    // Initialize first and last variables.
    pFirst = (T *) pArrStart;
    pLast = (T *) pArrEnd;

    // Check array order
    if (*pLast > *pFirst) {
        // Check that the value is within the array
        if (targetValue - tolerance > *pLast || targetValue + tolerance < *pFirst) {
            if (showWarning == asSHOW_WARNINGS) {
                wxLogWarning(_("The value (%f) is out of the array range."), float(targetValue));
            }
            return asOUT_OF_RANGE;
        }

        // Binary search
        while (pFirst <= pLast) {
            vlength = (int) (pLast - pFirst);
            pMid = pFirst + vlength / 2;
            if (targetValue - tolerance > *pMid) {
                pFirst = pMid + 1;
            } else if (targetValue + tolerance < *pMid) {
                pLast = pMid - 1;
            } else {
                // Return found index
                return int(pMid - pArrStart);
            }
        }

        // Check the pointers
        if (pLast - pArrStart < 0) {
            pLast = (T *) pArrStart;
        } else if (pLast - pArrEnd > 0) {
            pLast = (T *) pArrEnd;
        }

        // If the value was not found, return ceil value
        return int(pLast - pArrStart + 1);
    } else if (*pLast < *pFirst) {
        // Check that the value is within the array
        if (targetValue + tolerance < *pLast || targetValue - tolerance > *pFirst) {
            if (showWarning == asSHOW_WARNINGS) {
                wxLogWarning(_("The value (%f) is out of the array range."), float(targetValue));
            }
            return asOUT_OF_RANGE;
        }

        // Binary search
        while (pFirst <= pLast) {
            vlength = (int) (pLast - pFirst);
            pMid = pFirst + vlength / 2;
            if (targetValue - tolerance > *pMid) {
                pLast = pMid - 1;
            } else if (targetValue + tolerance < *pMid) {
                pFirst = pMid + 1;
            } else {
                // Return found index
                return int(pMid - pArrStart);
            }
        }

        // Check the pointers
        if (pFirst - pArrStart < 0) {
            pFirst = (T *) pArrStart;
        } else if (pFirst - pArrEnd > 0) {
            pFirst = (T *) pArrEnd;
        }

        // If the value was not found, return ceil value
        return int(pFirst - pArrStart - 1);
    } else {
        if (pLast - pFirst == 0) {
            if (abs(*pFirst - targetValue) <= tolerance) {
                return 0; // Value corresponds
            } else {
                return asOUT_OF_RANGE;
            }
        }

        if (abs(*pFirst - targetValue) <= tolerance) {
            return 0; // Value corresponds
        } else {
            return asOUT_OF_RANGE;
        }
    }
}

bool asTools::SortedArrayInsert(int *pArrStart, int *pArrEnd, const Order order, const int val)
{
    return SortedArrayInsert<int>(pArrStart, pArrEnd, order, val);
}

bool asTools::SortedArrayInsert(float *pArrStart, float *pArrEnd, const Order order, const float val)
{
    return SortedArrayInsert<float>(pArrStart, pArrEnd, order, val);
}

bool asTools::SortedArrayInsert(double *pArrStart, double *pArrEnd, const Order order, const double val)
{
    return SortedArrayInsert<double>(pArrStart, pArrEnd, order, val);
}

template<class T>
bool asTools::SortedArrayInsert(T *pArrStart, T *pArrEnd, const Order order, const T val)
{
    wxASSERT(pArrStart);
    wxASSERT(pArrEnd);

    // Check the array length
    int vlength = pArrEnd - pArrStart;

    // Index where to insert the new element
    int iNext = 0;

    // Check order
    switch (order) {
        case (Asc): {
            iNext = asTools::SortedArraySearchCeil(pArrStart, pArrEnd, val, 0, asHIDE_WARNINGS);
            if (iNext == asOUT_OF_RANGE) {
                iNext = 0;
            }
            break;
        }
        case (Desc): {
            iNext = asTools::SortedArraySearchFloor(pArrStart, pArrEnd, val, 0, asHIDE_WARNINGS);
            if (iNext == asOUT_OF_RANGE) {
                iNext = 0;
            }
            break;
        }
        default: {
            wxLogError(_("Incorrect value of the order enumeration."));
            return false;
        }
    }

    // Swap next elements
    for (int i = vlength - 1; i >= iNext; i--) // Minus 1 becuase we overwrite the last element by the previous one
    {
        pArrStart[i + 1] = pArrStart[i];
    }

    // Insert new element
    pArrStart[iNext] = val;

    return true;
}

bool asTools::SortedArraysInsert(int *pArrRefStart, int *pArrRefEnd, int *pArrOtherStart, int *pArrOtherEnd,
                                 const Order order, const int valRef, const int valOther)
{
    return SortedArraysInsert<int>(pArrRefStart, pArrRefEnd, pArrOtherStart, pArrOtherEnd, order, valRef, valOther);
}

bool asTools::SortedArraysInsert(float *pArrRefStart, float *pArrRefEnd, float *pArrOtherStart, float *pArrOtherEnd,
                                 const Order order, const float valRef, const float valOther)
{
    return SortedArraysInsert<float>(pArrRefStart, pArrRefEnd, pArrOtherStart, pArrOtherEnd, order, valRef, valOther);
}

bool asTools::SortedArraysInsert(double *pArrRefStart, double *pArrRefEnd, double *pArrOtherStart, double *pArrOtherEnd,
                                 const Order order, const double valRef, const double valOther)
{
    return SortedArraysInsert<double>(pArrRefStart, pArrRefEnd, pArrOtherStart, pArrOtherEnd, order, valRef, valOther);
}

template<class T>
bool asTools::SortedArraysInsert(T *pArrRefStart, T *pArrRefEnd, T *pArrOtherStart, T *pArrOtherEnd, const Order order,
                                 const T valRef, const T valOther)
{
    wxASSERT(pArrRefStart);
    wxASSERT(pArrRefEnd);
    wxASSERT(pArrOtherStart);
    wxASSERT(pArrOtherEnd);

    // Check the other array length
    int vlength = (int) (pArrRefEnd - pArrRefStart);
    int ovlength = (int) (pArrOtherEnd - pArrOtherStart);

    if (vlength != ovlength) {
        wxLogError(_("The dimension of the two arrays are not equal."));
        return false;
    } else if (vlength == 0) {
        wxLogVerbose(_("The array has an unique value."));
        return true;
    } else if (vlength < 0) {
        wxLogError(_("The array has a negative size..."));
        return false;
    }

    // Index where to insert the new element
    int iNext = 0;

    // Check order
    switch (order) {
        case (Asc): {
            iNext = asTools::SortedArraySearchCeil(pArrRefStart, pArrRefEnd, valRef, 0, asHIDE_WARNINGS);
            if (iNext == asOUT_OF_RANGE) {
                iNext = 0;
            }
            break;
        }
        case (Desc): {
            iNext = asTools::SortedArraySearchFloor(pArrRefStart, pArrRefEnd, valRef, 0, asHIDE_WARNINGS);
            if (iNext == asOUT_OF_RANGE) {
                iNext = 0;
            }
            break;
        }
        default: {
            wxLogError(_("Incorrect value of the order enumeration."));
            return false;
        }
    }

    // Swap next elements
    for (int i = vlength - 1; i >= iNext; i--) // Minus 1 because we overwrite the last element by the previous one
    {
        pArrRefStart[i + 1] = pArrRefStart[i];
        pArrOtherStart[i + 1] = pArrOtherStart[i];
    }

    // Insert new element
    pArrRefStart[iNext] = valRef;
    pArrOtherStart[iNext] = valOther;

    return true;
}

bool asTools::SortArray(int *pArrRefStart, int *pArrRefEnd, const Order order)
{
    return SortArrayT<int>(pArrRefStart, pArrRefEnd, order);
}

bool asTools::SortArray(float *pArrRefStart, float *pArrRefEnd, const Order order)
{
    return SortArrayT<float>(pArrRefStart, pArrRefEnd, order);
}

bool asTools::SortArray(double *pArrRefStart, double *pArrRefEnd, const Order order)
{
    return SortArrayT<double>(pArrRefStart, pArrRefEnd, order);
}

template<class T>
bool asTools::SortArrayT(T *pArrRefStart, T *pArrRefEnd, const Order order)
{
    wxASSERT(pArrRefStart);
    wxASSERT(pArrRefEnd);

    // Check the array length
    int vlength = (int) (pArrRefEnd - pArrRefStart);

    if (vlength > 0) {
        int low = 0, high = vlength;
        asTools::QuickSort<T>(pArrRefStart, low, high, order);
    } else if (vlength == 0) {
        wxLogVerbose(_("The array has an unique value."));
        return true;
    } else {
        wxLogError(_("The array has a negative size..."));
        return false;
    }
    return true;
}

bool asTools::SortArrays(int *pArrRefStart, int *pArrRefEnd, int *pArrOtherStart, int *pArrOtherEnd, const Order order)
{
    return SortArraysT<int>(pArrRefStart, pArrRefEnd, pArrOtherStart, pArrOtherEnd, order);
}

bool asTools::SortArrays(float *pArrRefStart, float *pArrRefEnd, float *pArrOtherStart, float *pArrOtherEnd,
                         Order order)
{
    return SortArraysT<float>(pArrRefStart, pArrRefEnd, pArrOtherStart, pArrOtherEnd, order);
}

bool asTools::SortArrays(double *pArrRefStart, double *pArrRefEnd, double *pArrOtherStart, double *pArrOtherEnd,
                         Order order)
{
    return SortArraysT<double>(pArrRefStart, pArrRefEnd, pArrOtherStart, pArrOtherEnd, order);
}

template<class T>
bool asTools::SortArraysT(T *pArrRefStart, T *pArrRefEnd, T *pArrOtherStart, T *pArrOtherEnd, const Order order)
{
    wxASSERT(pArrRefStart);
    wxASSERT(pArrRefEnd);
    wxASSERT(pArrOtherStart);
    wxASSERT(pArrOtherEnd);

    // Check the other array length
    int vlength = (int) (pArrRefEnd - pArrRefStart);
    int ovlength = (int) (pArrOtherEnd - pArrOtherStart);

    if (vlength > 0 && vlength == ovlength) {
        int low = 0, high = vlength;
        asTools::QuickSortMulti<T>(pArrRefStart, pArrOtherStart, low, high, order);
    } else if (vlength != ovlength) {
        wxLogError(_("The dimension of the two arrays are not equal."));
        return false;
    } else if (vlength == 0) {
        wxLogVerbose(_("The array has an unique value."));
        return true;
    } else {
        wxLogError(_("The array has a negative size..."));
        return false;
    }
    return true;
}

template<class T>
void asTools::QuickSort(T *pArr, const int low, const int high, const Order order)
{
    int L, R;
    T pivot, tmp;

    R = high;
    L = low;

    pivot = pArr[(low + high) / 2];

    do {

        switch (order) {
            case (Asc): {
                while (pArr[L] < pivot)
                    L++;
                while (pArr[R] > pivot)
                    R--;
                break;
            }
            case (Desc): {
                while (pArr[L] > pivot)
                    L++;
                while (pArr[R] < pivot)
                    R--;
                break;
            }
            default: {
                wxLogError(_("Incorrect value of the order enumeration."));
                break;
            }
        }

        if (R >= L) {
            if (R != L) {
                tmp = pArr[R];
                pArr[R] = pArr[L];
                pArr[L] = tmp;
            }

            R--;
            L++;
        }
    } while (L <= R);

    if (low < R)
        asTools::QuickSort<T>(pArr, low, R, order);
    if (L < high)
        asTools::QuickSort<T>(pArr, L, high, order);

}

template<class T>
void asTools::QuickSortMulti(T *pArrRef, T *pArrOther, const int low, const int high, const Order order)
{
    int L, R;
    T pivot, tmp;

    R = high;
    L = low;

    pivot = pArrRef[(low + high) / 2];

    do {

        switch (order) {
            case (Asc): {
                while (pArrRef[L] < pivot)
                    L++;
                while (pArrRef[R] > pivot)
                    R--;
                break;
            }
            case (Desc): {
                while (pArrRef[L] > pivot)
                    L++;
                while (pArrRef[R] < pivot)
                    R--;
                break;
            }
            default: {
                wxLogError(_("Incorrect value of the order enumeration."));
                break;
            }
        }

        if (R >= L) {
            if (R != L) {
                // Reference array
                tmp = pArrRef[R];
                pArrRef[R] = pArrRef[L];
                pArrRef[L] = tmp;
                // Other array
                tmp = pArrOther[R];
                pArrOther[R] = pArrOther[L];
                pArrOther[L] = tmp;
            }

            R--;
            L++;
        }
    } while (L <= R);

    if (low < R)
        asTools::QuickSortMulti<T>(pArrRef, pArrOther, low, R, order);
    if (L < high)
        asTools::QuickSortMulti<T>(pArrRef, pArrOther, L, high, order);

}

