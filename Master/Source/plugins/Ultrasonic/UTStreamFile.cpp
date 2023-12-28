#include "UTStreamFile.h"
#include <QApplication>
#include <QProgressDialog>
#include <QApplication>
#include <allrising/Util.h>
#include <QThread>

UTStreamFile::UTStreamFile(QObject *parent) :
    QFile(parent)
{}

UTStreamFile::UTStreamFile(const QString &name, QObject *parent) :
    QFile(name, parent)
{}

/*!
 * \brief UTStreamFile::IsDataChanged 是否有数据改动
 * \return  true:是
 *          false:否
 */
bool UTStreamFile::IsDataChanged()
{
    return bChanged;
}

/*! 重置文件改变状态 */
void UTStreamFile::ResetChangedFlag()
{
    bChanged = false;
}

void UTStreamFile::RequestReadAll(int segmentLength)
{
    static const int maximum = 10000;
    QProgressDialog progress(qApp->activeWindow());
    progress.setLabelText(tr("正在读取文件..."));
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setMinimum(0);
    progress.setMaximum(maximum);
    if (!readLock.tryLock()) {
        return;
    }
    seek(0);
    UTDataSegment segment;
    while (hasMoreFrame() && !progress.wasCanceled()) {
        progress.setValue(qreal(pos()) / qreal(size()) * maximum);
        segment = ReadSegment(segmentLength);
        emit DataReady(segment);
    }
    progress.hide();
    readLock.unlock();
    emit ReadAllCompleted();
}

void UTStreamFile::OnDataArrived(const UTDataSegment &segment)
{
    if (enabled) {
        WriteSegment(segment);
    }
}

/*!
 * \brief UTStreamFile::SetDataEnbale 设置数据的写入使能
 * \param enable 使能标志
 */
void UTStreamFile::SetDataEnable(bool enable)
{
    enabled = enable;
}

/*!
 * \brief 向文件流中写入一帧
 * \param frame 被写入的帧
 */
void UTStreamFile::WriteFrame(const UTDataFrame &frame)
{
    WriteVector(frame.AData);
    WriteVector(frame.CData);
    WriteVector(frame.Pose);                                                   /* Pose 只需要写入Vector部分即可 */
    WriteVector(frame.Context);
//    frame.FileOffset = pos();
//    if (!document()->IsFileChanged())            // 有数据写入之后，需要更改保存标志
//    {
//        document()->SetFileChanged(true);
//    }
}

/*!
 * \brief 向文件流中写入一个数据片段
 * \param segment 被写入的片段指针
 */
void UTStreamFile::WriteSegment(const UTDataSegment &segment)
{
    if (segment.isEmpty()) {
        return;
    }
    bChanged = true;
    for (const auto &frame : segment) {
        WriteFrame(frame);
    }
    flush();
}

/*!
 * \brief 从文件流中读取一帧，并填入指定的SharedDataFrame中
 * \param frame 接受存储数据的帧指针。可选参数，默认值为新建的空指针。
 * \return 存储数据的帧指针
 */
UTDataFrame UTStreamFile::ReadFrame(UTDataFrame frame)
{
    if (!isReadable()) {
        return {};
    }
    /* 成功状态，读取过程有任一失败则status为非0 */
    int status = ReadVector(frame.AData);
    status |= ReadVector(frame.CData);
    status |= ReadVector(frame.Pose);
    status |= ReadVector(frame.Context);
    if (Q_UNLIKELY(status)) {
        return {};
    }
    return frame;
}

/*!
 * \brief 从文件流中读取若干帧，组成一个片段
 * \param maxLength 读取的最大长度。
 * \return
 */
UTDataSegment UTStreamFile::ReadSegment(int maxLength)
{
    UTDataSegment segment;
    segment.reserve(maxLength);                                                 /* maxLength小于0时reserve不会有任何作用 */
    UTDataFrame frame;
    for (auto index = uint(maxLength); index; --index) {
        frame = ReadFrame();
        segment.append(frame);
    }
    return segment;
}

/*!
 * \brief UTDocument::WriteToFile 将数据写入另一个文件
 * \param file
 */
void UTStreamFile::WriteToFile(UTStreamFile *file)
{
    static const int maximum = 10000;
    QProgressDialog progress(qApp->activeWindow());
    progress.setLabelText(tr("正在保存文件..."));
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setMinimum(0);
    progress.setMaximum(maximum);

    qint64 startPos = pos();
    UTDataSegment segment;
    while (hasMoreFrame() && !progress.wasCanceled()) {
        progress.setValue(qreal(pos() - startPos) / qreal(size() - startPos) * maximum);
        segment = ReadSegment(1024);
        file->WriteSegment(segment);
    }
    progress.hide();
}
