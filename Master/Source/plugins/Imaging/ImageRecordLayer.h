#pragma once

#include <allrising/Util.h>
#include "ImagingGlobal.h"
#include "UTImage.h"
#include <QFile>

class IMAGING_EXPORT RecordLayerConfig
{
    Q_GADGET

    Q_PROPERTY(bool enable MEMBER enable)
    Q_PROPERTY(int feature MEMBER feature)
    Q_PROPERTY(QString featureName MEMBER featureName)
    Q_PROPERTY(QString fileName MEMBER fileName)
    Q_PROPERTY(QString comment MEMBER comment)

public:
    DECLARE_COMPARE_OPERATORS(RecordLayerConfig)

    void setFeature(int index);

    bool enable = true;
    int feature = -1;
    QString featureName;
    QString fileName;
    QString comment;
};

Q_DECLARE_METATYPE(RecordLayerConfig)

class IMAGING_EXPORT ImageRecordLayer : public QObject
{
    Q_OBJECT

public:
    ImageRecordLayer(QObject *parent = Q_NULLPTR);

    void ensureOpen();
    void saveFile();
    bool isImageValid();

    RecordLayerConfig config;
    UTImage image;
    bool modified = false;
    bool triedOpen = false;
};
