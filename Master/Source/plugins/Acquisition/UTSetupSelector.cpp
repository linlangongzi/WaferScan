#include "UTSetupSelector.h"
#include "ui_UTSetupSelector.h"
#include "arsUi/Widgets/ConfigLabel.h"
#include "arsCore/Platform.h"
#include <Ultrasonic/UTSetup.h>
#include <Ultrasonic/UltrasonicManager.h>

UTSetupSelector::UTSetupSelector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UTSetupSelector)
{
    ui->setupUi(this);
    connect( UltrasonicManager::setups(), SIGNAL( RegistryUpdated() ), this, SLOT( RefreshList() ) );  /* 当列表改变时自动刷新列表 */
    RefreshList();
    ui->selectUTSetup->setCurrentIndex(0);
}

UTSetupSelector::~UTSetupSelector()
{
    delete ui;
}

UTSetup *UTSetupSelector::GetUTSetup() const
{
    return UltrasonicManager::setups()->GetByID<UTSetup>( GetUTSetupID() );
}

ConfigID UTSetupSelector::GetUTSetupID() const
{
//    int ii = ui->selectUTSetup->currentData().value<int>();
    return ui->selectUTSetup->currentData().value<ConfigID>();
}

QJsonValue UTSetupSelector::GetUTSetupValue(const QString &key) const
{
    UTSetup *setup = GetUTSetup();
    return setup ? setup->Get(key) : QJsonValue();
}

void UTSetupSelector::SetUTSetup(UTSetup *setup)
{
    if (setup) {
        SetUTSetup( setup->GetID() );
    }
}

void UTSetupSelector::SetUTSetup(ConfigID id)
{
    int index = ui->selectUTSetup->findData(id);
    if (index >= 0) {
        ui->selectUTSetup->setCurrentIndex(index);
    }
}

void UTSetupSelector::AddDisplayKey(const QString &key, const QString &name)
{
    QLabel *nameLabel = new QLabel(name, this);
    ConfigLabel *valueLabel = new ConfigLabel(this);
    valueLabel->setAlignment(Qt::AlignRight);
    valueLabel->SetParameterKey(key);
    labels.append(valueLabel);
    ui->formLayout->addRow(nameLabel, valueLabel);
    RefreshSelection();
}

void UTSetupSelector::RefreshList()
{
    QVariant selection = ui->selectUTSetup->currentData();                      /* 保存旧的选择 */
    ui->selectUTSetup->clear();
    foreach ( UTSetup *setup, UltrasonicManager::setups()->GetRegistry<UTSetup>() ) {
        ui->selectUTSetup->addItem(
            QString("%1 %2").arg( setup->GetID() ).arg( setup->Get("Name").toString() ),
            setup->GetID()
        );
    }
    ui->selectUTSetup->setCurrentIndex( ui->selectUTSetup->findData(selection) );   /* 恢复旧的选择 */
}

void UTSetupSelector::RefreshSelection()
{
    UTSetup *setup = GetUTSetup();
    if (setup) {
        if (bindUTSetup) {
            disconnect(bindUTSetup, 0, this, 0);
        }
        connect(setup, &ConfigObject::ValueChanged, this, &UTSetupSelector::OnValueChanged);
        bindUTSetup = setup;
        foreach (ConfigLabel *label, labels) {
            label->ConnectTo(setup);
            label->UpdateText();
        }
        setup->RequestUpdate("PRF");
        setup->RequestUpdate("SampleAverage");
    }
}

void UTSetupSelector::OnValueChanged(const QString &key, const QJsonValue &value)
{
    emit UTSetupValueChanged(key, value);
}

void UTSetupSelector::on_selectUTSetup_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    RefreshSelection();
}
