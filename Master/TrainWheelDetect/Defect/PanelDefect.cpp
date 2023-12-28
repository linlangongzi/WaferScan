#include "PanelDefect.h"
#include "ui_PanelDefect.h"
#include <arsCore/Platform.h>
#include <WheelDataset/WheelDataset.h>
#include <QMenu>
#include <QPixmap>
#include <QPainter>
#include <QGraphicsColorizeEffect>
#include <QtPrintSupport/QPrinter>

PanelDefect::PanelDefect(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::PanelDefect)
{
    ui->setupUi(this);

    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->wheels->setScene(scene);
    root = new QGraphicsItemGroup;
    scene->addItem(root);

    defectModel.setRootItem(new DefectItem);
    defectModel.setHeader(QStringList() << tr("序号")
                                        << tr("轮号")
                                        << tr("通道号")
                                        << tr("Aec卡序号")
                                        << tr("传感器序号")
                                        << tr("通道组名称")
                                        << tr("特征数据")
                                        << tr("归并原则")
                                        << tr("判伤原则"));

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(&defectModel);
    proxyModel->setFilterKeyColumn(-1);
    connect(ui->filter, &QLineEdit::textChanged, proxyModel, &QSortFilterProxyModel::setFilterFixedString);
    ui->defects->setModel(proxyModel);
    ui->defects->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->defects->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    resizeColumnWidth();

    const auto control = controlPanel();
    connect(ui->defects->selectionModel(), &QItemSelectionModel::selectionChanged, this, &PanelDefect::treeSelectionChanged);
    connect(ui->defects->selectionModel(), &QItemSelectionModel::currentChanged, this, &PanelDefect::treeCurrentItemChanged);
    connect(scene, &QGraphicsScene::selectionChanged, this, &PanelDefect::sceneSelectionChanged);
    connect(ui->defects, &QTreeView::customContextMenuRequested, this, &PanelDefect::actionMenu);
    connect(control, &ControlPanel::trainInfoChanged, this, &PanelDefect::onTrainInfoChanged);
    connect(control, &ControlPanel::globalConfigChanged, this, &PanelDefect::refreshWheelsText);
}

PanelDefect::~PanelDefect()
{
    defectModel.rootItem()->removeChildren();
    onWheelSetMaxChanged(0);;

    delete ui;
}

QImage PanelDefect::renderWheels(qreal width)
{
    const QRectF rect = root->boundingRect();
    const QSize size(width, width / rect.width() * rect.height());

    QPixmap picture(size);
    picture.fill(Qt::white);
    QPainter painter(&picture);
    ui->wheels->scene()->render(&painter, QRectF(QPointF(), size), rect);

    return picture.toImage();
}

void PanelDefect::resizeColumnWidth()
{
    int columnCount = defectModel.columnCount(QModelIndex());
    for (int index = 0; index < columnCount; ++index) {
        ui->defects->resizeColumnToContents(index);
    }
}

Utils::TreeModel<DefectItem, DefectItem> *PanelDefect::model()
{
    return &defectModel;
}

void PanelDefect::setWheelDataset(WheelDataset *dataset)
{
    if (wheelDataset == dataset) {
        return;
    }

    wheelDataset = dataset;
    disconnect(0, 0, this, SLOT(refreshDefects));
    disconnect(0, 0, this, SLOT(onWheelSetMaxChanged));

    if (dataset) {
        connect(wheelDataset, &WheelDataset::judgeReady, this, &PanelDefect::refreshDefects);
        connect(wheelDataset, &WheelDataset::wheelSetMaxChanged, this, &PanelDefect::onWheelSetMaxChanged);
    }
}

void PanelDefect::refreshDefects()
{
    if (!ControlPanel::datasetValid(wheelDataset)) {
        return;
    }

    defectModel.rootItem()->removeChildren();

    QPainterPath path;
    path.moveTo(0, 0.9);
    path.lineTo(0, 1);
    path.lineTo(0.02, 1);
    path.lineTo(0.01, 0.9);
    QPen pen(Qt::darkBlue, 0);
    QBrush brush(Qt::blue);

    for (auto *wheelItem: wheelItems) {
        wheelItem->graphicsEffect()->setEnabled(false);
    }

    QMultiMap<int, int> defectiveChannels;
    const auto control = controlPanel();
    for (auto extreme: wheelDataset->calculateDefects()) {
        if (extreme.isNull()) {
            continue;
        }

        int wheel = ControlPanel::wheelId(extreme);
        const int wheelCount = wheelItems.size();
        if (uint(wheel) >= uint(wheelCount)) {
            continue;
        }
        defectiveChannels.insert(wheel, ControlPanel::channelId(extreme));

        /* 缺陷图源 */
        QGraphicsEllipseItem *wheelItem = wheelItems.value(wheel);
        if (!wheelItem) {
            continue;
        }
        auto *figure = new QGraphicsPathItem(path, wheelItem);
        figure->setFlag(QGraphicsItem::ItemIsSelectable);
        figure->setPen(pen);
        figure->setBrush(brush);
        figure->setRotation(control->channelRotation(extreme));

        /* 缺陷列表 */
        DefectItem *defectItem = new DefectItem;
        defectItem->setDefect(extreme);
        defectItem->setFigure(figure);
        defectModel.rootItem()->appendChild(defectItem);
        if (wheelCount == 2) {
            wheelItem->graphicsEffect()->setEnabled(true);
        }
    }

    int span = platform()->Get("DefectiveWheelJudgeSpan").toInt(5);
    for (int wheel = 0; wheel < wheelItems.count(); ++wheel) {
        QList<int> channels = defectiveChannels.values(wheel);
        for (int index = 0; (index + 1) < channels.count(); ++index) {
            int current = channels.value(index);
            int last = channels.value(index + 1);
            const auto *item = wheelItems.value(wheel);
            if (item && (current / 11000) == (last / 11000) && qAbs(last - current) < span) {
                item->graphicsEffect()->setEnabled(true);
                break;
            }
        }
    }

    resizeColumnWidth();
}

void PanelDefect::refreshWheelsText()
{
    const auto control = controlPanel();
    for (int index = 0; index < wheelItems.count(); ++index) {
        QGraphicsEllipseItem *wheelItem = wheelItems.value(index);
        if (!wheelItem) {
            continue;
        }

        auto items = wheelItem->childItems();
        for (auto item: items) {
            auto textItem = dynamic_cast<QGraphicsTextItem *>(item);
            if (!textItem) {
                continue;
            }

            textItem->setPlainText(control->wheelName(index));
        }
    }
}

void PanelDefect::onWheelSetMaxChanged(int wheelSetMax)
{
    static QRadialGradient g;
    g.setSpread(QGradient::PadSpread);
    g.setColorAt(0.203422, QColor(255, 255, 255, 255));
    g.setColorAt(0.24129, QColor(124, 124, 124, 255));
    g.setColorAt(0.274951, QColor(230, 230, 230, 255));
    g.setColorAt(0.305806, QColor(68, 68, 68, 255));
    g.setColorAt(0.354895, QColor(203, 203, 203, 255));
    g.setColorAt(0.405386, QColor(180, 180, 180, 255));
    g.setColorAt(0.649369, QColor(180, 180, 180, 255));
    g.setColorAt(0.687237, QColor(202, 202, 202, 255));
    g.setColorAt(0.71108, QColor(204, 204, 204, 255));
    g.setColorAt(0.831697, QColor(117, 117, 117, 255));
    g.setColorAt(0.862553, QColor(226, 226, 226, 255));
    g.setColorAt(0.890603, QColor(40, 40, 40, 255));
    g.setColorAt(0.892006, QColor(39, 39, 39, 255));
    g.setColorAt(0.924264, QColor(127, 127, 127, 255));
    g.setColorAt(0.97, QColor(208, 208, 208, 255));
    g.setColorAt(0.985, QColor(250, 250, 250, 255));
    g.setColorAt(0.999, QColor(127, 127, 127, 255));
    g.setColorAt(1, QColor(255, 255, 255, 255));
    static QBrush brush(g);

    int newCount = wheelSetMax * 2;
    const auto control = controlPanel();
    while (newCount > wheelItems.size()) {
        QGraphicsEllipseItem *wheelItem = new QGraphicsEllipseItem(root);
        int index = wheelItems.size();
        int x = (index / 2) * 2 + 1;
        int y = (index % 2) ? 3 : 1;
        wheelItem->setPos(x, y);
        wheelItem->setRect(-1, -1, 2, 2);
        wheelItem->setPen(QPen(Qt::transparent, 0));
        wheelItem->setBrush(brush);
        auto *effect = new QGraphicsColorizeEffect;
        effect->setColor(Qt::red);
        effect->setEnabled(false);
        wheelItem->setGraphicsEffect(effect);
        wheelItems.append(wheelItem);
        root->addToGroup(wheelItem);

        QString name = control->wheelName(index);
        QGraphicsTextItem *textItem = new QGraphicsTextItem(wheelItem);
        textItem->setPlainText(name);
        textItem->setPos(-0.26, -0.13);
        textItem->setScale(0.014);

        ui->wheel->addItem(name);
    }

    while (newCount < wheelItems.size()) {
        QGraphicsEllipseItem *wheel = wheelItems.takeLast();
        root->removeFromGroup(wheel);
        delete wheel;

        ui->wheel->removeItem(wheelItems.size());
    }

    resizeWheels();
}

void PanelDefect::treeCurrentItemChanged(const QModelIndex &index)
{
    DefectItem *item = defectModel.itemForIndex(proxyModel->mapToSource(index));
    const auto frame = item->getDefect();
    if (frame) {
        ControlPanel::getInstance()->setCurrentWheel(ControlPanel::wheelId(frame));
        ControlPanel::getInstance()->setCurrentChannel(ControlPanel::channelId(frame));
    }
}

void PanelDefect::treeSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(deselected);
    isUpdatingSelection = true;
    QModelIndexList indexes = selected.indexes();

    for (auto index: indexes) {
        DefectItem *defectItem = defectModel.itemForIndex(proxyModel->mapToSource(index));
        if (!defectItem) {
            continue;
        }

        QGraphicsPathItem *item = defectItem->getFigure();
        if (item) {
            item->setSelected(true);
        }
    }

    for (auto index: deselected.indexes()) {
        DefectItem *defectItem = defectModel.itemForIndex(proxyModel->mapToSource(index));
        if (!defectItem) {
            continue;
        }

        QGraphicsPathItem *item = defectItem->getFigure();
        if (item) {
            item->setSelected(false);
        }
    }
    isUpdatingSelection = false;
}

void PanelDefect::sceneSelectionChanged()
{
    if (isUpdatingSelection) {
        return;
    }

    QItemSelection selection;
    QModelIndex index;
    QList<QGraphicsItem *> items = ui->wheels->scene()->selectedItems();
    for (auto figure: items) {
        auto *defectItem = defectModel.findItemAtLevel<1>([&](DefectItem *item) {
            return (item->getFigure() == figure);
        });

        if (!defectItem) {
            continue;
        }

        index = proxyModel->mapFromSource(defectModel.indexForItem(defectItem));
        selection.select(index, index);
    }

    ui->defects->selectionModel()->select(selection, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    if (index.isValid()) {
        ui->defects->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Current);
    }
}

void PanelDefect::actionMenu()
{
    QMenu menu;
    menu.addAction(tr("删除"), this, &PanelDefect::actionDelete);
    menu.exec(QCursor::pos());
}

void PanelDefect::actionDelete()
{
    QSet<DefectItem *> defects;
    QModelIndexList indexes = ui->defects->selectionModel()->selectedIndexes();
    for (auto index: indexes) {
        QModelIndex indexSource = proxyModel->mapToSource(index);
        DefectItem *defectItem = defectModel.itemForIndex(indexSource);
        if (!defectItem || defectItem == defectModel.rootItem()) {
            continue;
        }

        defects << defectItem;
    }

    for (auto item: defects) {
        defectModel.destroyItem(item);
    }
}

void PanelDefect::onTrainInfoChanged(const TrainInfo &info)
{
    ui->trainId->setText(info.trainId);
    ui->trainType->setText(info.trainType);
}

void PanelDefect::setCurrentWheel(int wheel)
{
    if (wheel < ui->wheel->count()) {
        ui->wheel->setCurrentIndex(wheel);
    }
}

void PanelDefect::resizeEvent(QResizeEvent *)
{
    resizeWheels();
}

void PanelDefect::resizeWheels()
{
    ui->wheels->fitInView(root->boundingRect(), Qt::KeepAspectRatio);
}

void PanelDefect::on_resize_clicked()
{
    resizeWheels();
}

void PanelDefect::on_wheel_currentIndexChanged(int index)
{
    ControlPanel::getInstance()->setCurrentWheel(index);
}
