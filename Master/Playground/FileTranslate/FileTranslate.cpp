#include "FileTranslate.h"
#include "ui_FileTranslate.h"
#include <QDebug>
#include <QByteArray>
#include <QFileDialog>
#include <QDataStream>
#include <QTextStream>
#include <QMessageBox>

FileTranslate::FileTranslate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileTranslate),
    m_ReadPath(""),
    m_MakePath("")
{
    ui->setupUi(this);
    Connect();
}

FileTranslate::~FileTranslate()
{
    delete ui;
}

void FileTranslate::Connect()
{
    connect(ui->pushButtonReadPath, SIGNAL(clicked()), this, SLOT(ReadPath()));
    connect(ui->pushButtonRead, SIGNAL(clicked()), this, SLOT(Read()));
    connect(ui->pushButtonMakePath, SIGNAL(clicked()), this, SLOT(MakePath()));
}

void FileTranslate::ReadPath()
{
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle("choose file");
    QString temppath = QDir::currentPath() + "/ReadFile";

    QDir dir;
    dir.mkdir(temppath);
    fileDialog->setDirectory(temppath);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    fileDialog->setNameFilter("ReadFile Files(*.aaf)");

    if (fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles()[0];
        ui->lineEdit1->setText(path);
        m_ReadPath = path;
    }
}

void FileTranslate::Read()
{
    if (m_ReadPath == "")
    {
        return;
    }

    QFile tmpfile(m_ReadPath);
    tmpfile.open(QIODevice::ReadOnly);

    // 头参数
    tmpfile.read((char*)(&m_HeadStruct[0].xStart), sizeof(double));
    tmpfile.read((char*)(&m_HeadStruct[0].yStart), sizeof(double));
    tmpfile.read((char*)(&m_HeadStruct[0].xEnd), sizeof(double));
    tmpfile.read((char*)(&m_HeadStruct[0].yEnd), sizeof(double));
    tmpfile.read((char*)(&m_HeadStruct[0].pipeDiameter), sizeof(double));
    tmpfile.read((char*)(&m_HeadStruct[0].axisSelect), sizeof(int));
    tmpfile.read((char*)(&m_HeadStruct[0].gridX), sizeof(float));
    tmpfile.read((char*)(&m_HeadStruct[0].gridY), sizeof(float));
    tmpfile.read((char*)(&m_HeadStruct[0].gridXSize), sizeof(int));
    tmpfile.read((char*)(&m_HeadStruct[0].gridSize), sizeof(unsigned long));

    // CGateInfo
    for (int index = 0; index < 8; ++index)
    {
        tmpfile.read((char*)(&m_CGateInfo[index]), sizeof(CGateInfo) * 4);
    }

    // 各个网格点对应的A扫数的文件位置
    int count = m_HeadStruct[0].gridSize;
    QVector<unsigned long> I_Grid;
    I_Grid.resize(count);
    tmpfile.read((char*)(I_Grid.data()), sizeof(unsigned long) * count);

    // A扫数据
    if (m_MakePath == "")
    {
        qDebug() << "m_MakePath is empty";
        return;
    }

    QFile fileMake(m_MakePath);
    fileMake.open(QIODevice::ReadWrite);
    QTextStream srteam(&fileMake);
    for (int channel = 0; channel < count; ++channel)
    {
        int channelIndex = 0;
        CPicData_L *data = new CPicData_L;
        tmpfile.read((char*)(&channelIndex), sizeof(int));
        tmpfile.read((char*)(data), sizeof(CPicData_L));

        for (int j = 0; j < 512; ++j)
        {
            srteam << data->m_wData[j];
            srteam << " ";
            if (j == 511)
            {
                srteam << "\r\n" ;
            }
        }
    }

    endl(srteam);

    tmpfile.close();
    QMessageBox::information(this, "MakeFile", "Make file succeed", QMessageBox::Yes);
}

void FileTranslate::MakePath()
{
    QString FileName = QFileDialog::getSaveFileName( this, "New FileASCII", "FileASCII", "FileASCII File(*.txt)");

    if ( FileName.isEmpty() )
    {
        qDebug() << "FileTranslate::MakePath(): fileName is empty";
        return;
    }

    m_MakePath = FileName;
    ui->lineEdit2->setText(FileName);
}
