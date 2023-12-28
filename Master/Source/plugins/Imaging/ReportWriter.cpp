#include "ReportWriter.h"

ReportWriter::ReportWriter(QObject *parent) :
    QObject(parent)
{}

bool ReportWriter::SetLastError(const QString &err)
{
    lastError = err;
    return err.isEmpty();
}
