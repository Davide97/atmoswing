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
 
#ifndef ASCATALOG_H
#define ASCATALOG_H

#include <asIncludes.h>

class asGeo;


class asCatalog: public wxObject
{
public:


    //!< Structure for datasets IDs
    struct DatasetIdList
    {
        VectorString Id;
        VectorString Name;
        VectorString Description;
    };

    //!< Structure for data IDs
    struct DataIdListStr
    {
        VectorString Id;
        VectorString Name;
    };

    //!< Structure for data IDs
    struct DataIdListInt
    {
        VectorInt Id;
        VectorString Name;
    };

    /** Default constructor */
    asCatalog(const wxString &alternateFilePath = wxEmptyString);

    /** Default destructor */
    virtual ~asCatalog();

    /** Get the datasets IDs
     * \param Purpose Predictor or Predictand
     * \param AlternateFilePath The file path if not the default file
     * \return The datasets IDs list
     */
    static DatasetIdList GetDatasetIdList(DataPurpose Purpose, const wxString &AlternateFilePath = wxEmptyString);

    /** Get the data IDs
     * \param Purpose Predictor or Predictand
     * \param DataSetId The dataset ID
     * \param AlternateFilePath The file path if not the default file
     * \return The data IDs list
     */
    static DataIdListStr GetDataIdListStr(DataPurpose Purpose, const wxString &DataSetId, const wxString &AlternateFilePath = wxEmptyString);

    /** Get the data IDs
     * \param Purpose Predictor or Predictand
     * \param DataSetId The dataset ID
     * \param AlternateFilePath The file path if not the default file
     * \return The data IDs list
     */
    static DataIdListInt GetDataIdListInt(DataPurpose Purpose, const wxString &DataSetId, const wxString &AlternateFilePath = wxEmptyString);

    /** Get the data list as wxArrayString
     * \param Purpose Predictor or Predictand
     * \param DataSetId The dataset ID
     * \param AlternateFilePath The file path if not the default file
     * \return The data IDs list
     */
    static wxArrayString GetDataIdListwxArrayString(DataPurpose Purpose, const wxString &DataSetId, const wxString &AlternateFilePath = wxEmptyString);

    /** Access m_CatalogFilePath
     * \return The current value of m_CatalogFilePath
     */
    wxString GetCatalogFilePath()
    {
        return m_CatalogFilePath;
    }

    /** Set m_CatalogFilePath
     * \param val New value to set
     */
    void SetCatalogFilePath(const wxString &val)
    {
        m_CatalogFilePath = val;
    }

    /** Access m_SetId
     * \return The current value of m_SetId
     */
    wxString GetSetId()
    {
        return m_SetId;
    }

    /** Set m_SetId
     * \param val New value to set
     */
    void SetSetId(const wxString &val)
    {
        m_SetId = val;
    }

    /** Access m_Name
     * \return The current value of m_Name
     */
    wxString GetName()
    {
        return m_Name;
    }

    /** Set m_Name
     * \param val New value to set
     */
    void SetName(const wxString &val)
    {
        m_Name = val;
    }

    /** Access m_Description
     * \return The current value of m_Description
     */
    wxString GetDescription()
    {
        return m_Description;
    }

    /** Set m_Description
     * \param val New value to set
     */
    void SetDescription(const wxString &val)
    {
        m_Description = val;
    }

    /** Access m_Start
     * \return The current value of m_Start
     */
    double GetStart()
    {
        return m_Start;
    }

    /** Set m_Start
     * \param val New value to set
     */
    void SetStart(double val)
    {
        m_Start = val;
    }

    /** Access m_End
     * \return The current value of m_End
     */
    double GetEnd()
    {
        return m_End;
    }

    /** Set m_End
     * \param val New value to set
     */
    void SetEnd(double val)
    {
        m_End = val;
    }

    /** Access m_TimeZoneHours
     * \return The current value of m_TimeZoneHours
     */
    float GetTimeZoneHours()
    {
        return m_TimeZoneHours;
    }

    /** Set m_TimeZoneHours
     * \param val New value to set
     */
    void SetTimeZone(float val)
    {
        m_TimeZoneHours = val;
    }

    /** Access m_TimeStep in minutes
     * \return The current value of m_TimeStep in minutes
     */
    double GetTimeStepMinutes()
    {
        return m_TimeStepHours*60;
    }

    /** Set m_TimeStep in minutes
     * \param val New value to set in minutes
     */
    void SetTimeStepMinutes(double val)
    {
        m_TimeStepHours = val/60;
    }

    /** Access m_TimeStep in hours
     * \return The current value of m_TimeStep in hours
     */
    double GetTimeStepHours()
    {
        return m_TimeStepHours;
    }

    /** Set m_TimeStep in hours
     * \param val New value to set in hours
     */
    void SetTimeStepHours(double val)
    {
        m_TimeStepHours = val;
    }

    /** Access m_TimeStep in days
     * \return The current value of m_TimeStep in days
     */
    double GetTimeStepDays()
    {
        return m_TimeStepHours/24;
    }

    /** Set m_TimeStep in days
     * \param val New value to set in days
     */
    void SetTimeStepDays(double val)
    {
        m_TimeStepHours = val*24;
    }

    /** Access m_FirstTimeStepHour in minutes
     * \return The current value of m_FirstTimeStep in minutes
     */
    double GetFirstTimeStepMinutes()
    {
        return m_FirstTimeStepHour*60;
    }

    /** Set m_FirstTimeStepHour in minutes
     * \param val New value to set in minutes
     */
    void SetFirstTimeStepMinutes(double val)
    {
        m_FirstTimeStepHour = val/60;
    }

    /** Access m_FirstTimeStepHour in hours
     * \return The current value of m_FirstTimeStep in hours
     */
    double GetFirstTimeStepHours()
    {
        return m_FirstTimeStepHour;
    }

    /** Set m_FirstTimeStepHour in hours
     * \param val New value to set in hours
     */
    void SetFirstTimeStepHours(double val)
    {
        m_FirstTimeStepHour = val;
    }

    /** Access m_FormatRaw
     * \return The current value of m_FormatRaw
     */
    FileFormat GetFormatRaw()
    {
        return m_FormatRaw;
    }

    /** Set m_FormatRaw
     * \param val New value to set
     */
    void SetFormatRaw(FileFormat val)
    {
        m_FormatRaw = val;
    }

    /** Access m_FormatStorage
     * \return The current value of m_FormatStorage
     */
    FileFormat GetFormatStorage()
    {
        return m_FormatStorage;
    }

    /** Set m_FormatStorage
     * \param val New value to set
     */
    void SetFormatStorage(FileFormat val)
    {
        m_FormatStorage = val;
    }

    /** Access m_DataPath
     * \return The current value of m_DataPath
     */
    wxString GetDataPath()
    {
        return m_DataPath;
    }

    /** Set m_DataPath
     * \param val New value to set
     */
    void SetDataPath(const wxString &val)
    {
        m_DataPath = val;
    }

    /** Access m_Nan
     * \return The current value of m_Nan
     */
    VectorDouble GetNan()
    {
        return m_Nan;
    }

    /** Set m_Nan
     * \param val New value to set
     */
    void SetNan(const VectorDouble &val)
    {
        m_Nan = val;
    }

    /** Access m_CoordSys
     * \return The current value of m_CoordSys
     */
    CoordSys GetCoordSys()
    {
        return m_CoordSys;
    }

    /** Set m_CoordSys
     * \param val New value to set
     */
    void SetCoordSys(CoordSys val)
    {
        m_CoordSys = val;
    }


protected:

    wxString m_CatalogFilePath; //!< Member variable "m_CatalogFilePath"
    wxString m_SetId; //!< Member variable "m_SetId"
    wxString m_Name; //!< Member variable "m_Name"
    wxString m_Description; //!< Member variable "m_Description"
    double m_Start; //!< Member variable "m_Start"
    double m_End; //!< Member variable "m_End"
    float m_TimeZoneHours; //!< Member variable "m_Timezone" in hours
    double m_TimeStepHours; //!< Member variable "m_TimeStep" in hours
    double m_FirstTimeStepHour; //!< Member variable "m_FirstTimeStep" in hours
    FileFormat m_FormatRaw; //!< Member variable "m_FormatRaw"
    FileFormat m_FormatStorage; //!< Member variable "m_FormatStorage"
    wxString m_DataPath; //!< Member variable "m_FilePath"
    VectorDouble m_Nan; //!< Member variable "m_Nan"
    CoordSys m_CoordSys; //!< Member variable "m_CoordSys"

    /** Convert a string to a MJD number
     * \param date_s The date string
     * \param IsSerieBeginning If the date corresponds to the beginning or to the end of a serie
     * \param TimeZone The hours difference for time zone in reference to the UTC
     * \param TimeStepHours The dataset TimeStep in hours
     * \param FirstTimeStepHour The hours of the first value in a day (UTC)
     * \return The MJD number
     */
    static double ConvertStringToDatasetDate(const wxString &date_s, int IsSerieBeginning, float TimeZone, double TimeStepHours, double FirstHour);


private:

};

#endif // ASCATALOG_H
