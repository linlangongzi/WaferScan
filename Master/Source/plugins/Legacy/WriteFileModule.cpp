#include "WriteFileModule.h"
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <QDebug>
#include <QJsonDocument>

WriteFileModule::WriteFileModule(QObject *parent) :
    QObject(parent),
    dataFile(nullptr),
    parameterFile(nullptr),
    bWriteCompleted(false)
{}

/*!
 * \brief WriteFileModule::SetFileName 设置写入文件的文件名称
 * \param name  文件名称
 * \return      true:成功
 *              false:失败
 */
bool WriteFileModule::SetFileName(const QString &name)
{
    dataFile = new QFile(name);
    if ( !dataFile->open(QIODevice::WriteOnly) ) {
        qCritical() << "WriteFileModule::SetFileName data file can not be open";
        delete dataFile;
        dataFile = nullptr;
        return false;
    }

    QString jsonName = document()->GetParameterFileName(name);
    parameterFile = new QFile(jsonName);
    if ( !parameterFile->open(QIODevice::WriteOnly) ) {
        qCritical() << "WriteFileModule::SetFileName json file can not be open";
        delete parameterFile;
        parameterFile = nullptr;
        return false;
    }

    dataFile->resize(0);
    parameterFile->resize(0);
    return true;
}

/*!
 * \brief WriteFileModule::OnParameterReady 接收到参数信号
 * \param obj
 */
void WriteFileModule::OnParameterReady(const QJsonObject &obj)
{
    if (parameterFile == nullptr) {
        qCritical() << "WriteFileModule::OnParameterReady the parameterFile is NULL";
        return;
    }
    parameterFile->write( QJsonDocument(obj).toJson() );

    parameterFile->close();
    delete parameterFile;
    parameterFile = nullptr;

    emit ParameterWrited();
}

/*!
 * \brief WriteFileModule::OnDataReadReady 接收到数据读取的消息
 * \param frame
 */
void WriteFileModule::OnDataReadReady(const UTDataFrame &frame, int value)
{
    if (dataFile == nullptr) {
        qCritical() << "WriteFileModule::OnParameterReady the dataFile is NULL";
        return;
    }
    WriteFrame(frame);
    emit ProgressUpdate(value);
}

/*!
 * \brief WriteFileModule::OnDataCompleted 接收数据读取完成信号
 */
void WriteFileModule::OnDataCompleted()
{
    if (dataFile) {
        dataFile->close();
        emit TransformCompleted();
    }
}

/*!
 * \brief WriteFileModule::WriteFrame
 * \param frame
 */
void WriteFileModule::WriteFrame(const UTDataFrame &frame)
{
    if (dataFile == nullptr) {
        qCritical() << "WriteFileModule::OnParameterReady the dataFile is NULL";
        return;
    }
    WriteVector(frame.AData, dataFile);
    WriteVector(frame.CData, dataFile);
    WriteVector(frame.Pose, dataFile);                                         /* Pose 只需要写入Vector部分即可 */
    WriteVector(frame.Context, dataFile);
//    frame.FileOffset = dataFile->pos();
}
