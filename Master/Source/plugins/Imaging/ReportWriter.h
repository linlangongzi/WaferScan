#pragma once

#include <QObject>
#include <QMap>

/*!
 * \brief ReportWriter 报告生成器
 * 这个类是检测报告生成器的接口类。
 *
 * 报告生成
 */

class ReportWriter : public QObject
{
    Q_OBJECT

public:
    typedef QVariantMap ReportRow;
    typedef QList<ReportRow> ReportTable;
    typedef QMap<QString, ReportTable> ReportData;

public:
    explicit ReportWriter(QObject *parent = nullptr);

    virtual bool Generate(const QString &templatePath, const QString &outputPath, const ReportData &report) = 0;
    QString GetLastError() const {return lastError;}

protected:
    bool SetLastError(const QString &err = QString());

    QVariant GetReportValue(const QString &key, const QString &context, int index);

private:
    QString lastError;
};
