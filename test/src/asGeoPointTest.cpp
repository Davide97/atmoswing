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

#include "include_tests.h"
#include "asGeoPoint.h"

#include "UnitTest++.h"

namespace
{

TEST(ConstructorDefault)
{
	wxPrintf("Testing geo points management...\n");
	
    Coo Point;
    Point.x = 7;
    Point.y = 46;
    asGeoPoint geopoint(Point);

    CHECK_CLOSE(7, geopoint.GetX(), 0.001);
    CHECK_CLOSE(46, geopoint.GetY(), 0.001);
}

TEST(ConstructorOther)
{
    double x = 7;
    double y = 46;
    asGeoPoint geopoint(x, y);

    CHECK_CLOSE(7, geopoint.GetX(), 0.001);
    CHECK_CLOSE(46, geopoint.GetY(), 0.001);
}

TEST(ConstructorOutBoundsLon)
{
    double x = -10;
    double y = 46;
    asGeoPoint geopoint(x, y);

    CHECK_CLOSE(350, geopoint.GetX(), 0.001);
    CHECK_CLOSE(46, geopoint.GetY(), 0.001);
}

TEST(ConstructorOutBoundsLat)
{
    double x = 10;
    double y = -100;
    asGeoPoint geopoint(x, y);

    CHECK_CLOSE(190, geopoint.GetX(), 0.001);
    CHECK_CLOSE(-80, geopoint.GetY(), 0.001);
}

TEST(SetCooOutBounds)
{
    asGeoPoint geopoint(0, 0);
    Coo Point;
    Point.x = -10;
    Point.y = 46;
    geopoint.SetCoo(Point);

    CHECK_CLOSE(350, geopoint.GetX(), 0.001);
    CHECK_CLOSE(46, geopoint.GetY(), 0.001);
}

}
