/***************************************************************************
							main.h
------------------------------------------------------------------------
brief                : brief
date                 : 2018/06/11 8:30
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

#ifdef _MSC_VER
#undef APP_EXPORT
#define APP_EXPORT __declspec(dllimport)
#endif

#include <QtWidgets/QApplication>
#include <QBitmap>
#include <QDir>
#include <QFile>
#include <QFont>
#include <QFontDatabase>
#include <QPixmap>
#include <QLocale>
#include <QSettings>
#include <QSplashScreen>
#include <QString>
#include <QStringList>
#include <QStyle>
#include <QTime>

#include <QDesktopWidget>
#include <QTranslator>
#include <QImageReader>
#include <QMessageBox>

#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "qgis_dev.h"

#include "qgsproviderregistry.h"
#include "qgseditorwidgetregistry.h"
#include "qgscustomization.h"
#include "qgsfontutils.h"
#include "qgspluginlayerregistry.h"
#include "qgsmessagelog.h"
#include "qgslocalec.h"
#include "qgsmapcanvas.h"
#include "qgsapplication.h"
#include "qgsconfig.h"
#include "qgsversion.h"
#include "qgsexception.h"
#include "qgsproject.h"
#include "qgsrectangle.h"
#include "qgslogger.h"
#include "qgsmaplayerregistry.h"
#include "qgsdxfexport.h"
#include "qgspythonrunner.h"
#include "qgsvisibilitypresetcollection.h"


static QString myProjectFileName = "";
static QStringList myFileList;

int main(int argc, char *argv[])
{
	QgsDebugMsg(QString("Starting smart gis main")); //输出debug消息到控制台
	qsrand( time(nullptr) ); //初始化随机数种子

	QString mySnapshotFileName = "";
	int mySnapshotWidth = 800;
	int mySnapshotHeight = 600;

	bool myHideSplash = false;
	bool mySkipVersionCheck = false;

	bool myRestoreDefaultWindowState = false;
	bool myRestorePlugins = true;
	bool myCustomization = true;

	QString dxfOutputFile;
	QgsDxfExport::SymbologyExport dxfSymbologyMode = QgsDxfExport::SymbolLayerSymbology;
	double dxfScaleDenom = 50000.0;
	QString dxfEncoding = "CP1252";
	QString dxfPreset;
	QgsRectangle dxfExtent;

	QString myInitialExtent = "";
	if (argc == 1)
		myInitialExtent = "-1, -1, 1, 1";

	QString myTranslationCode;

	QString configpath;
	QString optionpath;
	QString authdbdirectory;
	QString pythonfile;
	QString customizationfile;

	QStringList args;

	
	if (myProjectFileName.isEmpty())
	{
		// check for a .qgs
		for (int i = 0; i < args.size(); i++)
		{
			QString arg = QDir::toNativeSeparators(QFileInfo(args[i]).absoluteFilePath());
			if (arg.contains(".qgs"))
			{
				myProjectFileName = arg;
				break;
			}
		}
	}

	bool myUseGuiFlag = true;
	if (!myUseGuiFlag)
	{
		std::cerr << QObject::tr(
			"QGIS starting in non-interactive mode not supported.\n"
			"You are seeing this message most likely because you "
			"have no DISPLAY environment variable set.\n"
		).toUtf8().constData();
		exit(1); //exit for now until a version of qgis is capabable of running non interactive
	}

	if (!optionpath.isEmpty() || !configpath.isEmpty())
	{
		// tell QSettings to use INI format and save the file in custom config path
		QSettings::setDefaultFormat(QSettings::IniFormat);
		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, optionpath.isEmpty() ? configpath : optionpath);
	}

	if (!myCustomization)
	{
		QgsCustomization::instance()->setEnabled(false);
	}

	QgsApplication app(argc, argv, myUseGuiFlag, configpath);
	QCoreApplication::addLibraryPath(QApplication::applicationDirPath()
									+ QDir::separator() + "qtplugins");

	app.setWindowIcon(QIcon(QApplication::applicationDirPath() + "/images/icon.png"));

	QCoreApplication::setOrganizationName(QgsApplication::QGIS_ORGANIZATION_NAME);
	QCoreApplication::setOrganizationDomain(QgsApplication::QGIS_ORGANIZATION_DOMAIN);
	QCoreApplication::setApplicationName(QgsApplication::QGIS_APPLICATION_NAME);
	QCoreApplication::setAttribute(Qt::AA_DontShowIconsInMenus, false);

	QSettings *customizationsettings;
	if (!optionpath.isEmpty() || !configpath.isEmpty())
	{
		QSettings::setDefaultFormat(QSettings::IniFormat);
		QString path = optionpath.isEmpty() ? configpath : optionpath;
		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, path);
		customizationsettings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "QGIS", "QGISCUSTOMIZATION2");
	}
	else
	{
		customizationsettings = new QSettings("QGIS", "QGISCUSTOMIZATION2");
	}

	if (!customizationfile.isEmpty())
	{
		customizationsettings = new QSettings(customizationfile, QSettings::IniFormat);
		QgsCustomization::instance()->setEnabled(true);
	}

	QgsCustomization::instance()->setSettings(customizationsettings);
	QgsCustomization::instance()->loadDefault();

	QSettings mySettings;
	if (mySettings.contains("/Themes"))
	{
		QString theme = mySettings.value("/Themes", "default").toString();
		if (theme == "gis" || theme == "classic" || theme == "nkids")
		{
			mySettings.setValue("/Themes", QString("default"));
		}
	}

	//自定义环境变量
	QMap<QString, QString> systemEnvVars = QgsApplication::systemEnvVars();
	bool useCustomVars = mySettings.value("qgis/customEnvVarsUse", QVariant(false)).toBool();
	if (useCustomVars)
	{
		QStringList customVarsList = mySettings.value("qgis/customEnvVars", "").toStringList();
		if (!customVarsList.isEmpty())
		{
			Q_FOREACH(const QString &varStr, customVarsList)
			{
				int pos = varStr.indexOf(QLatin1Char('|'));
				if (pos == -1)
					continue;
				QString envVarApply = varStr.left(pos);
				QString varStrNameValue = varStr.mid(pos + 1);
				pos = varStrNameValue.indexOf(QLatin1Char('='));
				if (pos == -1)
					continue;
				QString envVarName = varStrNameValue.left(pos);
				QString envVarValue = varStrNameValue.mid(pos + 1);

				if (systemEnvVars.contains(envVarName))
				{
					if (envVarApply == "prepend")
					{
						envVarValue += systemEnvVars.value(envVarName);
					}
					else if (envVarApply == "append")
					{
						envVarValue = systemEnvVars.value(envVarName) + envVarValue;
					}
				}

				if (systemEnvVars.contains(envVarName) && envVarApply == "unset")
				{
					putenv(envVarName.toUtf8().constData());
				}
				else
				{
					if (envVarApply != "undefined" || !getenv(envVarName.toUtf8().constData()))
						putenv(QString("%1=%2").arg(envVarName).arg(envVarValue).toUtf8().constData());
				}
			}
		}
	}

	//设置程序样式
	QString style = mySettings.value("/qgis/style").toString();
	if (!style.isNull())
	{
		QApplication::setStyle(style);
		mySettings.setValue("/qgis/style", QApplication::style()->objectName());
	}


	//QApplication app(argc, argv);
	app.setApplicationName("Qgis");
	
	const QString path = app.applicationDirPath() + "/translations";
	const QString lan_local = QLocale::system().name();

	QTranslator translator;
	translator.load("qgis_dev_zh.qm", path);
	app.installTranslator(&translator);
	
	if (!authdbdirectory.isEmpty())
	{
		QgsApplication::setAuthDbDirPath(authdbdirectory);
	}

	QString pluginsDir = "D:/QGIS_Win_Qt5.9_VS2015_x64_qgis2.18_ltr/debug/plugins";
	QgsProviderRegistry::instance(pluginsDir);

	//set up splash screen
	QString mySplashPath = app.applicationDirPath() + "/images/splash.png";
	QPixmap myPixmap(mySplashPath);
	//QPixmap myPixmap(mySplashPath + QLatin1String("splash.png"));
	
	int w = 600 * qApp->desktop()->logicalDpiX() / 96;
	int h = 300 * qApp->desktop()->logicalDpiY() / 96;

	QSplashScreen *mypSplash = new QSplashScreen(myPixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	if (!myHideSplash && !mySettings.value("/qgis/hideSplash").toBool())
	{
		mypSplash->setMask(myPixmap.mask());
		mypSplash->show();
	}

	if (myRestoreDefaultWindowState || mySettings.value("/qgis/restoreDefaultWindowState", false).toBool())
	{
		QgsDebugMsg("Resetting /UI/state settings!");
		mySettings.remove("/UI/state");
		mySettings.remove("/qgis/restoreDefaultWindowState");
	}

	//设置最大线程数
	//这个应该在QgsApplication::init()中设置，但是它不能设置dir
	QgsApplication::setMaxThreads(QSettings().value("/qgis/max_threads", -1).toInt());


	qgis_dev *myQgis = new qgis_dev(mypSplash, myRestorePlugins, mySkipVersionCheck);
	myQgis->setObjectName("QgisApp");

	app.connect(&app, SIGNAL(preNotify(QObject *, QEvent *, bool *)),
		QgsCustomization::instance(), SLOT(preNotify(QObject *, QEvent *, bool *)));

	if (!myProjectFileName.isEmpty())
	{
		myQgis->openProject(myProjectFileName);
	}
	
	/************************************************************************/
	/* aotuload any file names that were passed in on the command line                                                                     */
	/************************************************************************/
	QgsDebugMsg(QString("Number of files in myFileList: %1").arg(myFileList.count()));
	for (QStringList::Iterator myIterator = myFileList.begin(); myIterator != myFileList.end(); ++myIterator)
	{
		QgsDebugMsg(QString("Trying to load file : %1").arg((*myIterator)));
		QString myLayerName = *myIterator;
		//如果后缀是.qgs就不加载
		if (!myLayerName.contains(".qgs"))
		{
			myQgis->openLayer(myLayerName);
		}
	}

	/************************************************************************/
	/* set initial extent if requested                                                                     */
	/************************************************************************/
	if (!myInitialExtent.isEmpty())
	{
		QgsLocaleNumC l;
		double coords[4];
		int pos, posOld = 0;
		bool ok = true;
		
		//从字符串中解析
		for (int i = 0; i < 3; i++)
		{
			pos = myInitialExtent.indexOf(',', posOld);
			if (pos == -1)
			{
				ok = false;
				break;
			}
			coords[i] = QString(myInitialExtent.mid(posOld, pos - posOld)).toDouble(&ok);
			if (!ok)
				break;
			posOld = pos + 1;
		}
		//解析最后的坐标
		if (ok)
			coords[3] = QString(myInitialExtent.mid(posOld)).toDouble(&ok);
		if (!ok)
		{
			QgsDebugMsg("Error while parsing initial extent!");
		}
		else
		{
			//用解析出的值设置范围
			QgsRectangle rect(coords[0], coords[1], coords[2], coords[3]);
			myQgis->setExtent(rect);
			if (myQgis->mapCanvas())
			{
				myQgis->mapCanvas()->refresh();
			}
		}
	}

	if (!pythonfile.isEmpty())
	{
		pythonfile.replace('\\', '/');
		QgsPythonRunner::run(QString("exec(open('%1').read())").arg(pythonfile));
	}

	/************************************************************************/
	/* Take a snapshot of the map view then exit if snapshot mode requested                                                                     */
	/************************************************************************/
	if (mySnapshotFileName != "")
	{
		/*You must have at least one paintEvent() delivered for the window to be
		rendered properly.

		It looks like you don't run the event loop in non-interactive mode, so the
		event is never occurring.

		To achieve this without running the event loop: show the window, then call
		qApp->processEvents(), grab the pixmap, save it, hide the window and exit.
		*/
		//qgis->show();
		app.processEvents();
		QPixmap * myQPixmap = new QPixmap(mySnapshotWidth, mySnapshotHeight);
		myQPixmap->fill();
		myQgis->saveMapAsImage(mySnapshotFileName, myQPixmap);
		app.processEvents();
		myQgis->hide();

		return 1;
	}

	if ( !dxfOutputFile.isEmpty())
	{
		myQgis->hide();

		QgsDxfExport dxfExport;
		dxfExport.setSymbologyScaleDenominator(dxfScaleDenom);
		dxfExport.setSymbologyExport(dxfSymbologyMode);
		dxfExport.setExtent(dxfExtent);

		QStringList layerIds;
		QList<QgsDxfExport::DxfLayer> layers;
		if (!dxfPreset.isEmpty())
		{
			Q_FOREACH(const QString& layer, QgsProject::instance()->visibilityPresetCollection()->presetVisibleLayers(dxfPreset))
			{
				QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>(QgsMapLayerRegistry::instance()->mapLayer(layer));
				if (!vl)
					continue;
				layers << QgsDxfExport::DxfLayer(vl);
				layerIds << vl->id();
			}
		}
		else
		{
			Q_FOREACH(QgsMapLayer *ml, QgsMapLayerRegistry::instance()->mapLayers())
			{
				QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>(ml);
				if (vl)
					continue;
				layers << QgsDxfExport::DxfLayer(vl);
				layerIds << vl->id();
			}
		}
		if (!layers.isEmpty())
		{
			dxfExport.addLayers(layers);
		}

		QFile dxfFile;
		if (dxfOutputFile == "-")
		{
			if (!dxfFile.open(stdout, QIODevice::WriteOnly))
			{
				std::cerr << "could not open stdout" << std::endl;
				return 2;
			}
		}
		else
		{
			if (!dxfOutputFile.endsWith(".dxf", Qt::CaseInsensitive))
				dxfOutputFile += ".dxf";
			dxfFile.setFileName(dxfOutputFile);
		}

		int res = dxfExport.writeToFile(&dxfFile, dxfEncoding);
		if (res)
			std::cerr << "dxf output failed with error code" << res << std::endl;
		delete myQgis;
		
		return res;
	}

	/************************************************************************/
	/* Continue on to interative gui...                                                                     */
	/************************************************************************/
	myQgis->setWindowState(Qt::WindowMaximized);
	myQgis->show();
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	
	mypSplash->finish(myQgis);
	delete mypSplash;

	myQgis->completeInitialization();

	int retval = app.exec();	
	delete myQgis;
	return retval;
}
