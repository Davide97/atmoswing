/** 
 *
 *  This file is part of the AtmoSwing software.
 *
 *  Copyright (c) 2008-2012  University of Lausanne, Pascal Horton (pascal.horton@unil.ch). 
 *  All rights reserved.
 *
 *  THIS CODE, SOFTWARE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY  
 *  OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 */
 
#include "asFrameMain.h"

#include "asFramePredictandDB.h"
#include "asFramePreferences.h"
#include "asFrameXmlEditor.h"
#include "asFrameAbout.h"
#include <asPanelForecastingModel.h>
#include <asFileForecastingModels.h>
#include "img_bullets.h"
#include "img_toolbar.h"


BEGIN_EVENT_TABLE(asFrameMain, wxFrame)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_STARTING, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_RUNNING, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_FAILED, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_SUCCESS, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_DOWNLOADING, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_DOWNLOADED, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_LOADING, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_LOADED, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_SAVING, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_SAVED, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_PROCESSING, asFrameMain::OnStatusMethodUpdate)
	EVT_COMMAND(wxID_ANY, asEVT_STATUS_PROCESSED, asFrameMain::OnStatusMethodUpdate)
END_EVENT_TABLE()


asFrameMain::asFrameMain( wxWindow* parent )
:
asFrameMainVirtual( parent )
{
    m_Forecaster = NULL;

    // Toolbar
    m_ToolBar->AddTool( asID_RUN, wxT("Run"), img_run, img_run, wxITEM_NORMAL, _("Run forecast"), _("Run forecast now"), NULL );
    m_ToolBar->AddTool( asID_CANCEL, wxT("Cancel"), img_run_cancel, img_run_cancel, wxITEM_NORMAL, _("Cancel forecast"), _("Cancel current forecast"), NULL );
	m_ToolBar->AddTool( asID_DB_CREATE, wxT("Database creation"), img_database_run, img_database_run, wxITEM_NORMAL, _("Database creation"), _("Database creation"), NULL );
	m_ToolBar->AddTool( asID_PREFERENCES, wxT("Preferences"), img_preferences, img_preferences, wxITEM_NORMAL, _("Preferences"), _("Preferences"), NULL );
	m_ToolBar->AddSeparator();
	m_ToolBar->AddTool( asID_FRAME_VIEWER, wxT("Open viewer"), img_frame_viewer, img_frame_viewer, wxITEM_NORMAL, _("Go to viewer"), _("Go to viewer"), NULL );
	m_ToolBar->Realize();

	// Buttons
	m_BpButtonNow->SetBitmapLabel(img_clock_now);
	m_BpButtonAdd->SetBitmapLabel(img_plus);

    // Create panels manager
    m_PanelsManager = new asPanelsManagerForecastingModels();

    // Connect events
    this->Connect( asID_RUN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( asFrameMain::LaunchForecasting ) );
    this->Connect( asID_CANCEL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( asFrameMain::CancelForecasting ) );
	this->Connect( asID_DB_CREATE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( asFrameMain::OpenFramePredictandDB ) );
	this->Connect( asID_PREFERENCES, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( asFrameMain::OpenFramePreferences ) );
	this->Connect( asID_FRAME_VIEWER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( asFrameMain::GoToViewer ) );

    // Icon
#ifdef __WXMSW__
    SetIcon(wxICON(myicon));
#endif
}

asFrameMain::~asFrameMain()
{
    wxDELETE(m_PanelsManager);

    // Disconnect events
    this->Disconnect( asID_RUN, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( asFrameMain::LaunchForecasting ) );
    this->Disconnect( asID_CANCEL, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( asFrameMain::CancelForecasting ) );
	this->Disconnect( asID_DB_CREATE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( asFrameMain::OpenFramePredictandDB ) );
	this->Disconnect( asID_PREFERENCES, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( asFrameMain::OpenFramePreferences ) );
	this->Disconnect( asID_FRAME_VIEWER, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( asFrameMain::GoToViewer ) );
}

void asFrameMain::OnInit()
{
    DisplayLogLevelMenu();
    SetPresentDate();
    ModelsListLoadDefault();
}

void asFrameMain::Update()
{
    DisplayLogLevelMenu();
}

void asFrameMain::OpenFrameXmlEditor( wxCommandEvent& event )
{
    //asFrameXmlEditor* frame = new asFrameXmlEditor(this, asWINDOW_XML_EDITOR);
    //frame->Fit();
    //frame->Show();
}

void asFrameMain::GoToViewer( wxCommandEvent& event )
{
    wxConfigBase *pConfig = wxFileConfig::Get();
    wxString ViewerPath = pConfig->Read("/StandardPaths/ViewerPath", wxEmptyString);

    if(ViewerPath.IsEmpty())
    {
        asLogError(_("Please set the path to the viewer in the preferences."));
        return;
    }

    // Execute
    long processId = wxExecute(ViewerPath, wxEXEC_ASYNC);

    if (processId==0) // if wxEXEC_ASYNC
    {
        asLogError(_("The viewer could not be executed. Please check the path in the preferences."));
    }
}

void asFrameMain::OpenFramePredictandDB( wxCommandEvent& event )
{
    asFramePredictandDB* frame = new asFramePredictandDB(this);
    frame->Fit();
    frame->Show();
}

void asFrameMain::OpenFramePreferences( wxCommandEvent& event )
{
    asFramePreferences* frame = new asFramePreferences(this);
    frame->Fit();
    frame->Show();
}

void asFrameMain::OpenFrameAbout( wxCommandEvent& event )
{
    asFrameAbout* frame = new asFrameAbout(this);
    frame->Fit();
    frame->Show();
}

void asFrameMain::OnShowLog( wxCommandEvent& event )
{
    wxASSERT(m_LogWindow);
    m_LogWindow->DoShow();
}

void asFrameMain::OnLogLevel1( wxCommandEvent& event )
{
    Log().SetLevel(1);
    m_MenuLogLevel->FindItemByPosition(0)->Check(true);
    m_MenuLogLevel->FindItemByPosition(1)->Check(false);
    m_MenuLogLevel->FindItemByPosition(2)->Check(false);
    wxFileConfig::Get()->Write("/Standard/LogLevel", 1l);
    wxWindow *prefFrame = FindWindowById(asWINDOW_PREFERENCES);
    if (prefFrame) prefFrame->Update();
}

void asFrameMain::OnLogLevel2( wxCommandEvent& event )
{
    Log().SetLevel(2);
    m_MenuLogLevel->FindItemByPosition(0)->Check(false);
    m_MenuLogLevel->FindItemByPosition(1)->Check(true);
    m_MenuLogLevel->FindItemByPosition(2)->Check(false);
    wxFileConfig::Get()->Write("/Standard/LogLevel", 2l);
    wxWindow *prefFrame = FindWindowById(asWINDOW_PREFERENCES);
    if (prefFrame) prefFrame->Update();
}

void asFrameMain::OnLogLevel3( wxCommandEvent& event )
{
    Log().SetLevel(3);
    m_MenuLogLevel->FindItemByPosition(0)->Check(false);
    m_MenuLogLevel->FindItemByPosition(1)->Check(false);
    m_MenuLogLevel->FindItemByPosition(2)->Check(true);
    wxFileConfig::Get()->Write("/Standard/LogLevel", 3l);
    wxWindow *prefFrame = FindWindowById(asWINDOW_PREFERENCES);
    if (prefFrame) prefFrame->Update();
}

void asFrameMain::OnStatusMethodUpdate( wxCommandEvent& event )
{
    int eventInt = event.GetInt();
    wxEventType eventType = event.GetEventType();

    if(eventType==asEVT_STATUS_STARTING)
    {
        m_PanelsManager->SetForecastingModelsAllLedsOff();
    }
    else if(eventType==asEVT_STATUS_FAILED)
    {
        m_PanelsManager->SetForecastingModelLedError(eventInt);
    }
    else if(eventType==asEVT_STATUS_SUCCESS)
    {
        m_PanelsManager->SetForecastingModelLedDone(eventInt);
    }
    else if(eventType==asEVT_STATUS_DOWNLOADING)
    {
        m_LedDownloading->SetColour(awxLED_YELLOW);
        m_LedDownloading->SetState(awxLED_ON);
        m_LedDownloading->Refresh();
    }
    else if(eventType==asEVT_STATUS_DOWNLOADED)
    {
        m_LedDownloading->SetColour(awxLED_GREEN);
        m_LedDownloading->SetState(awxLED_ON);
        m_LedDownloading->Refresh();
    }
    else if(eventType==asEVT_STATUS_LOADING)
    {
        m_LedLoading->SetColour(awxLED_YELLOW);
        m_LedLoading->SetState(awxLED_ON);
        m_LedLoading->Refresh();
    }
    else if(eventType==asEVT_STATUS_LOADED)
    {
        m_LedLoading->SetColour(awxLED_GREEN);
        m_LedLoading->SetState(awxLED_ON);
        m_LedLoading->Refresh();
    }
    else if(eventType==asEVT_STATUS_SAVING)
    {
        m_LedSaving->SetColour(awxLED_YELLOW);
        m_LedSaving->SetState(awxLED_ON);
        m_LedSaving->Refresh();
    }
    else if(eventType==asEVT_STATUS_SAVED)
    {
        m_LedSaving->SetColour(awxLED_GREEN);
        m_LedSaving->SetState(awxLED_ON);
        m_LedSaving->Refresh();
    }
    else if(eventType==asEVT_STATUS_PROCESSING)
    {
        m_LedProcessing->SetColour(awxLED_YELLOW);
        m_LedProcessing->SetState(awxLED_ON);
        m_LedProcessing->Refresh();
    }
    else if(eventType==asEVT_STATUS_PROCESSED)
    {
        m_LedProcessing->SetColour(awxLED_GREEN);
        m_LedProcessing->SetState(awxLED_ON);
        m_LedProcessing->Refresh();
    }
    else if( (eventType==asEVT_STATUS_RUNNING) )
    {
        m_PanelsManager->SetForecastingModelLedRunning(eventInt);
        m_LedDownloading->SetState(awxLED_OFF);
        m_LedLoading->SetState(awxLED_OFF);
        m_LedProcessing->SetState(awxLED_OFF);
        m_LedSaving->SetState(awxLED_OFF);
    }
    else
    {
        asLogError(_("Event not identified."));
    }
}

void asFrameMain::DisplayLogLevelMenu()
{
    // Set log level in the menu
    int logLevel = (int)wxFileConfig::Get()->Read("/Standard/LogLevel", 2l);
    m_MenuLogLevel->FindItemByPosition(0)->Check(false);
    m_MenuLogLevel->FindItemByPosition(1)->Check(false);
    m_MenuLogLevel->FindItemByPosition(2)->Check(false);
    switch (logLevel)
    {
    case 1:
        m_MenuLogLevel->FindItemByPosition(0)->Check(true);
        Log().SetLevel(1);
        break;
    case 2:
        m_MenuLogLevel->FindItemByPosition(1)->Check(true);
        Log().SetLevel(2);
        break;
    case 3:
        m_MenuLogLevel->FindItemByPosition(2)->Check(true);
        Log().SetLevel(3);
        break;
    default:
        m_MenuLogLevel->FindItemByPosition(1)->Check(true);
        Log().SetLevel(2);
    }
}

void asFrameMain::LaunchForecasting( wxCommandEvent& event )
{
    // Get date
    double forecastDate = GetForecastDate();
    wxString forecastDateStr = asTime::GetStringTime(forecastDate, "DD.MM.YYYY hh:mm");
    asLogMessage(wxString::Format(_("Trying to run the forecast for the date %s"), forecastDateStr.c_str()));

    // Save models list to file
    if(!ModelsListSaveAsCurrent()) return;

    if (m_Forecaster)
    {
        asLogError(_("The forecaster is already processing."));
        return;
    }

    // Launch forecasting
    m_Forecaster = new asMethodForecasting(this);
    m_Forecaster->SetForecastDate(forecastDate);
    if(!m_Forecaster->Manager())
    {
        asLogError(_("Failed processing the forecast."));

// FIXME (Pascal#1#): Send email in case of failure.
        /*
        wxSMTP *smtp = new wxSMTP(NULL);
        smtp->SetHost("smtp.gmail.com");
        wxEmailMessage *msg = new wxEmailMessage("BLABLABLA",
                                                 "Your code really sucks.\n"
                                                 "Fix your code",
                                                 "pascal.horton.job@gmail.com");
        msg->AddAlternative("<html><body><h1>Bug report</h1>\n"
                            "Your code <b>really</b> sucks <p>Fix your code</html>",
                            "text","html");
        msg->AddTo("pascal.horton.job@gmail.com");
        smtp->Send(msg);

        wxSleep(60);
        smtp->Destroy();
        */

        m_Forecaster->Cleanup();
        wxDELETE(m_Forecaster);

        return;
    }

    double realForecastDate = m_Forecaster->GetForecastDate();
    SetForecastDate(realForecastDate);

    // Log message
    wxString realForecastDateStr = asTime::GetStringTime(realForecastDate, "DD.MM.YYYY hh:mm");
    asLogMessage(wxString::Format(_("Forecast processed for the date %s"), realForecastDateStr.c_str()));

    m_Forecaster->Cleanup();
    wxDELETE(m_Forecaster);
}

void asFrameMain::CancelForecasting( wxCommandEvent& event )
{
    if (m_Forecaster)
    {
        m_Forecaster->Cancel();
    }
}

void asFrameMain::AddForecastingModel( wxCommandEvent& event )
{
    Freeze();
    asPanelForecastingModel *panel = new asPanelForecastingModel( m_ScrolledWindowModels );
    panel->Layout();
    m_SizerModels->Add( panel, 0, wxALL|wxEXPAND, 5 );
    Layout(); // For the scrollbar
    Thaw();

    // Add to the array
    m_PanelsManager->AddPanel(panel);
}

void asFrameMain::OnSetPresentDate( wxCommandEvent& event )
{
    SetPresentDate();
}

void asFrameMain::SetPresentDate( )
{
    // Set the present date in the calendar and the hour field
    wxDateTime nowWx = asTime::NowWxDateTime(asUTM);
    TimeStruct nowStruct = asTime::NowTimeStruct(asUTM);
    wxString hourStr = wxString::Format("%d", nowStruct.hour);
    m_CalendarForecastDate->SetDate(nowWx);
    m_TextCtrlForecastHour->SetValue(hourStr);
}

double asFrameMain::GetForecastDate( )
{
    // Date
    wxDateTime forecastDateWx = m_CalendarForecastDate->GetDate();
    double forecastDate = asTime::GetMJD(forecastDateWx);

    // Hour
    wxString forecastHourStr = m_TextCtrlForecastHour->GetValue();
    double forecastHour = 0;
    forecastHourStr.ToDouble(&forecastHour);

    // Sum
    double total = forecastDate + forecastHour/(double)24;

    return total;
}

void asFrameMain::SetForecastDate( double date )
{
    // Calendar
    wxDateTime forecastDateWx = asTime::GetWxDateTime(date);
    m_CalendarForecastDate->SetDate(forecastDateWx);
    // Hour
    TimeStruct forecastDateStruct = asTime::GetTimeStruct(date);
    wxString hourStr = wxString::Format("%d", forecastDateStruct.hour);
    m_TextCtrlForecastHour->SetValue(hourStr);
}

void asFrameMain::ModelsListSaveAsDefault( wxCommandEvent& event )
{
    // Set file path
    wxString filePath = asConfig::GetDefaultUserConfigDir() + "DefaultForecastingModelsList.xml";

    asFileForecastingModels file(filePath, asFile::Replace);
    if(!file.Open())
    {
        asLogError(_("Cannot create the models list file."));
        return;
    }
    if(!file.InsertRootElement())
    {
        asLogError(_("Cannot insert document root element."));
        return;
    }
    if(!m_PanelsManager->GenerateXML(file))
    {
        asLogError(_("Forecasting models listing file couldn't be generated."));
        return;
    }
    file.Save();
}

bool asFrameMain::ModelsListSaveAsCurrent()
{
    // Set file path
    wxString filePath = asConfig::GetDefaultUserConfigDir() + "CurrentForecastingModelsList.xml";

    asFileForecastingModels file(filePath, asFile::Replace);
    if(!file.Open())
    {
        asLogError(_("Cannot create the models list file."));
        return false;
    }
    if(!file.InsertRootElement())
    {
        asLogError(_("Cannot insert document root element."));
        return false;
    }
    if(!m_PanelsManager->GenerateXML(file))
    {
        asLogError(_("Forecasting models listing file couldn't be generated."));
        return false;
    }
    file.Save();

    return true;
}

void asFrameMain::ModelsListLoadDefault( wxCommandEvent& event )
{
    ModelsListLoadDefault();
}

void asFrameMain::ModelsListLoadDefault()
{
    Freeze();

    // Cleanup the actual panels
    m_PanelsManager->Clear();

    // Set file path
    wxString filePath = asConfig::GetDefaultUserConfigDir() + "DefaultForecastingModelsList.xml";

    asFileForecastingModels file(filePath, asFile::ReadOnly);
    if(!file.Open())
    {
        asLogError(_("Cannot open the models list file."));
        Thaw();
        Layout();
        return;
    }

    try
    {
        // Parse the file
        bool xmlOK = true;
        if(!file.GoToRootElement()) xmlOK = false;
        if(!file.GoToFirstNodeWithPath("ModelsList")) xmlOK = false;
        if(!file.GoToFirstNodeWithPath("Model")) xmlOK = false;
        if (!xmlOK)
        {
            asLogError(_("Error while parsing the models list file."));
            Thaw();
            Layout();
            return;
        }

        while(true)
        {
            wxString modelName = file.GetThisElementAttributeValueText("name");
            wxString modelDescr = file.GetThisElementAttributeValueText("description");
            wxString parametersFileName = file.GetFirstElementAttributeValueText("ParametersFileName", "value");
            wxString predictandDB = file.GetFirstElementAttributeValueText("PredictandDB", "value");
            wxString predictorsArchiveDir = file.GetFirstElementAttributeValueText("PredictorsArchiveDir", "value");

            // Create the panel
            asPanelForecastingModel *panel = new asPanelForecastingModel( m_ScrolledWindowModels );
            panel->SetModelName(modelName);
            panel->SetModelDescription(modelDescr);
            panel->SetParametersFileName(parametersFileName);
            panel->SetPredictandDBName(predictandDB);
            panel->SetPredictorsArchiveDir(predictorsArchiveDir);
            panel->ReducePanel();
            panel->Layout();
            m_SizerModels->Add( panel, 0, wxALL|wxEXPAND, 5 );
            // Add to the array
            m_PanelsManager->AddPanel(panel);

            // Find the next model
            if (!file.GoToNextSameNode()) break;
        }
    }
    catch (asException &e)
    {
        asLogError(e.GetFullMessage());
        asLogError(_("Cannot get the forecasting models list from the file."));
        Thaw();
        Layout();
        return;
    }

    Layout(); // For the scrollbar
    Thaw();

}

void asFrameMain::ModelsListSave( wxCommandEvent& event )
{
    asDialogFileSaver filePicker(this, _("Please select the destination file path."));
    if (filePicker.ShowModal() == wxID_SAVE)
    {
        wxString filePath = filePicker.GetPath();

        asFileForecastingModels file(filePath, asFile::Replace);
        if(!file.Open())
        {
            asLogError(_("Cannot create the models list file."));
            return;
        }
        if(!file.InsertRootElement())
        {
            asLogError(_("Cannot insert document root element."));
            return;
        }
        if(!m_PanelsManager->GenerateXML(file))
        {
            asLogError(_("Forecasting models listing file couldn't be generated."));
        }
        file.Save();
    }
}

void asFrameMain::ModelsListLoad( wxCommandEvent& event )
{
    asDialogFilePicker filePicker(this, _("Please select the file to load."));
    if (filePicker.ShowModal() == wxID_OK)
    {
        Freeze();

        // Cleanup the actual panels
        m_PanelsManager->Clear();

        wxString filePath = filePicker.GetPath();

        asFileForecastingModels file(filePath, asFile::ReadOnly);
        if(!file.Open())
        {
            asLogError(_("Cannot open the models list file."));
            return;
        }

        try
        {
            // Parse the file
            bool xmlOK = true;
            if(!file.GoToRootElement()) xmlOK = false;
            if(!file.GoToFirstNodeWithPath("ModelsList")) xmlOK = false;
            if(!file.GoToFirstNodeWithPath("Model")) xmlOK = false;
            if (!xmlOK)
            {
                asLogError(_("Error while parsing the models list file."));
                return;
            }

            while(true)
            {
                wxString modelName = file.GetThisElementAttributeValueText("name");
                wxString modelDescr = file.GetThisElementAttributeValueText("description");
                wxString parametersFileName = file.GetFirstElementAttributeValueText("ParametersFileName", "value");
                wxString predictandDB = file.GetFirstElementAttributeValueText("PredictandDB", "value");
                wxString predictorsArchiveDir = file.GetFirstElementAttributeValueText("PredictorsArchiveDir", "value");

                // Create the panel
                asPanelForecastingModel *panel = new asPanelForecastingModel( m_ScrolledWindowModels );
                panel->SetModelName(modelName);
                panel->SetModelDescription(modelDescr);
                panel->SetParametersFileName(parametersFileName);
                panel->SetPredictandDBName(predictandDB);
                panel->SetPredictorsArchiveDir(predictorsArchiveDir);
                panel->ReducePanel();
                m_SizerModels->Add( panel, 0, wxALL|wxEXPAND, 5 );
                // Add to the array
                m_PanelsManager->AddPanel(panel);

                // Find the next model
                if (!file.GoToNextSameNode()) break;
            }
        }
        catch (asException &e)
        {
            asLogError(e.GetFullMessage());
            asLogError(_("Cannot get the forecasting models list from the file."));
            return;
        }

        Thaw();

        Layout(); // For the scrollbar
    }

}
