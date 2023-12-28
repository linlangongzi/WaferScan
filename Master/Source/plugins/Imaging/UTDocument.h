#pragma once

#include <QObject>
#include <Ultrasonic/UTStreamFile.h>
#include <Imaging/ImageOptions.h>
#include "UTLayer.h"
#include <Acquisition/ScanPlan.h>

#define BUFFERNAME_DATA "tempData"
#define BUFFERNAME_PARAMETER "tempParameter"

/* 用于断电续扫功能 */
#define DATA_BACK_FILE_NAME "DataBackFile"

class Axis;
class ScanTask;

class UTDocument : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVector3D origin READ origin WRITE setOrigin NOTIFY originChanged)

public:
    explicit UTDocument(QObject *parent = nullptr);
    ~UTDocument() override;

public:
    void InitializeLayers();
    UTLayer *GetLayerByID(int layerID);
    QMap<int, UTLayer *> GetLayerMap();

    QString GetParameterFileName(const QString &dataFileName);
    QString GetCurrentFileName();
    QString GetCurrentFilePath();

    void LoadScan(ScanPlan *plan);
    void LoadSetups();          // TODO: parameter changed
    UTSetup setupForChannel(int channelGlobalIndex);
    ScanPlan *scanPlan();
    ScanTask *scanTask();
    QJsonValue Get(const QString &key) const;
    QVector3D origin() const;

signals:
    void FileOpened();
    void DataReady(UTDataSegment segment);
    void DataCompleted();
    void originChanged(QVector3D origin);

public slots:
    void setScanTask(ScanTask *task);       // parameter changed
    int NewFile(const QString &name);
    int Open(const QString &name);
    bool Close(bool ignore = false);
    int Save(QString name = QString());
    int cutFiles(const QStringList &fileNameList);
    void clear();
    int Reload();

    // 完成对数据文件的槽函数调用
    void RequestDataReadAll();
    void OnDataArrived(const UTDataSegment &segment);
    void SetDataEnable(bool enable);
    DefectItemModel *GetDefectModel(int key);
    void Set(const QString &key, const QJsonValue &value);
    void setSaveFile(bool saved);
    void setOrigin(const QVector3D &origin);

private:
    void InitDoc();
    int NewLayerID();
    void notifyFileOpened();
    void loadJson(const QJsonValue &value);
    QJsonValue saveJson() const;
    void CheckLastTestIdle();

private slots:
    // 提供dataFile信号的转发
    void OnDataReady(const UTDataSegment &segment);

private:
    UTStreamFile dataFile;                                                      // 数据文件
    QString dataFilePath;
    QMap<int, UTLayer *> layerMap;
#ifdef PIPE_DETECT
    static const int NUM_LAYERS = 10;
#else
    static const int NUM_LAYERS = 3;
#endif

    QMap<int, UTSetup> setupManager;
    ScanTask *m_task = nullptr;
    QJsonObject data;
    bool saveFileEnable = false;
    QVector3D m_origin;
};

UTDocument *document();
