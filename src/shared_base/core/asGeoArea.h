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
 
#ifndef ASGEOAREA_H
#define ASGEOAREA_H

#include <asIncludes.h>
#include <asGeo.h>

class asGeoArea: public asGeo
{
public:

    /** Default constructor
     * \param coosys The coordinate system
     * \param CornerUL The coordinates of the upper left corner
     * \param CornerUR The coordinates of the upper right corner
     * \param CornerLL The coordinates of the lower left corner
     * \param CornerLR The coordinates of the lower right corner
     * \param Level The height in hPa
     * \param Height The height in m
     * \param flatAllowed Allows the area to have a dimension that is null
     */
    asGeoArea(CoordSys coosys, const Coo &CornerUL, const Coo &CornerUR, const Coo &CornerLL, const Coo &CornerLR, float Level = asNONE, float Height = asNONE, int flatAllowed = asFLAT_FORBIDDEN);

    /** Alternative constructor
     * \param coosys The coordinate system
     * \param Umin The left border
     * \param Uwidth The size on U axis
     * \param Vmin The left border
     * \param Vwidth The size on V axis
     * \param Level The height in hPa
     * \param Height The height in m
     * \param flatAllowed Allows the area to have a dimension that is null
     */
    asGeoArea(CoordSys coosys, double Umin, double Uwidth, double Vmin, double Vwidth, float Level = asNONE, float Height = asNONE, int flatAllowed = asFLAT_FORBIDDEN);


    asGeoArea(CoordSys coosys, float Level = asNONE, float Height = asNONE);


    /** Default destructor */
    virtual ~asGeoArea();


    void Generate(double Umin, double Uwidth, double Vmin, double Vwidth, int flatAllowed = asFLAT_FORBIDDEN);

    /** Access m_CornerUL
     * \return The current value of m_CornerUL
     */
    Coo GetCornerUL()
    {
        return m_CornerUL;
    }

    /** Set m_CornerUL
     * \param val New value to set
     */
    void SetCornerUL(const Coo &val)
    {
        m_CornerUL = val;
        Init();
    }

    /** Access m_CornerUR
     * \return The current value of m_CornerUR
     */
    Coo GetCornerUR()
    {
        return m_CornerUR;
    }

    /** Set m_CornerUR
     * \param val New value to set
     */
    void SetCornerUR(const Coo &val)
    {
        m_CornerUR = val;
        Init();
    }

    /** Access m_CornerLL
     * \return The current value of m_CornerLL
     */
    Coo GetCornerLL()
    {
        return m_CornerLL;
    }

    /** Set m_CornerLL
     * \param val New value to set
     */
    void SetCornerLL(const Coo &val)
    {
        m_CornerLL = val;
        Init();
    }

    /** Access m_CornerLR
     * \return The current value of m_CornerLR
     */
    Coo GetCornerLR()
    {
        return m_CornerLR;
    }

    /** Set m_CornerLR
     * \param val New value to set
     */
    void SetCornerLR(const Coo &val)
    {
        m_CornerLR = val;
        Init();
    }

    /** Access m_Level
     * \return The current value of m_Level
     */
    double GetLevel()
    {
        return m_Level;
    }

    /** Set m_Level
     * \param val New value to set
     */
    void SetLevel(double val)
    {
        m_Level = val;
    }

    /** Gives the area U min coordinate
     * \return The value of the minimum on the U axis
     */
    double GetUmin();

    /** Gives the area U max coordinate
     * \return The value of the maximum on the U axis
     */
    double GetUmax();

    /** Gives the area U size
     * \return The value of the U axis size
     */
    double GetUwidth();

    /** Gives the area V min coordinate
     * \return The value of the minimum on the V axis
     */
    double GetVmin();

    /** Gives the area V max coordinate
     * \return The value of the maximum on the V axis
     */
    double GetVmax();

    /** Gives the area V size
     * \return The value of the V axis size
     */
    double GetVwidth();

    /** Gives the area center coordinates
     * \return The coordinates of the center
     */
    Coo GetCenter();

    /** Tells if the area is a straight rectangle or not
     * \return True if the area is a straight rectangle
     */
    bool IsRectangle();

    /** Convert projection
     * \param newcoordsys The destination projection
     */
    void ProjConvert(const CoordSys &newcoordsys);

protected:
    Coo m_CornerUL; //!< Member variable "m_CornerUL"
    Coo m_CornerUR; //!< Member variable "m_CornerUR"
    Coo m_CornerLL; //!< Member variable "m_CornerDL"
    Coo m_CornerLR; //!< Member variable "m_CornerDR"
    float m_Level; //!< Member variable "m_Level" hPa
    float m_Height; //!< Member variable "m_Height" m
    int m_FlatAllowed; //!< Member variable "m_FlatAllowed"

private:

    /** Process to initialization and checks */
    void Init();

    /** Check every point
     * \return True on success
     */
    bool DoCheckPoints();

    /** Check the consistency of the points
     * \return True on success
     */
    bool CheckConsistency();
};

#endif
