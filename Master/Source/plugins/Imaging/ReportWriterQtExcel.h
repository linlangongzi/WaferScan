#pragma once

#include "ReportWriter.h"

class ReportWriterQtExcel : public ReportWriter
{
    Q_OBJECT

public:
    ReportWriterQtExcel(QObject *parent = nullptr) : ReportWriter(parent) {}

    virtual bool Generate(const QString &templatePath, const QString &outputPath, const ReportData &report);
};
