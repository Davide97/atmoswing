#include "asForecastScoreBS.h"

asForecastScoreBS::asForecastScoreBS()
:
asForecastScore()
{
    m_Score = asForecastScore::BS;
    m_Name = _("Brier score");
    m_FullName = _("Brier score");
    m_Order = Asc;
    m_ScaleBest = 0;
    m_ScaleWorst = NaNFloat;
}

asForecastScoreBS::~asForecastScoreBS()
{
    //dtor
}

float asForecastScoreBS::Assess(float ObservedVal, const Array1DFloat &ForcastVals, int nbElements)
{
    wxASSERT(ForcastVals.size()>1);
    wxASSERT(nbElements>0);
    wxASSERT(!asTools::IsNaN(m_Threshold));

    // Create the container to sort the data
    Array1DFloat x(nbElements);

    // Remove the NaNs and copy content
    int nbForecasts = CleanNans(ForcastVals, x, nbElements);
    if(nbForecasts==asNOT_FOUND)
    {
        asLogWarning(_("Only NaNs as inputs in the CRPS processing function."));
        return NaNFloat;
    }
    else if(nbForecasts<=2)
    {
        asLogWarning(_("Not enough elements to process the CRPS."));
        return NaNFloat;
    }

    // Sort the forcast array
    asTools::SortArray(&x[0], &x[nbForecasts-1], Asc);

    float score = NaNFloat;

    // Containers
    Array1DFloat F(nbForecasts);

    // Parameters for the estimated distribution from Gringorten (a=0.44, b=0.12).
    // Choice based on [Cunnane, C., 1978, Unbiased plotting positions�A review: Journal of Hydrology, v. 37, p. 205�222.]
    // Bontron used a=0.375, b=0.25, that are optimal for a normal distribution
    float irep = 0.44f;
    float nrep = 0.12f;

    // Build the cumulative distribution function for the middle of the x
    float divisor = 1.0f/(nbForecasts+nrep);
    for(float i=0; i<nbForecasts; i++)
    {

        F(i)=(i+1.0f-irep)*divisor; // i+1 as i starts from 0
    }

    // Search probability
    float probaOccurrence;
    if (m_Threshold<x[0])
    {
        probaOccurrence = 1;
    }
    else if (m_Threshold>x[nbForecasts-1])
    {
        probaOccurrence = 0;
    }
    else
    {
        int ind = asTools::SortedArraySearchFloor(&x[0], &x[nbForecasts-1], m_Threshold);
        while (x[ind]<=m_Threshold)
        {
            ind++;
        }

        if(m_Threshold>x[ind-1])
        {
            probaOccurrence = F(ind-1)+(F(ind)-F(ind-1))*(m_Threshold-x(ind-1))/(x(ind)-x(ind-1));
        }
        else
        {
            probaOccurrence = F[ind-1];
        }
    }

    float probaObservedVal = 0;
    if (ObservedVal>=m_Threshold)
    {
        probaObservedVal = 1;
    }

    score = (probaOccurrence-probaObservedVal)*(probaOccurrence-probaObservedVal);

    return score;
}

bool asForecastScoreBS::ProcessScoreClimatology(const Array1DFloat &refVals, const Array1DFloat &climatologyData)
{
    return true;
}