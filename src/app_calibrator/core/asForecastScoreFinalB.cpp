#include "asForecastScoreFinalB.h"

asForecastScoreFinalB::asForecastScoreFinalB(Period period)
:
asForecastScoreFinal(period)
{

}

asForecastScoreFinalB::asForecastScoreFinalB(const wxString& periodString)
:
asForecastScoreFinal(periodString)
{

}

asForecastScoreFinalB::~asForecastScoreFinalB()
{
    //dtor
}

float asForecastScoreFinalB::Assess(Array1DFloat &targetDates, Array1DFloat &forecastScores, asTimeArray &timeArray)
{
    wxASSERT(targetDates.rows()>1);
    wxASSERT(forecastScores.rows()>1);

    int countA=0, countB=0, countC=0, countD=0, countTot=0;

    switch (m_Period)
    {
        case (asForecastScoreFinal::Total):
        {
            for (int i=0; i<forecastScores.size(); i++)
            {
                countTot++;
                if (forecastScores[i]==1)
                {
                    countA++;
                }
                else if (forecastScores[i]==2)
                {
                    countB++;
                }
                else if (forecastScores[i]==3)
                {
                    countC++;
                }
                else if (forecastScores[i]==4)
                {
                    countD++;
                }
                else
                {
                    asLogError(wxString::Format(_("The B score (%f) is not an authorized value."), forecastScores[i]));
                    return NaNFloat;
                }
            }
            break;
        }

        default:
        {
            asThrowException(_("Period not yet implemented in asForecastScoreFinalB."));
        }
    }

    float score;

    if (countTot>0)
    {
        if (((float)countA+(float)countC)>0)
        {
            score = ((float)countA+(float)countB)/((float)countA+(float)countC);
        }
        else
        {
            score = 0;
        }
    }
    else
    {
        score = NaNFloat;
    }

    return score;
}
