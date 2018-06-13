#include "qgis_dev.h"

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>

#include <qgsvectorlayer.h>
#include <qgsmaplayerregistry.h>
#include <qgsrasterlayer.h>


qgis_dev::qgis_dev(QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	pMapCanvas = new QgsMapCanvas();
	this->setCentralWidget(pMapCanvas);

	pMapCanvas->enableAntiAliasing(true);
	pMapCanvas->setCanvasColor(QColor(255, 255, 255));
	pMapCanvas->setVisible(true);

	//connections
	connect(ui.actionadd_vector, &QAction::triggered, this, &qgis_dev::addVectorLayers);
	//connect(ui.actionadd_vector, SIGNAL(triggerd()), this, SLOT(addVectorLayers())); //qt4的写法
	connect(ui.actionadd_raster, &QAction::triggered, this, &qgis_dev::addRasterLayers);//qt5的写法



}


qgis_dev::~qgis_dev()
{

}

void qgis_dev::addVectorLayers()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("open vector"), "", "*.shp");
	QFileInfo fileinfo;
	fileinfo = QFileInfo(fileName);
	QString baseName = fileinfo.baseName();
	QgsVectorLayer *pVecLayer = new QgsVectorLayer(fileName, baseName, "ogr", false);
	if ( !pVecLayer->isValid())
	{
		QMessageBox::critical(this, "error", "layer is invalid");
		return;
	}

	QgsMapLayerRegistry::instance()->addMapLayer(pVecLayer);
	mListMapCanvesLayers.append(QgsMapCanvasLayer(pVecLayer));
	pMapCanvas->setExtent(pVecLayer->extent());
	pMapCanvas->freeze(false);
	pMapCanvas->setLayerSet(mListMapCanvesLayers);
	pMapCanvas->refresh();


}

void qgis_dev::addRasterLayers()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("open raster"), "", "*.tif");
	QFileInfo fileInfo;
	fileInfo = QFileInfo(fileName);
	QString baseName = fileInfo.baseName();
	
	QgsRasterLayer *rasterLayer = new QgsRasterLayer(fileName, baseName, "gdal", false);
	if ( !rasterLayer->isValid() )
	{
		QMessageBox::critical(this, "error", "layer is invalid");
		return;
	}
	QgsMapLayerRegistry::instance()->addMapLayer(rasterLayer);
	mListMapCanvesLayers.append(rasterLayer);
	pMapCanvas->setExtent(rasterLayer->extent());
	pMapCanvas->freeze(false);
	pMapCanvas->setLayerSet(mListMapCanvesLayers);
	pMapCanvas->refresh();


}
