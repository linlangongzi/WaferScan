#pragma once

#include "DcmModuleApi.h"
#include "DcmModule.h"

/*! General Equipment Attributes module.
 * This module handles the following tags:
 * 0008, 0070   Manufacturer                2
 * 0008, 0080   InstitutionName             3
 * 0008, 0081   InstitutionAddress          3
 * 0008, 1010   StationName                 3
 * 0008, 1040   InstitutionalDepartmentName 3
 * 0008, 1090   ManufacturerModelName       3
 * 0018, 1000   DeviceSerialNumber          3
 * 0018, 1020   SoftwareVersions            3
 * 0018, 1050   SpatialResolution           3
 * 0018, 1200   DateOfLastCalibration       3
 * 0018, 1201   TimeOfLastCalibration       3
 * 0028, 0120   PixelPaddingValue           3
 */
class DCM_MODULE_API DcmModuleGeneralEquipmentAttributes : public DcmModule
{
public:

    /**
     * Default constructor.
     */
    DcmModuleGeneralEquipmentAttributes();

    /**
     * Copy constructor.
     * \param module Module to be copied.
     */
    DcmModuleGeneralEquipmentAttributes(const DcmModuleGeneralEquipmentAttributes &module);

    /**
     * Assignment operator.
     * \param module Module to be assigned.
     * \return This module.
     */
    DcmModuleGeneralEquipmentAttributes& operator =(const DcmModuleGeneralEquipmentAttributes &module);

    /**
     * Clone this module.
     * \return Pointer to cloned module.
     */
    DcmModule* clone() const;

    QString manufacturer() const;
    void setManufacturer(const QString &value);
    QString institutionName() const;
    void setInstitutionName(const QString &value);
    QString institutionAddress() const;
    void setInstitutionAddress(const QString &value);
    QString stationName() const;
    void setStationName(const QString &value);
    QString institutionalDepartmentName() const;
    void setInstitutionalDepartmentName(const QString &value);
    QString manufacturerModelName() const;
    void setManufacturerModelName(const QString &value);
    QString deviceSerialNumber() const;
    void setDeviceSerialNumber(const QString &value);
    QString softwareVersions() const;
    void setSoftwareVersions(const QString &value);
    double spatialResolution() const;
    void setSpatialResolution(double value);
    QDate dateOfLastCalibration() const;
    void setDateOfLastCalibration(const QDate &value);
    QTime timeOfLastCalibration() const;
    void setTimeOfLastCalibration(const QTime &value);
    int pixelPaddingValue() const;
    void setPixelPaddingValue(int value);

private:

    /**
     * Initialize list of supported tags.
     */
    void initializeTags();
};
