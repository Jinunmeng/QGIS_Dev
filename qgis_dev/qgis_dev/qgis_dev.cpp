/***************************************************************************
                           qgis_dev.cpp
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
 
#include <QAction>
#include <QApplication>
#include <QBitmap>
#include <QCheckBox>
#include <QClipboard>
#include <QColor>
#include <QCursor>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDialog>
#include <QDir>
#include <QEvent>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QImageWriter>
#include <QInputDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QLibrary>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPictureIO>
#include <QPixmap>
#include <QPoint>
#include <QPrinter>
#include <QProcess>
#include <QProgressBar>
#include <QProgressDialog>
#include <QRegExp>
#include <QRegExpValidator>
#include <QSettings>
#include <QShortcut>
#include <QSpinBox>
#include <QSplashScreen>
#include <QStatusBar>
#include <QStringList>
#include <QTextStream>
#include <QToolButton>
#include <QtGlobal>
#include <QThread>
#include <QTimer>
#include <QUuid>
#include <QVBoxLayout>
#include <QWhatsThis>
#include <QWidgetAction>



#include "qgis_dev.h"

// QGis include

//#include "qgisappinterface.h"
//#include "qgisappstylesheet.h"
#include "qgsvectorlayer.h"
#include "qgsmaplayerregistry.h"
#include "qgsrasterlayer.h"
#include "qgsproject.h"
#include "qgslayertreemodel.h"
#include "qgsapplication.h"
#include "qgslayertreelayer.h"
#include "qgslayertreegroup.h"
#include "qgslayertreeregistrybridge.h"
#include "qgslayertreeviewdefaultactions.h"
#include "qgsattributedialog.h"
#include "qgscursors.h"
#include "qgsproviderregistry.h"
#include <qgsmessagebar.h>
#include "qgsmessagebaritem.h"
#include "qgsdockwidget.h"
#include "qgslayertreeviewdefaultactions.h"
#include "qgsnetworkaccessmanager.h"
#include "qgscredentials.h"
#include "qgsauthsslerrorsdialog.h"
#include "qgsauthguiutils.h"
#include "qgsauthmanager.h"
#include "qgswelcomepage.h"
#include "qgsmaplayeractionregistry.h"
#include "qgscomposer.h"
#include "qgscomposermanager.h"
#include "qgscomposerview.h"
#include "qgsmaplayerregistry.h"
#include "qgsvectordataprovider.h"
#include "qgsdataitem.h"
#include "qgssublayersdialog.h"
//other includes
#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iomanip>
#include <list>
#include <memory>
#include <vector>

// for map tools
#include "qgsmaptool.h"
#include "qgsmaptoolzoom.h"
#include "qgsmaptoolidentify.h"
#include "qgsmaptoolpan.h"


class QTreeWidgetItem;

/** Set the application title bar text

If the current project title is null
if the project file is null then
set title text to just application name and version
else
set set title text to the project file name
else
set the title text to project title
*/
static void setTitleBarText_(QWidget & qgisApp)
{
	QString caption = qgis_dev::tr("QGIS ");

	if (QGis::QGIS_RELEASE_NAME == "Master")
	{
		caption += QString("%1").arg(QGis::QGIS_DEV_VERSION);
	}
	else
	{
		caption += QGis::QGIS_VERSION;
	}

	if (QgsProject::instance()->title().isEmpty())
	{
		if (QgsProject::instance()->fileName().isEmpty())
		{
			// no project title nor file name, so just leave caption with
			// application name and version
		}
		else
		{
			QFileInfo projectFileInfo(QgsProject::instance()->fileName());
			caption += " - " + projectFileInfo.completeBaseName();
		}
	}
	else
	{
		caption += " - " + QgsProject::instance()->title();
	}

	qgisApp.setWindowTitle(caption);
} // setTitleBarText_( QWidget * qgisApp )



qgis_dev *qgis_dev::sInstance = nullptr;
qgis_dev::qgis_dev(QSplashScreen *splash, bool restorePlugins, bool skipVersionCheck, QWidget *parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags),
	mSplash(splash),
	mMousePrecisionDecimalPlaces(0)
{
	if (sInstance)
	{
		QMessageBox::critical(
			this,
			tr("Multiple Instance of QgisApp"),
			tr("Multiple instances of QGIS application object detected.\nPlease contact the developers.\n") );
		abort();
	}
	sInstance = this;
	mProfiler = QgsRuntimeProfiler::instance();

	namSetup();

	// load GUI: actions, menus, toolbars
	mProfiler->beginGroup("qgis_dev");
	mProfiler->beginGroup("startup");
	startProfile("Setting up UI");
	ui.setupUi(this);
	endProfile();

	/*startProfile("Checking database");
	mSplash->showMessage(tr("Checking database"), Qt::AlignHCenter | Qt::AlignBottom);
	qApp->processEvents();
	QString dbError;
	if (!QgsApplication::createDB(&dbError))
	{
		QMessageBox::critical(this, tr("Private qgis.db"), dbError);
	}
	endProfile();

	startProfile("Initializing authentication");
	mSplash->showMessage(tr("Initializing authentication"), Qt::AlignHCenter | Qt::AlignBottom);
	qApp->processEvents();
	QgsAuthManager::instance()->init(QgsApplication::pluginPath());
	if (!QgsAuthManager::instance()->isDisabled())
	{
		masterPasswordSetup();
	}*/

	//创建主题文件夹
	/*startProfile("Create theme folder");
	QgsApplication::createThemeFolder();
	endProfile();

	mSplash->showMessage(tr("Reading settings"), Qt::AlignHCenter | Qt::AlignBottom);
	qApp->processEvents();

	mSplash->showMessage(tr("Setting up the GUI"), Qt::AlignHCenter | Qt::AlignBottom);
	qApp->processEvents();*/

	QSettings settings;

	startProfile("Building style sheet");
	//设置样式生成器并应用已保存或默认的样式
	//mStyleSheetBuilder = new QgisAppStyleSheet(this);
	endProfile();

	
	QWidget *centralWidget = this->centralWidget();
	QGridLayout *centralLayout = new QGridLayout(centralWidget);
	centralWidget->setLayout(centralLayout);
	centralLayout->setContentsMargins(0, 0, 0, 0);

	//创建画布
	startProfile("Create map canvas");
	pMapCanvas = new QgsMapCanvas(centralWidget, "theMapCanvas");
	connect(pMapCanvas, SIGNAL(messageEmitted(const QString&, const QString&, QgsMessageBar::MessageLevel)),
		this, SLOT(displayMessage(const QString&, const QString&, QgsMessageBar::MessageLevel)));
	pMapCanvas->setWhatsThis(tr("Map canvas. This is where raster and vector "
								"layers are displayed when added to the map"));
	//设置画布的颜色
	int myRed = settings.value("/qgis/default_canvas_color_red", 255).toInt();
	int myGreen = settings.value("/qgis/default_canvas_color_green", 255).toInt();
	int myBlue = settings.value("/qgis/default_canvas_color_blue", 255).toInt();
	pMapCanvas->setCanvasColor(QColor(myRed, myGreen, myBlue));
	endProfile();

	//自动打开的工程文件类型
	mProjOpen = settings.value("/qgis/projOpenAtLaunch", 0).toInt();

	startProfile("Welcome page");
	//mWelcomePage = new QgsWelcomePage();
	endProfile();


	mCentralContainer = new QStackedWidget();
	mCentralContainer->insertWidget(0, pMapCanvas);

	centralLayout->addWidget(mCentralContainer, 0, 0, 2, 1);
	connect(pMapCanvas, SIGNAL(layersChanged), this, SLOT(showMapCanvas));

	mCentralContainer->setCurrentIndex(mProjOpen ? 0 : 1);

	startProfile("Message bar");
	mInfoBar = new QgsMessageBar(centralWidget);
	mInfoBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	centralLayout->addWidget(mInfoBar, 0, 0, 1, 1);
	endProfile();

	startProfile("User input dock");
	//User input dock widget;
	mUserInputDockWidget = new QgsUserInputDockWidget(this);
	mUserInputDockWidget->setObjectName("UserInputDockWidget");
	endProfile();


	//设置焦点
	pMapCanvas->setFocus();

	startProfile("Layer tree");
	mLayerTreeView = new QgsLayerTreeView(this);
	mLayerTreeView->setObjectName(QStringLiteral("theLayerTreeView"));
	endProfile();
	

	functionProfile(&qgis_dev::createStatusBar, this, "Create status bar");
	functionProfile(&qgis_dev::createCanvasTools, this, "Create canvas tools");
	pMapCanvas->freeze();
	functionProfile(&qgis_dev::initLayerTreeView, this, "Init layer tree view");
	mSaveRollbackInProgress = false;

	
	
	mLogViewer = new QgsMessageLogViewer(statusBar(), this);
	mLogDock = new QgsDockWidget(tr("Log Messages"), this);
	mLogDock->setObjectName(QStringLiteral("MessageLog"));
	mLogDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
	addDockWidget(Qt::BottomDockWidgetArea, mLogDock);
	mLogDock->setWidget(mLogViewer);
	mLogDock->hide();
	connect(mMessageButton, SIGNAL(toggled(bool)), mLogDock, SLOT(setVisible(bool)));
	connect(mLogDock, SIGNAL(visibilityChanged(bool)), mMessageButton, SLOT(setChecked(bool)));
	connect(QgsMessageLog::instance(), SIGNAL(messageReceived(bool)), this, SLOT(toggleLogMessageIcon(bool)));
	connect(mMessageButton, SIGNAL(toggled(bool)), this, SLOT(toggleLogMessageIcon(bool)));


	activateDeactivateLayerRelatedActions(nullptr);
	connect(QgsMapLayerActionRegistry::instance(), SIGNAL(changed()), this, SLOT(refreshActionFeatureAction()));

	//设置程序标题
	//QString caption = tr("QGIS - %1 ('%2')").arg(qgis_dev::QGIS_VERSION, qgis_dev::QGIS_RELEASE_NAME);
	setWindowTitle( "Smart GIS" );

	QgsMessageLog::logMessage(tr("Smart GIS starting..."), QString(), QgsMessageLog::INFO);

	qApp->processEvents();

	mSplash->showMessage(tr("Checking provider plugins"), Qt::AlignHCenter | Qt::AlignBottom);
	qApp->processEvents();
	QgsApplication::initQgis();

	mSplash->showMessage(tr("Starting Python"), Qt::AlignHCenter | Qt::AlignBottom);
	qApp->processEvents();
	//loadPythonSupport();

	//设置工具栏图标大小
	int size = settings.value("/IconSize", QGIS_ICON_SIZE).toInt();
	setIconSizes(size);

	mSplash->showMessage(tr("Initializing file filters"), Qt::AlignHCenter | Qt::AlignBottom);
	qApp->processEvents();

	//
	// Please make sure this is the last thing the ctor does so that we can ensure the
	// widgets are all initialized before trying to restore their state.
	//
	mSplash->showMessage(tr("Restoring window state"), Qt::AlignHCenter | Qt::AlignBottom);
	qApp->processEvents();
	startProfile("Restore window state");
	//restoreWindowState();
	endProfile();

	// do main window customization - after window state has been restored, before the window is shown
	startProfile("Update customiziation on main window");
	//QgsCustomization::instance()->updateMainWindow(mToolbarMenu);
	endProfile();

	mSplash->showMessage(tr("QGIS Ready!"), Qt::AlignHCenter | Qt::AlignBottom);

	QgsMessageLog::logMessage(QgsApplication::showSettings(), QString::null, QgsMessageLog::INFO);

	QgsMessageLog::logMessage(tr("QGIS Ready!"), QString::null, QgsMessageLog::INFO);

	startProfile("Show main window");
	show();
	qApp->processEvents();
	endProfile();

	pMapCanvas->freeze(false);
	pMapCanvas->clearExtentHistory();



	////connections
	//connect(ui.actionadd_vector, &QAction::triggered, this, &qgis_dev::addVectorLayers);
	////connect(ui.actionadd_vector, SIGNAL(triggerd()), this, SLOT(addVectorLayers())); //qt4的写法
	//connect(ui.actionadd_raster, &QAction::triggered, this, &qgis_dev::addRasterLayers);//qt5的写法
}


qgis_dev::qgis_dev()
	: QMainWindow(nullptr, nullptr),
	mSplash(nullptr)
{
	sInstance = this;
	ui.setupUi(this);
	pMapCanvas = new QgsMapCanvas();
	connect(pMapCanvas, SIGNAL(messageEmitted(const QString&, const QString&, QgsMessageBar::MessageLevel)),
		this, SLOT(displayMessage(const QString&, const QString&, QgsMessageBar::MessageLevel)));
	pMapCanvas->freeze();
	mLayerTreeView = new QgsLayerTreeView(this);
	mInfoBar = new QgsMessageBar(centralWidget());
	mProgressBar = new QProgressBar(this);
}

qgis_dev::~qgis_dev()
{

}

QgsVectorLayer * qgis_dev::addVectorLayer(const QString &vectorLayerPath, const QString &baseName, const QString &providerKey)
{
	bool wasfrozen = pMapCanvas->isFrozen();
	pMapCanvas->freeze();

	QgsDebugMsg("Creating new vector layer using" + vectorLayerPath
				+ "with baseName of " + baseName
				+ "and providerKey of " + providerKey);

	//create the layer
	QgsVectorLayer *layer = new QgsVectorLayer(vectorLayerPath, baseName, providerKey, false);
	
	if (layer && layer->isValid())
	{
		QStringList subLayers = layer->dataProvider()->subLayers();
		QgsDebugMsg(QString("got valid layer with %1 sublayers").arg(subLayers.count()));

		if (subLayers.count() > 1)
		{
			askUserForOGRSublayers(layer);

			// The first layer loaded is not useful in that case. The user can select it in
			// the list if he wants to load it.
			delete layer;
			layer = nullptr;
		}

	}


	return layer;
}

bool qgis_dev::addVectorLayers(const QStringList & theLayerQStringList, const QString & enc, const QString & dataSourceType)
{
	return false;
}

bool qgis_dev::addRasterLayers(const QStringList & theLayerQStringList, bool guiWarning)
{
	return false;
}

QgsRasterLayer * qgis_dev::addRasterLayer(const QString & rasterFile, const QString & baseName, bool guiWarning)
{
	return addRasterLayerPrivate(rasterFile, baseName, QString(), guiWarning, true);
}

QgsRasterLayer * qgis_dev::addRasterLayerPrivate(const QString & uri, const QString & baseName, const QString & providerKey, bool guiWarning, bool guiUpdate)
{
	if (guiUpdate)
	{
		// let the user know we're going to possibly be taking a while
		// QApplication::setOverrideCursor( Qt::WaitCursor );
		pMapCanvas->freeze(true);
	}

	QgsDebugMsg("Creating new raster layer using " + uri
		+ " with baseName of " + baseName);

	QgsRasterLayer *layer = nullptr;
	// XXX ya know QgsRasterLayer can snip out the basename on its own;
	// XXX why do we have to pass it in for it?
	// ET : we may not be getting "normal" files here, so we still need the baseName argument
	if (!providerKey.isEmpty() && uri.endsWith(".adf", Qt::CaseInsensitive))
	{
		QFileInfo fileInfo(uri);
		QString dirName = fileInfo.path();
		layer = new QgsRasterLayer(dirName, QFileInfo(dirName).completeBaseName(), QString("gdal"));
	}
	else if (providerKey.isEmpty())
		layer = new QgsRasterLayer(uri, baseName); // fi.completeBaseName());
	else
		layer = new QgsRasterLayer(uri, baseName, providerKey);

	QgsDebugMsg("Constructed new layer");

	QgsError error;
	QString title;
	bool ok = false;

	if (!layer->isValid())
	{
		error = layer->error();
		title = tr("Invalid Layer");

		if (shouldAskUserForGDALSublayers(layer))
		{
			askUserForGDALSublayers(layer);
			ok = true;

			// The first layer loaded is not useful in that case. The user can select it in
			// the list if he wants to load it.
			delete layer;
			layer = nullptr;
		}
	}
	else
	{
		ok = addRasterLayer(layer);
		if (!ok)
		{
			error.append(QGS_ERROR_MESSAGE(tr("Error adding valid layer to map canvas"),
				tr("Raster layer")));
			title = tr("Error");
		}
	}

	if (!ok)
	{
		if (guiUpdate)
			pMapCanvas->freeze(false);

		// don't show the gui warning if we are loading from command line
		if (guiWarning)
		{
			messageBar()->pushMessage(title, error.message(QgsErrorMessage::Text),
				QgsMessageBar::CRITICAL, messageTimeout());
		}

		if (layer)
		{
			delete layer;
			layer = nullptr;
		}
	}

	if (guiUpdate)
	{
		// draw the map
		pMapCanvas->freeze(false);
		pMapCanvas->refresh();
		// Let render() do its own cursor management
		//    QApplication::restoreOverrideCursor();
	}

	return layer;

}

void qgis_dev::setExtent(const QgsRectangle& theRect)
{
	pMapCanvas->setExtent(theRect);
}

bool qgis_dev::openLayer(const QString & fileName, bool allowInteractive /*= false*/)
{
	return true;
}

bool qgis_dev::fileSave()
{

	return true;
}

void qgis_dev::openProject(const QString &fileName)
{
	if (saveDirty())
	{
		addProject(fileName);
	}
	return;
}

bool qgis_dev::addProject(const QString& projectFile)
{

	//QFileInfo pfi(projectFile);
	//statusBar()->showMessage(tr("Loading project: %1").arg(pfi.fileName()));
	//qApp->processEvents();

	//QApplication::setOverrideCursor(Qt::WaitCursor);//设置光标为等待光标
	//closeProject();
	//
	//if (!QgsProject::instance()->read(projectFile))
	//{
	//	QString backupFile = projectFile + "~";
	//	QString loadBackupPrompt;
	//	QMessageBox::StandardButtons buttons;
	//	if (QFile(backupFile).exists())
	//	{
	//		loadBackupPrompt = "\n\n" + tr("Do you want to open the backup file\n%1\ninstead?").arg(backupFile);
	//		buttons |= QMessageBox::Yes;
	//		buttons |= QMessageBox::No;
	//	}
	//	else
	//	{
	//		buttons |= QMessageBox::Ok;
	//	}
	//	QApplication::restoreOverrideCursor();
	//	statusBar()->clearMessage();
	//	
	//	int r = QMessageBox::critical(this,
	//		tr("Unable to open project"), QgsProject::instance()->error() + loadBackupPrompt, buttons);

	//	if (QMessageBox::Yes == r && addProject(backupFile))
	//	{
	//		QgsProject::instance()->setFileName(projectFile);
	//		QgsProject::instance()->setDirty(true);
	//		mProjectLastModified = pfi.lastModified;
	//		return true;
	//	}
	//	pMapCanvas->freeze(false);
	//	pMapCanvas->refresh();
	//	return false;
	//}
	//mProjectLastModified = pfi.lastModified();

	//setTitleBarText_(*this);
	//int  myRedInt = QgsProject::instance()->readNumEntry("Gui", "/CanvasColorRedPart", 255);
	//int  myGreenInt = QgsProject::instance()->readNumEntry("Gui", "/CanvasColorGreenPart", 255);
	//int  myBlueInt = QgsProject::instance()->readNumEntry("Gui", "/CanvasColorBluePart", 255);
	//QColor myColor = QColor(myRedInt, myGreenInt, myBlueInt);
	//pMapCanvas->setCanvasColor(myColor); //this is fill color before rendering starts
	//mOverviewCanvas->setBackgroundColor(myColor);

	//QgsDebugMsg("Canvas background color restored...");
	//int myAlphaInt = QgsProject::instance()->readNumEntry("Gui", "/SelectionColorAlphaPart", 255);
	//myRedInt = QgsProject::instance()->readNumEntry("Gui", "/SelectionColorRedPart", 255);
	//myGreenInt = QgsProject::instance()->readNumEntry("Gui", "/SelectionColorGreenPart", 255);
	//myBlueInt = QgsProject::instance()->readNumEntry("Gui", "/SelectionColorBluePart", 0);
	//myColor = QColor(myRedInt, myGreenInt, myBlueInt, myAlphaInt);
	//pMapCanvas->setSelectionColor(myColor); //this is selection color before rendering starts

	//										//load project scales
	//bool projectScales = QgsProject::instance()->readBoolEntry("Scales", "/useProjectScales");
	//if (projectScales)
	//{
	//	mScaleWidget->updateScales(QgsProject::instance()->readListEntry("Scales", "/ScalesList"));
	//}

	//pMapCanvas->updateScale();
	//QgsDebugMsg("Scale restored...");

	//mActionFilterLegend->setChecked(QgsProject::instance()->readBoolEntry("Legend", "filterByMap"));

	//QSettings settings;

	//// does the project have any macros?
	//if (mPythonUtils && mPythonUtils->isEnabled())
	//{
	//	if (!QgsProject::instance()->readEntry("Macros", "/pythonCode", QString::null).isEmpty())
	//	{
	//		int enableMacros = settings.value("/qgis/enableMacros", 1).toInt();
	//		// 0 = never, 1 = ask, 2 = just for this session, 3 = always

	//		if (enableMacros == 3 || enableMacros == 2)
	//		{
	//			enableProjectMacros();
	//		}
	//		else if (enableMacros == 1) // ask
	//		{
	//			// create the notification widget for macros


	//			QToolButton *btnEnableMacros = new QToolButton();
	//			btnEnableMacros->setText(tr("Enable macros"));
	//			btnEnableMacros->setStyleSheet("background-color: rgba(255, 255, 255, 0); color: black; text-decoration: underline;");
	//			btnEnableMacros->setCursor(Qt::PointingHandCursor);
	//			btnEnableMacros->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
	//			connect(btnEnableMacros, SIGNAL(clicked()), mInfoBar, SLOT(popWidget()));
	//			connect(btnEnableMacros, SIGNAL(clicked()), this, SLOT(enableProjectMacros()));

	//			QgsMessageBarItem *macroMsg = new QgsMessageBarItem(
	//				tr("Security warning"),
	//				tr("project macros have been disabled."),
	//				btnEnableMacros,
	//				QgsMessageBar::WARNING,
	//				0,
	//				mInfoBar);
	//			// display the macros notification widget
	//			mInfoBar->pushItem(macroMsg);
	//		}
	//	}
	//}

	//emit projectRead(); // let plug-ins know that we've read in a new
	//					// project so that they can check any project
	//					// specific plug-in state

	//					// add this to the list of recently used project files
	//saveRecentProjectPath(projectFile, false);

	//QApplication::restoreOverrideCursor();

	//pMapCanvas->freeze(false);
	//pMapCanvas->refresh();

	//statusBar()->showMessage(tr("Project loaded"), 3000);

	return true;
}

void qgis_dev::startProfile(const QString &name)
{
	mProfiler->start(name);
}

void qgis_dev::endProfile()
{
	mProfiler->end();
}

void qgis_dev::functionProfile(void (qgis_dev::*fnc)(), qgis_dev *instance, QString name)
{
	startProfile(name);
	(instance->*fnc)();
	endProfile();
}

/**   
 * @brief  如果工程有改动，提示并保存
 * @return true if saved or discared, false if cancelled  
*/
bool qgis_dev::saveDirty()
{
	//QString whyDirty = "";
	//bool hasUnsavedEdits = false;

	//if (QgsMapLayerRegistry::instance()->count() > 0)
	//{
	//	QMap<QString, QgsMapLayer*> layers = QgsMapLayerRegistry::instance()->mapLayers();
	//	for (QMap<QString, QgsMapLayer*>::iterator it  = layers.begin(); it != layers.end(); ++it)
	//	{
	//		QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>(it.value());
	//		if (!vl)
	//		{
	//			continue;
	//		}
	//		hasUnsavedEdits = (vl->isEditable() && vl->isModified());
	//		if (hasUnsavedEdits)
	//		{
	//			break;
	//		}
	//	}
	//	if (hasUnsavedEdits)
	//	{
	//		whyDirty = "<p style='color:darkred;'>";
	//		whyDirty += tr("Project has layer(s) in edit mode with unsaved edits, which will NOT be saved!");
	//		whyDirty += "</p>";
	//	}
	//}
	//QMessageBox::StandardButton answer(QMessageBox::Discard);
	//pMapCanvas->freeze(true);

	////QgsDebugMsg(QString("Layer count is %1").arg(mMapCanvas->layerCount()));
	////QgsDebugMsg(QString("Project is %1dirty").arg( QgsProject::instance()->isDirty() ? "" : "not "));
	////QgsDebugMsg(QString("Map canvas is %1dirty").arg(mMapCanvas->isDirty() ? "" : "not "));

	//QSettings settings;
	//bool askThem = settings.value("qgis/askToSaveProjectChanges", true).toBool();

	//if (askThem && QgsProject::instance()->isDirty() && QgsMapLayerRegistry::instance()->count() > 0)
	//{
	//	markDirty();
	//	answer = QMessageBox::information(this, tr("Save?"),
	//		tr("Do you want to save the current project? %1")
	//		.arg(whyDirty),
	//		QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard,
	//		hasUnsavedEdits ? QMessageBox::Cancel : QMessageBox::Save);

	//	if (QMessageBox::Save == answer)
	//	{
	//		if (!fileSave())
	//			answer = QMessageBox::Cancel;
	//	}
	//}
	//pMapCanvas->freeze(false);
	//return answer != QMessageBox::Cancel;
	return true;
}

void qgis_dev::addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget)
{
	QMainWindow::addDockWidget(area, dockwidget);
	setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
	setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
	setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
	setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

	dockwidget->show();
	pMapCanvas->refresh();
}

void qgis_dev::saveMapAsImage(const QString&, QPixmap *)
{

}

void qgis_dev::namSetup()
{
	QgsNetworkAccessManager *nam = QgsNetworkAccessManager::instance();
	connect(nam, SIGNAL(authenticationRequired(QNetworkProxy *, QAuthenticator *)),
		this, SLOT(namAuthenticationRequired(QNetworkReply *, QAuthenticator *)));
	connect(nam, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)),
		this, SLOT(namProxyAuthenticationRequired(const QNetworkProxy &, QAuthenticator *)));

	connect(nam, SIGNAL(requestTimedOut(QNetworkReply*)),
		this, SLOT(namRequestTimedOut(QNetworkReply*)));
#ifndef QT_NO_OPENSSL
	connect(nam, SIGNAL(sslErrors(QNetworkReply *, const QList<QSslError> &)),
		this, SLOT(namSslErrors(QNetworkReply *, const QList<QSslError> &)));
#endif
}

void qgis_dev::masterPasswordSetup()
{
	connect(QgsAuthManager::instance(), SIGNAL(messageOut(const QString&, const QString&, QgsAuthManager::MessageLevel)),
		this, SLOT(authMessageOut(const QString&, const QString&, QgsAuthManager::MessageLevel)));
	connect(QgsAuthManager::instance(), SIGNAL(authDatabaseEraseRequested()),
		this, SLOT(eraseAuthenticationDatabase()));
}

void qgis_dev::namAuthenticationRequired(QNetworkReply *inReply, QAuthenticator *auth)
{
	QPointer<QNetworkReply> reply(inReply);
	Q_ASSERT(qApp->thread() == QThread::currentThread());

	QString username = auth->user();
	QString password = auth->password();

	if (username.isEmpty() && password.isEmpty() && reply->request().hasRawHeader("Authorization"))
	{
		QByteArray header(reply->request().rawHeader("Authorization"));
		if (header.startsWith("Basic"))
		{
			QByteArray auth(QByteArray::fromBase64(header.mid(6)));
			int pos = auth.indexOf(':');
			if (pos	>= 0)
			{
				username = auth.left(pos);
				password = auth.mid(pos + 1);
			}
		}
	}

	for (;;)
	{
		bool ok;
		{
			QMutexLocker lock(QgsCredentials::instance()->mutex());
			ok = QgsCredentials::instance()->get(
				QString("%1 at %2").arg(auth->realm(), reply->url().host()), username, password,
				tr("Authentication required"));
		}
		if (!ok)
			return;

		if (reply.isNull() || reply->isFinished())
			return;

		if (auth->user() != username || (password != auth->password() && !password.isNull()))
			return;

		{
			QMutexLocker lock(QgsCredentials::instance()->mutex());
			QgsCredentials::instance()->put(
				QString("%1 at %2").arg(auth->realm(), reply->url().host()),
				username, QString::null);
		}
	}

	// save credentials
	{
		QMutexLocker lock(QgsCredentials::instance()->mutex());
		QgsCredentials::instance()->put(
			QString("%1 at %2").arg(auth->realm(), reply->url().host()),
			username, password
		);
	}
	auth->setUser(username);
	auth->setPassword(password);
}

void qgis_dev::namProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *auth)
{
	QSettings settings;
	if (!settings.value("proxy/proxyEnabled", false).toBool() ||
		settings.value("proxy/proxyType", "").toString() == "DefaultProxy")
	{
		auth->setUser("");
		return;
	}

	QString username = auth->user();
	QString password = auth->password();

	for (;; )
	{
		bool ok;

		{
			QMutexLocker lock(QgsCredentials::instance()->mutex());
			ok = QgsCredentials::instance()->get(
				QString("proxy %1:%2 [%3]").arg(proxy.hostName()).arg(proxy.port()).arg(auth->realm()),
				username, password,
				tr("Proxy authentication required"));
		}
		if (!ok)
			return;

		if (auth->user() != username || (password != auth->password() && !password.isNull()))
			break;

		// credentials didn't change - stored ones probably wrong? clear password and retry
		{
			QMutexLocker lock(QgsCredentials::instance()->mutex());
			QgsCredentials::instance()->put(
				QString("proxy %1:%2 [%3]").arg(proxy.hostName()).arg(proxy.port()).arg(auth->realm()),
				username, QString::null);
		}
	}

	{
		QMutexLocker lock(QgsCredentials::instance()->mutex());
		QgsCredentials::instance()->put(
			QString("proxy %1:%2 [%3]").arg(proxy.hostName()).arg(proxy.port()).arg(auth->realm()),
			username, password
		);
	}

	auth->setUser(username);
	auth->setPassword(password);
}

#ifndef QT_NO_OPENSSL
void qgis_dev::namSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
	// stop the timeout timer, or app crashes if the user (or slot) takes longer than
	// singleshot timeout and tries to update the closed QNetworkReply
	QTimer *timer = reply->findChild<QTimer *>("timeoutTimer");
	if (timer)
	{
		QgsDebugMsg("Stopping network reply timeout");
		timer->stop();
	}

	QgsDebugMsg(QString("SSL errors occurred accessing URL:\n%1").arg(reply->request().url().toString()));

	QString hostport(QString("%1:%2")
		.arg(reply->url().host())
		.arg(reply->url().port() != -1 ? reply->url().port() : 443)
		.trimmed());
	QString digest(QgsAuthCertUtils::shaHexForCert(reply->sslConfiguration().peerCertificate()));
	QString dgsthostport(QString("%1:%2").arg(digest, hostport));

	const QHash<QString, QSet<QSslError::SslError> > &errscache(QgsAuthManager::instance()->getIgnoredSslErrorCache());

	if (errscache.contains(dgsthostport))
	{
		QgsDebugMsg(QString("Ignored SSL errors cahced item found, ignoring errors if they match for %1").arg(hostport));
		const QSet<QSslError::SslError>& errenums(errscache.value(dgsthostport));
		bool ignore = !errenums.isEmpty();
		int errmatched = 0;
		if (ignore)
		{
			Q_FOREACH(const QSslError& error, errors)
			{
				if (error.error() == QSslError::NoError)
					continue;

				bool errmatch = errenums.contains(error.error());
				ignore = ignore && errmatch;
				errmatched += errmatch ? 1 : 0;
			}
		}

		if (ignore && errenums.size() == errmatched)
		{
			QgsDebugMsg(QString("Errors matched cached item's, ignoring all for %1").arg(hostport));
			reply->ignoreSslErrors();
			return;
		}

		QgsDebugMsg(QString("Errors %1 for cached item for %2")
			.arg(errenums.isEmpty() ? "not found" : "did not match",
				hostport));
	}


	QgsAuthSslErrorsDialog *dlg = new QgsAuthSslErrorsDialog(reply, errors, this, digest, hostport);
	dlg->setWindowModality(Qt::ApplicationModal);
	dlg->resize(580, 512);
	if (dlg->exec())
	{
		if (reply)
		{
			QgsDebugMsg(QString("All SSL errors ignored for %1").arg(hostport));
			reply->ignoreSslErrors();
		}
	}
	dlg->deleteLater();

	// restart network request timeout timer
	if (reply)
	{
		QSettings s;
		QTimer *timer = reply->findChild<QTimer *>("timeoutTimer");
		if (timer)
		{
			QgsDebugMsg("Restarting network reply timeout");
			timer->setSingleShot(true);
			timer->start(s.value("/qgis/networkAndProxy/networkTimeout", "60000").toInt());
		}
	}
}

void qgis_dev::namRequestTimedOut(QNetworkReply *reply)
{
	Q_UNUSED(reply);
	QLabel *msgLabel = new QLabel(tr("A network request timed out, any data received is likely incomplete.") +
		tr(" Please check the <a href=\"#messageLog\">message log</a> for further info."), messageBar());
	msgLabel->setWordWrap(true);
	connect(msgLabel, SIGNAL(linkActivated(QString)), mLogDock, SLOT(show()));
	messageBar()->pushItem(new QgsMessageBarItem(msgLabel, QgsMessageBar::WARNING, messageTimeout()));
}

void qgis_dev::eraseAuthenticationDatabase()
{
	// First check if now is a good time to interact with the user, e.g. project is done loading.
	// If not, ask QgsAuthManager to re-emit authDatabaseEraseRequested from the schedule timer.
	// No way to know if user interaction will interfere with plugins loading layers.

	if (!QgsProject::instance()->fileName().isNull()) // a non-blank project is loaded
	{
		// Apparently, as of QGIS 2.9, the only way to query that the project is in a
		// layer-loading state is via a custom property of the project's layer tree.
		QgsLayerTreeGroup *layertree(QgsProject::instance()->layerTreeRoot());
		if (layertree && layertree->customProperty("loading").toBool())
		{
			QgsDebugMsg("Project loading, skipping auth db erase");
			QgsAuthManager::instance()->setScheduledAuthDbEraseRequestEmitted(false);
			return;
		}
	}

	// TODO: Check is Browser panel is also still loading?
	//       It has auto-connections in parallel (if tree item is expanded), though
	//       such connections with possible master password requests *should* be ignored
	//       when there is an authentication db erase scheduled.

	// This funtion should tell QgsAuthManager to stop any erase db schedule timer,
	// *after* interacting with the user
	QgsAuthGuiUtils::eraseAuthenticationDatabase(messageBar(), messageTimeout(), this);
}

void qgis_dev::activateDeactivateLayerRelatedActions(QgsMapLayer *layer)
{
	
}

void qgis_dev::refreshActionFeatureAction()
{
	
}

void qgis_dev::loadGDALSublayers(const QString& uri, const QStringList& list)
{
	QString path, name;
	QgsRasterLayer *subLayer = nullptr;

	//add layers in reverse order so they appear in the right order in the layer dock
	for (int i = list.size() - 1; i >= 0; i--)
	{
		path = list[i];
		// shorten name by replacing complete path with filename
		name = path;
		name.replace(uri, QFileInfo(uri).completeBaseName());
		subLayer = new QgsRasterLayer(path, name);
		if (subLayer)
		{
			if (subLayer->isValid())
				addRasterLayer(subLayer);
			else
				delete subLayer;
		}

	}
}

#endif

QIcon qgis_dev::getThemeIcon(const QString &theName)
{
	QString myPreferredPath = activeThemePath() + QDir::separator() + theName;
	QString myDefaultPath = defaultThemePath() + QDir::separator() + theName;

	if (QFile::exists(myPreferredPath))
	{
		return QIcon(myPreferredPath);
	}
	else if (QFile::exists(myDefaultPath))
	{
		return QIcon(myDefaultPath);
	}
	else
	{
		return QIcon();
	}
}

void qgis_dev::setIconSizes(int size)
{
	int dockSize;
	if (size > 32)
	{
		dockSize = size - 16;
	}
	else if (size == 32)
	{
		dockSize = 24;
	}
	else
	{
		dockSize = 16;
	}

	//Set the icon size of for all the toolbars created in the future.
	setIconSize(QSize(size, size));

	//Change all current icon sizes.
	QList<QToolBar *> toolbars = findChildren<QToolBar *>();
	Q_FOREACH(QToolBar * toolbar, toolbars)
	{
		QString className = toolbar->parent()->metaObject()->className();
		if (className == "QgisApp")
		{
			toolbar->setIconSize(QSize(size, size));
		}
		else
		{
			toolbar->setIconSize(QSize(dockSize, dockSize));
		}
	}

	Q_FOREACH(QgsComposer *c, mPrintComposers)
	{
		//c->setIconSizes(size);
	}
}


void qgis_dev::openMessageLog()
{
	mMessageButton->setChecked(true);
}

int qgis_dev::messageTimeout()
{
	QSettings settings;
	return settings.value("/qgis/messageTimeout", 5).toInt();
}

void qgis_dev::completeInitialization()
{
	emit initializationCompleted();
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

QgsMapLayer * qgis_dev::activeLayer()
{
	return mLayerTreeView ? mLayerTreeView->currentLayer() : nullptr;
}


void qgis_dev::initLayerTreeView()
{
	mLayerTreeView->setWhatsThis(tr("Map legend that displays all the layers currently on the map canvas. Click on the checkbox to turn a layer on or off. Double-click on a layer in the legend to customize its appearance and set other properties."));
	mLayerTreeDock = new QgsDockWidget(tr("Layers"), this);
	mLayerTreeDock->setObjectName(QStringLiteral("Layers"));
	mLayerTreeDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	
	QgsLayerTreeModel *model = new QgsLayerTreeModel(QgsProject::instance()->layerTreeRoot(), this);
	model->setFlag(QgsLayerTreeModel::AllowNodeRename);
	model->setFlag(QgsLayerTreeModel::AllowNodeReorder);
	model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
	model->setFlag(QgsLayerTreeModel::ShowLegendAsTree);
	model->setAutoCollapseLegendNodes(10);

	mLayerTreeView->setModel(model);


	//add group action
	QAction *actionAddGroup = new QAction(tr("Add Group"), this);
	actionAddGroup->setIcon(qgis_dev::getThemeIcon(QStringLiteral("mActionAdd.png")));
	actionAddGroup->setToolTip(tr("Add Group"));
	//connect(actionAddGroup, &QAction::triggered, mLayerTreeView->defaultActions(), &QgsLayerTreeViewDefaultActions::actionAddGroup);

	// expand / collapse tool buttons
	QAction *actionExpandAll = new QAction(tr("Expand All"), this);
	actionExpandAll->setIcon(qgis_dev::getThemeIcon(QStringLiteral("mActionExpandTree.png")));
	actionExpandAll->setToolTip(tr("Expand All"));
	//connect(actionExpandAll, &QAction::triggered, mLayerTreeView, &QgsLayerTreeView::expandAllNodes);
	QAction *actionCollapseAll = new QAction(tr("Collapse All"), this);
	actionCollapseAll->setIcon(qgis_dev::getThemeIcon(QStringLiteral("mActionCollapseTree.png")));
	actionCollapseAll->setToolTip(tr("Collapse All"));
	//connect(actionCollapseAll, &QAction::triggered, mLayerTreeView, &QgsLayerTreeView::collapseAllNodes);

	//remove action
	QAction *actionRemoveAll = new QAction(tr("Add Group"), this);
	actionRemoveAll->setIcon(qgis_dev::getThemeIcon("mActionRemoveLayer.svg"));
	actionRemoveAll->setToolTip(tr("Remove Action"));

	QToolBar *toolbar = new QToolBar();
	toolbar->setIconSize(QSize(20,20));
	toolbar->addAction(actionAddGroup);
	toolbar->addAction(actionExpandAll);
	toolbar->addAction(actionCollapseAll);
	toolbar->addAction(actionRemoveAll);

	QVBoxLayout *vboxLayout = new QVBoxLayout;
	vboxLayout->setMargin(0);
	vboxLayout->setContentsMargins(0, 0, 0, 0);
	vboxLayout->setSpacing(0);
	vboxLayout->addWidget(toolbar);
	vboxLayout->addWidget(mLayerTreeView);

	QWidget *w = new QWidget;
	w->setLayout(vboxLayout);
	mLayerTreeDock->setWidget(w);
	addDockWidget(Qt::LeftDockWidgetArea, mLayerTreeDock);
	
	mLayerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge(QgsProject::instance()->layerTreeRoot(), pMapCanvas, this);

}

void qgis_dev::createStatusBar()
{
	statusBar()->setStyleSheet(QStringLiteral("QStatusBar::item {border: none;}"));

	//添加比例尺、坐标以及进度条
	//! 添加进度条
	mProgressBar = new QProgressBar(statusBar());
	mProgressBar->setObjectName(QStringLiteral("mProgressBar"));
	mProgressBar->setMaximumWidth(100);
	mProgressBar->hide();
	mProgressBar->setWhatsThis(tr("Progress bar that displays the status "
								  "of rendering layers and other time-intensive operations"));
	statusBar()->addPermanentWidget(mProgressBar, 1);
	
	//connect(pMapCanvas, SIGNAL(renderStarting()), this, SLOT(canvasRefreshStarted()));
	//connect(pMapCanvas, SIGNAL(mapCanvasRefreshed()), this, SLOT(canvasRefreshFinished()));

	QFont myFont("Arial", 9);
	statusBar()->setFont(myFont);

	//! coords status bar widget
	mCoordsEdit = new QLineEdit(QString(), statusBar());
	mCoordsEdit->setObjectName("mCoordsEdit");
	mCoordsEdit->setFont(myFont);
	mCoordsEdit->setMinimumWidth(10);
	mCoordsEdit->setMaximumWidth(300);
	mCoordsEdit->setContentsMargins(0, 0, 0, 0);
	mCoordsEdit->setAlignment(Qt::AlignCenter);
	statusBar()->addPermanentWidget(mCoordsEdit, 0);
	//mCoordsEdit->setReadOnly(true);
	//connect();


	QComboBox *coordFormatCombox = new QComboBox(statusBar());
	coordFormatCombox->addItem( tr("Decimal Degree") );
	coordFormatCombox->addItem( tr("Degree") );
	statusBar()->addPermanentWidget(coordFormatCombox);
	//connect();

	//! 添加坐标显示标签
	mCoordsLabel = new QLabel(QString(), statusBar());
	mCoordsLabel->setObjectName("mCoordsLabel");
	mCoordsLabel->setFont(myFont);
	mCoordsLabel->setMidLineWidth(10);
	mCoordsLabel->setMargin(3);
	mCoordsLabel->setAlignment(Qt::AlignCenter);
	mCoordsLabel->setFrameStyle(QFrame::NoFrame);
	mCoordsLabel->setText(tr("Coordinate:"));
	mCoordsLabel->setToolTip(tr("Current map coordinate"));
	statusBar()->addPermanentWidget(mCoordsLabel, 0);



	mDizzyTimer = new QTimer(this);
	//connect();

	//! 比例尺标签
	mScaleLabel = new QLabel(QString(), statusBar());
	mScaleLabel->setObjectName("mScaleLabel");
	mScaleLabel->setFont(myFont);
	mScaleLabel->setMinimumWidth(10);
	mScaleLabel->setMargin(3);
	mScaleLabel->setAlignment(Qt::AlignCenter);
	mScaleLabel->setFrameStyle(QFrame::NoFrame);
	mScaleLabel->setText(tr("Scale"));
	mScaleLabel->setToolTip(tr("Current map scale"));
	statusBar()->addPermanentWidget(mScaleLabel, 0);
	connect(mScaleLabel, SIGNAL(scaleLockChanged(bool)), pMapCanvas, SLOT(setScaleLocked(bool)));

	mScaleEdit = new QgsScaleComboBox(statusBar());
	mScaleEdit->setObjectName("mScaleEdit");
	mScaleEdit->setFont(myFont);
	mScaleEdit->setMinimumWidth(10);
	mScaleEdit->setMinimumWidth(100);
	mScaleEdit->setContentsMargins(0, 0, 0, 0);
	mScaleEdit->setToolTip(tr("Current map scale (formatted as x:y)"));
	statusBar()->addPermanentWidget(mScaleEdit, 0);
	//connect();

	statusBar()->showMessage(tr("Ready"));

	//! 消息视图按钮标签
	mMessageButton = new QToolButton(statusBar());
	mMessageButton->setAutoRaise(true);
	mMessageButton->setIcon(qgis_dev::getThemeIcon("bubble.svg"));
	mMessageButton->setToolTip(tr("Messages"));
	mMessageButton->setWhatsThis(tr("Messages"));
	mMessageButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	mMessageButton->setObjectName(QStringLiteral("mMessageLogViewerButton"));
	mMessageButton->setMaximumHeight(mScaleLabel->height());
	mMessageButton->setCheckable(true);
	statusBar()->addPermanentWidget(mMessageButton, 0);

}

void qgis_dev::createCanvasTools()
{
	//create tools
	mMapTools.mZoomIn = new QgsMapToolZoom(pMapCanvas, false/*zoomin*/);
	//mMapTools.mZoomIn->setAction(mActionZoomIn);
	mMapTools.mZoomOut = new QgsMapToolZoom(pMapCanvas, true/*zoomout*/);
	//mMapTools.mZoomOut->setAction(mActionZoomOut);
	mMapTools.mPan = new QgsMapToolPan(pMapCanvas);
	// mMapTools.mPan->setAction( mActionPan );
	mMapTools.mIdentify = new QgsMapToolIdentify(pMapCanvas);
	//mMapTools.mIdentify->setAction(mActionIdentify);
	//connect( mMapTools.mIdentify, &QgsMapToolIdentifyAction::copyToClipboard,this, &QgisApp::copyFeatures );

}

void qgis_dev::createComposer()
{

}

const QString qgis_dev::activeThemePath()
{
	return "";
}

const QString qgis_dev::defaultThemePath()
{
	return ":/images/themes/default";
}

void qgis_dev::histogramStretch(bool visibleAreaOnly /*= false*/, QgsRaster::ContrastEnhancementLimits theLimits /*= QgsRaster::ContrastEnhancementMinMax*/)
{

}

void qgis_dev::adjustBrightnessContrast(int delta, bool updateBrightness /*= true*/)
{

}

void qgis_dev::enableRasterTools(bool val)
{

}

void qgis_dev::enableVectorTools(bool val)
{

}

bool qgis_dev::askUserForZipItemlayers(QString path)
{
	bool ok = false;
	QVector<QgsDataItem*> childItems;
	QgsZipItem *zipItem = nullptr;
	QSettings settings;
	int promptLayers = settings.value("/qgis/promptForRasterSublayers", 1).toInt();
	
	QgsDebugMsg("askUserForZipItemLayers(" + path + ')');

	//如果scanZipBrowser == no, 则跳过，进行下一个文件
	if (settings.value("/qgis/scanZipInBrowser2", "basic").toString() == "no")
	{
		return false;
	}

	zipItem = new QgsZipItem(nullptr, path, path);
	if (!zipItem)
		return false;
	
	zipItem->populate();
	QgsDebugMsg(QString("Path= %1 got zipitem with %2 children").arg(path).arg(zipItem->rowCount()));

	//如果子项为1或0，则退出，由gdal或ogr驱动创建item。
	if (zipItem->rowCount() <= 1)
	{
		zipItem->deleteLater();
		return false;
	}
	// if promptLayers=Load all, load all layers without prompting
	if (promptLayers == 3)
	{
		childItems = zipItem->children();
	}
	else if (promptLayers == 2) // exit if promptLayers=Never
	{
		zipItem->deleteLater();
		return false;
	}
	else //初始化一个选择对话框并显示
	{
		QgsSublayersDialog chooseSublayersDialog(QgsSublayersDialog::Vsifile, "vsi", this);
		QgsSublayersDialog::LayerDefinitionList layers;

		for (int i = 0; i < zipItem->children().size(); i++)
		{
			QgsDataItem *item = zipItem->children().at(i);
			QgsLayerItem *layerItem = dynamic_cast<QgsLayerItem*>(item);
			if (!layerItem)
				continue;

			QgsDebugMsgLevel(QString("item path=%1 provider=%2").arg(item->path(), layerItem->providerKey()), 2);
			
			QgsSublayersDialog::LayerDefinition def;
			def.layerId = i;
			def.layerName = item->name();
			if (layerItem->providerKey() == "gdal")
			{
				def.type = tr("Raster");
			}
			else if (layerItem->providerKey() == "ogr")
			{
				def.type = tr("Vector");
			}
			layers << def;
		}
		chooseSublayersDialog.populateLayerTable(layers);
		if (chooseSublayersDialog.exec())
		{
			Q_FOREACH(const QgsSublayersDialog::LayerDefinition &def, chooseSublayersDialog.selection())
			{
				childItems << zipItem->children().at(def.layerId);
			}
		}

	}

	if (childItems.isEmpty())
	{
		ok = true;
	}

	//add childItems
	Q_FOREACH(QgsDataItem *item, childItems)
	{
		QgsLayerItem *layerItem = dynamic_cast<QgsLayerItem *>(item);
		if (!layerItem)
			continue;

		QgsDebugMsg(QString("item path=%1 provider=%2").arg(item->path(), layerItem->providerKey()));

		if (layerItem->providerKey() == "gdal")
		{
			if (addRasterLayer(item->path(), QFileInfo(item->name()).completeBaseName()))
				ok = true;
		}
		else if (layerItem->providerKey() == "ogr")
		{
			if (addVectorLayers(QStringList(item->path()), "System", "file"))
				ok = true;
		}
	}
	zipItem->deleteLater();
	return ok;
}

void qgis_dev::askUserForGDALSublayers(QgsRasterLayer * layer)
{
	if (!layer)
		return;

	QStringList sublayers = layer->subLayers();
	QgsDebugMsg(QString("raster has %1 sublayers").arg(layer->subLayers().size()));

	if (sublayers.size() < 1)
		return;

	// if promptLayers=Load all, load all sublayers without prompting
	QSettings settings;
	if (settings.value("/qgis/promptForRasterSublayers", 1).toInt() == 3)
	{
		loadGDALSublayers(layer->source(), sublayers);
		return;
	}

	// We initialize a selection dialog and display it.
	QgsSublayersDialog chooseSublayersDialog(QgsSublayersDialog::Gdal, "gdal", this);

	QgsSublayersDialog::LayerDefinitionList layers;
	QStringList names;
	for (int i = 0; i < sublayers.size(); i++)
	{
		// simplify raster sublayer name - should add a function in gdal provider for this?
		// code is copied from QgsGdalLayerItem::createChildren
		QString name = sublayers[i];
		QString path = layer->source();
		// if netcdf/hdf use all text after filename
		// for hdf4 it would be best to get description, because the subdataset_index is not very practical
		if (name.startsWith("netcdf", Qt::CaseInsensitive) ||
			name.startsWith("hdf", Qt::CaseInsensitive))
			name = name.mid(name.indexOf(path) + path.length() + 1);
		else
		{
			// remove driver name and file name
			name.remove(name.split(':')[0]);
			name.remove(path);
		}
		// remove any : or " left over
		if (name.startsWith(':'))
			name.remove(0, 1);

		if (name.startsWith('\"'))
			name.remove(0, 1);

		if (name.endsWith(':'))
			name.chop(1);

		if (name.endsWith('\"'))
			name.chop(1);

		names << name;

		QgsSublayersDialog::LayerDefinition def;
		def.layerId = i;
		def.layerName = name;
		layers << def;
	}

	chooseSublayersDialog.populateLayerTable(layers);

	if (chooseSublayersDialog.exec())
	{
		// create more informative layer names, containing filename as well as sublayer name
		QRegExp rx("\"(.*)\"");
		QString uri, name;

		Q_FOREACH(const QgsSublayersDialog::LayerDefinition& def, chooseSublayersDialog.selection())
		{
			int i = def.layerId;
			if (rx.indexIn(sublayers[i]) != -1)
			{
				uri = rx.cap(1);
				name = sublayers[i];
				name.replace(uri, QFileInfo(uri).completeBaseName());
			}
			else
			{
				name = names[i];
			}

			QgsRasterLayer *rlayer = new QgsRasterLayer(sublayers[i], name);
			if (rlayer && rlayer->isValid())
			{
				addRasterLayer(rlayer);
			}
		}
	}
}

bool qgis_dev::shouldAskUserForGDALSublayers(QgsRasterLayer * layer)
{
	// return false if layer is empty or raster has no sublayers
	if (!layer || layer->providerType() != "gdal" || layer->subLayers().size() < 1)
		return false;

	QSettings settings;
	int promptLayers = settings.value("/qgis/promptForRasterSublayers", 1).toInt();
	// 0 = Always -> always ask (if there are existing sublayers)
	// 1 = If needed -> ask if layer has no bands, but has sublayers
	// 2 = Never -> never prompt, will not load anything
	// 3 = Load all -> never prompt, but load all sublayers

	return promptLayers == 0 || promptLayers == 3 || (promptLayers == 1 && layer->bandCount() == 0);
}
void qgis_dev::askUserForOGRSublayers(QgsVectorLayer * layer)
{
	if (!layer)
	{
		layer = qobject_cast<QgsVectorLayer *>(activeLayer());
		if (!layer || layer->dataProvider()->name() != "ogr")
			return;
	}

	QStringList sublayers = layer->dataProvider()->subLayers();
	QString layertype = layer->dataProvider()->storageType();

	QgsSublayersDialog::LayerDefinitionList list;
	Q_FOREACH(const QString& sublayer, sublayers)
	{
		// OGR provider returns items in this format:
		// <layer_index>:<name>:<feature_count>:<geom_type>

		QStringList elements = sublayer.split(":");
		// merge back parts of the name that may have been split
		while (elements.size() > 4)
		{
			elements[1] += ":" + elements[2];
			elements.removeAt(2);
		}

		if (elements.count() == 4)
		{
			QgsSublayersDialog::LayerDefinition def;
			def.layerId = elements[0].toInt();
			def.layerName = elements[1];
			def.count = elements[2].toInt();
			def.type = elements[3];
			list << def;
		}
		else
		{
			QgsDebugMsg("Unexpected output from OGR provider's subLayers()! " + sublayer);
		}
	}


	// We initialize a selection dialog and display it.
	QgsSublayersDialog chooseSublayersDialog(QgsSublayersDialog::Ogr, "ogr", this);
	chooseSublayersDialog.populateLayerTable(list);

	if (!chooseSublayersDialog.exec())
		return;

	QString uri = layer->source();
	//the separator char & was changed to | to be compatible
	//with url for protocol drivers
	if (uri.contains('|', Qt::CaseSensitive))
	{
		// If we get here, there are some options added to the filename.
		// A valid uri is of the form: filename&option1=value1&option2=value2,...
		// We want only the filename here, so we get the first part of the split.
		QStringList theURIParts = uri.split('|');
		uri = theURIParts.at(0);
	}
	QgsDebugMsg("Layer type " + layertype);

	// The uri must contain the actual uri of the vectorLayer from which we are
	// going to load the sublayers.
	QString fileName = QFileInfo(uri).baseName();
	QList<QgsMapLayer *> myList;
	Q_FOREACH(const QgsSublayersDialog::LayerDefinition& def, chooseSublayersDialog.selection())
	{
		QString layerGeometryType = def.type;
		QString composedURI = uri + "|layerid=" + QString::number(def.layerId);

		if (!layerGeometryType.isEmpty())
		{
			composedURI += "|geometrytype=" + layerGeometryType;
		}

		QgsDebugMsg("Creating new vector layer using " + composedURI);
		QString name = fileName + " " + def.layerName;
		if (!layerGeometryType.isEmpty())
			name += " " + layerGeometryType;
		QgsVectorLayer *layer = new QgsVectorLayer(composedURI, name, "ogr", false);
		if (layer && layer->isValid())
		{
			myList << layer;
		}
		else
		{
			QString msg = tr("%1 is not a valid or recognized data source").arg(composedURI);
			messageBar()->pushMessage(tr("Invalid Data Source"), msg, QgsMessageBar::CRITICAL, messageTimeout());
			if (layer)
				delete layer;
		}
	}

	if (!myList.isEmpty())
	{
		// Register layer(s) with the layers registry
		QgsMapLayerRegistry::instance()->addMapLayers(myList);
		Q_FOREACH(QgsMapLayer *l, myList)
		{
			bool ok;
			l->loadDefaultStyle(ok);
		}
	}
}
bool qgis_dev::addRasterLayer(QgsRasterLayer * theRasterLayer)
{
	Q_CHECK_PTR(theRasterLayer);

	if (!theRasterLayer)
	{
		// XXX insert meaningful whine to the user here; although be
		// XXX mindful that a null layer may mean exhausted memory resources
		return false;
	}

	if (!theRasterLayer->isValid())
	{
		delete theRasterLayer;
		return false;
	}

	// register this layer with the central layers registry
	QList<QgsMapLayer *> myList;
	myList << theRasterLayer;
	QgsMapLayerRegistry::instance()->addMapLayers(myList);

	return true;
}
