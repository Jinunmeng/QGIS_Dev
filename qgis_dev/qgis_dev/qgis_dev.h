/***************************************************************************
                           qgis_dev.h
  ------------------------------------------------------------------------
  brief                : brief
  date                 : 2018/06/11 8:32
  copyright            : (C) 2018 by Jinunmeng
  email                : jinunmeng@163.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGIS_DEV_H__
#define QGIS_DEV_H__

#include <QtWidgets/QMainWindow>
#include "ui_qgis_dev.h"


#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <QList>

class qgis_dev : public QMainWindow
{
	Q_OBJECT

public:
	qgis_dev(QWidget *parent =	Q_NULLPTR, Qt::WindowFlags flags = 0);
	~qgis_dev();


public slots:
	/**   
	 * @brief  ÃÌº” ∏¡øÕº≤„
	 * @date 2018/06/11
	 * @return void  
	 * @note  
	*/
	void addVectorLayers();

	/**   
	 * @brief  ÃÌº”’§∏ÒÕº≤„
	 * @date 2018/06/11
	 * @return void  
	 * @note  
	*/
	void addRasterLayers();

private:
	Ui::qgis_devClass ui;

	QgsMapCanvas *pMapCanvas; 

	QList<QgsMapCanvasLayer> mListMapCanvesLayers;

};

#endif // QGIS_DEV_H__