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
 */

#include "asException.h"

asException::asException() noexcept
        : std::exception(),
          m_message(wxEmptyString),
          m_fileName(wxEmptyString),
          m_lineNum(0)
{
#ifndef UNIT_TESTING
    wxLogError(_("An exception occured."));
#endif
}

asException::asException(const wxString &message, const char *filename, unsigned int line)
        : std::exception(),
          m_message(message),
          m_fileName(wxString(filename, wxConvUTF8)),
          m_lineNum(line)
{
#ifndef UNIT_TESTING
    wxLogError(_("An exception occured: %s. File: %s (%d)"), m_message, m_fileName, m_lineNum);
#endif
}

asException::asException(const std::string &message, const char *filename, unsigned int line)
        : std::exception(),
          m_message(wxString(message.c_str(), wxConvUTF8)),
          m_fileName(wxString(filename, wxConvUTF8)),
          m_lineNum(line)
{
#ifndef UNIT_TESTING
    wxLogError(_("An exception occured: %s. File: %s (%d)"), m_message, m_fileName, m_lineNum);
#endif
}

asException::asException(const char *message, const char *filename, unsigned int line)
        : std::exception(),
          m_message(wxString(message, wxConvUTF8)),
          m_fileName(wxString(filename, wxConvUTF8)),
          m_lineNum(line)
{
#ifndef UNIT_TESTING
    wxLogError(_("An exception occured: %s. File: %s (%d)"), m_message, m_fileName, m_lineNum);
#endif
}

asException::asException(const wxString &message, const char *filename, unsigned int line, asException prevexception)
        : std::exception(),
          m_message(message),
          m_fileName(wxString(filename, wxConvUTF8)),
          m_lineNum(line)
{
    m_previous = prevexception.m_previous;

    PrevExceptions previous;
    previous.message = prevexception.m_message;
    previous.fileName = prevexception.m_fileName;
    previous.lineNum = prevexception.m_lineNum;
    m_previous.push_back(&previous);

#ifndef UNIT_TESTING
    wxLogError(_("An exception occured: %s. File: %s (%d)"), m_message, m_fileName, m_lineNum);
#endif
}

asException::asException(const std::string &message, const char *filename, unsigned int line, asException prevexception)
        : std::exception(),
          m_message(wxString(message.c_str(), wxConvUTF8)),
          m_fileName(wxString(filename, wxConvUTF8)),
          m_lineNum(line)
{
    m_previous = prevexception.m_previous;

    PrevExceptions previous;
    previous.message = prevexception.m_message;
    previous.fileName = prevexception.m_fileName;
    previous.lineNum = prevexception.m_lineNum;
    m_previous.push_back(&previous);

#ifndef UNIT_TESTING
    wxLogError(_("An exception occured: %s. File: %s (%d)"), m_message, m_fileName, m_lineNum);
#endif
}

asException::asException(const char *message, const char *filename, unsigned int line, asException prevexception)
        : std::exception(),
          m_message(wxString(message, wxConvUTF8)),
          m_fileName(wxString(filename, wxConvUTF8)),
          m_lineNum(line)
{
    m_previous = prevexception.m_previous;

    PrevExceptions previous;
    previous.message = prevexception.m_message;
    previous.fileName = prevexception.m_fileName;
    previous.lineNum = prevexception.m_lineNum;
    m_previous.push_back(&previous);

#ifndef UNIT_TESTING
    wxLogError(_("An exception occured: %s. File: %s (%d)"), m_message, m_fileName, m_lineNum);
#endif
}

wxString asException::GetFullMessage() const
{
    wxString fullmessage;

    if (HasChild()) {
        int prevnb = m_previous.size();

        for (int i = 0; i < prevnb; i++) {
            int prevlinenum;
            wxString prevmessage;
            wxString prevfilename;

            prevlinenum = m_previous[i]->lineNum;
            prevmessage = m_previous[i]->message;
            prevfilename = m_previous[i]->fileName;
            prevmessage.Replace("\n", " // ");
            prevfilename = prevfilename.AfterLast('/');
            prevfilename = prevfilename.AfterLast('\\');

            if (!prevmessage.IsEmpty() && !prevfilename.IsEmpty()) {
                fullmessage.Append(wxString::Format(_("%s\n    File: %s\n    Line: %d\n\n"), prevmessage, prevfilename,
                                                    prevlinenum));
            }
        }
    }

    int currlinenum = m_lineNum;
    wxString currmessage = m_message;
    wxString currfilename = m_fileName;
    currmessage.Replace("\n", " // ");
    currfilename = currfilename.AfterLast('/');
    currfilename = currfilename.AfterLast('\\');

    fullmessage.Append(wxString::Format(_("%s\n    File: %s\n    Line: %d\n\n"), currmessage, currfilename,
                                        currlinenum));

    return fullmessage;
}
