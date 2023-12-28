#include "FileTransform.h"
#include "ui_FileTransform.h"
#include <Core/CoreConstants.h>
#include <QFileDialog>
#include <QMessageBox>

using namespace Core::Constants;

FileTransform::FileTransform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileTransform)
{
    ui->setupUi(this);
    InitUI();
}

FileTransform::~FileTransform()
{
    delete ui;
}

/*! 初始化界面 */
void FileTransform::InitUI()
{
    ui->lineEdit_newfileName->setText( tr("数据%1").arg(ARS_FILE_EXT_DATA) );

    connect(ui->btn_oldfilepath, &QAbstractButton::clicked, this, &FileTransform::OnOldFilePath);
    connect(ui->btn_newfilepath, &QAbstractButton::clicked, this, &FileTransform::OnNewFilePath);
    connect(ui->btn_trans, &QAbstractButton::clicked, this, &FileTransform::OnTransform);
    connect(ui->btn_cancel, &QAbstractButton::clicked, this, &FileTransform::OnCancel);

    readModule = new ReadFileModule(this);
    writeModule = new WriteFileModule(this);

    connect(readModule, &ReadFileModule::ParameterReady, writeModule, &WriteFileModule::OnParameterReady);
    connect(readModule, &ReadFileModule::DataReadReady, writeModule, &WriteFileModule::OnDataReadReady);
    connect(readModule, &ReadFileModule::DataReadCompleted, writeModule, &WriteFileModule::OnDataCompleted);
    connect(writeModule, &WriteFileModule::TransformCompleted, this, &FileTransform::OnTransformCompleted);
    connect(writeModule, &WriteFileModule::ProgressUpdate, this, &FileTransform::OnProgressUpdate);
}

/*! 选择旧水浸文件槽函数 */
void FileTransform::OnOldFilePath()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr("打开文件..."), QString(), tr("文件(*.uit)") );
    if (fileName.isEmpty()) {
        return;
    }
    ui->lineEdit_oldfileName->setText(fileName);
}

void FileTransform::OnNewFilePath()
{
    QString fileName = QFileDialog::getSaveFileName( this, tr("新建文件"), QString(), tr("文件(*%1)").arg(ARS_FILE_EXT_DATA) );
    if (fileName.isEmpty()) {
        return;
    }
    ui->lineEdit_newfileName->setText(fileName);
}

void FileTransform::OnTransform()
{
    if ( (readModule == nullptr) || (writeModule == nullptr) ) {
        return;
    }
    if ( !readModule->SetFileName( ui->lineEdit_oldfileName->text() ) ) {
        QMessageBox::warning( this, tr("警告"), tr("旧文件是无效的") );
        return;
    }
    if ( !writeModule->SetFileName( ui->lineEdit_newfileName->text() ) ) {
        QMessageBox::warning( this, tr("警告"), tr("新文件是无效的") );
        return;
    }
    readModule->ReadFile();
}

void FileTransform::OnCancel()
{
    close();
}

/*! 更新转换进度 */
void FileTransform::OnProgressUpdate(int value)
{
    ui->progressBar->setValue(value);
}

/*! 文件转换完成信号 */
void FileTransform::OnTransformCompleted()
{
    QMessageBox::information(this, QString(), tr("文件转换成功"));
}
