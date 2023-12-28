#include "PanelChannelMgr.h"
#include "ui_PanelChannelMgr.h"

#include <QCompleter>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include "Platform/Platform.h"
#include "ChannelManager/ChannelManager.h"
#include "AbstractModel/ItemModel.h"
#include "AbstractModel/ChannelItemModel.h"

PanelChannelMgr::PanelChannelMgr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelChannelMgr)
{
    ui->setupUi(this);
    platform()->GetChannelManager()->OnReadFile("./ChannelList");
    Init();
    ConnectTo();
}

PanelChannelMgr::~PanelChannelMgr()
{
    delete ui;
}
/*!
 * \brief PanelChannelMgr::Init 初始化lineEdit_Filter，设置清空按钮、限制其最多只能输入5个数字
 */
void PanelChannelMgr::Init()
{
    ui->lineEdit_Filter->setClearButtonEnabled(true);
    QRegExp regExp("[0-9][0-9][0-9][0-9][0-9]"); //5个字符
    QRegExpValidator *validator = new QRegExpValidator (regExp, this);
    ui->lineEdit_Filter->setValidator(validator);
    ExpandTreeView();
}

void PanelChannelMgr::ConnectTo()
{
    connect(ui->lineEdit_Filter, SIGNAL(textChanged(const QString &)), this, SLOT(applyFilter(const QString&)));
    connect(ui->treeView, SIGNAL(SendSelectedChannelNo(int, WaveType)), this, SIGNAL(SendChannelNo(int, WaveType)));
}
/*!
 * \brief PanelChannelMgr::applyFilter 槽函数，Edit中数据改变时将Edit中所输入数据作为正则表达式设置到代理Model中
 * \param text 输入框Edit中的数据
 */
void PanelChannelMgr::applyFilter(const QString &text)
{
    QRegExp regExp(text, Qt::CaseInsensitive, QRegExp::RegExp);
    ChannelFilterProxyModel *pProxyModel = ui->treeView->GetModel();

    if (pProxyModel != NULL)
    {
        pProxyModel->setFilterRegExp(regExp);
        ExpandTreeView();
    }
}

void PanelChannelMgr::ExpandTreeView()
{
    ui->treeView->expandAll();
}


