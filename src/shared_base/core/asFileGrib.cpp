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
 * Portions Copyright 2018-2019 Pascal Horton, University of Bern.
 */

#include "asFileGrib.h"
#include "eccodes.h"

asFileGrib::asFileGrib(const wxString &fileName, const FileMode &fileMode)
        : asFile(fileName, fileMode),
          m_filtPtr(nullptr),
          m_index(asNOT_FOUND)
{
    switch (fileMode) {
        case (ReadOnly):
            // OK
            break;
        case (Write):
        case (Replace):
        case (New):
        case (Append):
        default :
            asThrowException(_("Grib files edition is not implemented."));
    }
}

asFileGrib::~asFileGrib()
{
    Close();
}

bool asFileGrib::Open()
{
    if (!Find())
        return false;

    // Let GDAL open the dataset
    if (!OpenDataset())
        return false;

    m_opened = true;

    return true;
}

bool asFileGrib::Close()
{
    if (m_filtPtr) {
        fclose(m_filtPtr);
        m_filtPtr = nullptr;
    }

    return true;
}

bool asFileGrib::OpenDataset()
{
    // Filepath
    wxString filePath = m_fileName.GetFullPath();

    // Open file
    m_filtPtr = fopen(filePath.mb_str(), "r");

    if (!m_filtPtr) // Failed
    {
        wxLogError(_("The opening of the grib file failed."));
        wxFAIL;
        return false;
    }

    // Parse structure
    return ParseStructure();
}

bool asFileGrib::ParseStructure()
{
    int err = 0;
    codes_handle *h;

    // Loop over the GRIB messages in the source
    while ((h = codes_handle_new_from_file(0, m_filtPtr, PRODUCT_GRIB, &err)) != NULL) {
        if (!h) {
            wxLogError("Unable to create handle from file %s", m_fileName.GetFullPath());
            return false;
        }
        if (!CheckGribErrorCode(err)) {
            return false;
        }

        ExtractAxes(h);
        ExtractLevel(h);
        ExtractTime(h);
        ExtractGribCode(h);

        codes_handle_delete(h);
    }

    // Check unique time value
    if (!m_times.empty()) {
        for (double time : m_times) {
            if (time != m_times[0]) {
                wxLogError(_("Handling of multiple time values in a Grib file is not yet implemented."));
                return false;
            }
        }
    }

    return true;
}

void asFileGrib::ExtractTime(codes_handle *h)
{
    // Keys: https://apps.ecmwf.int/codes/grib/format/edition-independent/2/

    // Get reference date
    size_t dataDateLength = 20;
    char buffer1[dataDateLength];
    CODES_CHECK(codes_get_string(h, "dataDate", &buffer1[0], &dataDateLength), 0);
    wxString dataDate(buffer1, wxConvUTF8);
    size_t dataTimeLength = 20;
    char buffer2[dataTimeLength];
    CODES_CHECK(codes_get_string(h, "dataTime", &buffer2[0], &dataTimeLength), 0);
    wxString dataTime(buffer2, wxConvUTF8);
    double refTime = asTime::GetTimeFromString(dataDate + dataTime, YYYYMMDDhhmm);
    m_refTimes.push_back(refTime);

    // Get forecast time
    double forecastTime;
    CODES_CHECK(codes_get_double(h, "forecastTime", &forecastTime), 0);
    double time = refTime + forecastTime / 24.0;
    m_times.push_back(time);
}

void asFileGrib::ExtractLevel(codes_handle *h)
{
    // Keys: https://apps.ecmwf.int/codes/grib/format/edition-independent/3/

    // Get level type
    size_t typeLength = 255;
    char typeVal[typeLength];
    CODES_CHECK(codes_get_string(h, "typeOfLevel", &typeVal[0], &typeLength), 0);
    wxString type(typeVal, wxConvUTF8);
    m_levelTypesStr.push_back(type);

    // Get level type code
    long typeCode;
    CODES_CHECK(codes_get_long(h, "typeOfFirstFixedSurface", &typeCode), 0);
    m_levelTypes.push_back((int) typeCode);

    // Get level value
    double level;
    CODES_CHECK(codes_get_double(h, "level", &level), 0);
    if (type.IsSameAs("isobaricInPa")) {
        level /= 100;
    }
    m_levels.push_back(level);
}

void asFileGrib::ExtractAxes(codes_handle *h)
{
    // Keys: https://apps.ecmwf.int/codes/grib/format/edition-independent/1/

    // Get lat axis
    size_t latLength;
    codes_get_size(h, "distinctLatitudes", &latLength);
    double latVals[latLength];
    CODES_CHECK(codes_get_double_array(h, "distinctLatitudes", &latVals[0], &latLength), 0);
    a1d latAxis = Eigen::Map<a1d>(latVals, latLength);

    // Get lon axis
    size_t lonLength;
    codes_get_size(h, "distinctLongitudes", &lonLength);
    double lonVals[lonLength];
    CODES_CHECK(codes_get_double_array(h, "distinctLongitudes", &lonVals[0], &lonLength), 0);
    a1d lonAxis = Eigen::Map<a1d>(lonVals, lonLength);

    m_xAxes.push_back(lonAxis);
    m_yAxes.push_back(latAxis);
}

void asFileGrib::ExtractGribCode(codes_handle *h)
{
    // Get discipline
    long discipline;
    CODES_CHECK(codes_get_long(h, "discipline", &discipline), 0);
    m_parameterDisciplines.push_back((int) discipline);

    // Get category
    long category;
    CODES_CHECK(codes_get_long(h, "parameterCategory", &category), 0);
    m_parameterCategories.push_back((int) category);

    // Get parameter number
    long number;
    CODES_CHECK(codes_get_long(h, "parameterNumber", &number), 0);
    m_parameterNums.push_back((int) number);
}

bool asFileGrib::CheckGribErrorCode(int ierr) const
{
    if (ierr == CODES_SUCCESS) {
        return true;
    }

    wxString msg;

    switch (ierr) {
        case CODES_END_OF_FILE :
            msg = "End of resource reached.";
            break;
        case CODES_INTERNAL_ERROR :
            msg = "Internal error.";
            break;
        case CODES_BUFFER_TOO_SMALL :
            msg = "Passed buffer is too small.";
            break;
        case CODES_NOT_IMPLEMENTED :
            msg = "Function not yet implemented.";
            break;
        case CODES_7777_NOT_FOUND :
            msg = "Missing 7777 at end of message.";
            break;
        case CODES_ARRAY_TOO_SMALL :
            msg = "Passed array is too small.";
            break;
        case CODES_FILE_NOT_FOUND :
            msg = "File not found.";
            break;
        case CODES_CODE_NOT_FOUND_IN_TABLE :
            msg = "Code not found in code table.";
            break;
        case CODES_WRONG_ARRAY_SIZE :
            msg = "Array size mismatch.";
            break;
        case CODES_NOT_FOUND :
            msg = "Key/value not found.";
            break;
        case CODES_IO_PROBLEM :
            msg = "Input output problem.";
            break;
        case CODES_INVALID_MESSAGE :
            msg = "Message invalid.";
            break;
        case CODES_DECODING_ERROR :
            msg = "Decoding invalid.";
            break;
        case CODES_ENCODING_ERROR  :
            msg = "Encoding invalid.";
            break;
        case CODES_NO_MORE_IN_SET :
            msg = "Code cannot unpack because of string too small.";
            break;
        case CODES_GEOCALCULUS_PROBLEM :
            msg = "Problem with calculation of geographic attributes.";
            break;
        case CODES_OUT_OF_MEMORY :
            msg = "Memory allocation error.";
            break;
        case CODES_READ_ONLY :
            msg = "Value is read only.";
            break;
        case CODES_INVALID_ARGUMENT :
            msg = "Invalid argument.";
            break;
        case CODES_NULL_HANDLE :
            msg = "Null handle.";
            break;
        case CODES_INVALID_SECTION_NUMBER :
            msg = "Invalid section number.";
            break;
        case CODES_VALUE_CANNOT_BE_MISSING :
            msg = "Value cannot be missing.";
            break;
        case CODES_WRONG_LENGTH :
            msg = "Wrong message length.";
            break;
        case CODES_INVALID_TYPE :
            msg = "Invalid key type.";
            break;
        case CODES_WRONG_STEP :
            msg = "Unable to set step.";
            break;
        case CODES_WRONG_STEP_UNIT :
            msg = "Wrong units for step (step must be integer)";
            break;
        case CODES_INVALID_FILE :
            msg = "Invalid file id.";
            break;
        case CODES_INVALID_GRIB :
            msg = "Invalid grib id.";
            break;
        case CODES_INVALID_INDEX :
            msg = "Invalid index id.";
            break;
        case CODES_INVALID_ITERATOR  :
            msg = "Invalid iterator id.";
            break;
        case CODES_INVALID_KEYS_ITERATOR :
            msg = "Invalid keys iterator id.";
            break;
        case CODES_INVALID_NEAREST :
            msg = "Invalid nearest id.";
            break;
        case CODES_INVALID_ORDERBY :
            msg = "Invalid order by.";
            break;
        case CODES_MISSING_KEY :
            msg = "Missing a key from the fieldset.";
            break;
        case CODES_OUT_OF_AREA :
            msg = "The point is out of the grid area.";
            break;
        case CODES_CONCEPT_NO_MATCH  :
            msg = "Concept no match.";
            break;
        case CODES_HASH_ARRAY_NO_MATCH :
            msg = "Hash array no match.";
            break;
        case CODES_NO_DEFINITIONS :
            msg = "Definitions files not found.";
            break;
        case CODES_WRONG_TYPE :
            msg = "Wrong type while packing.";
            break;
        case CODES_END :
            msg = "End of resource.";
            break;
        case CODES_NO_VALUES :
            msg = "Unable to code a field without values.";
            break;
        case CODES_WRONG_GRID :
            msg = "Grid description is wrong or inconsistent.";
            break;
        case CODES_END_OF_INDEX :
            msg = "End of index reached.";
            break;
        case CODES_NULL_INDEX :
            msg = "Null index.";
            break;
        case CODES_PREMATURE_END_OF_FILE :
            msg = "End of resource reached when reading message.";
            break;
        case CODES_INTERNAL_ARRAY_TOO_SMALL :
            msg = "An internal array is too small.";
            break;
        case CODES_MESSAGE_TOO_LARGE :
            msg = "Message is too large for the current architecture.";
            break;
        case CODES_CONSTANT_FIELD :
            msg = "Constant field.";
            break;
        case CODES_SWITCH_NO_MATCH :
            msg = "Switch unable to find a matching case.";
            break;
        case CODES_UNDERFLOW :
            msg = "Underflow.";
            break;
        case CODES_MESSAGE_MALFORMED :
            msg = "Message malformed.";
            break;
        case CODES_CORRUPTED_INDEX :
            msg = "Index is corrupted.";
            break;
        case CODES_INVALID_BPV :
            msg = "Invalid number of bits per value.";
            break;
        case CODES_DIFFERENT_EDITION :
            msg = "Edition of two messages is different.";
            break;
        case CODES_VALUE_DIFFERENT  :
            msg = "Value is different.";
            break;
        case CODES_INVALID_KEY_VALUE :
            msg = "Invalid key value.";
            break;
        case CODES_STRING_TOO_SMALL :
            msg = "String is smaller than requested.";
            break;
        case CODES_WRONG_CONVERSION :
            msg = "Wrong type conversion.";
            break;
        case CODES_MISSING_BUFR_ENTRY :
            msg = "Missing BUFR table entry for descriptor.";
            break;
        case CODES_NULL_POINTER :
            msg = "Null pointer.";
            break;
        case CODES_ATTRIBUTE_CLASH :
            msg = "Attribute is already present, cannot add.";
            break;
        case CODES_TOO_MANY_ATTRIBUTES :
            msg = "Too many attributes.";
            break;
        case CODES_ATTRIBUTE_NOT_FOUND :
            msg = "Attribute not found.";
            break;
        case CODES_UNSUPPORTED_EDITION :
            msg = "Edition not supported.";
            break;
        case CODES_OUT_OF_RANGE :
            msg = "Value out of coding range.";
            break;
        case CODES_WRONG_BITMAP_SIZE :
            msg = "Size of bitmap is incorrect.";
            break;
        default:
            msg = "Unknown error";
    }

    wxLogError(_("Grib error: %s"), msg);

    return false;
}

bool asFileGrib::GetXaxis(a1d &uaxis) const
{
    wxASSERT(m_opened);
    wxASSERT(m_index != asNOT_FOUND);
    wxASSERT(m_xAxes.size() > m_index);

    uaxis = m_xAxes[m_index];

    return true;
}

bool asFileGrib::GetYaxis(a1d &vaxis) const
{
    wxASSERT(m_opened);
    wxASSERT(m_index != asNOT_FOUND);
    wxASSERT(m_yAxes.size() > m_index);

    vaxis = m_yAxes[m_index];

    return true;
}

double asFileGrib::GetTime() const
{
    wxASSERT(m_opened);
    wxASSERT(m_index != asNOT_FOUND);
    wxASSERT(m_times.size() > m_index);

    return m_times[m_index];
}

bool asFileGrib::SetIndexPosition(const vi gribCode, const float level)
{
    wxASSERT(gribCode.size() == 4);

    // Find corresponding data
    m_index = asNOT_FOUND;
    for (int i = 0; i < m_parameterNums.size(); ++i) {
        if (m_parameterDisciplines[i] == gribCode[0] && m_parameterCategories[i] == gribCode[1] &&
            m_parameterNums[i] == gribCode[2] && m_levelTypes[i] == gribCode[3] && m_levels[i] == level) {

            if (m_index >= 0) {
                wxLogError(_("The desired parameter was found twice in the file."));
                return false;
            }

            m_index = i;
        }
    }

    if (m_index == asNOT_FOUND) {
        wxLogError(_("The desired parameter was not found in the file."));
        return false;
    }

    return true;
}

bool asFileGrib::GetVarArray(const int IndexStart[], const int IndexCount[], float *pValue)
{
    wxASSERT(m_opened);
    wxASSERT(m_index != asNOT_FOUND);

    int iLonStart = IndexStart[0];
    int iLonEnd = IndexStart[0] + IndexCount[0] - 1;
    int iLatStart = IndexStart[1];
    int iLatEnd = IndexStart[1] + IndexCount[1] - 1;
    auto nLons = (int) m_xAxes[m_index].size();
    auto nLats = (int) m_yAxes[m_index].size();

    codes_index *index = nullptr;
    codes_handle *h = nullptr;
    int err = 0;
    int count = 0;

    //index = codes_index_new(0, "discipline,parameterCategory,parameterNumber,typeOfFirstFixedSurface,level", &err);
    index = codes_index_new(0, "discipline,parameterCategory,parameterNumber,level", &err);
    if (!CheckGribErrorCode(err)) {
        return false;
    }

    err = codes_index_add_file(index, m_fileName.GetFullPath().mb_str());
    if (!CheckGribErrorCode(err)) {
        return false;
    }

    codes_index_select_long(index, "discipline", m_parameterDisciplines[m_index]);
    codes_index_select_long(index, "parameterCategory", m_parameterCategories[m_index]);
    codes_index_select_long(index, "parameterNumber", m_parameterNums[m_index]);
    //codes_index_select_long(index, "typeOfFirstFixedSurface", m_levelTypes[m_index]);
    codes_index_select_double(index, "level", m_levels[m_index]);

    while ((h = codes_handle_new_from_index(index, &err)) != NULL) {
        if (!CheckGribErrorCode(err)) {
            return false;
        }
        if (count > 0) {
            wxLogError(_("Multiple messages found in GRIB file for the given constraints."));
            return false;
        }
        count++;

        // Get data
        double *values = NULL;
        size_t valuesLenth = 0;
        CODES_CHECK(codes_get_size(h, "values", &valuesLenth), 0);
        values = (double *) malloc(valuesLenth * sizeof(double));
        CODES_CHECK(codes_get_double_array(h, "values", values, &valuesLenth), 0);

        int finalIndex = 0;

        if (nLats > 0 && m_yAxes[m_index][0] > m_yAxes[m_index][1]) {
            for (int iLat = nLats - 1; iLat >= 0; iLat--) {
                if (iLat >= iLatStart && iLat <= iLatEnd) {
                    for (int iLon = 0; iLon < nLons; iLon++) {
                        if (iLon >= iLonStart && iLon <= iLonEnd) {
                            pValue[finalIndex] = (float)values[iLat * nLons + iLon];
                            finalIndex++;
                        }
                    }
                }
            }
        } else {
            for (int iLat = 0; iLat < nLats; iLat++) {
                if (iLat >= iLatStart && iLat <= iLatEnd) {
                    for (int iLon = 0; iLon < nLons; iLon++) {
                        if (iLon >= iLonStart && iLon <= iLonEnd) {
                            pValue[finalIndex] = (float)values[iLat * nLons + iLon];
                            finalIndex++;
                        }
                    }
                }
            }
        }

        free(values);
        codes_handle_delete(h);
    }

    if (count == 0) {
        wxLogError(_("GRIB message not found for the given constraints."));
        return false;
    }

    return true;
}