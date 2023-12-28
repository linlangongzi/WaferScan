#include "PipeParameter.h"
#include "ui_PipeParameter.h"
#include <Imaging/UTDocument.h>
#include <allrising/Util.h>

const QString PipeParameter::DEFAULT_PIPE_PARAMETER_PATH = "./PipeParameter.json";

PipeParameter::PipeParameter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PipeParameter)
{
    ui->setupUi(this);
    Connect();
}

PipeParameter::~PipeParameter()
{
    SaveFile(DEFAULT_PIPE_PARAMETER_PATH);
    delete ui;
}

void PipeParameter::OpenFile(const QString &path)
{
    FromJson(ReadFileAsJson(path));
}

PipeParameter::OrderInfo PipeParameter::getOrderInfo()
{
    return orderInfo;
}

void PipeParameter::UpdateParamInfo()
{
    // 材料参数
    ui->materialDiameter->setValue(orderInfo.materialDiameter);
    ui->materialLength->setValue(orderInfo.materialLength);
    ui->thickShowMax->setValue(orderInfo.thickShowMax);
    ui->thickShowMin->setValue(orderInfo.thickShowMin);
    ui->thickMax->setValue(orderInfo.thickMax);
    ui->thickMin->setValue(orderInfo.thickMin);
    ui->materialThick->setValue(orderInfo.materialThick);
    ui->orderCount->setValue(orderInfo.orderCount);
    ui->materialNumber->setText(orderInfo.materialNumber);
    ui->orderNumber->setText(orderInfo.orderNumber);

    CheckComboBoxText(ui->name, orderInfo.workerName);
    CheckComboBoxText(ui->classes, orderInfo.workerClasses);
    CheckComboBoxText(ui->materialLevel, orderInfo.materialLevel);
    CheckComboBoxText(ui->comboBoxStandard, orderInfo.testStandard);
    CheckComboBoxText(ui->comboBoxLevel, orderInfo.testLevel);
}

void PipeParameter::saveToDocument()
{
    if (document()) {
        document()->Set("OrderParameter", ToJson());
    }
}

QJsonValue PipeParameter::ToJson()
{
    QJsonObject obj;
    obj["materialDiameter"]  = orderInfo.materialDiameter;
    obj["materialLength"]    = orderInfo.materialLength;
    obj["thickShowMax"]      = orderInfo.thickShowMax;
    obj["thickShowMin"]      = orderInfo.thickShowMin;
    obj["showStart"]         = orderInfo.showStart;
    obj["showEnd"]           = orderInfo.showEnd;
    obj["thickMax"]          = orderInfo.thickMax;
    obj["thickMin"]          = orderInfo.thickMin;
    obj["materialThick"]     = orderInfo.materialThick;
    obj["orderCount"]        = orderInfo.orderCount;
    obj["orderNumber"]       = orderInfo.orderNumber;
    obj["materialNumber"]    = orderInfo.materialNumber;
    obj["testLevelText"]     = orderInfo.testLevel;
    obj["workerNameText"]    = orderInfo.workerName;
    obj["materialLevelText"] = orderInfo.materialLevel;
    obj["workerClassesText"] = orderInfo.workerClasses;
    obj["testStandardText"]  = orderInfo.testStandard;

    obj["testLevel"] = ComboBoxInfo2Json(ui->comboBoxLevel, "testLevel");
    obj["workerName"] = ComboBoxInfo2Json(ui->name, "workerName");
    obj["materialLevel"] = ComboBoxInfo2Json(ui->materialLevel, "materialLevel");
    obj["workerClasses"] = ComboBoxInfo2Json(ui->classes, "workerClasses");
    obj["testStandard"] = ComboBoxInfo2Json(ui->comboBoxStandard, "testStandard");

    return obj;
}

/*!
 * \brief PipeParameter::FromJson
 * \param json
 * \param emitChanged false:刷新控件和报表 true：全部刷新(B扫，C扫)
 */
void PipeParameter::FromJson(const QJsonValue &json, bool emitChanged)
{
    const QJsonObject obj = json.toObject();
    orderInfo.materialDiameter  = obj["materialDiameter"].toDouble();
    orderInfo.materialLength    = obj["materialLength"].toDouble();
    orderInfo.thickShowMax      = obj["thickShowMax"].toDouble();
    orderInfo.thickShowMin      = obj["thickShowMin"].toDouble();
    orderInfo.showStart         = obj["showStart"].toDouble();
    orderInfo.showEnd           = obj["showEnd"].toDouble();
    orderInfo.thickMax          = obj["thickMax"].toDouble();
    orderInfo.thickMin          = obj["thickMin"].toDouble();
    orderInfo.materialThick     = obj["materialThick"].toDouble();
    orderInfo.orderCount        = obj["orderCount"].toInt();
    orderInfo.orderNumber       = obj["orderNumber"].toString();
    orderInfo.materialNumber    = obj["materialNumber"].toString();
    orderInfo.workerName        = obj["workerNameText"].toString();
    orderInfo.workerClasses     = obj["workerClassesText"].toString();
    orderInfo.materialLevel     = obj["materialLevelText"].toString();
    orderInfo.testStandard      = obj["testStandardText"].toString();
    orderInfo.testLevel         = obj["testLevelText"].toString();

    Json2ComboBoxInfo(ui->name, "workerName", obj["workerName"].toArray());
    Json2ComboBoxInfo(ui->classes, "workerClasses", obj["workerClasses"].toArray());
    Json2ComboBoxInfo(ui->materialLevel, "materialLevel", obj["materialLevel"].toArray());
    Json2ComboBoxInfo(ui->comboBoxStandard, "testStandard", obj["testStandard"].toArray());
    Json2ComboBoxInfo(ui->comboBoxLevel, "testLevel", obj["testLevel"].toArray());

    resetReportKeys();
    UpdateParamInfo();

    if (emitChanged) {
        emit ParamChanged(orderInfo);
    }
}

void PipeParameter::resetOrderParameter()
{
    OpenFile();
}

void PipeParameter::Connect()
{
    connect(ui->pushButtonYes, &QAbstractButton::clicked, this, &PipeParameter::Yes);
    connect(ui->pushButtonClose, &QAbstractButton::clicked, this, &PipeParameter::Close);
    connect(ui->pushButtonReset, &QAbstractButton::clicked, this, &PipeParameter::resetOrderParameter);
}

void PipeParameter::SaveFile(const QString &path)
{
    WriteFileAsJson(path, ToJson());
    saveToDocument();
}

/*!
 * \brief PipeParameter::UpdateComboBoxText
 * \param box
 * \param text
 * 检查comboBox中是否含有此text，并切换到当前text的index
 */
void PipeParameter::CheckComboBoxText(QComboBox *&box, const QString &text)
{
    if (-1 == box->findText(text)) {
        box->addItem(text);
    }


    box->setCurrentIndex(box->findText(text));
}

QJsonObject PipeParameter::ReportObj(const QJsonArray &array, const QString &key, const QString &name, const QString &value)
{
    QJsonObject obj;
    obj["Enum"] = array;
    obj["Key"] = key;
    obj["Name"] = name;
    obj["Value"] = value;
    return obj;
}

void PipeParameter::resetReportKeys()
{
    QJsonArray array;
    array.append(ReportObj(QJsonArray(), "OrderID",         tr("订单编号"),    orderInfo.orderNumber));
    array.append(ReportObj(QJsonArray(), "OrderCount",      tr("订单数量"),    QString::number(orderInfo.orderCount)));
    array.append(ReportObj(QJsonArray(), "MaterialID",      tr("材料编号"),    orderInfo.materialNumber));
    array.append(ReportObj(QJsonArray(), "MaterialThick",   tr("材料厚度"),    QString::number(orderInfo.materialThick)));
    array.append(ReportObj(QJsonArray(), "MaterialDiameter",tr("材料直径"),    QString::number(orderInfo.materialDiameter)));
    array.append(ReportObj(QJsonArray(), "MaterialLength",  tr("材料长度"),    QString::number(orderInfo.materialLength)));
    array.append(ReportObj(QJsonArray(), "MaterialLevel",   tr("材料等级"),    orderInfo.materialLevel));
    array.append(ReportObj(QJsonArray(), "Standard",        tr("检测标准"),    orderInfo.testStandard));
    array.append(ReportObj(QJsonArray(), "Level",           tr("判伤等级"),    orderInfo.testLevel));
    array.append(ReportObj(QJsonArray(), "Shift",           tr("班次"),        orderInfo.workerClasses));
    array.append(ReportObj(QJsonArray(), "Personal",        tr("操作工"),      orderInfo.workerName));
    array.append(ReportObj(QJsonArray(), "Result",          tr("检测结果"),    tr("未判伤")));

    platform()->Set("ReportKeys", array);
}

void PipeParameter::Json2ComboBoxInfo(QComboBox *box, const QString &key, const QJsonArray &array)
{
    box->clear();
    for (int i = 0; i < array.count(); ++i) {
        QJsonObject obj = array[i].toObject();
        box->addItem(obj[key].toString());
    }
}

QJsonArray PipeParameter::ComboBoxInfo2Json(QComboBox *box, const QString &key)
{
    QJsonArray array;

    for (int index = 0; index < box->count(); ++index) {
        box->setCurrentIndex(index);
        QJsonObject obj;
        obj[key] = box->currentText();
        array.append(obj);
    }

    return array;
}

void PipeParameter::Close()
{
    this->close();
}

void PipeParameter::Yes()
{
    // 材料参数
    orderInfo.materialDiameter = ui->materialDiameter->value();
    orderInfo.materialLength = ui->materialLength->value();
    orderInfo.materialNumber = ui->materialNumber->text();
    orderInfo.thickShowMax = ui->thickShowMax->value();
    orderInfo.thickShowMin = ui->thickShowMin->value();
    orderInfo.showStart = 0;
    orderInfo.showEnd = orderInfo.showStart + orderInfo.materialLength;
    orderInfo.thickMax = ui->thickMax->value();
    orderInfo.thickMin = ui->thickMin->value();
    orderInfo.materialThick = ui->materialThick->value();
    orderInfo.orderCount = ui->orderCount->value();
    orderInfo.orderNumber = ui->orderNumber->text();
    orderInfo.workerName = ui->name->currentText();
    orderInfo.workerClasses = ui->classes->currentText();
    orderInfo.materialLevel = ui->materialLevel->currentText();
    orderInfo.testStandard = ui->comboBoxStandard->currentText();
    orderInfo.testLevel = ui->comboBoxLevel->currentText();

    CheckComboBoxText(ui->name, orderInfo.workerName);
    CheckComboBoxText(ui->classes, orderInfo.workerClasses);
    CheckComboBoxText(ui->materialLevel, orderInfo.materialLevel);
    CheckComboBoxText(ui->comboBoxStandard, orderInfo.testStandard);
    CheckComboBoxText(ui->comboBoxLevel, orderInfo.testLevel);

    resetReportKeys();
    SaveFile(DEFAULT_PIPE_PARAMETER_PATH);

    emit ParamChanged(orderInfo);
    Close();
}
