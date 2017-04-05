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

#include "asGeoAreaCompositeRegularGrid.h"

asGeoAreaCompositeRegularGrid::asGeoAreaCompositeRegularGrid(const Coo &CornerUL, const Coo &CornerUR,
                                                             const Coo &CornerLL, const Coo &CornerLR, double Xstep,
                                                             double Ystep, float Level, float Height, int flatAllowed)
        : asGeoAreaCompositeGrid(CornerUL, CornerUR, CornerLL, CornerLR, Level, Height, flatAllowed)
{
    m_gridType = Regular;
    m_xStep = Xstep;
    m_yStep = Ystep;

    if (!IsOnGrid(Xstep, Ystep))
        asThrowException(_("The given area does not match a grid."));
}

asGeoAreaCompositeRegularGrid::asGeoAreaCompositeRegularGrid(double Xmin, double Xwidth, double Xstep, double Ymin,
                                                             double Ywidth, double Ystep, float Level, float Height,
                                                             int flatAllowed)
        : asGeoAreaCompositeGrid(Xmin, Xwidth, Ymin, Ywidth, Level, Height, flatAllowed)
{
    m_gridType = Regular;
    m_xStep = Xstep;
    m_yStep = Ystep;

    if (!IsOnGrid(Xstep, Ystep))
        asThrowException(_("The given area does not match a grid."));
}

asGeoAreaCompositeRegularGrid::~asGeoAreaCompositeRegularGrid()
{
    //dtor
}

bool asGeoAreaCompositeRegularGrid::GridsOverlay(asGeoAreaCompositeGrid *otherarea) const
{
    if (otherarea->GetGridType() != Regular)
        return false;
    asGeoAreaCompositeRegularGrid *otherareaRegular(dynamic_cast<asGeoAreaCompositeRegularGrid *>(otherarea));
    if (GetXstep() != otherareaRegular->GetXstep())
        return false;
    if (GetYstep() != otherareaRegular->GetYstep())
        return false;
    return true;
}

a1d asGeoAreaCompositeRegularGrid::GetXaxisComposite(int compositeNb)
{
    // Get axis size
    int size = GetXaxisCompositePtsnb(compositeNb);
    a1d Xaxis(size);

    // Build array
    double Xmin = GetComposite(compositeNb).GetXmin();
    if (compositeNb == 0) // Left border
    {
        double Xmax = GetComposite(compositeNb).GetXmax();
        double restovers = Xmax - Xmin - m_xStep * (size - 1);
        Xmin += restovers;
    }

    for (int i = 0; i < size; i++) {
        Xaxis(i) = Xmin + (double) i * m_xStep;
    }
    //wxASSERT_MSG(Xaxis(size-1)==GetComposite(compositeNb).GetXmax(), wxString::Format("Xaxis(size-1)=%f, GetComposite(%d).GetXmax()=%f", Xaxis(size-1), compositeNb, GetComposite(compositeNb).GetXmax()));  // Not always true

    return Xaxis;
}

a1d asGeoAreaCompositeRegularGrid::GetYaxisComposite(int compositeNb)
{
    // Get axis size
    int size = GetYaxisCompositePtsnb(compositeNb);
    a1d Yaxis(size);

    // Build array
    double Ymin = GetComposite(compositeNb).GetYmin();
    // FIXME (Pascal#3#): Check the compositeNb==0 in this case
    if (compositeNb == 0) // Not sure...
    {
        double Ymax = GetComposite(compositeNb).GetYmax();
        double restovers = Ymax - Ymin - m_yStep * (size - 1);
        Ymin += restovers;
    }

    for (int i = 0; i < size; i++) {
        Yaxis(i) = Ymin + i * m_yStep;
    }
    //wxASSERT(Yaxis(size-1)==GetComposite(compositeNb).GetYmax()); // Not always true

    return Yaxis;
}

int asGeoAreaCompositeRegularGrid::GetXaxisCompositePtsnb(int compositeNb)
{
    double diff = std::abs((GetComposite(compositeNb).GetXmax() - GetComposite(compositeNb).GetXmin())) / m_xStep;
    double size;
    double rest = modf(diff, &size);

    if (compositeNb == 0) // from 0
    {
        size += 1;
    } else if (compositeNb == 1) // to 360
    {
        size += 1;
    } else {
        asThrowException(_("The latitude split is not implemented yet."));
    }

    if (rest < 0.0000001 || rest > 0.999999) //Precision issue
    {
        return size + asTools::Round(rest);
    } else {
        return size;
    }
}

int asGeoAreaCompositeRegularGrid::GetYaxisCompositePtsnb(int compositeNb)
{
    double diff = std::abs((GetComposite(compositeNb).GetYmax() - GetComposite(compositeNb).GetYmin())) / m_yStep;
    double size;
    double rest = modf(diff, &size);
    size += 1;

    if (rest < 0.0000001 || rest > 0.999999) //Precision issue
    {
        return size + asTools::Round(rest);
    } else {
        asThrowException(_("The latitude split is not implemented yet."));
    }
}

double asGeoAreaCompositeRegularGrid::GetXaxisCompositeWidth(int compositeNb) const
{
    return std::abs(GetComposite(compositeNb).GetXmax() - GetComposite(compositeNb).GetXmin());
}

double asGeoAreaCompositeRegularGrid::GetYaxisCompositeWidth(int compositeNb) const
{
    return std::abs(GetComposite(compositeNb).GetYmax() - GetComposite(compositeNb).GetYmin());
}

double asGeoAreaCompositeRegularGrid::GetXaxisCompositeStart(int compositeNb) const
{
    // If only one composite
    if (GetNbComposites() == 1) {
        return GetComposite(compositeNb).GetXmin();
    }

    // If multiple composites
    if (compositeNb == 0) // from 0
    {
        // Composites are not forced on the grid. So we may need to adjust the split of the longitudes axis.
        double dX = std::abs(GetComposite(1).GetXmax() - GetComposite(1).GetXmin());

        if (std::fmod(dX, m_xStep) < 0.000001) {
            return GetComposite(compositeNb).GetXmin();
        } else {
            double rest = std::fmod(dX, m_xStep);
            return m_xStep - rest;
        }
    } else if (compositeNb == 1) // to 360
    {
        return GetComposite(compositeNb).GetXmin();
    } else {
        asThrowException(_("The latitude split is not implemented yet."));
    }
}

double asGeoAreaCompositeRegularGrid::GetYaxisCompositeStart(int compositeNb) const
{
    // If only one composite
    if (GetNbComposites() == 1) {
        return GetComposite(compositeNb).GetYmin();
    }

    // If multiple composites
    if (compositeNb == 0) // from 0
    {
        return GetComposite(compositeNb).GetYmin();
    } else if (compositeNb == 1) // to 360
    {
        return GetComposite(compositeNb).GetYmin();
    } else {
        asThrowException(_("The latitude split is not implemented yet."));
    }
}

double asGeoAreaCompositeRegularGrid::GetXaxisCompositeEnd(int compositeNb) const
{
    // If only one composite
    if (GetNbComposites() == 1) {
        return GetComposite(compositeNb).GetXmax();
    }

    // If multiple composites
    if (compositeNb == 1) // to 360
    {
        // Composites are not forced on the grid. So we may need to adjust the split of the longitudes axis.
        double dX = std::abs(GetComposite(1).GetXmax() - GetComposite(1).GetXmin());
        double rest = std::fmod(dX, m_xStep);
        if (rest < 0.000001) {
            return GetComposite(compositeNb).GetXmax();
        } else {
            return GetComposite(compositeNb).GetXmax() - rest;
        }
    } else if (compositeNb == 0) // from 0
    {
        return GetComposite(compositeNb).GetXmax();
    } else {
        asThrowException(_("The latitude split is not implemented yet."));
    }
}

double asGeoAreaCompositeRegularGrid::GetYaxisCompositeEnd(int compositeNb) const
{
    // If only one composite
    if (GetNbComposites() == 1) {
        return GetComposite(compositeNb).GetYmax();
    }

    // If multiple composites
    if (compositeNb == 1) // to 360
    {
        return GetComposite(compositeNb).GetYmax();
    } else if (compositeNb == 0) // from 0
    {
        return GetComposite(compositeNb).GetYmax();
    } else {
        asThrowException(_("The latitude split is not implemented yet."));
    }
}

bool asGeoAreaCompositeRegularGrid::IsOnGrid(double step) const
{
    if (!IsRectangle())
        return false;

    if (std::abs(std::fmod(GetXaxisWidth(), step)) > 0.0000001)
        return false;
    if (std::abs(std::fmod(GetYaxisWidth(), step)) > 0.0000001)
        return false;

    return true;
}

bool asGeoAreaCompositeRegularGrid::IsOnGrid(double stepX, double stepY) const
{
    if (!IsRectangle())
        return false;

    if (std::abs(std::fmod(GetXaxisWidth(), stepX)) > 0.0000001)
        return false;
    if (std::abs(std::fmod(GetYaxisWidth(), stepY)) > 0.0000001)
        return false;

    return true;
}
