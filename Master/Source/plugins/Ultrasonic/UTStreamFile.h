#pragma once

#include <QFile>
#include <QDebug>
#include <QMutex>
#include <Ultrasonic/UTData.h>

#define ARS_DUMP_CSV

class UTStreamFile : public QFile
{
    Q_OBJECT
    friend class UTScanDocument;

public:
    explicit UTStreamFile(QObject *parent = nullptr);
    explicit UTStreamFile(const QString &name, QObject *parent = nullptr);

    bool IsDataChanged();
    void ResetChangedFlag();

    void WriteSegment(const UTDataSegment &segment);
    void WriteToFile(UTStreamFile *file);

    void WriteFrame(const UTDataFrame &frame);
    UTDataFrame ReadFrame(UTDataFrame frame = UTDataFrame());
    UTDataSegment ReadSegment(int maxLength);

public slots:
    void RequestReadAll(int segmentLength = -1);
    void OnDataArrived(const UTDataSegment &segment);

    void SetDataEnable(bool enable);
    inline bool hasMoreFrame();

signals:
    void DataReady(UTDataSegment segment);
    void ReadAllCompleted();

private:
    bool enabled = true;
    QMutex readLock;
    bool bChanged = false;

private:
    static const qint64 MIN_BYTES = UTDataContext::CONTEXT_SIZE;

private:
    /* 模板方法，无法写入cpp文件 */
    /*!
     * \brief 从文件中读取一个QVector。这个操作会导致当前文件指针位置改变。
     * \param value 存储数据的目标QVector
     * \return 当成功读取时，返回0；否则返回非0值。
     */
    template <typename T>
    inline int ReadVector(QVector<T> &value, int maxCount = 1024)
    {
        int count;
        qint64 size = read(reinterpret_cast<char *>(&count), sizeof(count));
        if (Q_UNLIKELY((count < 0) || (size != sizeof(count)) || (count > maxCount)) ) {
            return count;
        }
        value.resize(count);
        value.squeeze();
        if (Q_LIKELY(count > 0)) {
            size = read(reinterpret_cast<char *>( value.data() ), sizeof(T) * count);
            if (Q_UNLIKELY(size != sizeof(T) * count)) {
                return size;
            }
        }
        return 0;
    }

    /*!
     * \brief 向文件中写入一个QVector。这个操作会导致当前文件指针位置改变。
     * \param value 被写入的QVector
     */
    template <typename T>
    inline void WriteVector(const QVector<T> &value)
    {
        int count = value.count();
        write(reinterpret_cast<const char *>(&count), sizeof(count));
        if (Q_LIKELY(count > 0)) {
            write(reinterpret_cast<const char *>( value.data() ), sizeof(T) * count);
        }
    }

};

bool UTStreamFile::hasMoreFrame()
{
    return bytesAvailable() >= MIN_BYTES;
}
