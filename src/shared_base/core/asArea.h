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

#ifndef ASAREA_H
#define ASAREA_H

#include <asIncludes.h>

class asArea
        : public wxObject
{
public:
    asArea(const Coo &cornerUL, const Coo &cornerUR, const Coo &cornerLL, const Coo &cornerLR,
           int flatAllowed = asFLAT_FORBIDDEN);

    asArea(double xMin, double xWidth, double yMin, double yWidth, int flatAllowed = asFLAT_FORBIDDEN);

    asArea();

    ~asArea() override = default;

    bool CheckPoint(Coo &point, int changesAllowed = asEDIT_FORBIDDEN);

    Coo GetCornerUL() const
    {
        return m_cornerUL;
    }

    void SetCornerUL(const Coo &val)
    {
        m_cornerUL = val;
        Init();
    }

    Coo GetCornerUR() const
    {
        return m_cornerUR;
    }

    void SetCornerUR(const Coo &val)
    {
        m_cornerUR = val;
        Init();
    }

    Coo GetCornerLL() const
    {
        return m_cornerLL;
    }

    void SetCornerLL(const Coo &val)
    {
        m_cornerLL = val;
        Init();
    }

    Coo GetCornerLR() const
    {
        return m_cornerLR;
    }

    void SetCornerLR(const Coo &val)
    {
        m_cornerLR = val;
        Init();
    }

    virtual double GetXmin() const;

    virtual double GetXmax() const;

    double GetXwidth() const;

    virtual double GetYmin() const;

    virtual double GetYmax() const;

    double GetYwidth() const;

    virtual bool IsRectangle() const;

protected:
    Coo m_cornerUL;
    Coo m_cornerUR;
    Coo m_cornerLL;
    Coo m_cornerLR;
    int m_flatAllowed;

    virtual void Init();

private:
    bool DoCheckPoints();

    bool CheckConsistency();
};

#endif
