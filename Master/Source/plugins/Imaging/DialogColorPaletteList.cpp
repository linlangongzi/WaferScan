#include "DialogColorPaletteList.h"
#include "ui_DialogColorPaletteList.h"
#include <QDebug>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QMetaEnum>
#include <QTreeWidgetItem>

class GridDelegate : public QStyledItemDelegate
{
public:
    GridDelegate(QObject *parent = nullptr): QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);
        painter->eraseRect(QRect(option.rect.left(), option.rect.bottom(), option.rect.width(), 1));
        ColorPalette p = index.data(Qt::DisplayRole).value<ColorPalette>();
        p.paint(painter, option.rect);
    }
};

DialogColorPaletteList::DialogColorPaletteList(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogColorPaletteList)
{
    ui->setupUi(this);

    QMetaEnum meta = QMetaEnum::fromType<ColorPalette::DefaultGradient>();
    for (int index = 0; index < meta.keyCount(); ++index) {
        auto *item = new QTreeWidgetItem(ui->view);
        ColorPalette p = ColorPalette::DefaultGradient(meta.value(index));
        item->setText(0, meta.key(index));
        item->setData(1, Qt::DisplayRole, QVariant::fromValue<ColorPalette>(p));
//        ui->view->setItemWidget(item, 1);
    }

    ui->view->setColumnCount(2);
    ui->view->setItemDelegateForColumn(1, new GridDelegate(this));
}

DialogColorPaletteList::~DialogColorPaletteList()
{
    delete ui;
}

ColorPalette::DefaultGradient DialogColorPaletteList::globalPalette() const
{
    return m_globalPalette;
}

ColorPalette DialogColorPaletteList::colorPalette() const
{
    auto *item = ui->view->currentItem();
    if (item) {
        return item->data(1, Qt::DisplayRole).value<ColorPalette>();
    }
    return ColorPalette();
}

void DialogColorPaletteList::setGlobalPalette(ColorPalette::DefaultGradient globalPalette)
{
    if (m_globalPalette == globalPalette) {
        return;
    }

    m_globalPalette = globalPalette;
    emit globalPaletteChanged(globalPalette);
}
