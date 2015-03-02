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
 * The Original Software is AtmoSwing. The Initial Developer of the 
 * Original Software is Pascal Horton of the University of Lausanne. 
 * All Rights Reserved.
 * 
 */

/*
 * Portions Copyright 2008-2013 University of Lausanne.
 */
 
#ifndef asGeoAreaRegularGrid_H
#define asGeoAreaRegularGrid_H

#include <asIncludes.h>
#include <asGeoArea.h>

class asGeoAreaRegularGrid: public asGeoArea
{
public:

    /** Default constructor
     * \param coosys The coordinate system
     * \param CornerUL The coordinates of the upper left corner
     * \param CornerUR The coordinates of the upper right corner
     * \param CornerLL The coordinates of the lower left corner
     * \param CornerLR The coordinates of the lower right corner
     * \param Xstep The step according to the X axis
     * \param Ystep The step according to the v axis
     * \param Level The height in hPa
     * \param Height The height in m
     * \param flatAllowed Allows the area to have a dimension that is null
     */
    asGeoAreaRegularGrid(const Coo &CornerUL, const Coo &CornerUR, const Coo &CornerLL, const Coo &CornerLR, double Xstep, double Ystep, float Level = asNONE, float Height = asNONE, int flatAllowed = asFLAT_ALLOWED);

    /** Alternative constructor
     * \param coosys The coordinate system
     * \param Xmin The left border
     * \param Xwidth The size on X axis
     * \param Xstep The step according to the X axis
     * \param Ymin The left border
     * \param Ywidth The size on Y axis
     * \param Ystep The step according to the v axis
     * \param Level The height in hPa
     * \param Height The height in m
     * \param flatAllowed Allows the area to have a dimension that is null
     */
    asGeoAreaRegularGrid(double Xmin, double Xwidth, double Xstep, double Ymin, double Ywidth, double Ystep, float Level = asNONE, float Height = asNONE, int flatAllowed = asFLAT_ALLOWED);

    /** Default destructor */
    virtual ~asGeoAreaRegularGrid();

    /** Access m_xstep
     * \return The current value of m_xstep
     */
    double GetXstep()
    {
        return m_xstep;
    }

    /** Access m_ystep
     * \return The current value of m_ystep
     */
    double GetYstep()
    {
        return m_ystep;
    }

    /** Get the size of the X axis
     * \return The size of the X axis
     */
    int GetXaxisPtsnb();

    /** Get the size of the Y axis
     * \return The size of the Y axis
     */
    int GetYaxisPtsnb();

    /** Get the X axis
     * \return The axis built on the boundaries and the step
     */
    Array1DDouble GetXaxis();

    /** Get the Y axis
     * \param step The step of the desired axis
     * \return The axis built on the boundaries and the step
     */
    Array1DDouble GetYaxis();

protected:

private:
    double m_xstep; //!< Member variable "m_xstep"
    double m_ystep; //!< Member variable "m_ystep"

    /** Tells if the area is a straight square compatible with the given step or not
     * \return True if the area is a straight square compatible with the given step
     */
    bool IsOnGrid(double step);

    /** Tells if the area is a straight square compatible with the given steps or not
     * \return True if the area is a straight square compatible with the given steps
     */
    bool IsOnGrid(double stepX, double stepY);
};

#endif // asGeoAreaRegularGrid_H
