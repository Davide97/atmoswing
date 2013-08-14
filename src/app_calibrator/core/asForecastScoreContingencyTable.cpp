#include "asForecastScoreContingencyTable.h"

asForecastScoreContingencyTable::asForecastScoreContingencyTable()
:
asForecastScore()
{
    m_Score = asForecastScore::ContingencyTable;
    m_Name = _("Contingency table");
    m_FullName = _("Contingency table class");
    m_Order = NoOrder;
    m_ScaleBest = NaNFloat;
    m_ScaleWorst = NaNFloat;
}

asForecastScoreContingencyTable::~asForecastScoreContingencyTable()
{
    //dtor
}

float asForecastScoreContingencyTable::Assess(float ObservedVal, const Array1DFloat &ForcastVals, int nbElements)
{
    wxASSERT(ForcastVals.size()>1);
    wxASSERT(nbElements>0);
    wxASSERT(!asTools::IsNaN(m_Threshold));
    wxASSERT(!asTools::IsNaN(m_Percentile));
    wxASSERT(m_Percentile>0);
    wxASSERT(m_Percentile<1);

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

    float xPercentile = NaNFloat;
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

    // Indices for the left and right part (according to xObs)
    int indLeft = asTools::SortedArraySearchFloor(&F[0], &F[nbForecasts-1], m_Percentile);
    int indRight = asTools::SortedArraySearchCeil(&F[0], &F[nbForecasts-1], m_Percentile);
    wxASSERT(indLeft>=0);
    wxASSERT(indRight>=0);
    wxASSERT(indLeft<=indRight);

    if (indLeft==indRight)
    {
        xPercentile = x[indLeft];
    }
    else
    {
        xPercentile = x(indLeft)+(x(indRight)-x(indLeft))*(m_Percentile-F(indLeft))/(F(indRight)-F(indLeft));
    }

	// Forecasted and observed
	if (xPercentile>=m_Threshold && ObservedVal>=m_Threshold)
    {
        score = 1;
	}
	// Forecasted but not observed
	else if (xPercentile>=m_Threshold && ObservedVal<m_Threshold)
    {
        score = 2;
	}
	// Not forecasted but observed
	else if (xPercentile<m_Threshold && ObservedVal>=m_Threshold)
    {
        score = 3;
	}
	// Not forecasted and not observed
	else if (xPercentile<m_Threshold && ObservedVal<m_Threshold)
    {
        score = 4;
	}

    return score;
}

bool asForecastScoreContingencyTable::ProcessScoreClimatology(const Array1DFloat &refVals, const Array1DFloat &climatologyData)
{
    return true;
}