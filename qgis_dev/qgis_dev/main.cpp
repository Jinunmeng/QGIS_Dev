#include "qgis_dev.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qgis_dev w;
	w.show();
	return a.exec();
}
