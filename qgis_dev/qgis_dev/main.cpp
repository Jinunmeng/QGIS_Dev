
#include "qgis_dev.h"
#include <QtWidgets/QApplication>

#include <qgsproviderregistry.h>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	//app.setWindowIcon(nullptr);
	app.setApplicationName("Qgis");
	

	const QString path = app.applicationDirPath() + "/translations";
	const QString lan_local = QLocale::system().name();

	QTranslator translator;
	translator.load("qgis_dev_zh.qm", path);
	app.installTranslator(&translator);
	
	QString pluginsDir = "D:/QGIS_Win_Qt5.9_VS2015_x64_qgis2.18_ltr/debug/plugins";
	QgsProviderRegistry::instance(pluginsDir);


	qgis_dev w;
	w.show();
	return app.exec();
}
