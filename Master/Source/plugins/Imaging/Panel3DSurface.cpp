#include "Panel3DSurface.h"
#include "ui_Panel3DSurface.h"
#include <Ars500/ARS500.h>
#include <Core/LicenseManager.h>
#include <Core/Platform.h>
#include <Imaging/UTDocument.h>
#include <Imaging/UTLayer.h>
#include <QApplication>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QCustom3DVolume>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <Ultrasonic/UTDataHeader.h>

using namespace QtDataVisualization;

Panel3DSurface::Panel3DSurface(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Panel3DSurface),
    surface(new Q3DSurface(nullptr, nullptr))
{
    ui->setupUi(this);

    surface->setShadowQuality(Q3DSurface::ShadowQualityNone);
    ui->mainLayout->addWidget(QWidget::createWindowContainer(surface), 0, 0);

    surface->setOrthoProjection(false);

    surface->axisX()->setTitle("X");
    surface->axisZ()->setTitle("Y");
    surface->axisY()->setTitle("Z");

    surface->activeTheme()->setType(Q3DTheme::ThemePrimaryColors);
    UTLayer *layer = document()->GetLayerByID(1);
    Q_ASSERT(layer);
    connect(ui->imageIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateSurface()));
    connect(ui->textureIndex, SIGNAL(currentIndexChanged(int)), this, SLOT(UpdateSurface()));
    connect(ui->palette, &ColorPaletteView::colorPaletteChanged, this, &Panel3DSurface::UpdateSurface);
    connect(surface, &QAbstract3DGraph::currentFpsChanged, this, &Panel3DSurface::OnFPSUpdated);
    connect(layer, &UTLayer::ImagesUpdated, this, &Panel3DSurface::OnImagesUpdated);

    UTDataHeader header = Ars500::UTDriverARS500::InitDataHeader();
    for (int index = 0; index < header.GetNumFeatures(); ++index) {
        if (header.GetDataType(index) == DataTime) {
            ui->imageIndex->addItem(header.GetDescription(index), index);
        } else {
            ui->textureIndex->addItem(header.GetDescription(index), index);
        }
    }

    LicenseManager::registerLicenseUpdate(this, &Panel3DSurface::onLicenseUpdated);
}

void Panel3DSurface::showEvent(QShowEvent *)
{
    UpdateSurface();
}

void Panel3DSurface::onLicenseUpdated()
{
    bool develop = LicenseManager::instance()->hasDevelopLicense();
    surface->setMeasureFps(develop);
}

void Panel3DSurface::OnImagesUpdated(UTImageList images)
{
//    qDebug() << "3DSurface: images updated" << images.count();
    surface->scene()->activeCamera()->setMaxZoomLevel(5000);

    imageList = images;
    UpdateSurface();
}

void Panel3DSurface::OnFPSUpdated(qreal value)
{
    ui->fps->setText( tr("FPS: %1").arg(value, 0, 'f', 1) );
}

void Panel3DSurface::UpdateSurface()
{
    if (!isVisibleTo(QApplication::activeWindow())) {
        needUpdate = true;
        return;
    }
    needUpdate = false;
    qDebug() << "Panel3DSurface: Updating images with" << imageList.count() << "images";
    foreach ( QSurface3DSeries *series, surface->seriesList() ) {
        surface->removeSeries(series);
    }

    UTImage image = imageList.value(ui->imageIndex->currentData().toInt());
    qDebug() << "3DSurface: creating surface" << image.GetSize();

    auto *array = new QSurfaceDataArray;
    int numCols = image.GetWidth();
    int numRows = image.GetHeight();
    for (int rowIndex = 0; rowIndex < numRows; ++rowIndex) {
        *array << new QSurfaceDataRow;
    }

    for (int rowIndex = 0; rowIndex < numRows; ++rowIndex) {
        for (int colIndex = 0; colIndex < numCols; ++colIndex) {
            *( (*array)[rowIndex] ) << QVector3D(float(colIndex) / numCols, image(colIndex, rowIndex), 1 - float(rowIndex) / numRows);
        }
    }

    UTImagePalette p = ui->palette->colorPalette().GetColorPalette();
    int textureId = ui->textureIndex->currentData().toInt();
    UTImage surfaceImage = imageList.value(0);
    QImage texture = textureId >= imageList.count() ? QImage() : imageList.value(textureId).ToImage(p, &surfaceImage);

    auto *series = new QSurface3DSeries;
    series->setDrawMode(QSurface3DSeries::DrawSurface);
    series->setFlatShadingEnabled(true);
    series->setBaseColor(Qt::white);

    series->setTexture(texture);
    series->dataProxy()->resetArray(array);
    qDebug() << "Texture:" << series->texture() << series->textureFile();
    surface->setHorizontalAspectRatio(1);
    surface->setAspectRatio(1);
    surface->axisY()->setRange(0, 100);
    surface->addSeries(series);
}
