#pragma once

#include <QObject>
#include <QFile>
#include <Ultrasonic/UTData.h>
#include <Ultrasonic/UTStreamFile.h>

class WriteFileModule : public QObject
{
    Q_OBJECT

public:
    explicit WriteFileModule(QObject *parent = nullptr);

public:
    bool SetFileName(const QString &name);

signals:
    void ParameterWrited();
    void TransformCompleted();
    void ProgressUpdate(int value);

public slots:
    void OnParameterReady(const QJsonObject &obj);
    void OnDataReadReady(const UTDataFrame &frame, int value);
    void OnDataCompleted();

private:
    /*!
     * \brief 向文件中写入一个QVector。这个操作会导致当前文件指针位置改变。
     * \param value 被写入的QVector
     */
    template <typename T>
    inline void WriteVector(const QVector<T> &value, QFile *file)
    {
        if (file == nullptr)
        {
            return;
        }
        int count = value.count();
        file->write( reinterpret_cast<const char *>(&count), sizeof(count) );
        if ( Q_LIKELY(count > 0) )
        {
            file->write(reinterpret_cast<const char *>( value.data() ), sizeof(T) * count);
        }
    }

    void WriteFrame(const UTDataFrame &frame);

private:
    QFile *dataFile;
    QFile *parameterFile;
    bool bWriteCompleted;
};
