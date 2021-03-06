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

#include "asFramePlotTimeSeries.h"

#include "asForecastManager.h"
#include "asFileAscii.h"

BEGIN_EVENT_TABLE(asFramePlotTimeSeries, wxFrame)
    EVT_CLOSE(asFramePlotTimeSeries::OnClose)
END_EVENT_TABLE()

asFramePlotTimeSeries::asFramePlotTimeSeries(wxWindow *parent, int selectedMethod, int selectedForecast,
                                             int selectedStation, asForecastManager *forecastManager, wxWindowID id)
        : asFramePlotTimeSeriesVirtual(parent, id),
          m_forecastManager(forecastManager),
          m_selectedStation(selectedStation),
          m_selectedMethod(selectedMethod),
          m_selectedForecast(selectedForecast),
          m_maxVal(100)
{
    auto paneMinSize = (int) (m_splitter->GetMinimumPaneSize() * g_ppiScaleDc);
    m_splitter->SetMinimumPaneSize(paneMinSize);

    m_panelPlot = new asPanelPlot(m_panelRight);
    m_panelPlot->GetPlotCtrl()->HideScrollBars();
    m_panelPlot->Layout();
    m_sizerPlot->Add(m_panelPlot, 1, wxALL | wxEXPAND, 0);
    m_sizerPlot->Fit(m_panelRight);

    m_staticTextStationName->SetLabel(forecastManager->GetStationNameWithHeight(m_selectedMethod, m_selectedForecast,
                                                                                m_selectedStation));
    wxFont titleFont = m_staticTextStationName->GetFont();
    titleFont.SetPointSize(titleFont.GetPointSize() + 2);
    m_staticTextStationName->SetFont(titleFont);

    // Icon
#ifdef __WXMSW__
    SetIcon(wxICON(myicon));
#endif

    Layout();
}

void asFramePlotTimeSeries::OnClose(wxCloseEvent &evt)
{
    // Save checked layers
    wxConfigBase *pConfig = wxFileConfig::Get();
    bool doPlotAllQuantiles = m_checkListToc->IsChecked(AllQuantiles);
    pConfig->Write("/PlotsTimeSeries/DoPlotAllQuantiles", doPlotAllQuantiles);
    bool doPlotAllAnalogs = m_checkListToc->IsChecked(AllAnalogs);
    pConfig->Write("/PlotsTimeSeries/DoPlotAllAnalogs", doPlotAllAnalogs);
    bool doPlotBestAnalogs10 = m_checkListToc->IsChecked(BestAnalogs10);
    pConfig->Write("/PlotsTimeSeries/DoPlotBestAnalogs10", doPlotBestAnalogs10);
    bool doPlotBestAnalogs5 = m_checkListToc->IsChecked(BestAnalogs5);
    pConfig->Write("/PlotsTimeSeries/DoPlotBestAnalogs5", doPlotBestAnalogs5);
    bool doPlotAllReturnPeriods = m_checkListToc->IsChecked(AllReturnPeriods);
    pConfig->Write("/PlotsTimeSeries/DoPlotAllReturnPeriods", doPlotAllReturnPeriods);
    bool doPlotClassicReturnPeriod = m_checkListToc->IsChecked(ClassicReturnPeriod);
    pConfig->Write("/PlotsTimeSeries/DoPlotClassicReturnPeriod", doPlotClassicReturnPeriod);
    bool doPlotClassicQuantiles = m_checkListToc->IsChecked(ClassicQuantiles);
    pConfig->Write("/PlotsTimeSeries/DoPlotClassicQuantiles", doPlotClassicQuantiles);
    bool doPlotPreviousForecasts = m_checkListToc->IsChecked(PreviousForecasts);
    pConfig->Write("/PlotsTimeSeries/DoPlotPreviousForecasts", doPlotPreviousForecasts);
    //bool doPlotInterpretation = m_checkListToc->IsChecked(Interpretation);
    //pConfig->Write("/PlotsTimeSeries/DoPlotInterpretation", doPlotInterpretation);

    evt.Skip();
}

void asFramePlotTimeSeries::Init()
{
    InitCheckListBox();
    InitPlotCtrl();
}

void asFramePlotTimeSeries::InitCheckListBox()
{
    wxArrayString checkList;

    checkList.Add(_("3 quantiles"));
    checkList.Add(_("All quantiles"));
    checkList.Add(_("All analogs"));
    checkList.Add(_("10 best analogs"));
    checkList.Add(_("5 best analogs"));
    checkList.Add(_("10 year return period"));
    checkList.Add(_("All return periods"));
    checkList.Add(_("Previous forecasts"));
    //checkList.Add(_("Interpretation"));

    m_checkListToc->Set(checkList);

    wxArrayString listPast;
    for (int i = 0; i < m_forecastManager->GetPastForecastsNb(m_selectedMethod, m_selectedForecast); i++) {
        asResultsForecast *forecast = m_forecastManager->GetPastForecast(m_selectedMethod, m_selectedForecast, i);
        listPast.Add(forecast->GetLeadTimeOriginString());
    }
    m_checkListPast->Set(listPast);

    for (int i = 0; i < m_forecastManager->GetPastForecastsNb(m_selectedMethod, m_selectedForecast); i++) {
        m_checkListPast->Check(i);
    }
}

void asFramePlotTimeSeries::InitPlotCtrl()
{
    // Get a pointer to the plotctrl
    wxPlotCtrl *plotctrl = m_panelPlot->GetPlotCtrl();

    // Set the axis lables
    plotctrl->SetShowXAxisLabel(false);
    plotctrl->SetShowYAxisLabel(true);
    plotctrl->SetYAxisLabel(_("Precipitation [mm]"));
    plotctrl->SetYAxisTicksWidth(25);

    // Legend
    plotctrl->SetShowKey(true);

    // Title
    plotctrl->SetShowPlotTitle(true);
    plotctrl->SetPlotTitle(_("Forecasted time series"));
    wxFont titleFont = plotctrl->GetPlotTitleFont();
    titleFont.SetPointSize(titleFont.GetPointSize() + 2);
    plotctrl->SetPlotTitleFont(titleFont);

    // Set the grid color
    wxColour gridColor(240, 240, 240);
    plotctrl->SetGridColour(gridColor);

    // Set the x axis
    asResultsForecast *forecast = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast);
    int length = forecast->GetTargetDatesLength();
    a1f dates = forecast->GetTargetDates();
    m_leadTimes.resize((unsigned long) length);
    for (int i = 0; i < length; i++) {
        m_leadTimes[i] = dates[i];
    }

    // Add a large vertical line at present time
    wxGenericPen p(gridColor, 5);
    wxPlotMarker m;
    m.CreateVertLineMarker(floor(forecast->GetLeadTimeOrigin()), p);
    plotctrl->AddMarker(m);

    // Freeze the ticks of the x axis and set dates label
    plotctrl->SetFixXAxisTickStep(1);
    plotctrl->SetXAxisTickType(wxPLOTCTRL_DATE_DDMM_FROMMJD);

    // Open layers defined in the preferences
    wxConfigBase *pConfig = wxFileConfig::Get();
    bool doPlotAllQuantiles;
    pConfig->Read("/PlotsTimeSeries/DoPlotAllQuantiles", &doPlotAllQuantiles, false);
    if (doPlotAllQuantiles)
        m_checkListToc->Check(AllQuantiles);
    bool doPlotAllAnalogs;
    pConfig->Read("/PlotsTimeSeries/DoPlotAllAnalogs", &doPlotAllAnalogs, true);
    if (doPlotAllAnalogs)
        m_checkListToc->Check(AllAnalogs);
    bool doPlotBestAnalogs10;
    pConfig->Read("/PlotsTimeSeries/DoPlotBestAnalogs10", &doPlotBestAnalogs10, true);
    if (doPlotBestAnalogs10)
        m_checkListToc->Check(BestAnalogs10);
    bool doPlotBestAnalogs5;
    pConfig->Read("/PlotsTimeSeries/DoPlotBestAnalogs5", &doPlotBestAnalogs5, false);
    if (doPlotBestAnalogs5)
        m_checkListToc->Check(BestAnalogs5);
    bool doPlotAllReturnPeriods;
    pConfig->Read("/PlotsTimeSeries/DoPlotAllReturnPeriods", &doPlotAllReturnPeriods, false);
    if (doPlotAllReturnPeriods)
        m_checkListToc->Check(AllReturnPeriods);
    bool doPlotClassicReturnPeriod;
    pConfig->Read("/PlotsTimeSeries/DoPlotClassicReturnPeriod", &doPlotClassicReturnPeriod, true);
    if (doPlotClassicReturnPeriod)
        m_checkListToc->Check(ClassicReturnPeriod);
    bool doPlotClassicQuantiles;
    pConfig->Read("/PlotsTimeSeries/DoPlotClassicQuantiles", &doPlotClassicQuantiles, true);
    if (doPlotClassicQuantiles)
        m_checkListToc->Check(ClassicQuantiles);
    bool doPlotPreviousForecasts;
    pConfig->Read("/PlotsTimeSeries/DoPlotPreviousForecasts", &doPlotPreviousForecasts, true);
    if (doPlotPreviousForecasts)
        m_checkListToc->Check(PreviousForecasts);
    //bool doPlotInterpretation;
    //pConfig->Read("/PlotsTimeSeries/DoPlotInterpretation", &doPlotInterpretation, true);
    //if (doPlotInterpretation) m_checkListToc->Check(Interpretation);
}

void asFramePlotTimeSeries::OnTocSelectionChange(wxCommandEvent &event)
{
    Plot();
}

void asFramePlotTimeSeries::OnExportTXT(wxCommandEvent &event)
{
    wxBusyCursor wait;

    wxString stationName = m_forecastManager->GetStationName(m_selectedMethod, m_selectedForecast, m_selectedStation);
    wxString forecastName = m_forecastManager->GetForecastName(m_selectedMethod, m_selectedForecast);
    wxString date = asTime::GetStringTime(m_forecastManager->GetLeadTimeOrigin(), "YYYY.MM.DD hh");
    wxString filename = wxString::Format("%sh - %s - %s", date, forecastName, stationName);

    wxFileDialog dialog(this, wxT("Save file as"), wxEmptyString, filename, wxT("Text files (*.txt)|*.txt"),
                        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (dialog.ShowModal() == wxID_OK) {
        asFileAscii file(dialog.GetPath(), asFile::Write);
        file.Open();

        // Add header
        file.AddLineContent(wxString::Format("Forecast of the %sh",
                                             asTime::GetStringTime(m_forecastManager->GetLeadTimeOrigin(),
                                                                   "DD.MM.YYYY hh")));
        file.AddLineContent(wxString::Format("Forecast: %s", forecastName));
        file.AddLineContent(wxString::Format("Station: %s", stationName));
        file.AddLineContent();

        // Quantiles
        a1f pc(11);
        pc << 1, 0.9f, 0.8f, 0.7f, 0.6f, 0.5f, 0.4f, 0.3f, 0.2f, 0.1f, 0;

        // Get forecast
        asResultsForecast *forecast = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast);

        // Set lead times
        file.AddLineContent("Quantiles:");
        wxString leadTimes = "\t";
        for (double leadTime : m_leadTimes) {
            leadTimes.Append(wxString::Format("%s\t", asTime::GetStringTime(leadTime, "DD.MM")));
        }
        file.AddLineContent(leadTimes);

        // Loop over the quantiles to display as polygons
        for (int iPc = 0; iPc < pc.size(); iPc++) {
            float thisQuantile = pc[iPc];

            wxString quantilesStr = wxString::Format("%f\t", thisQuantile);

            for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
                a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
                float pcVal = asGetValueForQuantile(analogs, thisQuantile);

                quantilesStr.Append(wxString::Format("%f\t", pcVal));
            }

            file.AddLineContent(quantilesStr);
        }
        file.AddLineContent();

        // Set best analogs values
        file.AddLineContent("Best analogs values:");
        file.AddLineContent(leadTimes);

        // Loop over the quantiles to display as polygons
        for (int rk = 0; rk < 10; rk++) {
            wxString rankStr = wxString::Format("%d\t", rk + 1);

            for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
                a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
                rankStr.Append(wxString::Format("%f\t", analogs[rk]));
            }

            file.AddLineContent(rankStr);
        }
        file.AddLineContent();

        // Set best analogs values
        file.AddLineContent("Best analogs dates:");
        file.AddLineContent(leadTimes);

        // Loop over the quantiles to display as polygons
        for (int rk = 0; rk < 10; rk++) {
            wxString rankStr = wxString::Format("%d\t", rk + 1);

            for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
                a1f dates = forecast->GetAnalogsDates(iLead);
                rankStr.Append(asTime::GetStringTime(dates[rk], "DD.MM.YYYY") + "\t");
            }

            file.AddLineContent(rankStr);
        }
        file.AddLineContent();

        // All traces
        file.AddLineContent("All traces:");

        asResultsForecast *oldestForecast = m_forecastManager->GetPastForecast(m_selectedMethod, m_selectedForecast,
                                                                               m_forecastManager->GetPastForecastsNb(
                                                                                       m_selectedMethod,
                                                                                       m_selectedForecast) - 1);
        float leadtimeStart = oldestForecast->GetTargetDates()[0];
        float leadtimeEnd = forecast->GetTargetDates()[forecast->GetTargetDatesLength() - 1];

        wxLogVerbose(asTime::GetStringTime(leadtimeStart));
        wxLogVerbose(asTime::GetStringTime(leadtimeEnd));

        a1f leadtimes = a1f::LinSpaced(leadtimeEnd - leadtimeStart + 1, leadtimeStart, leadtimeEnd);

        wxString allLeadtimesStr = "\t";
        for (int iLead = 0; iLead < leadtimes.size(); iLead++) {
            allLeadtimesStr.Append(wxString::Format("%s\t", asTime::GetStringTime(leadtimes[iLead], "DD.MM")));
        }

        a1f pcAll(4);
        pcAll << 0.9f, 0.6f, 0.5f, 0.2f;

        for (int iPc = 0; iPc < pcAll.size(); iPc++) {
            file.AddLineContent();
            file.AddLineContent(wxString::Format("Quantile %f:", pcAll[iPc]));
            file.AddLineContent(allLeadtimesStr);

            for (int past = 0;
                 past < m_forecastManager->GetPastForecastsNb(m_selectedMethod, m_selectedForecast); past++) {
                asResultsForecast *pastForecast = m_forecastManager->GetPastForecast(m_selectedMethod,
                                                                                     m_selectedForecast, past);
                a1f dates = pastForecast->GetTargetDates();
                wxString currentLine = asTime::GetStringTime(pastForecast->GetLeadTimeOrigin(), "DD.MM") + "\t";

                for (int iLead = 0; iLead < pastForecast->GetTargetDatesLength(); iLead++) {
                    a1f analogs = pastForecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
                    float pcVal = asGetValueForQuantile(analogs, pcAll[iPc]);

                    if (iLead == 0) {
                        int index = asFind(&leadtimes[0], &leadtimes[leadtimes.size() - 1], dates[iLead]);

                        if (index > 0) {
                            for (int i = 0; i < index; i++) {
                                currentLine.Append("\t");
                            }
                        }
                    }

                    currentLine.Append(wxString::Format("%f\t", pcVal));
                }

                file.AddLineContent(currentLine);
            }
        }

        file.Close();
    }
}

void asFramePlotTimeSeries::OnExportSVG(wxCommandEvent &event)
{
    m_panelPlot->ExportSVG();
}

void asFramePlotTimeSeries::OnPreview(wxCommandEvent &event)
{
    m_panelPlot->PrintPreview();
}

void asFramePlotTimeSeries::OnPrint(wxCommandEvent &event)
{
    m_panelPlot->Print();
}

bool asFramePlotTimeSeries::Plot()
{
    wxBusyCursor wait;

    // Get a pointer to the plotctrl
    wxPlotCtrl *plotctrl = m_panelPlot->GetPlotCtrl();

    // Check that there is no NaNs
    asResultsForecast *forecast = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast);
    for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
        a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
        if (asHasNaN(&analogs[0], &analogs[analogs.size() - 1])) {
            wxLogError(_("The forecast contains NaNs. Plotting has been canceled."));
            return false;
        }
    }

    // Clear previous curves
    int curvesNb = plotctrl->GetCurveCount();
    for (int i = curvesNb - 1; i >= 0; i--) {
        wxPlotData *plotData = plotctrl->GetDataCurve(i);
        if (plotData) {
            plotctrl->DeleteCurve(plotData);
        }
    }

    // Clear previous markers
    plotctrl->ClearMarkers();

    // Add a large vertical line at present time
    wxGenericPen p(wxColour(200, 200, 200), 5);
    wxPlotMarker m;
    m.CreateVertLineMarker(floor(forecast->GetLeadTimeOrigin()), p);
    plotctrl->AddMarker(m);

    // Set a first threshold for the zoom
    m_maxVal = 50;

    // Get curves to plot
    bool DoPlotAllQuantiles = false;
    bool DoPlotAllAnalogs = false;
    bool DoPlotBestAnalogs10 = false;
    bool DoPlotBestAnalogs5 = false;
    bool DoPlotAllReturnPeriods = false;
    bool DoPlotClassicReturnPeriod = false;
    bool DoPlotClassicQuantiles = false;
    bool DoPlotPreviousForecasts = false;
    bool DoPlotInterpretation = false;

    for (int curve = 0; curve < 8; curve++) {
        if (m_checkListToc->IsChecked(curve)) {
            switch (curve) {
                case (AllQuantiles):
                    DoPlotAllQuantiles = true;
                    break;
                case (AllAnalogs):
                    DoPlotAllAnalogs = true;
                    break;
                case (BestAnalogs10):
                    DoPlotBestAnalogs10 = true;
                    break;
                case (BestAnalogs5):
                    DoPlotBestAnalogs5 = true;
                    break;
                case (AllReturnPeriods):
                    DoPlotAllReturnPeriods = true;
                    break;
                case (ClassicReturnPeriod):
                    DoPlotClassicReturnPeriod = true;
                    break;
                case (ClassicQuantiles):
                    DoPlotClassicQuantiles = true;
                    break;
                case (PreviousForecasts):
                    DoPlotPreviousForecasts = true;
                    break;
                default:
                    wxLogError(_("The option was not found."));

            }
        }
    }

	// Plot order matters.
    if (DoPlotAllQuantiles)
        PlotAllQuantiles();
    if (DoPlotAllAnalogs)
        PlotAllAnalogs();
    if (DoPlotBestAnalogs10)
        PlotBestAnalogs(10);
    if (DoPlotBestAnalogs5)
        PlotBestAnalogs(5);
    if (forecast->HasReferenceValues()) {
        if (DoPlotAllReturnPeriods)
            PlotAllReturnPeriods();
        if (DoPlotClassicReturnPeriod)
            PlotReturnPeriod(10);
    }
    if (DoPlotPreviousForecasts)
        PlotPastForecasts();
    if (DoPlotClassicQuantiles)
        PlotClassicQuantiles();
    if (DoPlotInterpretation)
        PlotInterpretation();

    // Set the view rectangle
    wxRect2DDouble view(m_leadTimes[0] - 2.5, 0, m_leadTimes.size() + 2, m_maxVal * 1.1);
    plotctrl->SetViewRect(view);

    // Redraw
    plotctrl->Redraw(wxPLOTCTRL_REDRAW_PLOT);

    return true;
}

void asFramePlotTimeSeries::PlotAllReturnPeriods()
{
    // Get a pointer to the plotctrl
    wxPlotCtrl *plotctrl = m_panelPlot->GetPlotCtrl();

    // Get return periods
    a1f retPeriods = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast)->GetReferenceAxis();

    for (int i = retPeriods.size() - 1; i >= 0; i--) {
        if (std::abs(retPeriods[i] - 2.33) < 0.1)
            continue;

        // Get precipitation value
        float val = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast)->GetReferenceValue(
                m_selectedStation, i);

        // Color (from yellow to red)
        float ratio = (float) i / (float) (retPeriods.size() - 1);
        wxGenericPen pen(wxGenericColour(255, 255 - ratio * 255, 0), 2);

        // Markers -> cannot add legend entries
        //wxPlotMarker marker;
        //marker.CreateHorizLineMarker(val, pen);
        //plotctrl->AddMarker(marker);

        // Store max val
        if (val > m_maxVal)
            m_maxVal = val;

        // Create plot data
        wxPlotData plotData;
        plotData.Create(2);
        if (std::abs(retPeriods[i] - 2.33) < 0.1) {
            plotData.SetFilename(wxString::Format("P%3.2f", retPeriods[i]));
        } else {
            auto roundedVal = (int) asRound(retPeriods[i]);
            plotData.SetFilename(wxString::Format("P%d", roundedVal));
        }
        plotData.SetValue(0, m_leadTimes[0] - 10, val);
        plotData.SetValue(1, m_leadTimes[m_leadTimes.size() - 1] + 10, val);

        // Check and add to the plot
        if (plotData.Ok()) {
            // wxPlotPen_Type : wxPLOTPEN_NORMAL, wxPLOTPEN_ACTIVE, wxPLOTPEN_SELECTED, wxPLOTPEN_MAXTYPE
            plotData.SetPen(wxPLOTPEN_NORMAL, pen);

            plotData.SetDrawSymbols(false);
            plotData.SetDrawLines(true);

            // Add the curve
            bool select = false;
            bool send_event = false;
            plotctrl->AddCurve(plotData, select, send_event);
        } else {
            wxLogError(_("The return periods couldn't be added to the plot"));
        }

        plotData.Destroy();
    }

}

void asFramePlotTimeSeries::PlotReturnPeriod(int returnPeriod)
{
    // Get a pointer to the plotctrl
    wxPlotCtrl *plotctrl = m_panelPlot->GetPlotCtrl();

    // Get return periods
    a1f retPeriods = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast)->GetReferenceAxis();

    // Find the value 10
    int index = asFind(&retPeriods[0], &retPeriods[retPeriods.size() - 1], returnPeriod);

    if ((index != asNOT_FOUND) && (index != asOUT_OF_RANGE)) {
        // Get precipitation value
        float val = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast)->GetReferenceValue(
                m_selectedStation, index);

        // Color (red)
        wxGenericPen pen(wxGenericColour(255, 0, 0), 2);

        // Lines
        wxPlotMarker marker;
        marker.CreateHorizLineMarker(val, pen);
        plotctrl->AddMarker(marker);

        // Store max val
        if (val > m_maxVal)
            m_maxVal = val;
    } else {
        wxLogError(_("The 10 year return period was not found in the data."));
    }
}

void asFramePlotTimeSeries::PlotAllAnalogs()
{
    // Get a pointer to the plotctrl
    wxPlotCtrl *plotctrl = m_panelPlot->GetPlotCtrl();

    // Get forecast
    asResultsForecast *forecast = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast);

    // Get the total number of points
    int nbPoints = 0;
    for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
        a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
        for (int iAnalog = 0; iAnalog < analogs.size(); iAnalog++) {
            nbPoints++;
        }
    }

    // Create plot data
    wxPlotData plotData;
    plotData.Create(nbPoints);
    int counter = 0;
    for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
        a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
        for (int iAnalog = 0; iAnalog < analogs.size(); iAnalog++) {
            plotData.SetValue(counter, m_leadTimes[iLead], analogs[iAnalog]);
            counter++;

            // Store max val
            if (analogs[iAnalog] > m_maxVal)
                m_maxVal = analogs[iAnalog];
        }
    }

    // Check and add to the plot
    if (plotData.Ok()) {
        wxPen pen(wxColour(180, 180, 180), 1);

        // wxPlotPen_Type : wxPLOTPEN_NORMAL, wxPLOTPEN_ACTIVE, wxPLOTPEN_SELECTED, wxPLOTPEN_MAXTYPE
        plotData.SetPen(wxPLOTPEN_NORMAL, pen);
        // wxPlotSymbol_Type : wxPLOTSYMBOL_ELLIPSE, wxPLOTSYMBOL_RECTANGLE, wxPLOTSYMBOL_CROSS, wxPLOTSYMBOL_PLUS, wxPLOTSYMBOL_MAXTYPE
        plotData.SetSymbol(wxPLOTSYMBOL_CROSS, wxPLOTPEN_NORMAL, 5, 5, &pen, nullptr);

        plotData.SetDrawSymbols(true);
        plotData.SetDrawLines(false);

        // Add the curve
        bool select = false;
        bool send_event = false;
        plotctrl->AddCurve(plotData, select, send_event);
    } else {
        wxLogError(_("The analogs data couldn't be added to the plot"));
    }

    plotData.Destroy();
}

void asFramePlotTimeSeries::PlotBestAnalogs(int pointsNb)
{
    // Get a pointer to the plotctrl
    wxPlotCtrl *plotctrl = m_panelPlot->GetPlotCtrl();

    // Get forecast
    asResultsForecast *forecast = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast);

    // Loop over the analogs to set the color (from the less important to the best)
    for (int iAnalog = pointsNb - 1; iAnalog >= 0; iAnalog--) {
        // Get the total number of points
        int nbPoints = 0;
        for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
            a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
            if (analogs.size() > iAnalog)
                nbPoints++;
        }

        // Create plot data
        wxPlotData plotData;
        plotData.Create(nbPoints);
        int counter = 0;
        for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
            a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
            if (analogs.size() > iAnalog) {
                plotData.SetValue(counter, m_leadTimes[iLead], analogs[iAnalog]);
                counter++;

                // Store max val
                if (analogs[iAnalog] > m_maxVal)
                    m_maxVal = analogs[iAnalog];
            }
        }

        // Check and add to the plot
        if (plotData.Ok()) {
            // Color (from red to yellow)
            float ratio = (float) iAnalog / (float) (pointsNb - 1);
            wxPen pen(wxColor(255, ratio * 255, 0), 2);

            // wxPlotPen_Type : wxPLOTPEN_NORMAL, wxPLOTPEN_ACTIVE, wxPLOTPEN_SELECTED, wxPLOTPEN_MAXTYPE
            plotData.SetPen(wxPLOTPEN_NORMAL, pen);
            // wxPlotSymbol_Type : wxPLOTSYMBOL_ELLIPSE, wxPLOTSYMBOL_RECTANGLE, wxPLOTSYMBOL_CROSS, wxPLOTSYMBOL_PLUS, wxPLOTSYMBOL_MAXTYPE
            plotData.SetSymbol(wxPLOTSYMBOL_CROSS, wxPLOTPEN_NORMAL, 9, 9, &pen, nullptr);

            plotData.SetDrawSymbols(true);
            plotData.SetDrawLines(false);

            // Add the curve
            bool select = false;
            bool send_event = false;
            plotctrl->AddCurve(plotData, select, send_event);
        } else {
            wxLogError(_("The analogs data couldn't be added to the plot"));
        }

        plotData.Destroy();
    }
}

void asFramePlotTimeSeries::PlotClassicQuantiles()
{
    // Quantiles
    a1f pc(3);
    pc << 0.9f, 0.6f, 0.2f;
    std::vector<wxColour> colours;
    colours.emplace_back(0, 0, 175);
    colours.emplace_back(0, 83, 255);
    colours.emplace_back(0, 226, 255);

    // Get a pointer to the plotctrl
    wxPlotCtrl *plotctrl = m_panelPlot->GetPlotCtrl();

    // Get forecast
    asResultsForecast *forecast = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast);

    // Loop over the quantiles
    for (int iPc = 0; iPc < pc.size(); iPc++) {
        float thisQuantile = pc[iPc];

        // Create plot data
        wxPlotData plotData;
        plotData.Create(m_leadTimes.size());
        auto quantileRounded = (int) (asRound(thisQuantile * 100.0));
        plotData.SetFilename(wxString::Format("Quantile %d", quantileRounded));
        int counter = 0;
        for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
            a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
            float pcVal = asGetValueForQuantile(analogs, thisQuantile);
            plotData.SetValue(counter, m_leadTimes[iLead], pcVal);
            counter++;

            // Store max val
            if (pcVal > m_maxVal)
                m_maxVal = pcVal;
        }

        // Check and add to the plot
        if (plotData.Ok()) {
            wxPen pen(colours[iPc], 2, wxPENSTYLE_SOLID);

            // wxPlotPen_Type : wxPLOTPEN_NORMAL, wxPLOTPEN_ACTIVE, wxPLOTPEN_SELECTED, wxPLOTPEN_MAXTYPE
            plotData.SetPen(wxPLOTPEN_NORMAL, pen);

            plotData.SetDrawSymbols(false);
            plotData.SetDrawLines(true);

            // Add the curve
            bool select = false;
            bool send_event = false;
            plotctrl->AddCurve(plotData, select, send_event);
        } else {
            wxLogError(_("The quantiles couldn't be added to the plot"));
        }

        plotData.Destroy();
    }
}

void asFramePlotTimeSeries::PlotPastForecasts()
{
    for (int past = 0; past < m_forecastManager->GetPastForecastsNb(m_selectedMethod, m_selectedForecast); past++) {
        if (m_checkListPast->IsChecked((unsigned int) past)) {
            PlotPastForecast(past);
        }
    }
}

void asFramePlotTimeSeries::PlotPastForecast(int i)
{
    // Quantiles
    a1f pc(3);
    pc << 0.9f, 0.6f, 0.2f;
    std::vector<wxColour> colours;
    colours.emplace_back(152, 152, 222);
    colours.emplace_back(152, 187, 255);
    colours.emplace_back(153, 243, 254);

    // Get a pointer to the plotctrl
    wxPlotCtrl *plotctrl = m_panelPlot->GetPlotCtrl();

    // Get forecast
    asResultsForecast *forecast = m_forecastManager->GetPastForecast(m_selectedMethod, m_selectedForecast, i);
    int length = forecast->GetTargetDatesLength();
    a1f dates = forecast->GetTargetDates();

    // Loop over the quantiles
    for (int iPc = 0; iPc < pc.size(); iPc++) {
        float thisQuantile = pc[iPc];

        // Create plot data
        wxPlotData plotData;
        plotData.Create(length);
        //int quantileRounded = (int)(asRound(thisQuantile*100.0));
        int counter = 0;
        for (int iLead = 0; iLead < length; iLead++) {
            a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
            float pcVal = asGetValueForQuantile(analogs, thisQuantile);
            plotData.SetValue(counter, dates[iLead], pcVal);
            counter++;

            // Store max val
            if (pcVal > m_maxVal)
                m_maxVal = pcVal;
        }

        // Check and add to the plot
        if (plotData.Ok()) {
            wxPen pen(colours[iPc], 1, wxPENSTYLE_SOLID);

            // wxPlotPen_Type : wxPLOTPEN_NORMAL, wxPLOTPEN_ACTIVE, wxPLOTPEN_SELECTED, wxPLOTPEN_MAXTYPE
            plotData.SetPen(wxPLOTPEN_NORMAL, pen);

            plotData.SetDrawSymbols(false);
            plotData.SetDrawLines(true);

            // Add the curve
            bool select = false;
            bool send_event = false;
            plotctrl->AddCurve(plotData, select, send_event);
        } else {
            wxLogError(_("The quantiles couldn't be added to the plot"));
        }

        plotData.Destroy();
    }
}

void asFramePlotTimeSeries::PlotAllQuantiles()
{
    // Quantiles
    a1f pcUp(5);
    pcUp << 1, 0.9f, 0.8f, 0.7f, 0.6f;
    a1f pcDown(5);
    pcDown << 0, 0.1f, 0.2f, 0.3f, 0.4f;
    float pcMid = 0.5f;
    std::vector<wxColour> colours;
    colours.emplace_back(252, 252, 252);
    colours.emplace_back(220, 220, 220);
    colours.emplace_back(200, 200, 200);
    colours.emplace_back(150, 150, 150);
    colours.emplace_back(100, 100, 100);
    wxColour colourMid = wxColour(50, 50, 50);

    // Get a pointer to the plotctrl
    wxPlotCtrl *plotctrl = m_panelPlot->GetPlotCtrl();

    // Get forecast
    asResultsForecast *forecast = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast);

    // Loop over the quantiles to display as polygons
    for (int iPc = 0; iPc < pcUp.size(); iPc++) {
        float thisQuantileUp = pcUp[iPc];
        float thisQuantileDown = pcDown[iPc];

        // Create plot data
        wxPlotData plotData;
        plotData.Create(2 * m_leadTimes.size() + 1);
        int counter = 0;
        float bkpVal = 0;
        // Left to right
        for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
            a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
            float pcVal = asGetValueForQuantile(analogs, thisQuantileUp);
            plotData.SetValue(counter, m_leadTimes[iLead], pcVal);
            counter++;
            if (iLead == 0)
                bkpVal = pcVal;

            // Store max val
            if (pcVal > m_maxVal)
                m_maxVal = pcVal;
        }
        // Right to left
        for (int iLead = (int) m_leadTimes.size() - 1; iLead >= 0; iLead--) {
            a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
            float pcVal = asGetValueForQuantile(analogs, thisQuantileDown);
            plotData.SetValue(counter, m_leadTimes[iLead], pcVal);
            counter++;
        }
        // Close the polygon
        plotData.SetValue(counter, m_leadTimes[0], bkpVal);

        // Check and add to the plot
        if (plotData.Ok()) {
            wxPen pen(colours[iPc], 1, wxPENSTYLE_SOLID);
            wxBrush brush(colours[iPc], wxBRUSHSTYLE_SOLID);

            // wxPlotPen_Type : wxPLOTPEN_NORMAL, wxPLOTPEN_ACTIVE, wxPLOTPEN_SELECTED, wxPLOTPEN_MAXTYPE
            plotData.SetPen(wxPLOTPEN_NORMAL, pen);
            plotData.SetBrush(brush);

            plotData.SetDrawSymbols(false);
            plotData.SetDrawLines(false);
            plotData.SetDrawPolygon(true);

            // Add the curve
            bool select = false;
            bool send_event = false;
            plotctrl->AddCurve(plotData, select, send_event);
        } else {
            wxLogError(_("The quantiles couldn't be added to the plot"));
        }

        plotData.Destroy();

    }

    // Set the quantile to display as line
    float thisQuantile = pcMid;

    // Create plot data
    wxPlotData plotData;
    plotData.Create(m_leadTimes.size());
    int counter = 0;
    for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
        a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
        float pcVal = asGetValueForQuantile(analogs, thisQuantile);
        plotData.SetValue(counter, m_leadTimes[iLead], pcVal);
        counter++;
    }

    // Check and add to the plot
    if (plotData.Ok()) {
        wxPen pen(colourMid, 2, wxPENSTYLE_SOLID);

        // wxPlotPen_Type : wxPLOTPEN_NORMAL, wxPLOTPEN_ACTIVE, wxPLOTPEN_SELECTED, wxPLOTPEN_MAXTYPE
        plotData.SetPen(wxPLOTPEN_NORMAL, pen);

        plotData.SetDrawSymbols(false);
        plotData.SetDrawLines(true);
        plotData.SetDrawPolygon(false);

        // Add the curve
        bool select = false;
        bool send_event = false;
        plotctrl->AddCurve(plotData, select, send_event);
    } else {
        wxLogError(_("The quantiles couldn't be added to the plot"));
    }

    plotData.Destroy();

}

void asFramePlotTimeSeries::PlotInterpretation()
{
    // Get a pointer to the plotctrl
    wxPlotCtrl *plotctrl = m_panelPlot->GetPlotCtrl();

    // Get forecast
    asResultsForecast *forecast = m_forecastManager->GetForecast(m_selectedMethod, m_selectedForecast);

    // Create plot data
    wxPlotData plotData;
    plotData.Create(m_leadTimes.size());
    int counter = 0;

    for (unsigned int iLead = 0; iLead < m_leadTimes.size(); iLead++) {
        // Process quantiles
        a1f analogs = forecast->GetAnalogsValuesRaw(iLead, m_selectedStation);
        float pc30 = asGetValueForQuantile(analogs, 0.3f);
        float pc60 = asGetValueForQuantile(analogs, 0.6f);
        float pc90 = asGetValueForQuantile(analogs, 0.9f);

        // Follow the rules
        float val = 0;
        if (pc60 == 0) // if quantile 60% is null, there will be no rain
        {
            val = 0;
        } else if (pc30 > 0) // if quantile 30% is not null, it's gonna rain
        {
            val = pc90;
        } else {
            val = pc60;
        }

        plotData.SetValue(counter, m_leadTimes[iLead], val);
        counter++;

        // Store max val
        if (val > m_maxVal)
            m_maxVal = val;
    }

    // Check and add to the plot
    if (plotData.Ok()) {
        wxPen pen(wxColour(213, 0, 163), 2, wxPENSTYLE_SHORT_DASH);

        // wxPlotPen_Type : wxPLOTPEN_NORMAL, wxPLOTPEN_ACTIVE, wxPLOTPEN_SELECTED, wxPLOTPEN_MAXTYPE
        plotData.SetPen(wxPLOTPEN_NORMAL, pen);

        plotData.SetDrawSymbols(false);
        plotData.SetDrawLines(true);

        // Add the curve
        bool select = false;
        bool send_event = false;
        plotctrl->AddCurve(plotData, select, send_event);
    } else {
        wxLogError(_("The interpretation curve couldn't be added to the plot"));
    }

    plotData.Destroy();

}

