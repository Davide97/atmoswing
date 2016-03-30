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

#include "asLog.h"

#include <wx/ffile.h>


asLog::asLog()
{
    m_logFile = NULL;
    m_logChain = NULL;
    m_level = 2;
    m_target = asLog::File;
    m_active = true;
    m_messageBoxOnError = false;
#if wxUSE_GUI
    m_messageBoxOnError = true;
#endif
    m_state = wxEmptyString;
}

asLog::~asLog()
{
    delete wxLog::SetActiveTarget(NULL); // Instead of deleting m_logChain
    if (m_logFile) {
        m_logFile->Close();
        m_logFile->Detach();
        wxDELETE(m_logFile);
    }
}

bool asLog::CreateFile(const wxString &fileName)
{
    // Create the log file
    wxDELETE(m_logFile);
    wxString logpath = asConfig::GetLogDir();
    logpath.Append(DS);
    logpath.Append(fileName);
    m_logFile = new wxFFile(logpath, "w");
    wxLogStderr *pLogFile = new wxLogStderr(m_logFile->fp());
    m_logChain = new wxLogChain(pLogFile);

    return true;
}

bool asLog::CreateFileOnly(const wxString &fileName)
{
    // Create the log file
    wxDELETE(m_logFile);
    wxString logpath = asConfig::GetLogDir();
    logpath.Append(DS);
    logpath.Append(fileName);
    m_logFile = new wxFFile(logpath, "w");
    wxLogStderr *pLogFile = new wxLogStderr(m_logFile->fp());
    wxLog::SetActiveTarget(pLogFile);

    return true;
}

bool asLog::CreateFileOnlyAtPath(const wxString &fullPath)
{
    // Create the log file
    wxDELETE(m_logFile);
    m_logFile = new wxFFile(fullPath, "w");
    wxLogStderr *pLogFile = new wxLogStderr(m_logFile->fp());
    wxLog::SetActiveTarget(pLogFile);

    return true;
}

bool asLog::IsVerbose()
{
    return m_level >= 3;
}

void asLog::SetLogNull()
{
    wxLogNull log;
}

void asLog::Suspend()
{
    wxLog::Suspend();
    wxLogMessage(_("Log suspended."));
}

void asLog::Resume()
{
    wxLog::Resume();
    wxLogMessage(_("Log resumed."));
}

void asLog::Flush()
{
#if wxUSE_GUI
    if (m_logChain) {
        m_logChain->Flush();
    } else if (m_logFile) {
        m_logFile->Flush();
    }
#else
    if(m_logFile) {
        m_logFile->Flush();
    }
#endif
}

void asLog::Error(const wxString &msg)
{
    if (m_level >= 1) {
        if (g_guiMode && m_active) {
            m_critSectionLog.Enter();
            wxLogError(msg);
            if (m_messageBoxOnError) {
#if wxUSE_GUI
                wxMessageBox(msg, _("An error occured"));
#endif
            }
            m_critSectionLog.Leave();
        } else {
            bool processed = false;

            if (m_target == asLog::File || m_target == asLog::Both) {
                wxLogError(msg);
                processed = true;
            }

            if (m_target == asLog::Screen || m_target == asLog::Both) {
                wxMessageOutput *msgOut = wxMessageOutput::Get();
                if (msgOut) {
                    wxString msgNew = _("Error: ") + msg;
                    msgOut->Printf(msgNew);
                    processed = true;
                } else {
                    wxFAIL_MSG(_("No wxMessageOutput object?"));
                }
            }

            if (!processed) {
                wxPrintf("Error: %s\n", msg);
            }
        }
    }
}

void asLog::Warning(const wxString &msg)
{
    if (m_level >= 2) {
        if (g_guiMode && m_active) {
            m_critSectionLog.Enter();
            wxLogWarning(msg);
            m_critSectionLog.Leave();
        } else {
            bool processed = false;

            if (m_target == asLog::File || m_target == asLog::Both) {
                wxLogWarning(msg);
                processed = true;
            }

            if (m_target == asLog::Screen || m_target == asLog::Both) {
                wxMessageOutput *msgOut = wxMessageOutput::Get();
                if (msgOut) {
                    wxString msgNew = _("Warning: ") + msg;
                    msgOut->Printf(msgNew);
                    processed = true;
                } else {
                    wxFAIL_MSG(_("No wxMessageOutput object?"));
                }
            }

            if (!processed) {
                wxPrintf("Warning: %s\n", msg);
            }
        }
    }
}

void asLog::Message(const wxString &msg, bool force)
{
    if ((m_level > 0 && force) || m_level >= 3) {
        if (g_guiMode && m_active) {
            m_critSectionLog.Enter();
            wxLogMessage(msg);
            m_critSectionLog.Leave();
        } else {
            bool processed = false;

            if (m_target == asLog::File || m_target == asLog::Both) {
                wxLogMessage(msg);
                processed = true;
            }

            if (m_target == asLog::Screen || m_target == asLog::Both) {
                wxMessageOutput *msgOut = wxMessageOutput::Get();
                if (msgOut) {
                    msgOut->Printf(msg);
                    processed = true;
                } else {
                    wxFAIL_MSG(_("No wxMessageOutput object?"));
                }
            }

            if (!processed) {
                wxPrintf("%s\n", msg);
            }
        }
    }
}

void asLog::State(const wxString &msg)
{
    m_critSectionLog.Enter();
    m_state = msg;
    m_critSectionLog.Leave();
    Message(msg); // Also log it
}
