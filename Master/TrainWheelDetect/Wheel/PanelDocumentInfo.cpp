#include "PanelDocumentInfo.h"
#include "ui_PanelDocumentInfo.h"
#include <QDebug>
#include <Core/IDocument.h>
#include <Core/DocumentManager.h>

PanelDocumentInfo::PanelDocumentInfo(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelDocumentInfo)
{
    ui->setupUi(this);

    connect(DocumentManager::instance(), &DocumentManager::currentDocumentChanged, this, &PanelDocumentInfo::onDocumentChanged);
}

PanelDocumentInfo::~PanelDocumentInfo()
{
    delete ui;
}

void PanelDocumentInfo::refresh()
{
    const auto info = DocumentManager::currentDocument()->info();
    ui->uuid->setText(info.uuid.toString());
    ui->title->setText(info.title);
    ui->comment->setText(info.comment);
    ui->creationTool->setText(info.creationTool);
    ui->creationToolVersion->setText(info.creationToolVersion);
    ui->creationPath->setText(info.creationPath);
    ui->creationHost->setText(info.creationHost);
    ui->creationDomain->setText(info.creationDomain);
    ui->partType->setText(info.partType);
    ui->partModel->setText(info.partModel);
    ui->partIdentifier->setText(info.partIdentifier);

    QString pattern = tr("yyyy年MM月dd日 hh时mm分ss秒");
    ui->creationTime->setText(info.creationTime.toString(pattern));
    ui->aquisitionStart->setText(info.aquisitionStart.toString(pattern));
    ui->aquisitionEnd->setText(info.aquisitionEnd.toString(pattern));
}

void PanelDocumentInfo::onDocumentChanged()
{
    disconnect(SLOT(refresh()));
    connect(DocumentManager::currentDocument(), &IDocument::infoChanged, this, &PanelDocumentInfo::refresh);
    refresh();
}

void PanelDocumentInfo::actionModify(bool checked)
{
    ui->comment->setEnabled(checked);

    if (!checked) {
        const auto document = DocumentManager::currentDocument();
        auto info = document->info();
        info.comment = ui->comment->text();
        document->setInfo(info);
    }
}
