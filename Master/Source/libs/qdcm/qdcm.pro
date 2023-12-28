include(../library.pri)

QT += gui xml

DEFINES += DCM_CORE_EXPORTS DCM_IMAGE_EXPORTS DCM_MODULE_EXPORTS

RESOURCES += \
    dcmcore.qrc

HEADERS += \
    DcmModuleApi.h \
    DcmModuleGeneralEquipmentAttributes.h \
    DcmModulePatient.h \
    DcmModuleSOPCommon.h \
    DcmModuleStudy.h

SOURCES += \
    DcmModuleGeneralEquipmentAttributes.cpp \
    DcmModulePatient.cpp \
    DcmModuleSOPCommon.cpp \
    DcmModuleStudy.cpp

HEADERS += \
    DcmImage.h \
    DcmImageApi.h \
    DcmImageTransferFunction.h \
    DcmMonochromeImage.h \
    DcmPhotometricInterpretation.h

SOURCES += \
    DcmImage.cpp \
    DcmImageTransferFunction.cpp \
    DcmMonochromeImage.cpp \
    DcmPhotometricInterpretation.cpp

HEADERS += \
    DcmCharSet.h \
    DcmCoreApi.h \
    DcmDataset.h \
    DcmDictionary.h \
    DcmFile.h \
    DcmGlobal.h \
    DcmModule.h \
    DcmPDUItem.h \
    DcmReader.h \
    DcmStream.h \
    DcmTag.h \
    DcmTagAE.h \
    DcmTagAS.h \
    DcmTagAT.h \
    DcmTagBinary.h \
    DcmTagCS.h \
    DcmTagDA.h \
    DcmTagDescription.h \
    DcmTagDS.h \
    DcmTagDT.h \
    DcmTagFD.h \
    DcmTagFL.h \
    DcmTagGroup.h \
    DcmTagIS.h \
    DcmTagItem.h \
    DcmTagKey.h \
    DcmTagList.h \
    DcmTagLO.h \
    DcmTagLT.h \
    DcmTagPixelData.h \
    DcmTagPixelDataItem.h \
    DcmTagPN.h \
    DcmTagSH.h \
    DcmTagSL.h \
    DcmTagSQ.h \
    DcmTagSS.h \
    DcmTagST.h \
    DcmTagString.h \
    DcmTagTM.h \
    DcmTagUI.h \
    DcmTagUL.h \
    DcmTagUS.h \
    DcmTagUT.h \
    DcmTransferSyntax.h \
    DcmTypes.h \
    DcmVr.h \
    DcmWriter.h

SOURCES += \
    DcmCharSet.cpp \
    DcmDataset.cpp \
    DcmDictionary.cpp \
    DcmFile.cpp \
    DcmGlobal.cpp \
    DcmModule.cpp \
    DcmPDUItem.cpp \
    DcmReader.cpp \
    DcmStream.cpp \
    DcmTag.cpp \
    DcmTagAE.cpp \
    DcmTagAS.cpp \
    DcmTagAT.cpp \
    DcmTagBinary.cpp \
    DcmTagCS.cpp \
    DcmTagDA.cpp \
    DcmTagDescription.cpp \
    DcmTagDS.cpp \
    DcmTagDT.cpp \
    DcmTagFD.cpp \
    DcmTagFL.cpp \
    DcmTagGroup.cpp \
    DcmTagIS.cpp \
    DcmTagItem.cpp \
    DcmTagKey.cpp \
    DcmTagList.cpp \
    DcmTagLO.cpp \
    DcmTagLT.cpp \
    DcmTagPixelData.cpp \
    DcmTagPixelDataItem.cpp \
    DcmTagPN.cpp \
    DcmTagSH.cpp \
    DcmTagSL.cpp \
    DcmTagSQ.cpp \
    DcmTagSS.cpp \
    DcmTagST.cpp \
    DcmTagString.cpp \
    DcmTagTM.cpp \
    DcmTagUI.cpp \
    DcmTagUL.cpp \
    DcmTagUS.cpp \
    DcmTagUT.cpp \
    DcmTransferSyntax.cpp \
    DcmVr.cpp \
    DcmWriter.cpp
