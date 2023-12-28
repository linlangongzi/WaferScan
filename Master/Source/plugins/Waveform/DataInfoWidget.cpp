#include "DataInfoWidget.h"
#include "ui_DataInfoWidget.h"
#include <Core/LicenseManager.h>
#include <Core/Platform.h>
#include <Motion/Axis.h>
#include <Motion/MotionManager.h>
#include <QDebug>
#include <QFileDialog>
#include <QInputDialog>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <allrising/Util.h>
#include <lua.hpp>
#include <tuple>

const char REGISTRY_KEY[] = "FeatureMath";                                      /*!< 函数表在Lua注册表中的字段名 reg.FeatureMath = {...} */
const char MATH_KEY[] = "FeatureMath";                                          /*!< 函数表在platform中的字段名 */

const char MATH_NAME[] = "Name";
const char MATH_EXPRESSION[] = "Expression";

enum {
    MathName,
    MathExpression,
};

DataInfoWidget::DataInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataInfoWidget)
{
    ui->setupUi(this);

    /* 初始化Lua引擎 */
    L = luaL_newstate();
    /* 载入标准库 */
    luaL_openlibs(L);
    /* 创建存储表达式的函数表 */
    lua_newtable(L);
    lua_setfield(L, LUA_REGISTRYINDEX, REGISTRY_KEY);

    connect(platform(), &Platform::ValueChanged, this, &DataInfoWidget::OnPlatformValueChanged);
    connect(MotionManager::instance(), &MotionManager::axisListChanged, this, &DataInfoWidget::refreshAxisList);
    refreshAxisList();
    refreshMathList();

    InitUI();
    setNoData();

    LicenseManager::registerLicenseUpdate(this, &DataInfoWidget::onLicenseUpdated);
}

DataInfoWidget::~DataInfoWidget()
{
    delete ui;
    lua_close(L);
}

/*! 初始化界面 */
void DataInfoWidget::InitUI()
{
    // 特征值列表
    ui->preferAmplitude->setChecked(true);
    ui->preferTime->setChecked(false);
    connect(ui->preferTime, &QAbstractButton::clicked, this, &DataInfoWidget::refreshFrame);
    connect(ui->preferAmplitude, &QAbstractButton::clicked, this, &DataInfoWidget::refreshFrame);
    connect(ui->ampReference, &QAbstractSpinBox::editingFinished, this, &DataInfoWidget::refreshFrame);

    ui->table->setColumnCount(5);
    ui->table->setHorizontalHeaderLabels({tr("触发时间"), tr("最大波峰时间"), tr("最大波峰高度"), tr("首次波峰时间"), tr("首次波峰高度")});
}

/*! 设置为无数据 */
void DataInfoWidget::setNoData()
{
    ui->channel->setText(tr("--"));
    ui->waterLength->setText(tr("--"));
    foreach (QLabel *label, axisCoords) {
        label->setText(tr("--"));
    }
    for (int row = 0; row < ui->table->rowCount(); ++row) {
        for (int col = 0; col < ui->table->columnCount(); ++col) {
            QTableWidgetItem *item = ui->table->item(row, col);
            if (item) {
                item->setText( tr("--") );
            }
        }
    }
}

/*! 设置数据信息 */
void DataInfoWidget::setDataFrame(const UTDataFrame &frame)
{
    lastFrame = frame;
    /* 此控件使用延迟刷新, 在控件不可见时, 暂缓刷新界面内容 */
    if (!isVisibleTo(QApplication::activeWindow())) {
        needRefresh = true;
        return;
    }
    needRefresh = false;

    if (frame.isEmpty()) {
        setNoData();
        return;
    }
    if (frame.headerId != currentHeaderId) {
        currentHeaderId = frame.headerId;
        setDataHeader(frame.GetHeader());
    }
    /* 刷新列表行数 */
    int numGates = frame.GetNumGates();
    for (int i = ui->table->rowCount(); i < numGates; ++i) {
        ui->table->insertRow(i);
        for (int col = 0; col < 5; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem( tr("--") );
            item->setTextAlignment(Qt::AlignCenter);
            ui->table->setItem(i, col, item);
        }
    }
    QStringList header;
    for (int i = 0; i < ui->table->rowCount(); ++i) {
        header.append( tr("闸门 %1").arg(i + 1) );
        ui->table->setRowHidden(i, i >= numGates);
    }
    ui->table->setVerticalHeaderLabels(header);

    /* 填充坐标数据 */
    for (int index = 0; index < frame.Pose.count(); ++index) {
        if (index < axisCoords.count()) {
            axisCoords[index]->setText(frame.GetPosInfo(index));
        }
    }
    /* 通道上下文 */
    ui->channel->setText(frame.channelName());
    qreal waterPath = frame.waterPathLength();
    ui->waterLength->setText(tr("%1 mm").arg(waterPath, 0, 'f', 2));

    bool preferTime = ui->preferTime->isChecked();
    bool preferAmp = ui->preferAmplitude->isChecked();
    qreal ampRef = ui->ampReference->value();
    /* 设置特征值信息 */
    for (int row = 0; row < ui->table->rowCount(); ++row) {
        for (int col = 0; col < ui->table->columnCount(); ++col) {
            auto item = ui->table->item(row, col);
            if (!item) {
                continue;
            }

            int featureIndex = item->data(Qt::UserRole).toInt();
            item->setText(frame.GetFeatureText(featureIndex, preferTime, preferAmp, ampRef));
        }
    }

    if (LicenseManager::instance()->hasDevelopLicense()) {
//        UTChannel *channel = UltrasonicManager::channel(frame.Context.GetChannelID());
//        QList<QMatrix4x4> chain = channel ? channel->GetTransformChain(frame.Pose, waterPath) : QList<QMatrix4x4>();
//        if (!chain.isEmpty()) {
//            QTime now = QTime::currentTime();
//            if (prevUpdate.msecsTo(now) > 500 && ui->actionPlot3D->isChecked()) {
//                prevUpdate = now;

//                QStringList x, y, z;
//                foreach (const QMatrix4x4 &m, chain) {
//                    QVector3D pos = m.map(QVector3D(0, 0, 0));
//                    x << QString::number(pos.x());
//                    y << QString::number(pos.y());
//                    z << QString::number(pos.z());
//                }
//                QString command = QString("plot3([0 %1], [0 %2], [0 %3], 'o-');").arg(x.join(' ')).arg(y.join(' ')).arg(z.join(' '));
////                    platform()->Matlab()->DoString("clf;");
//                platform()->Matlab()->DoString(command);
//                platform()->Matlab()->DoString("xlim([0 500]);");
//                platform()->Matlab()->DoString("ylim([0 500]);");
//                platform()->Matlab()->DoString("zlim([-500 0]);");
////                    platform()->Matlab()->DoString("axis vis3d;");
//            }
//        }

        auto position = frame.UpdatePosition();
        ui->position->setText(Vector3DToString(position.first));
        ui->direction->setText(Vector3DToString(position.second));
    }

    /* 重新计算所有表达式 */
    int programIndex = ui->math->topLevelItemCount();
    if (programIndex > 0) {
//        QElapsedTimer timer;
//        timer.start();
        /* 获取注册表 */
        lua_getfield(L, LUA_REGISTRYINDEX, REGISTRY_KEY);

        /* 创建特征值参数表, 将特征值压入该表 */
        lua_newtable(L);                                                        /* {data}, {reg} */
        for (int index = 0; index < frame.CData.length(); ++index) {
            lua_pushnumber(L, frame.CData.at(index));                          /* value, {data}, {reg} */
            lua_rawseti(L, -2, index + 1);                                      /* {data}, {reg} */
        }                                                                       /* (balanced) */

        /* 依次调用各表达式对应的函数 */
        for (; programIndex; --programIndex) {
            lua_rawgeti(L, -2, programIndex);                                   /* func, {data}, {reg} */
            /* 编译成功的function */
            if (lua_type(L, -1) == LUA_TFUNCTION) {
                lua_pushvalue(L, -2);                                           /* {data}, func, {data}, {reg} */
                lua_pcall(L, 1, 1, 0);                                          /* result, {data}, {reg} */
            }
            const auto result = QString::fromUtf8(lua_tostring(L, -1));
            QTreeWidgetItem *item = ui->math->topLevelItem(programIndex - 1);
            if (item) {
                item->setText(1, result);
            }
            lua_pop(L, 1);                                                      /* {data}, {reg} */
        }                                                                       /* (balanced) */

        /* 将特征值表及函数表弹出堆栈 */
        lua_pop(L, 2);                                                          /* (empty) */
//        qDebug() << timer.nsecsElapsed() << "ns";
    }
}

void DataInfoWidget::setDataHeader(const UTDataHeader &header)
{
    /* 构造一个 GateType - DataType - Name 均唯一的列表，作为表头 */
    using Item = std::tuple<UTGateType, UTDataType, QString>;
    QList<Item> headerItems;
    for (int index = 0; index < header.GetNumFeatures(); ++index) {
        Item item(header.GetGateType(index), header.GetDataType(index), header.GetName(index));
        if (!headerItems.contains(item)) {
            headerItems << item;
        }
    }

    QStringList labels;
    foreach (const Item &item, headerItems) {
        labels << std::get<2>(item);
    }
    ui->table->setColumnCount(labels.count());
    ui->table->setHorizontalHeaderLabels(labels);

    int numGates = header.GetNumGates();
    ui->table->setRowCount(numGates);
    for (int colIndex = 0; colIndex < headerItems.count(); ++colIndex) {
        for (int gateIndex = 0; gateIndex < numGates; ++gateIndex) {
            Item headerItem = headerItems[colIndex];
            UTGateType gt = std::get<0>(headerItem);
            UTDataType dt = std::get<1>(headerItem);
            QTableWidgetItem *item = nullptr;
            if (header.HasFeature(gateIndex, gt, dt)) {
                item = new QTableWidgetItem;
                item->setData(Qt::UserRole, header.dataIndex(gateIndex, gt, dt));
            }
            ui->table->setItem(gateIndex, colIndex, item);
        }
    }
}

//void DataInfoWidget::setSetup(const UTSetup &setup)
//{

//}

void DataInfoWidget::OnPlatformValueChanged(const QString &key, const QJsonValue &value)
{
    Q_UNUSED(value);
    if (key == MATH_KEY) {
        refreshMathList();
    }
}

void DataInfoWidget::refreshAxisList()
{
    while (!axisCoords.isEmpty()) {
        ui->coordLayout->removeRow(axisCoords.takeLast());
    }
    // 位置信息列表
    foreach (const QString &name, MotionManager::axisNames()) {
        QLabel *label = new QLabel(this);
        ui->coordLayout->addRow(name, label);
        axisCoords << label;
    }
    refreshFrame();
}

/*! 刷新界面 */
void DataInfoWidget::refreshFrame()
{
    setDataFrame(lastFrame);
}

/*! 读取表达式列表，并刷新Lua函数表和UI */
void DataInfoWidget::refreshMathList()
{
    const QJsonArray programs = platform()->Get(MATH_KEY).toArray();
    int len = ui->math->topLevelItemCount();

    /* 获取函数注册表 */
    lua_getfield(L, LUA_REGISTRYINDEX, REGISTRY_KEY);                           /* {reg} */

    /* 移除多余的函数和控件 */
    while (len > programs.size()) {
        delete ui->math->topLevelItem(len - 1);
        lua_pushnil(L);                                                         /* nil, {reg} */
        lua_rawseti(L, -2, len);                                                /* {reg} */
        len--;
    }                                                                           /* (balanced) */

    /* 添加不足的控件 */
    while (ui->math->topLevelItemCount() < programs.size()) {
        ui->math->addTopLevelItem(new QTreeWidgetItem);
    }

    /* 重新编译所有表达式，更新名称 */
    for (int index = 0; index < programs.size(); ++index) {
        const QJsonObject item = programs.at(index).toObject();
        const QString program = item[MATH_EXPRESSION].toString();
        const QString name = item[MATH_NAME].toString();
        ui->math->topLevelItem(index)->setText(MathName, name);

        luaL_loadstring(L, qUtf8Printable(program));                            /* func, {reg} */
        lua_rawseti(L, -2, index + 1);                                          /* {reg} */
    }

    /* 将函数注册表从堆栈中弹出 */
    lua_pop(L, 1);                                                              /* (empty) */
    refreshFrame();
}

/*! 添加表达式 */
void DataInfoWidget::on_addMath_clicked()
{
    bool ok = false;
    const QString name = QInputDialog::getText(this, QString(), tr("请输入表达式名称"), QLineEdit::Normal, QString(), &ok);
    if (!ok) {
        return;
    }
    const QString program = QInputDialog::getMultiLineText(this, QString(), tr("请输入表达式"), QString(), &ok);
    if (!ok) {
        return;
    }
    QJsonObject item;
    item[MATH_NAME] = name;
    item[MATH_EXPRESSION] = program;
    auto programs = platform()->Get(MATH_KEY).toArray();
    programs.append(item);
    platform()->Set(MATH_KEY, programs);
}

/*! 移除表达式 */
void DataInfoWidget::on_removeMath_clicked()
{
    int index = ui->math->currentIndex().row();
    QJsonArray array = platform()->Get(MATH_KEY).toArray();
    if (index >= 0 && index < array.size()) {
        array.removeAt(index);
        platform()->Set(MATH_KEY, array);
    }
}

void DataInfoWidget::showEvent(QShowEvent *)
{
    if (needRefresh) {
        refreshFrame();
    }
}

void DataInfoWidget::onLicenseUpdated()
{
    bool develop = LicenseManager::instance()->hasDevelopLicense();
    ui->position->setVisible(develop);
    ui->positionTitle->setVisible(develop);
    ui->direction->setVisible(develop);
    ui->directionTitle->setVisible(develop);
    ui->sampleButton->setVisible(develop);
}

/*! 双击表达式列表，编辑表达式 */
void DataInfoWidget::on_math_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    QJsonArray programs = platform()->Get(MATH_KEY).toArray();
    if (row >= 0 && row < programs.size()) {
        QJsonObject item = programs[row].toObject();
        QString key;
        QString value;
        bool ok = false;
        if (index.column() == MathName) {
            key = MATH_NAME;
            value = QInputDialog::getText(this, QString(), tr("请输入表达式名称"), QLineEdit::Normal, item[key].toString(), &ok);
        } else {
            key = MATH_EXPRESSION;
            value = QInputDialog::getMultiLineText(this, QString(), tr("请输入表达式"), item[key].toString(), &ok);
        }
        if (ok) {
            item[key] = value;
            programs[row] = item;
            platform()->Set(MATH_KEY, programs);
        }
    }
}

void DataInfoWidget::on_sampleButton_clicked()
{
    QFile csvSample(QString("SampleAndShapeBaseOnPoints.csv"));
    if (!csvSample.open(QIODevice::Append)) {
        return;
    }
    QTextStream csv(&csvSample);
    auto position = lastFrame.UpdatePosition();
    csv << position.first[0] << "," << position.first[1] << "," << position.first[2] << endl;
}
