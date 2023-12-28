#include "WizardArdFileMaintain.h"
#include "ui_WizardArdFileMaintain.h"
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QProgressDialog>
#include <Ultrasonic/UTData.h>
#include <Ultrasonic/UTStreamFile.h>
#include <Ultrasonic/UltrasonicManager.h>

#ifdef QT_CONCURRENT_LIB
#   include <QtConcurrent/QtConcurrent>
#   include <functional>
#endif

WizardArdFileMaintain::WizardArdFileMaintain(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::WizardArdFileMaintain)
{
    ui->setupUi(this);

    setButtonText(QWizard::NextButton, tr("下一步"));
    setButtonText(QWizard::CancelButton, tr("取消"));
    setButtonText(QWizard::FinishButton, tr("完成"));

    QStringList str;
    str << tr("请选择闸门") << tr("闸门 1") << tr("闸门 2") << tr("闸门 3") << tr("闸门 4");
    ui->ArdGateFilter->addItems(str);
    ui->ArdGateFilter->setCurrentIndex(0);

    for (UTChannel *channel: UltrasonicManager::channels()) {
        ui->channel->addItem(channel->name(), channel->channelId());
    }
}

WizardArdFileMaintain::~WizardArdFileMaintain()
{
    delete ui;
}

bool WizardArdFileMaintain::validateCurrentPage()
{
    QWizardPage *page = currentPage();
    if (page == ui->wizardPageInput) {
        if (ui->InputList->count() == 0) {
            QMessageBox::warning(this, QString(), tr("请至少选择一个ard文件"));
            return false;
        }

        if (ui->DefaultExportFileName->text().isEmpty()) {
            QMessageBox::warning(this, QString(), tr("请填写默认导出文件名后缀"));
            return false;
        }

        QFileInfo fileInfo(ui->DefaultExportPath->text());
        if (!fileInfo.isAbsolute()) {
            QMessageBox::warning(this, QString(), tr("请确认默认导出文件路径是否有效"));
            return false;
        }
        QDir dir(ui->DefaultExportPath->text());
        if (!dir.exists()) {
            dir.mkdir(ui->DefaultExportPath->text());
        }

        ui->ArdFilterAscanClear->setChecked(true);
        ui->ArdGateFilter->setEnabled(false);
    }

    if ((page == ui->wizardPageArdFilter) || (page == ui->wizardPageCsvFilter)) {
        if (ui->ArdFilterAscanTrigger->isChecked() && (ui->ArdGateFilter->currentIndex() == 0)) {
            QMessageBox::warning(this, QString(), tr("请选择过滤闸门"));
            return false;
        }

        ui->TotalProgressBar->setMinimum(0);
        ui->TotalProgressBar->setMaximum(ui->InputList->count());
        ui->TotalProgressBar->setValue(0);

        ui->CurrentProgressBar->setValue(0);
        ui->CurrentFile->clear();

        QMetaObject::invokeMethod(this, "OnLoopTask", Qt::QueuedConnection);
    }

    return true;
}

int WizardArdFileMaintain::nextId() const
{
    if (currentPage() == ui->wizardPageInput) {
        if (ui->ExportFileArd->isChecked()) {
            return ui->wizardPageArdFilter->nextId() - 1;
        } else {
            return ui->wizardPageCsvFilter->nextId() - 1;
        }
    }
    if (currentPage() == ui->wizardPageArdFilter) {
        return pageIds().count() - 1;
    }
    return currentPage()->nextId();
}

void WizardArdFileMaintain::reject()
{
    if (currentPage() == ui->wizardPageEnd) {
        if (isStop) {
            done(Rejected);
        } else {
            isStop = true;
            button(QWizard::FinishButton)->setEnabled(true);
        }
    } else {
        done(Rejected);
    }
}

void WizardArdFileMaintain::on_InputToolButton_clicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("导入ard源文件"),
                                        QString(), tr("DataFile(*.ard)") );
    foreach (QString str, fileNames) {
        ui->InputList->addItem(str);
    }
}

void WizardArdFileMaintain::on_DelFile_clicked()
{
    delete ui->InputList->currentItem();
}

void WizardArdFileMaintain::on_DefaultExportBtn_clicked()
{
    QString filePath = QFileDialog::getExistingDirectory(this, tr("导出路径"),
               QString(),QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (filePath.isEmpty()) {
        qDebug() << "default export path not choosed";
        return;
    }
    ui->DefaultExportPath->setText(filePath);
}

void WizardArdFileMaintain::OnLoopTask()
{
    isStop = false;
    button(QWizard::FinishButton)->setEnabled(false);

    QString outFileSuffix;
    if (ui->ExportFileArd->isChecked()) {
        outFileSuffix = QString(".ard");
    }
    if (ui->ExportFileCsv->isChecked()) {
        outFileSuffix = QString(".csv");
    }

    for (int index = 0; (index < ui->InputList->count()) && (!isStop); ++index) {
        ui->TotalProgressBar->setValue(index + 1);
        QFileInfo inputInfo(ui->InputList->item(index)->text());

        QString outPath = ui->DefaultExportPath->text() + "/" + inputInfo.completeBaseName()
                           + "_" + ui->DefaultExportFileName->text() + outFileSuffix;
        QFileInfo outInfo(outPath);

        ui->CurrentFile->setText(outInfo.filePath());

        UTStreamFile file(inputInfo.filePath());
        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed opening source file " << file.fileName()
                       << ":" << file.errorString();
            continue;
        }

        UTStreamFile outFile(outInfo.filePath());
        if (!outFile.open(QIODevice::WriteOnly)) {
            qWarning() << "Failed opening output file " << outFile.fileName()
                       << ":" << outFile.errorString();
            continue;
        }

        const int maximum = 100;
        ui->CurrentProgressBar->setMaximum(maximum);

        QTextStream s(&outFile);

        qreal fileSize = file.size();
        while (!file.atEnd() && !isStop) {
            ui->CurrentProgressBar->setValue(file.pos() / fileSize * maximum);
            UTDataFrame frame = file.ReadFrame();
            if (frame.isEmpty()) {
                return;
            }

            QApplication::processEvents();
            // export ard file
            if (ui->ExportFileArd->isChecked()) {
                if (ui->ArdFilterAscanClear->isChecked()) {
                    frame.AData.clear();
                    outFile.WriteFrame(frame);
                }
                if (ui->ArdFilterAscanTrigger->isChecked()) {
                    if (!qFuzzyIsNull(frame.FeatureValue(
                        ui->ArdGateFilter->currentIndex() - 1, GateTrigger, DataTime))) {
                        frame.AData.clear();
                    }
                    outFile.WriteFrame(frame);
                }
            }
            // export csv file
            if (ui->ExportFileCsv->isChecked()) {
                if ((ui->AscanData->isChecked()) && !frame.AData.isEmpty()) {
                    s << qPrintable(frame.exportCsv(UTDataFrame::AScanDataFrame)) << endl;
                }
                if ((ui->CscanData->isChecked()) && !frame.CData.isEmpty()) {
                    s << qPrintable(frame.exportCsv(UTDataFrame::CScanDataFrame)) << endl;
                }
                if ((ui->PoseData->isChecked()) && !frame.Pose.isEmpty()) {
                    if (ui->channel->currentData() == frame.channelId()) {
                        s << qPrintable(frame.exportCsv(UTDataFrame::PoseDataFrame)) << endl;
                    }
                }
            }
        }
        ui->CurrentProgressBar->setValue(maximum);
        file.close();
        outFile.close();
    }

    ui->CurrentFile->setText(tr("全部任务已完成"));
    button(QWizard::FinishButton)->setEnabled(true);
    isStop = true;
    return;
}

void WizardArdFileMaintain::on_ArdFilterAscanClear_toggled(bool checked)
{
    if (checked) {
        ui->ArdGateFilter->setEnabled(false);
    }
}

void WizardArdFileMaintain::on_ArdFilterAscanTrigger_toggled(bool checked)
{
    if (checked) {
        ui->ArdGateFilter->setEnabled(true);
    }
}
