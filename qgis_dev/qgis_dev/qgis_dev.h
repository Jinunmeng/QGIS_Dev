#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qgis_dev.h"

class qgis_dev : public QMainWindow
{
	Q_OBJECT

public:
	qgis_dev(QWidget *parent = Q_NULLPTR);

private:
	Ui::qgis_devClass ui;
};
