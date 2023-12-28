#pragma once

#include <QObject>

class FileOperateManager : public QObject
{
    Q_OBJECT

public:
    FileOperateManager(QObject *parent = nullptr);
    ~FileOperateManager();

    static QString newFile();
    static QString openFile();
    static QString saveFile();
    static int closeFile();

private:
    void createActions();

private slots:
    void on_actionOpen_triggered();
    void on_actionTaskInfo_triggered();
    void on_actionReloadFile_triggered();
    void on_actionCutFile_triggered();
};
