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
 
#ifndef __asPanelSidebarStationsList__
#define __asPanelSidebarStationsList__

#include "asPanelSidebar.h"

#include "asIncludes.h"

/** Implementing asPanelSidebarStationsList */
class asPanelSidebarStationsList : public asPanelSidebar
{
public:
    /** Constructor */
    asPanelSidebarStationsList( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL);
    ~asPanelSidebarStationsList();

    void SetChoices(wxArrayString &arrayStation);

    wxChoice* GetChoiceCtrl()
    {
        return m_ChoiceStationSelection;
    }

private:
    wxChoice* m_ChoiceStationSelection;

    void OnStationSelection( wxCommandEvent& event );

};

#endif // __asPanelSidebarStationsList__
