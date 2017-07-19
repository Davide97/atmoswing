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

#include "asCatalogPredictands.h"

#include <asFileXml.h>


asCatalogPredictands::asCatalogPredictands(const wxString &filePath)
        : wxObject(),
          m_catalogFilePath(filePath),
          m_setId(wxEmptyString),
          m_name(wxEmptyString),
          m_description(wxEmptyString),
          m_start(0),
          m_end(0),
          m_timeZoneHours(0),
          m_timeStepHours(0),
          m_firstTimeStepHour(0),
          m_dataPath(wxEmptyString),
          m_coordSys(wxEmptyString),
          m_parameter(asPredictand::Precipitation),
          m_unit(asPredictand::mm),
          m_temporalResolution(asPredictand::Daily),
          m_spatialAggregation(asPredictand::Station)
{
    // Get the xml file path
    if (m_catalogFilePath.IsEmpty()) {
        wxLogError(_("No path was given for the predictand catalog."));
    }
}

asCatalogPredictands::~asCatalogPredictands()
{
    //dtor
}

bool asCatalogPredictands::Load()
{
    // Load xml file
    asFileXml xmlFile(m_catalogFilePath, asFile::ReadOnly);
    if (!xmlFile.Open())
        return false;

    if (!xmlFile.CheckRootElement())
        return false;

    // Get data
    wxXmlNode *nodeDataset = xmlFile.GetRoot()->GetChildren();
    while (nodeDataset) {
        if (nodeDataset->GetName() == "dataset") {

            wxXmlNode *nodeProp = nodeDataset->GetChildren();
            while (nodeProp) {
                if (nodeProp->GetName() == "id") {
                    m_setId = xmlFile.GetString(nodeProp);
                } else if (nodeProp->GetName() == "name") {
                    m_name = xmlFile.GetString(nodeProp);
                } else if (nodeProp->GetName() == "description") {
                    m_description = xmlFile.GetString(nodeProp);
                } else if (nodeProp->GetName() == "parameter") {
                    m_parameter = asPredictand::StringToParameterEnum(xmlFile.GetString(nodeProp));
                } else if (nodeProp->GetName() == "unit") {
                    m_unit = asPredictand::StringToUnitEnum(xmlFile.GetString(nodeProp));
                } else if (nodeProp->GetName() == "temporal_resolution") {
                    m_temporalResolution = asPredictand::StringToTemporalResolutionEnum(xmlFile.GetString(nodeProp));
                } else if (nodeProp->GetName() == "spatial_aggregation") {
                    m_spatialAggregation = asPredictand::StringToSpatialAggregationEnum(xmlFile.GetString(nodeProp));
                } else if (nodeProp->GetName() == "time_zone") {
                    m_timeZoneHours = xmlFile.GetFloat(nodeProp);
                } else if (nodeProp->GetName() == "start") {
                    m_start = asTime::GetTimeFromString(xmlFile.GetString(nodeProp), guess);
                } else if (nodeProp->GetName() == "end") {
                    m_end = asTime::GetTimeFromString(xmlFile.GetString(nodeProp), guess);
                } else if (nodeProp->GetName() == "first_time_step") {
                    m_firstTimeStepHour = xmlFile.GetFloat(nodeProp);
                } else if (nodeProp->GetName() == "path") {
                    m_dataPath = xmlFile.GetString(nodeProp);
                } else if (nodeProp->GetName() == "nan") {
                    m_nan.push_back(xmlFile.GetString(nodeProp));
                } else if (nodeProp->GetName() == "coordinate_system") {
                    m_coordSys = xmlFile.GetString(nodeProp);
                } else if (nodeProp->GetName() == "stations") {
                    wxXmlNode *nodeData = nodeProp->GetChildren();
                    while (nodeData) {
                        if (nodeData->GetName() == "station") {
                            DataStruct station;

                            wxXmlNode *nodeDetail = nodeData->GetChildren();
                            while (nodeDetail) {
                                if (nodeDetail->GetName() == "official_id") {
                                    station.officialId = xmlFile.GetString(nodeDetail);
                                } else if (nodeDetail->GetName() == "id") {
                                    wxString idStr = xmlFile.GetString(nodeDetail);
                                    long id;
                                    idStr.ToLong(&id);
                                    station.id = id;
                                } else if (nodeDetail->GetName() == "name") {
                                    station.name = xmlFile.GetString(nodeDetail);
                                } else if (nodeDetail->GetName() == "x_coordinate") {
                                    wxString coordXStr = xmlFile.GetString(nodeDetail);
                                    double x;
                                    coordXStr.ToDouble(&x);
                                    station.coord.x = x;
                                } else if (nodeDetail->GetName() == "y_coordinate") {
                                    wxString coordYStr = xmlFile.GetString(nodeDetail);
                                    double y;
                                    coordYStr.ToDouble(&y);
                                    station.coord.y = y;
                                } else if (nodeDetail->GetName() == "height") {
                                    wxString heightStr = xmlFile.GetString(nodeDetail);
                                    double height;
                                    heightStr.ToDouble(&height);
                                    station.height = (float) height;
                                } else if (nodeDetail->GetName() == "file_name") {
                                    station.fileName = xmlFile.GetString(nodeDetail);
                                } else if (nodeDetail->GetName() == "file_pattern") {
                                    station.filePattern = xmlFile.GetString(nodeDetail);
                                } else if (nodeDetail->GetName() == "start") {
                                    station.startDate = asTime::GetTimeFromString(xmlFile.GetString(nodeDetail), guess);
                                } else if (nodeDetail->GetName() == "end") {
                                    station.endDate = asTime::GetTimeFromString(xmlFile.GetString(nodeDetail), guess);
                                } else {
                                    xmlFile.UnknownNode(nodeDetail);
                                }

                                nodeDetail = nodeDetail->GetNext();
                            }
                            m_stations.push_back(station);

                        } else {
                            xmlFile.UnknownNode(nodeData);
                        }

                        nodeData = nodeData->GetNext();
                    }

                } else {
                    xmlFile.UnknownNode(nodeProp);
                }

                nodeProp = nodeProp->GetNext();
            }

        } else {
            xmlFile.UnknownNode(nodeDataset);
        }

        nodeDataset = nodeDataset->GetNext();
    }

    // Get the timestep
    switch (m_temporalResolution) {
        case (asPredictand::Daily):
            m_timeStepHours = 24.0;
            break;
        case (asPredictand::SixHourly):
            m_timeStepHours = 6.0;
            break;
        case (asPredictand::Hourly):
            m_timeStepHours = 1.0;
            break;
        case (asPredictand::OneHourlyMTW):
            m_timeStepHours = 1.0;
            break;
        case (asPredictand::ThreeHourlyMTW):
            m_timeStepHours = 3.0;
            break;
        case (asPredictand::SixHourlyMTW):
            m_timeStepHours = 6.0;
            break;
        case (asPredictand::TwelveHourlyMTW):
            m_timeStepHours = 12.0;
            break;
        case (asPredictand::TwoDays):
            m_timeStepHours = 48.0;
            break;
        case (asPredictand::ThreeDays):
            m_timeStepHours = 72.0;
            break;
        case (asPredictand::Weekly):
            m_timeStepHours = 168.0;
            break;
        default:
            wxFAIL;
            m_timeStepHours = 0;
    }

    xmlFile.Close();

    return true;
}
