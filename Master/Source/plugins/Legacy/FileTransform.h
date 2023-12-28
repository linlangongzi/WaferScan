#pragma once

#include <QWidget>
#include "ReadFileModule.h"
#include "WriteFileModule.h"

namespace Ui {
class FileTransform;
}

class FileTransform : public QWidget
{
    Q_OBJECT

public:
    explicit FileTransform(QWidget *parent = 0);
    ~FileTransform();

private slots:
    void OnOldFilePath();
    void OnNewFilePath();
    void OnTransform();
    void OnCancel();
    void OnProgressUpdate(int value);
    void OnTransformCompleted();

private:
    void InitUI();

private:
    Ui::FileTransform *ui;
    ReadFileModule *readModule;
    WriteFileModule *writeModule;
};
