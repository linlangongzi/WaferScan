#include "PanelUTGate.h"
#include "ui_PanelUTGate.h"

const double PanelUTGate::MIN_GATE_START = 0.0;
const double PanelUTGate::MAX_GATE_START = 999999.0;

const double PanelUTGate::MIN_GATE_WIDTH = 0.0;
const double PanelUTGate::MAX_GATE_WIDTH = 999999.0;

const double PanelUTGate::MIN_GATE_AMP = 0.0;
const double PanelUTGate::MAX_GATE_AMP = 100.0;

PanelUTGate::PanelUTGate(UTSetup *setup, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PanelUTGate),
    m_pUTSetup(setup)
{
    ui->setupUi(this);
    InitUI();
}

PanelUTGate::~PanelUTGate()
{
    if (m_pUTSetup)
    {
        m_pUTSetup = NULL;
    }
    if (gateItemModel)
    {
        delete gateItemModel;
        gateItemModel = NULL;
    }
    if (dGateStartValidator)
    {
        delete dGateStartValidator;
        dGateStartValidator = NULL;
    }
    if (dGateWidthValidator)
    {
        delete dGateWidthValidator;
        dGateWidthValidator = NULL;
    }
    if (dGateAmpValidator)
    {
        delete dGateAmpValidator;
        dGateAmpValidator = NULL;
    }
    delete ui;
}

void PanelUTGate::ModifyGate(const QModelIndex &index)
{
    int nRow = index.row();
    int nCol = index.column();
    QString ItemStr = gateItemModel->item(nRow, nCol)->text();
    switch (nCol)
    {
    case 1:
        ModifyGateValue(nRow, nCol, ItemStr, dGateStartValidator);
        break;
    case 2:
        ModifyGateValue(nRow, nCol, ItemStr, dGateWidthValidator);
        break;
    case 3:
        ModifyGateValue(nRow, nCol, ItemStr, dGateAmpValidator);
        break;
    default:
        break;
    }
}

void PanelUTGate::InitGateControl(UTSetup *setup, int index)
{
    gateIndex = index; // 记录当前表中操作的闸门号
    // 将控件与闸门的参数进行绑定
    QString gateStart = QString("Start");
    QString gateWidth = QString("Width");
    QString gateAmp = QString("Amp");
    ui->gateStartSpinBox->SetParameterKey(gateStart);
    ui->gateWidthSpinBox->SetParameterKey(gateWidth);
    ui->gateAmpSpinBox->SetParameterKey(gateAmp);
    ui->gateStartSlider->SetParameterKey(gateStart);
    ui->gateWidthSlider->SetParameterKey(gateWidth);
    ui->gateAmpSlider->SetParameterKey(gateAmp);
    if (setup)
    {
        // 先设定slider控件的范围，再进行连接
        UpdateSliderRange("Delay", setup->Get(m_pUTSetup->GetWaveType().append("Delay")));
        UpdateSliderRange("Range", setup->Get("Range"));

        ui->gateStartSpinBox->ConnectTo(setup);
        ui->gateAmpSpinBox->ConnectTo(setup);
        ui->gateWidthSpinBox->ConnectTo(setup);
        ui->gateStartSlider->ConnectTo(setup);
        ui->gateWidthSlider->ConnectTo(setup);
        ui->gateAmpSlider->ConnectTo(setup);
    }
}

/**
 * 更新slider和spinbox的控件
 * @brief PanelUTGate::UpdateGateControl
 * @param setup // 超声工艺的指针
 * @param index // 闸门的序号
 */
void PanelUTGate::UpdateGateControl(UTSetup *setup, int index)
{
    if (setup)
    {
        ui->gateStartSpinBox->Disconnect(setup);
        ui->gateAmpSpinBox->Disconnect(setup);
        ui->gateWidthSpinBox->Disconnect(setup);
        ui->gateStartSlider->Disconnect(setup);
        ui->gateWidthSlider->Disconnect(setup);
        ui->gateAmpSlider->Disconnect(setup);
    }
    InitGateControl(setup, index);
}

void PanelUTGate::InitUI()
{
    gateItemModel = new QStandardItemModel(this);

    ui->gateStartSpinBox->setRange(MIN_GATE_START, MAX_GATE_START);
    ui->gateWidthSpinBox->setRange(MIN_GATE_WIDTH, MAX_GATE_WIDTH);
    ui->gateAmpSpinBox->setRange(MIN_GATE_AMP, MAX_GATE_AMP);

    ui->gateTableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->gateTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->gateTableView->setSelectionMode(QAbstractItemView::SingleSelection);


    dGateStartValidator = new QDoubleValidator(MIN_GATE_START, MAX_GATE_START, 10, this);
    dGateWidthValidator = new QDoubleValidator(MIN_GATE_WIDTH, MAX_GATE_WIDTH, 10, this);
    dGateAmpValidator = new QDoubleValidator(MIN_GATE_AMP, MAX_GATE_AMP, 10, this);

    InitModel();
    // 连接UTSetup
    if (m_pUTSetup)
    {
        connect(m_pUTSetup, SIGNAL(ValueChangedSetup(QString, QJsonValue)), this, SLOT(UpdateModel(QString, QJsonValue)));
        InitGateControl(m_pUTSetup, 1);
    }
    connect(gateItemModel, SIGNAL(dataChanged(QModelIndex, QModelIndex)), this, SLOT(ModifyGate(QModelIndex)));
    ui->gateTableView->horizontalScrollBar()->setValue(1);   // 显示表中需要查看的闸门数据的位置
}

void PanelUTGate::InitModel()
{
    QStringList headerList;
    headerList << tr("GateName") << tr("GateStart") << tr("GateWidth") << tr("GateAmp");
    gateItemModel->setHorizontalHeaderLabels(headerList);
    if (m_pUTSetup)
    {
        int GateNums = m_pUTSetup->GetNumGates();
        for (int GateIndex = 0; GateIndex < GateNums; GateIndex++)
        {
            InsertGateListFromUTSetup(m_pUTSetup, GateIndex);
        }
    }
    ui->gateTableView->setModel(gateItemModel);
}

void PanelUTGate::ModifyGateValue(int row, int col, QString &value, QDoubleValidator *validator)
{
    QString gateParameter;
    switch(col)
    {
    case 1:
        gateParameter = QString(m_pUTSetup->GetWaveType().append("Gate%1Start")).arg(row + 1);
        break;
    case 2:
        gateParameter = QString(m_pUTSetup->GetWaveType().append("Gate%1Width")).arg(row + 1);
        break;
    case 3:
        gateParameter = QString(m_pUTSetup->GetWaveType().append("Gate%1Amp")).arg(row + 1);
        break;
    default:
        break;
    }
    int pos = 1;
    if (QValidator::Invalid == validator->validate(value, pos))
    {
        QMessageBox::warning(this, tr("Warning"), tr("Warning::input is invalid!"));
        gateItemModel->item(row, col)->setText(QString::number(m_pUTSetup->Get(gateParameter).toDouble()));
        return;
    }
    else
    {
        m_pUTSetup->Set(gateParameter, value.toDouble());
    }
}

void PanelUTGate::InsertGateListFromUTSetup(UTSetup *setup, int index)
{
    StdItemList itemList;
    itemList = GetGateStandardItemList(setup, index);
    gateItemModel->appendRow(itemList);
}

PanelUTGate::StdItemList PanelUTGate::GetGateStandardItemList(UTSetup *setup, int index)
{
    StdItemList itemList;
    for (int i = 0; i < 4; i++)  // 4为Table表所包含的列数
    {
        QStandardItem *item;
        switch(i)
        {
        case 0:
            item = new QStandardItem(QString("Gate%1").arg(index + 1));
            break;
        case 1:
            item = new QStandardItem(QString::number(setup->Get(QString(m_pUTSetup->GetWaveType().append("Gate%1Start")).arg(index + 1)).toDouble(), '.', 2));
            break;
        case 2:
            item = new QStandardItem(QString::number(setup->Get(QString(m_pUTSetup->GetWaveType().append("Gate%1Width")).arg(index + 1)).toDouble(), '.', 2));
            break;
        case 3:
            item = new QStandardItem(QString::number(setup->Get(QString(m_pUTSetup->GetWaveType().append("Gate%1Amp")).arg(index + 1)).toDouble(), '.', 2));
            break;
        default:
            break;
        }
        itemList.append(item);
    }
    return itemList;
}

/**
 * // 重新选择一行时更新相关的控制控件
 * @brief PanelUTGate::on_gateTableView_pressed
 * @param index   // 闸门的号
 */
void PanelUTGate::on_gateTableView_pressed(const QModelIndex &index)
{
    int nRow = index.row();
    if (m_pUTSetup)
    {
        UpdateGateControl(m_pUTSetup, nRow + 1);
    }
    ui->gateTableView->selectRow(nRow);  // 设定当前所选的行
}

void PanelUTGate::UpdateModel(const QString &key, const QJsonValue &value)
{
    Q_UNUSED(value);
    if (key.startsWith(m_pUTSetup->GetWaveType().append("Gate")))
    {
        gateItemModel->clear();
        InitModel();        
    }
    UpdateSliderRange(key, value);
    ui->gateTableView->horizontalScrollBar()->setValue(1);   // 显示表中需要查看的闸门数据的位置
}

void PanelUTGate::UpdateSliderRange(const QString &key, const QJsonValue &value)
{
    double gate1Start = m_pUTSetup->Get(m_pUTSetup->GetWaveType().append("Gate1Start")).toDouble();   // 其他闸门的零点是以1号闸门的起点
    double cGateStart = m_pUTSetup->Get(QString(m_pUTSetup->GetWaveType().append("Gate%1Start")).arg(gateIndex)).toDouble();
    double cgateWidth = m_pUTSetup->Get(QString(m_pUTSetup->GetWaveType().append("Gate%1Width")).arg(gateIndex)).toDouble();
    if (key == "Delay")
    {
        m_delay = value.toDouble();
    }
    if (key == "Range")
    {
        m_range = value.toDouble();
    }

    if (gateIndex != 1)
    {
        ui->gateWidthSlider->setRange(MIN_GATE_WIDTH, static_cast<int>(m_range + m_delay - cGateStart - gate1Start) * 100);
        ui->gateStartSlider->setRange(MIN_GATE_START, static_cast<int>((m_range + m_delay - cgateWidth - gate1Start) * 100));
    }
    else
    {
        ui->gateWidthSlider->setRange(MIN_GATE_WIDTH, static_cast<int>((m_range + m_delay- gate1Start) * 100));
        ui->gateStartSlider->setRange(MIN_GATE_START, static_cast<int>((m_range + m_delay - cgateWidth) * 100));
    }

    ui->gateAmpSlider->setRange(MIN_GATE_AMP, static_cast<int>(MAX_GATE_AMP * 100));
}
