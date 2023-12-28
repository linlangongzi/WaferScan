#pragma once

#include <Ars1000/PipeDataDef.h>
#include <Core/Platform.h>
#include <QComboBox>
#include <QDialog>
#include <QJsonArray>

namespace Ui {
class PipeParameter;
}

class PipeParameter : public QDialog
{
    Q_OBJECT
public:
    typedef PipeDetect::OrderInfo OrderInfo;

public:
    explicit PipeParameter(QWidget *parent = 0);
    ~PipeParameter();
    void OpenFile(const QString &path = DEFAULT_PIPE_PARAMETER_PATH);
    OrderInfo getOrderInfo();
    void UpdateParamInfo();
    void saveToDocument();
    QJsonValue ToJson();
    void FromJson(const QJsonValue &json, bool emitChanged = true);

public slots:
    void resetOrderParameter();

signals:
    void ParamChanged(const OrderInfo &info);

private:
    void Connect();
    void SaveFile(const QString &path);
    void CheckComboBoxText(QComboBox *&box, const QString &text);
    QJsonObject ReportObj(const QJsonArray &array, const QString &key, const QString &name, const QString &value);
    void resetReportKeys();

private slots:
    void Json2ComboBoxInfo(QComboBox *box, const QString &key, const QJsonArray &array);
    QJsonArray ComboBoxInfo2Json(QComboBox *box, const QString &key);
    void Close();
    void Yes();

private:
    Ui::PipeParameter *ui;
    OrderInfo orderInfo;
    static const QString DEFAULT_PIPE_PARAMETER_PATH;
};
