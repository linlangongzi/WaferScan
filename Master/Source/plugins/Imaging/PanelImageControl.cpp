#include "PanelImageControl.h"
#include "ui_PanelImageControl.h"
#include "ImagingConstants.h"
#include "PanelCScan.h"
#include "PanelImport.h"
#include <Core/actionmanager/actioncontainer.h>
#include <Core/actionmanager/actionmanager.h>
#include <Core/DocumentManager.h>
#include <Core/ICore.h>
#include <Core/IDocument.h>
#include <QDockWidget>
#include <QListWidgetItem>
#include <QLoggingCategory>
#include <QMessageBox>

using namespace Core;

static Q_LOGGING_CATEGORY(debug, "image.control");

PanelImageControl::PanelImageControl(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PanelImageControl)
{
    ui->setupUi(this);
    connect(DocumentManager::instance(), &DocumentManager::currentDocumentChanged, this, &PanelImageControl::refresh);
    connect(DocumentManager::instance(), &DocumentManager::currentDocumentChanged, [=](){
        IDocument *doc = DocumentManager::currentDocument();
        if (doc) {
            connect(doc, &IDocument::partChanged, this, &PanelImageControl::refresh, Qt::UniqueConnection);
        }
    });

    ActionContainer *fileMenu = ActionManager::actionContainer(Core::Constants::M_FILE);
    fileMenu->insertGroup(Core::Constants::G_FILE_OTHER, Imaging::G_IMPORT);

    Command *cmd = ActionManager::registerAction(ui->actionImport, Imaging::IMPORT);
    fileMenu->addAction(cmd, Imaging::G_IMPORT);
    fileMenu->addSeparator(Imaging::G_IMPORT);
}

PanelImageControl::~PanelImageControl()
{
    delete ui;
}

void PanelImageControl::refresh()
{
    ui->partList->clear();

    MainWindow *mw = ICore::mainWindow();
    auto *doc = DocumentManager::currentDocument();
    auto records = doc->parts<ImageRecord>();
    if (records.size() <= 0) {
        return;
    }

    PanelCScan *cscan = mw->findChild<PanelCScan *>();
    if (!cscan) {
        cscan = new PanelCScan(mw);
        cscan->setObjectName("CScan");
        mw->addDockWidget(Qt::LeftDockWidgetArea, mw->addDockForWidget(cscan));
    }

    for (ImageRecord *record: records) {
        QString name = record->fileName().section("/", -1);
        auto *item = new QListWidgetItem();
        item->setText(name);
        item->setData(Qt::UserRole, QVariant::fromValue(record));
        ui->partList->addItem(item);
    }

    cscan->setImageRecord(records.first());
    ui->partList->item(0)->setSelected(true);
}

void PanelImageControl::on_partList_itemClicked(QListWidgetItem *item)
{
    PanelCScan *cscan = ICore::mainWindow()->findChild<PanelCScan *>();
    if (!cscan) {
        return;
    }
    cscan->setImageRecord(item->data(Qt::UserRole).value<ImageRecord *>());
}

void PanelImageControl::on_actionImport_triggered()
{
    PanelImport dialog;
    dialog.exec();
}
