#include "DialogNamedPosition.h"
#include "ui_DialogNamedPosition.h"
#include <Motion/Axis.h>
#include <QDoubleSpinBox>

DialogNamedPosition::DialogNamedPosition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNamedPosition)
{
    ui->setupUi(this);
    initUi();
}

DialogNamedPosition::~DialogNamedPosition()
{
    delete ui;
}

QString DialogNamedPosition::name() const
{
    return ui->name->text();
}

void DialogNamedPosition::setName(const QString &name)
{
    ui->name->setText(name);
}

MotionPose DialogNamedPosition::position() const
{
    MotionPose pose;
//    std::transform(posEditList.begin(), posEditList.end(), pose.begin(),
//                   [] (QDoubleSpinBox *box) {return box->value();});
    foreach (auto *spin, posEditList) {
        pose << spin->value();
    }
    return pose;
}

void DialogNamedPosition::setPosition(const MotionPose &position)
{
    for (int index = 0; index < posEditList.count(); ++index) {
        posEditList[index]->setValue( position.value(index) );
    }
}

//MotionAxisList DialogNamedPosition::axis() const
//{
//    return ;
//}
//void DialogNamedPosition::setAxis(MotionAxisList axis)
//{
////    m_axis = axis;
//}

bool DialogNamedPosition::isLoadingPosition() const
{
    return ui->load->isChecked();
}

void DialogNamedPosition::setLoadingPosition(bool isLoadingPosition)
{
    ui->load->setChecked(isLoadingPosition);
}

/*!
 * \brief DialogNamedPosition::InitUI 初始化界面
 */
void DialogNamedPosition::initUi()
{
    for (Axis *axis: MotionManager::axisList()) {
        auto *linePos = new QDoubleSpinBox(this);
        linePos->setRange(-qInf(), qInf());
        linePos->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        posEditList.append(linePos);
        ui->positionLayout->addRow(axis->name(), linePos);
        linePos->setSuffix(axis->axisUnit());
    }
}
