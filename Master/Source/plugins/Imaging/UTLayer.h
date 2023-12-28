#pragma once

#include <Ultrasonic/UTData.h>
#include <Imaging/UTImage.h>
#include <QObject>

class DefectItemModel;

/*!
 * \brief The UTLayer class
 * 记录层，由全局的document管理，负责记录注册的object，同时负责维护由pipeline发出的数据以及defectprocessor发出的defectlistupdate信号，
 * 用来刷新model，此model受记录层管理，负责刷新model数据
 */
class UTLayer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ImageOptions imageOptions READ GetImageOptions WRITE SetImageOptions NOTIFY ImageOptionsChanged)
    Q_PROPERTY(bool dataInputEnabled READ GetDataInputEnabled WRITE SetDataInputEnabled)

public:
    explicit UTLayer(QObject *parent = nullptr);

    void SetID(int newID) {id = newID;}
    int GetID() const {return id;}
    int layerIndex() const {return qMax(0, id - 1);}
    int GetColorPaletteID() {return id;}
    DefectItemModel *GetModel();
    void FromJson(const QJsonObject &obj, const ImageOptions &fallbackOptions = ImageOptions());
    QJsonObject ToJson() const;
    ImageOptions GetImageOptions() const;
    bool GetDataInputEnabled() const;

    UTDataFrame dataFrameAt(const QPoint &point) const;                     /* 获取C扫图像上指定点对应的A扫数据 */
    UTDataSegment dataRectAt(const QRect &rect) const;
    UTDataSegment sliceAt(Qt::Orientation orientation, int index) const;
    UTDataSegment dataAll() const;
    UTImageList imageList() const;

public slots:
    void SetImageOptions(const ImageOptions &options);
    void RequestDataAll(void *reference);
    void Reset();
    void OnDataArrived(const UTDataSegment &segment);
    void ReduceFrame(const QPoint &point, const UTDataFrame &frame);
    void SetDataInputEnabled(bool enabled);

signals:
    void ImagesUpdated(UTImageList images);
    void SegmentUpdate(UTDataSegment Segment, void *reference);
    void MatrixDataUpdated(QHash<QPoint, UTDataFrame> segment);
    void ProcessorFinished();
    void ImageOptionsChanged(ImageOptions imageOptions);

private:
    void timerEvent(QTimerEvent *);

    typedef std::vector<UTDataFrame> UTDataMatrix;                        /*!< 按行列保存的数据帧，索引方式为(y * width + x)，由于QVector的内存分配能力有限，此处使用std::vector */
    UTDataMatrix data;
    QHash<QPoint, UTDataFrame> outputBuffer;                              /*!< 归并后等待渲染的像素 */
    UTImageList images;                                                         /*!< 渲染后的图像 */

    DefectItemModel *model;
    int id;
    ImageOptions imageOptions;
    bool dataInputEnabled = true;
};
