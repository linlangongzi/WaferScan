#include "UltrasonicOptions.h"
#include "ui_UltrasonicOptions.h"
#include "UTSetup.h"
#include "UltrasonicConstants.h"

using namespace Ultrasonic;

UltrasonicOptions::UltrasonicOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UltrasonicOptions)
{
    ui->setupUi(this);

    ui->timebasePrecision->setValue(UTSetupTimebase::timebasePrecision());
}

UltrasonicOptions::~UltrasonicOptions()
{
    delete ui;
}

void UltrasonicOptions::on_timebasePrecision_editingFinished()
{
    UTSetupTimebase::setTimebasePrecision(ui->timebasePrecision->value());
}

UltrasonicOptionsPage::UltrasonicOptionsPage(QObject *parent) :
    IOptionsPage(parent)
{
    setId(SETTINGS_ID_ULTRASONIC);
    setCategory(SETTINGS_CATEGORY_ULTRASONIC);
    setDisplayName(tr("显示设置"));
}

QWidget *UltrasonicOptionsPage::widget()
{
    if (!m_widget) {
        m_widget = new UltrasonicOptions;
    }
    return m_widget;
}

void UltrasonicOptionsPage::apply()
{

}

void UltrasonicOptionsPage::finish()
{
    delete m_widget;
}
