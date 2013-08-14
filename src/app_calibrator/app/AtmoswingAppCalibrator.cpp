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

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "AtmoswingAppCalibrator.h"
#if wxUSE_GUI
    #include "AtmoswingMainCalibrator.h"
#endif
#include "asMethodCalibratorClassicPlus.h"
#include "asMethodCalibratorClassicPlusVarExplo.h"
#include "asMethodCalibratorEvaluateAllScores.h"


IMPLEMENT_APP(AtmoswingAppCalibrator);

#include <wx/debug.h>
#include "wx/fileconf.h"
#include "wx/cmdline.h"
#include <asIncludes.h>
#include <asFileAscii.h>
#include <asMethodCalibratorSingle.h>
#include <asMethodCalibratorExhaustive.h>
#include <asMethodCalibratorClassic.h>
#if wxUSE_GUI
    #include "img_toolbar.h"
#endif

static const wxCmdLineEntryDesc g_cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, "v", "version", "print version" },
    { wxCMD_LINE_SWITCH, "s", "silent", "silent mode" },
    { wxCMD_LINE_SWITCH, "l", "local", "work in local directory" },
    { wxCMD_LINE_OPTION, "tn", "threadsnb", "number of threads to use" },
    { wxCMD_LINE_OPTION, "ll", "loglevel", "set a log level"
                                "\n \t\t\t\t 0: minimum"
                                "\n \t\t\t\t 1: errors"
                                "\n \t\t\t\t 2: warnings"
                                "\n \t\t\t\t 3: verbose" },
    { wxCMD_LINE_OPTION, "lt", "logtarget", "set log target"
                                "\n \t\t\t\t file: file only"
                                "\n \t\t\t\t prompt: command prompt"
                                "\n \t\t\t\t both: command prompt and file (default)" },
    { wxCMD_LINE_OPTION, "r", "runnumber", "choice of number associated with the run" },
    { wxCMD_LINE_OPTION, "fp", "fileparams", "choice of the calibration parameters file" },
    { wxCMD_LINE_OPTION, "fd", "filepredicand", "choice of the predictand DB" },
    { wxCMD_LINE_OPTION, "dc", "dirconfig", "choice of the config directory" },
    { wxCMD_LINE_OPTION, "di", "dirpredictors", "choice of the predictors directory" },
    { wxCMD_LINE_OPTION, "cm", "calibmethod", "choice of the calibration method"
                                "\n \t\t\t\t single: single assessment"
                                "\n \t\t\t\t exhaustive: exhaustive exploration"
                                "\n \t\t\t\t classic: classic calibration"
                                "\n \t\t\t\t classicp: classic+ calibration"
                                "\n \t\t\t\t varexplocp: variables exploration classic+"
                                "\n \t\t\t\t evalscores: Evaluate all scores" },
    { wxCMD_LINE_OPTION, "cpresizeite", "cpresizeite", "options ClassicPlusResizingIterations" },
    { wxCMD_LINE_OPTION, "cplatstepmap", "cplatstepmap", "options ClassicPlusStepsLatPertinenceMap" },
    { wxCMD_LINE_OPTION, "cplonstepmap", "cplonstepmap", "options ClassicPlusStepsLonPertinenceMap" },
    { wxCMD_LINE_OPTION, "cpprosseq", "cpprosseq", "options ClassicPlusProceedSequentially" },
    { wxCMD_LINE_OPTION, "varexpstep", "varexpstep", "options VariablesExploStep" },
    { wxCMD_LINE_OPTION, "mcrunsnb", "mcrunsnb", "options MonteCarloRandomNb" },
    { wxCMD_LINE_OPTION, "nmrunsnb", "nmrunsnb", "options NelderMeadNbRuns" },
    { wxCMD_LINE_OPTION, "nmrho", "nmrho", "options NelderMeadRho" },
    { wxCMD_LINE_OPTION, "nmchi", "nmchi", "options NelderMeadChi" },
    { wxCMD_LINE_OPTION, "nmgamma", "nmgamma", "options NelderMeadGamma" },
    { wxCMD_LINE_OPTION, "nmsigma", "nmsigma", "options NelderMeadSigma" },
    { wxCMD_LINE_OPTION, "gaopenatsel", "gaopenatsel", "options NaturalSelectionOperator" },
    { wxCMD_LINE_OPTION, "gaopecoupsel", "gaopecoupsel", "options CouplesSelectionOperator" },
    { wxCMD_LINE_OPTION, "gaopecross", "gaopecross", "options CrossoverOperator" },
    { wxCMD_LINE_OPTION, "gaopemut", "gaopemut", "options MutationOperator" },
    { wxCMD_LINE_OPTION, "garunsnb", "garunsnb", "options GAsRunNumbers" },
    { wxCMD_LINE_OPTION, "gapopsize", "gapopsize", "options GAsPopulationSize" },
    { wxCMD_LINE_OPTION, "gaconvsteps", "gaconvsteps", "options GAsConvergenceStepsNb" },
    { wxCMD_LINE_OPTION, "gaintermgen", "gaintermgen", "options GAsRatioIntermediateGeneration" },
    { wxCMD_LINE_OPTION, "ganatseltourp", "ganatseltourp", "options GAsNaturalSelectionTournamentProbability" },
    { wxCMD_LINE_OPTION, "gacoupseltournb", "gacoupseltournb", "options GAsCouplesSelectionTournamentNb" },
    { wxCMD_LINE_OPTION, "gacrossmultptnb", "gacrossmultptnb", "options GAsCrossoverMultiplePointsNb" },
    { wxCMD_LINE_OPTION, "gacrossblenptnb", "gacrossblenptnb", "options GAsCrossoverBlendingPointsNb" },
    { wxCMD_LINE_OPTION, "gacrossblenshareb", "gacrossblenshareb", "options GAsCrossoverBlendingShareBeta" },
    { wxCMD_LINE_OPTION, "gacrosslinptnb", "gacrosslinptnb", "options GAsCrossoverLinearPointsNb" },
    { wxCMD_LINE_OPTION, "gacrossheurptnb", "gacrossheurptnb", "options GAsCrossoverHeuristicPointsNb" },
    { wxCMD_LINE_OPTION, "gacrossheurshareb", "gacrossheurshareb", "options GAsCrossoverHeuristicShareBeta" },
    { wxCMD_LINE_OPTION, "gacrossbinptnb", "gacrossbinptnb", "options GAsCrossoverBinaryLikePointsNb" },
    { wxCMD_LINE_OPTION, "gacrossbinshareb", "gacrossbinshareb", "options GAsCrossoverBinaryLikeShareBeta" },
    { wxCMD_LINE_OPTION, "gamutunifcstp", "gamutunifcstp", "options GAsMutationsUniformConstantProbability" },
    { wxCMD_LINE_OPTION, "gamutnormcstp", "gamutnormcstp", "options GAsMutationsNormalConstantProbability" },
    { wxCMD_LINE_OPTION, "gamutnormcstdev", "gamutnormcstdev", "options GAsMutationsNormalConstantStdDevRatioRange" },
    { wxCMD_LINE_OPTION, "gamutunifvargens", "gamutunifvargens", "options GAsMutationsUniformVariableMaxGensNbVar" },
    { wxCMD_LINE_OPTION, "gamutunivarpstrt", "gamutunivarpstrt", "options GAsMutationsUniformVariableProbabilityStart" },
    { wxCMD_LINE_OPTION, "gamutunivarpend", "gamutunivarpend", "options GAsMutationsUniformVariableProbabilityEnd" },
    { wxCMD_LINE_OPTION, "gamutnormvargensp", "gamutnormvargensp", "options GAsMutationsNormalVariableMaxGensNbVarProb" },
    { wxCMD_LINE_OPTION, "gamutnormvargensd", "gamutnormvargensd", "options GAsMutationsNormalVariableMaxGensNbVarStdDev" },
    { wxCMD_LINE_OPTION, "gamutnormvarpstrt", "gamutnormvarpstrt", "options GAsMutationsNormalVariableProbabilityStart" },
    { wxCMD_LINE_OPTION, "gamutnormvarpend", "gamutnormvarpend", "options GAsMutationsNormalVariableProbabilityEnd" },
    { wxCMD_LINE_OPTION, "gamutnormvardstrt", "gamutnormvardstrt", "options GAsMutationsNormalVariableStdDevStart" },
    { wxCMD_LINE_OPTION, "gamutnormvardend", "gamutnormvardend", "options GAsMutationsNormalVariableStdDevEnd" },
    { wxCMD_LINE_OPTION, "gamutnonunip", "gamutnonunip", "options GAsMutationsNonUniformProb" },
    { wxCMD_LINE_OPTION, "gamutnonunigens", "gamutnonunigens", "options GAsMutationsNonUniformMaxGensNbVar" },
    { wxCMD_LINE_OPTION, "gamutnonuniminr", "gamutnonuniminr", "options GAsMutationsNonUniformMinRate" },
    { wxCMD_LINE_OPTION, "gamutmultiscalep", "gamutmultiscalep", "options GAsMutationsMultiScaleProb" },
    { wxCMD_LINE_OPTION, "s1wu", "s1wu", "options S1weights/wU" },
    { wxCMD_LINE_OPTION, "s1wv", "s1wv", "options S1weights/wV" },
    { wxCMD_LINE_OPTION, "savedatesstep", "savedatesstep", "options SaveAnalogDatesStep (with given number)" },
    { wxCMD_LINE_OPTION, "loaddatesstep", "loaddatesstep", "options LoadAnalogDatesStep (with given number)" },
    { wxCMD_LINE_OPTION, "savedatesallsteps", "savedatesallsteps", "options SaveAnalogDatesStep (all the steps)" },
    { wxCMD_LINE_OPTION, "savevalues", "savevalues", "options SaveAnalogValues" },
    { wxCMD_LINE_OPTION, "savescores", "savescores", "options SaveForecastScores" },
    { wxCMD_LINE_OPTION, "skipvalid", "skipvalid", "Skip the validation calculation" },

    { wxCMD_LINE_NONE }
};

bool AtmoswingAppCalibrator::OnInit()
{
    #if _DEBUG
        _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    #endif

    // Set application name
    wxString appName = "Atmoswing calibrator";
    wxApp::SetAppName(appName);

    g_AppViewer = false;
    g_AppForecaster = false; // None of them. This is correct and handles the ini file correctly.
    g_GuiMode = true;
    g_Local = false;
    m_CalibParamsFile = wxEmptyString;
    m_PredictandDB = wxEmptyString;
    m_PredictorsDir = wxEmptyString;
    m_CalibMethod = wxEmptyString;

    // Saving and loading of intermediate results files: reinitialized as it may be catastrophic to forget that it is enabled...
    wxConfigBase *pConfig = wxFileConfig::Get();
    pConfig->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep1", false);
    pConfig->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep2", false);
    pConfig->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep3", false);
    pConfig->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep4", false);
    pConfig->Write("/Calibration/IntermediateResults/SaveAnalogDatesAllSteps", false);
    pConfig->Write("/Calibration/IntermediateResults/SaveAnalogValues", false);
    pConfig->Write("/Calibration/IntermediateResults/SaveForecastScores", false);
    pConfig->Write("/Calibration/IntermediateResults/SaveFinalForecastScore", false);
    pConfig->Write("/Calibration/IntermediateResults/LoadAnalogDatesStep1", false);
    pConfig->Write("/Calibration/IntermediateResults/LoadAnalogDatesStep2", false);
    pConfig->Write("/Calibration/IntermediateResults/LoadAnalogDatesStep3", false);
    pConfig->Write("/Calibration/IntermediateResults/LoadAnalogDatesStep4", false);
    pConfig->Write("/Calibration/IntermediateResults/LoadAnalogDatesAllSteps", false);
    pConfig->Write("/Calibration/IntermediateResults/LoadAnalogValues", false);
    pConfig->Write("/Calibration/IntermediateResults/LoadForecastScores", false);
    pConfig->Write("/Calibration/IntermediateResults/LoadFinalForecastScore", false);

    // Call default behaviour (mandatory for command-line mode)
    if (!wxApp::OnInit()) // When false, we are in CL mode
        return false;

    #if wxUSE_GUI
    if (g_GuiMode)
    {
        // Check that it is the unique instance
        bool multipleInstances;
        pConfig->Read("/Standard/MultiInstances", &multipleInstances, false);

        if (!multipleInstances)
        {
            const wxString instanceName = wxString::Format(wxT("AtmoswingCalibrator-%s"),wxGetUserId().c_str());
            m_SingleInstanceChecker = new wxSingleInstanceChecker(instanceName);
            if ( m_SingleInstanceChecker->IsAnotherRunning() )
            {
                wxMessageBox(_("Program already running, aborting."));
                return false;
            }
        }

        // Following for GUI only
        wxInitAllImageHandlers();

        // Initialize images
        initialize_images_toolbar();

        // Create frame
        AtmoswingFrameCalibrator* frame = new AtmoswingFrameCalibrator(0L);
        frame->OnInit();

        #ifdef __WXMSW__
            frame->SetIcon(wxICON(myicon)); // To Set App Icon
        #endif
        frame->Show();
        SetTopWindow(frame);
    }
    #endif

    return true;
}

bool AtmoswingAppCalibrator::InitForCmdLineOnly()
{
    g_GuiMode = false;
    g_UnitTesting = false;
    g_SilentMode = true;
    g_VerboseMode = false;
    g_Responsive = false;

    // Prepare local path
    wxString localPath = wxFileName::GetCwd() + DS;
    if (g_RunNb>0)
    {
        localPath.Append("runs");
        localPath.Append(DS);
        localPath.Append(wxString::Format("%d", g_RunNb));
        localPath.Append(DS);
    }

    if (g_Local)
    {
        wxString fullPath = localPath;
        fullPath.Append("AtmoswingCalibrator.log");
        Log().CreateFileOnlyAtPath(fullPath);
    }
    else
    {
        Log().CreateFileOnly("AtmoswingCalibrator.log");
    }

    Log().DisableMessageBoxOnError();

    if (g_Local)
    {
        wxString dirConfig = wxFileName::GetCwd()+DS+"config"+DS;
        wxString dirData = wxFileName::GetCwd()+DS+"data"+DS;

        wxConfigBase *pConfig = wxFileConfig::Get();

        // Define the default preferences
        pConfig->Write("/Standard/MultiInstances", true);
        pConfig->Write("/Standard/GuiOptions", 0l);
        pConfig->Write("/Standard/Responsive", false);
        pConfig->Write("/Standard/DisplayLogWindow", false);
        pConfig->Write("/Standard/ProcessingThreadsPriority", 100);
        pConfig->Write("/StandardPaths/CatalogPredictorsArchiveFilePath", dirConfig+"CatalogPredictorsArchive.xml");
        pConfig->Write("/StandardPaths/CatalogPredictorsRealtimeFilePath", dirConfig+"CatalogPredictorsRealtime.xml");
        pConfig->Write("/StandardPaths/CatalogPredictandsFilePath", dirConfig+"CatalogPredictands.xml");
        pConfig->Write("/StandardPaths/DataPredictandDBDir", dirData);
        pConfig->Write("/StandardPaths/IntermediateResultsDir", localPath+"temp");
        pConfig->Write("/StandardPaths/CalibrationResultsDir", localPath+"results");
        pConfig->Write("/StandardPaths/ArchivePredictorsDir", dirData);
        pConfig->Write("/StandardPaths/ForecastParametersDir", dirConfig);
        pConfig->Write("/ProcessingOptions/ProcessingLinAlgebra", (long)asCOEFF);
        pConfig->Write("/Calibration/ParallelEvaluations", true);
        pConfig->Write("/Calibration/GeneticAlgorithms/AllowElitismForTheBest", true);
        pConfig->Write("/Standard/AllowMultithreading", true);
        pConfig->Write("/ProcessingOptions/ProcessingMethod", (long)asMULTITHREADS);

        pConfig->Flush();

    }

    return true;
}

void AtmoswingAppCalibrator::OnInitCmdLine(wxCmdLineParser& parser)
{
    wxAppConsole::OnInitCmdLine(parser);

    // From http://wiki.wxwidgets.org/Command-Line_Arguments
    parser.SetDesc (g_cmdLineDesc);
    // Must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("-"));
}

bool AtmoswingAppCalibrator::OnCmdLineParsed(wxCmdLineParser& parser)
{
    // From http://wiki.wxwidgets.org/Command-Line_Arguments

    /*
     * General options
     */

    // Check if the user asked for command-line help
    if (parser.Found("h"))
    {
        parser.Usage();
        return false;
    }

    // Check if the user asked for the version
    if (parser.Found("v"))
    {
        wxMessageOutput* msgOut = wxMessageOutput::Get();
        if ( msgOut )
        {
            wxString msg;
            wxString date(wxString::FromAscii(__DATE__));
            msg.Printf("Atmoswing, (c) University of Lausanne, 2011. Version %s, %s", g_Version.c_str(), (const wxChar*) date);

            msgOut->Printf( wxT("%s"), msg.c_str() );
        }
        else
        {
            wxFAIL_MSG( _("No wxMessageOutput object?") );
        }

        return false; // We don0t want to continue
    }

    // Check for a run number
    wxString runNbStr = wxEmptyString;
    long runNb = 0;
    if (parser.Found("r", & runNbStr))
    {
        runNbStr.ToLong(&runNb);
        g_RunNb = (int)runNb;
    }

    // Local mode
    if (parser.Found("l"))
    {
        g_Local = true;
        wxString localPath = wxFileName::GetCwd() + DS;
        if (g_RunNb>0)
        {
            localPath.Append("runs");
            localPath.Append(DS);
            localPath.Append(wxString::Format("%d", g_RunNb));
            localPath.Append(DS);

            // Create directory
            wxFileName userDir = wxFileName::DirName(localPath);
            userDir.Mkdir(wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);
        }

        // Create local ini file
        wxString iniPath = localPath;
        iniPath.Append("Atmoswing.ini");

        // Set the local config object
        wxFileConfig *pConfig = new wxFileConfig("Atmoswing",wxEmptyString,iniPath,iniPath,wxCONFIG_USE_LOCAL_FILE);
        wxFileConfig::Set(pConfig);
    }
    else
    {
        // Create user directory
        wxFileName userDir = wxFileName::DirName(asConfig::GetUserDataDir());
        userDir.Mkdir(wxS_DIR_DEFAULT,wxPATH_MKDIR_FULL);

        // Set the local config object
        wxFileConfig *pConfig = new wxFileConfig("Atmoswing",wxEmptyString,asConfig::GetUserDataDir()+"Atmoswing.ini",asConfig::GetUserDataDir()+"Atmoswing.ini",wxCONFIG_USE_LOCAL_FILE);
        wxFileConfig::Set(pConfig);
    }

    // Check for a log level option
    wxString logLevelStr = wxEmptyString;
    if (parser.Found("ll", & logLevelStr))
    {
        long logLevel = -1;
        logLevelStr.ToLong(&logLevel);

        // Check and apply
        if (logLevel==0)
        {
            Log().SetLevel(0);
        }
        else if (logLevel==1)
        {
            Log().SetLevel(1);
        }
        else if (logLevel==2)
        {
            Log().SetLevel(2);
        }
        else if (logLevel==3)
        {
            Log().SetLevel(3);
        }
        else
        {
            Log().SetLevel(2);
        }
    }
    else
    {
        long logLevel = wxFileConfig::Get()->Read("/Standard/LogLevel", 2l);
        Log().SetLevel((int)logLevel);
    }

    // Check for the log target option
    wxString logTargetStr = wxEmptyString;
    if (parser.Found("lt", & logTargetStr))
    {
        // Check and apply
        if (logTargetStr.IsSameAs("file", false))
        {
            Log().SetTarget(asLog::File);
        }
        else if (logTargetStr.IsSameAs("screen", false))
        {
            Log().SetTarget(asLog::Screen);
        }
        else if (logTargetStr.IsSameAs("both", false))
        {
            Log().SetTarget(asLog::Both);
        }
        else
        {
            Log().SetTarget(asLog::Both);

            wxMessageOutput* msgOut = wxMessageOutput::Get();
            if ( msgOut )
            {
                msgOut->Printf( _("The given log target (%s) does not correspond to any possible option."), logTargetStr.c_str() );
            }
        }
    }

    // Check if the user asked for the silent mode
    if (parser.Found("s"))
    {
        Log().SetTarget(asLog::File);
    }

    // Check for a calibration params file
    wxString threadsNb = wxEmptyString;
    if (parser.Found("tn", & threadsNb))
    {
        wxFileConfig::Get()->Write("/Standard/ProcessingMaxThreadNb", threadsNb);
    }

    // Check for a calibration params file
    if (parser.Found("fp", & m_CalibParamsFile))
    {
        if (g_Local)
        {
            m_CalibParamsFile = wxFileName::GetCwd() + DS + m_CalibParamsFile;
        }

        if (!wxFileName::FileExists(m_CalibParamsFile))
        {
            asLogError(wxString::Format(_("The given calibration file (%s) couldn't be found."), m_CalibParamsFile.c_str()));
            return false;
        }
    }

    // Check for a calibration predictand DB
    if (parser.Found("fd", & m_PredictandDB))
    {
        if (g_Local)
        {
            m_PredictandDB = wxFileName::GetCwd() + DS + m_PredictandDB;
        }

        if (!wxFileName::FileExists(m_PredictandDB))
        {
            asLogError(wxString::Format(_("The given predictand DB (%s) couldn't be found."), m_PredictandDB.c_str()));
            return false;
        }
    }

    // Check for a config directory
    wxString configDir;
    if (parser.Found("dc", & configDir))
    {
        if (g_Local)
        {
            configDir = wxFileName::GetCwd() + DS + configDir;
        }

        if (!wxFileName::DirExists(configDir))
        {
            asLogError(wxString::Format(_("The given config directory (%s) couldn't be found."), configDir.c_str()));
            return false;
        }

        wxString CatalogPredictorsArchiveFilePath = configDir+DS+"CatalogPredictorsArchive.xml";
        wxString CatalogPredictandsFilePath = configDir+DS+"CatalogPredictands.xml";
        // Not checking CatalogPredictorsRealtimeFilePath as we should not using it

        if (!wxFileName::FileExists(CatalogPredictorsArchiveFilePath))
        {
            asLogError(wxString::Format(_("The file (%s) couldn't be found."), CatalogPredictorsArchiveFilePath.c_str()));
            return false;
        }
        if (!wxFileName::FileExists(CatalogPredictandsFilePath))
        {
            asLogError(wxString::Format(_("The file (%s) couldn't be found."), CatalogPredictandsFilePath.c_str()));
            return false;
        }

        wxFileConfig::Get()->Write("/StandardPaths/CatalogPredictorsArchiveFilePath", CatalogPredictorsArchiveFilePath);
        wxFileConfig::Get()->Write("/StandardPaths/CatalogPredictandsFilePath", CatalogPredictandsFilePath);
    }

    // Check for a predictors directory
    if (parser.Found("di", & m_PredictorsDir))
    {
        if (g_Local)
        {
            m_PredictorsDir = wxFileName::GetCwd() + DS + m_PredictorsDir;
        }

        if (!wxFileName::DirExists(m_PredictorsDir))
        {
            asLogError(wxString::Format(_("The given predictors directory (%s) couldn't be found."), m_PredictorsDir.c_str()));
            return false;
        }
    }

    /*
     * Methods options
     */

    wxString option = wxEmptyString;

    // Classic+ calibration
    if (parser.Found("cpresizeite", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/ClassicPlus/ResizingIterations", option);
    }

    if (parser.Found("cplatstepmap", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/ClassicPlus/StepsLatPertinenceMap", option);
    }

    if (parser.Found("cplonstepmap", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/ClassicPlus/StepsLonPertinenceMap", option);
    }

    if (parser.Found("cpprosseq", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/ClassicPlus/ProceedSequentially", option);
    }

    // Variables exploration
    if (parser.Found("varexpstep", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/VariablesExplo/Step", option);
    }

    // S1 weighted
    if (parser.Found("s1wu", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/S1weights/wU", option);
    }

    if (parser.Found("s1wv", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/S1weights/wV", option);
    }

    // Skip validation option
    if (parser.Found("skipvalid", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/SkipValidation", option);
    }

    // Saving/loading options
    wxString saveDatesStep;
    if (parser.Found("savedatesstep", & saveDatesStep))
    {
        long step = -1;
        saveDatesStep.ToLong(&step);
        int intStep = (int)step;

        switch (intStep)
        {
            case 1:
            {
                wxFileConfig::Get()->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep1", true);
                break;
            }
            case 2:
            {
                wxFileConfig::Get()->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep2", true);
                break;
            }
            case 3:
            {
                wxFileConfig::Get()->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep3", true);
                break;
            }
            case 4:
            {
                wxFileConfig::Get()->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep4", true);
                break;
            }
            default:
            {
                asLogError(wxString::Format(_("Wrong given step for the intermediate saving option (%d)."), intStep));
            }
        }
    }

    wxString loadDatesStep;
    if (parser.Found("loaddatesstep", & loadDatesStep))
    {
        long step = -1;
        loadDatesStep.ToLong(&step);
        int intStep = (int)step;

        switch (intStep)
        {
            case 1:
            {
                wxFileConfig::Get()->Write("/Calibration/IntermediateResults/LoadAnalogDatesStep1", true);
                break;
            }
            case 2:
            {
                wxFileConfig::Get()->Write("/Calibration/IntermediateResults/LoadAnalogDatesStep2", true);
                break;
            }
            case 3:
            {
                wxFileConfig::Get()->Write("/Calibration/IntermediateResults/LoadAnalogDatesStep3", true);
                break;
            }
            case 4:
            {
                wxFileConfig::Get()->Write("/Calibration/IntermediateResults/LoadAnalogDatesStep4", true);
                break;
            }
            default:
            {
                asLogError(wxString::Format(_("Wrong given step for the intermediate loading option (%d)."), intStep));
            }
        }
    }

    // Save analogs values
    if (parser.Found("savedatesallsteps", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep1", true);
        wxFileConfig::Get()->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep2", true);
        wxFileConfig::Get()->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep3", true);
        wxFileConfig::Get()->Write("/Calibration/IntermediateResults/SaveAnalogDatesStep4", true);
    }

    // Save analogs values
    if (parser.Found("savevalues", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/IntermediateResults/SaveAnalogValues", option);
    }

    // Save forecast scores
    if (parser.Found("savescores", & option))
    {
        wxFileConfig::Get()->Write("/Calibration/IntermediateResults/SaveForecastScores", option);
    }

    /*
     * Method choice
     */

    // Check for a calibration method option
    if (parser.Found("cm", & m_CalibMethod))
    {
        if(!InitForCmdLineOnly()) return false;
        asLogMessage(wxString::Format(_("Given calibration method: %s"), m_CalibMethod.c_str()));
    }

    return wxAppConsole::OnCmdLineParsed(parser);
}

int AtmoswingAppCalibrator::OnExit()
{
	#if wxUSE_GUI
		// Instance checker
		delete m_SingleInstanceChecker;
	#endif

    // Config file (from wxWidgets samples)
    delete wxFileConfig::Set((wxFileConfig *) NULL);

    // Delete threads manager and log
//    DeleteThreadsManager();
    DeleteLog();

// TODO (phorton#5#): Do the cleanup here
// Override this member function for any processing which needs to be done as the application is about to exit.
// OnExit is called after destroying all application windows and controls, but before wxWidgets cleanup.

    #ifdef _CRTDBG_MAP_ALLOC
        _CrtDumpMemoryLeaks();
    #endif

    return 0;
}

int AtmoswingAppCalibrator::OnRun()
{
    if (!g_GuiMode)
    {
        if (m_CalibParamsFile.IsEmpty())
        {
            asLogError(_("The parameters file is not given."));
            return 1001;
        }

        if (m_PredictandDB.IsEmpty())
        {
            asLogError(_("The predictand DB is not given."));
            return 1002;
        }

        if (m_PredictorsDir.IsEmpty())
        {
            asLogError(_("The predictors directory is not given."));
            return 1003;
        }

        wxMessageOutput* msgOut = wxMessageOutput::Get();

        try
        {
            if (m_CalibMethod.IsSameAs("single", false))
            {
                asMethodCalibratorSingle calibrator;
                calibrator.SetParamsFilePath(m_CalibParamsFile);
                calibrator.SetPredictandDBFilePath(m_PredictandDB);
                calibrator.SetPredictorDataDir(m_PredictorsDir);
                calibrator.Manager();
                calibrator.Cleanup();
            }
            else if (m_CalibMethod.IsSameAs("exhaustive", false))
            {
                asMethodCalibratorExhaustive calibrator;
                calibrator.SetParamsFilePath(m_CalibParamsFile);
                calibrator.SetPredictandDBFilePath(m_PredictandDB);
                calibrator.SetPredictorDataDir(m_PredictorsDir);
                calibrator.Manager();
                calibrator.Cleanup();
            }
            else if (m_CalibMethod.IsSameAs("classic", false))
            {
                asMethodCalibratorClassic calibrator;
                calibrator.SetParamsFilePath(m_CalibParamsFile);
                calibrator.SetPredictandDBFilePath(m_PredictandDB);
                calibrator.SetPredictorDataDir(m_PredictorsDir);
                calibrator.Manager();
                calibrator.Cleanup();
            }
            else if (m_CalibMethod.IsSameAs("classicp", false))
            {
                asMethodCalibratorClassicPlus calibrator;
                calibrator.SetParamsFilePath(m_CalibParamsFile);
                calibrator.SetPredictandDBFilePath(m_PredictandDB);
                calibrator.SetPredictorDataDir(m_PredictorsDir);
                calibrator.Manager();
                calibrator.Cleanup();
            }
            else if (m_CalibMethod.IsSameAs("varexplocp", false))
            {
                asMethodCalibratorClassicPlusVarExplo calibrator;
                calibrator.SetParamsFilePath(m_CalibParamsFile);
                calibrator.SetPredictandDBFilePath(m_PredictandDB);
                calibrator.SetPredictorDataDir(m_PredictorsDir);
                calibrator.Manager();
                calibrator.Cleanup();
            }
            else if (m_CalibMethod.IsSameAs("evalscores", false))
            {
                asMethodCalibratorEvaluateAllScores calibrator;
                calibrator.SetParamsFilePath(m_CalibParamsFile);
                calibrator.SetPredictandDBFilePath(m_PredictandDB);
                calibrator.SetPredictorDataDir(m_PredictorsDir);
                calibrator.Manager();
                calibrator.Cleanup();
            }
            else
            {
                if ( msgOut )
                {
                    msgOut->Printf( "Wrong calibration method selection (%s).", m_CalibMethod.c_str() );
                }
            }
        }
        catch(bad_alloc& ba)
        {
            wxString msg(ba.what(), wxConvUTF8);
            asLogError(wxString::Format(_("Bad allocation caught: %s"), msg.c_str()));
            return 1011;
        }
        catch(asException& e)
        {
            wxString fullMessage = e.GetFullMessage();
            if (!fullMessage.IsEmpty())
            {
                asLogError(fullMessage);
            }
            asLogError(_("Failed to process the calibration."));
            return 1010;
        }

        asLogMessageImportant(_("Calibration over."));

        return 0;
    }

    return wxApp::OnRun();
}

bool AtmoswingAppCalibrator::OnExceptionInMainLoop()
{
    asLogError(_("An exception occured in the main loop"));
    return false;
}

void AtmoswingAppCalibrator::OnFatalException()
{
    asLogError(_("An fatal exception occured"));
}

void AtmoswingAppCalibrator::OnUnhandledException()
{
    asLogError(_("An unhandled exception occured"));
}
