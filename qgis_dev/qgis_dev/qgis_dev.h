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

class QgisAppInterface;
class QgisAppStyleSheet;
class QgsWelcomePage;
class QgsComposer;
class QgsComposerManager;
class QgsComposerView;

#include "ui_qgis_dev.h"
#include <QtWidgets/QMainWindow>
#include <QDockWidget>
#include <QLabel>
#include <QIcon>
#include <QStackedWidget>
#include <QTimer>
#include <QComboBox>
#include <QList>
#include <QProgressBar>
#include <QLineEdit>
#include <QCursor>
#include <QToolBar>
#include <QPointer>
#include <QDateTime>
#include <QSplashScreen>

#include <QNetworkReply>
#include <QNetworkProxy>
#include <QAuthenticator>

#include "qgsconfig.h"
#include "qgsmapcanvas.h"
#include "qgsmaplayer.h"
#include "qgslayertreelayer.h"
#include "qgslayertreeview.h"
#include "qgslayertreemapcanvasbridge.h"
#include "qgsscalecombobox.h"
#include "qgsrendererv2.h"
#include "qgsmessagebar.h"
#include "qgsbrowsertreeview.h"
#include "qgsraster.h"
#include "qgsmessagelogviewer.h"
#include "qgsdockwidget.h"
#include "qgsmapoverviewcanvas.h"
#include "qgsmaptoolidentify.h"
#include "qgsuserinputdockwidget.h"
#include "qgsruntimeprofiler.h"
//#include "qgswelcomepageitemsmodel.h"

class qgis_dev : public QMainWindow
{
	Q_OBJECT
public:
	qgis_dev(QSplashScreen *splash, bool restorePlugins = true, bool skipVersionCheck = false, QWidget *parent = Q_NULLPTR, Qt::WindowFlags flags = Qt::Window);
	qgis_dev();
	~qgis_dev();
	/**
	* Add a vector layer to the canvas, returns pointer to it
	*/
	QgsVectorLayer *addVectorLayer(const QString& vectorLayerPath, const QString& baseName, const QString& providerKey);

	/** \brief overloaded version of the private addLayer method that takes a list of
	* file names instead of prompting user with a dialog.
	@param enc encoding type for the layer
	@param dataSourceType type of ogr datasource
	@returns true if successfully added layer
	*/
	bool addVectorLayers(const QStringList &theLayerQStringList, const QString &enc, const QString &dataSourceType);

	/** Overloaded vesion of the private addRasterLayer()
	Method that takes a list of file names instead of prompting
	user with a dialog.
	@returns true if successfully added layer(s)
	*/
	bool addRasterLayers(const QStringList &theLayerQStringList, bool guiWarning = true);

	/** Open a raster layer for the given file
	@returns false if unable to open a raster layer for rasterFile
	@note
	This is essentially a simplified version of the above
	*/
	QgsRasterLayer *addRasterLayer(const QString &rasterFile, const QString &baseName, bool guiWarning = true);
	/** Open a raster layer - this is the generic function which takes all parameters */
	QgsRasterLayer* addRasterLayerPrivate(const QString & uri, const QString & baseName,
		const QString & providerKey, bool guiWarning,
		bool guiUpdate);
	/** Set the extents of the map canvas */
	void setExtent(const QgsRectangle& theRect);
	/** Open a raster or vector file; ignore other files.
	Used to process a commandline argument, FileOpen or Drop event.
	Set interactive to true if it is ok to ask the user for information (mostly for
	when a vector layer has sublayers and we want to ask which sublayers to use).
	@returns true if the file is successfully opened
	*/
	bool openLayer(const QString & fileName, bool allowInteractive = false);
	//! Save project. Returns true if the user selected a file to save to, false if not.
	bool fileSave();
	/**   
	 * @brief  打开指定的工程文件，如有必要会提示保存之前的工程
	 * @return void  
	 * @param fileName  
	 * @note  
	*/
	void openProject(const QString &fileName);
	/**   
	 * @brief  打开qgis工程文件
	 * @return bool  
	 * @param projectFile  
	 * @note  
	*/
	bool addProject(const QString& projectFile);
	//! 添加dock widget
	void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget);
	//!Overloaded version of the private function with same name that takes the imagename as a parameter
	void saveMapAsImage(const QString&, QPixmap *);
	//! 保持单一实例
	static qgis_dev *instance() { return sInstance; };

	//! 初始化网络管理
	void namSetup();

	//! 设置密码
	void masterPasswordSetup();

	//! 根据名称获取主题图标
	static QIcon getThemeIcon(const QString &theName);
	
	void setIconSizes(int size);

	//! 获取当前mapcanvas
	QgsMapCanvas *mapCanvas() { return pMapCanvas; }

	//! 获取当前信息展示栏
	QgsMessageBar *messageBar() { return mInfoBar; }

	/** Open the message log dock widget **/
	void openMessageLog();

	//! 控制信息显示条显示的时间
	int messageTimeout();
	
    //! 发射初始化完成信号
    void completeInitialization();

	//! 表示当前工程中设置有改变，需要更新
	void markDirty();


public slots:
	/**   
	 * @brief  添加矢量图层
	 * @date 2018/06/11
	 * @return void  
	 * @note  
	*/
	void addVectorLayers();

	/**   
	 * @brief  添加栅格图层
	 * @date 2018/06/11
	 * @return void  
	 * @note  
	*/
	void addRasterLayers();

	//! Return pointer to the active layer
	QgsMapLayer *activeLayer();

	//! request credentials for network manager
	void namAuthenticationRequired(QNetworkReply *reply, QAuthenticator *auth);
	void namProxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *auth);
#ifndef QT_NO_OPENSSL
	void namSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
#endif
	void namRequestTimedOut(QNetworkReply *reply);

	//! Schedule and erase of the authentication database upon confirmation
	void eraseAuthenticationDatabase();

	//! Push authentication manager output to messagebar
	//void authMessageOut(const QString& message, const QString& authtag, QgsAuthManager::MessageLevel level);

	/** Activates or deactivates actions depending on the current maplayer type.
	Is called from the legend when the current legend item has changed*/
	void activateDeactivateLayerRelatedActions(QgsMapLayer *layer);
	/** Refreshes the state of the layer actions toolbar action
	* @note added in 2.1 */
	void refreshActionFeatureAction();

	void loadGDALSublayers(const QString& uri, const QStringList& list);


signals:

	//! this signal is emitted when QGI initialization is complete 
	void initializationCompleted();

private:
	Ui::qgis_devClass ui;
	static qgis_dev *sInstance;

	void startProfile(const QString &name);
	void endProfile();
	void functionProfile(void (qgis_dev::*fnc)(), qgis_dev *instance, QString name);
	//! Read Well Known Binary stream from PostGIS
	//void readWKB(const char *, QStringList tables);
	//! shows the paste-transformations dialog
	// void pasteTransformations();
	//! check to see if file is dirty and if so, prompt the user th save it
	bool saveDirty();
	//! Flag to indicate an edits save/rollback for active layer is in progress
	bool mSaveRollbackInProgress;
	
	QgsRuntimeProfiler *mProfiler = nullptr;

	QgisAppStyleSheet *mStyleSheetBuilder = nullptr;

	// 地图工具
	class Tools
	{
	public:
		Tools() = default;
		QgsMapTool *mZoomIn = nullptr;
		QgsMapTool *mZoomOut = nullptr;
		QgsMapTool *mPan = nullptr;
		QgsMapToolIdentify *mIdentify = nullptr;	
	} mMapTools;

	QgsMapCanvas *pMapCanvas = nullptr; //画布
	QgsMapOverviewCanvas *mOverviewCanvas = nullptr;
	QStackedWidget *mStackedWidget;//用于mapCanvas和mapComposor的切换
	QStackedWidget *mCentralContainer = nullptr;//
	QgsLayerTreeView *mLayerTreeView = nullptr;//图层管理器
	QgsLayerTreeMapCanvasBridge *mLayerTreeCanvasBridge = nullptr; //连接图层管理器和地图画布的桥梁
	QgsMessageBarItem *mLastMapToolMessage = nullptr;
	QgsMessageLogViewer *mLogViewer = nullptr;//消息视图
	QgsWelcomePage *mWelcomePage = nullptr;
	QgsMapTool *mNonEditMapTool = nullptr;
	
	QSplashScreen *mSplash = nullptr;
	//! Print composers of this project, accessible by id string
	QSet<QgsComposer*> mPrintComposers;
	QgsDockWidget *mLayerTreeDock = nullptr;
	QgsDockWidget *mLayerOrderDock = nullptr;
	QgsDockWidget *mOverviewDock = nullptr;
	QgsDockWidget *mpGpsDock = nullptr;
	QgsDockWidget *mLogDock = nullptr;



	QLabel *mScaleLabel = nullptr; //状态栏中显示scale
	QgsScaleComboBox *mScaleEdit = nullptr; //在状态栏中显示比例尺
	QProgressBar *mProgressBar = nullptr;
	QLabel *mCoordsLabel = nullptr;//在状态栏中显示 Coordinate / Extent
	QLineEdit *mCoordsEdit = nullptr;//在状态栏中显示地理坐标

	QList<QgsMapCanvasLayer> mListMapCanvesLayers;//画布中所有图层的集合

	QCursor *mOverviewMapCursor = nullptr;//鹰眼图的鼠标指针
	QRect *mMapWindow = nullptr; //当前地图窗口范围用实际坐标表示

	QString mFullPathName; //当前地图文件的完整路径名（已保存或加载）

	unsigned int mMousePrecisionDecimalPlaces;//指定鼠标坐标小数点位数
	QDateTime mProjectLastModified;
	
	QTimer *mDizzyTimer;//
	int mProjOpen = 0;

	
	


	QgsMessageBar *mInfoBar = nullptr;//非阻塞方式的消息栏
	QWidget *mMacroshWarn = nullptr;
	QToolButton *mMessageButton;
	QgsUserInputDockWidget *mUserInputDockWidget = nullptr;


	//! 用于获取images资源库当中的Icon路径
	static const QString activeThemePath();
	static const QString defaultThemePath();

	/************************************************************************/
	/* Private Member Function                                                                     */
	/************************************************************************/
	#pragma region private member function
	//! 初始化图层管理器
	void initLayerTreeView();
	//! 构建状态栏
	void createStatusBar();
	//! 初始化地图工具
	void createCanvasTools();
	//! 构建打印出图视图
	void createComposer();
	//! 直方图拉伸
	void histogramStretch(bool visibleAreaOnly = false, QgsRaster::ContrastEnhancementLimits theLimits = QgsRaster::ContrastEnhancementMinMax);
	//! 对比度调整，updateBrightness用来控制调整对比度时是否需要同时调整亮度
	void adjustBrightnessContrast(int delta, bool updateBrightness = true);
	//! 开关栅格图层操作
	void enableRasterTools(bool val);
	//! 开关矢量图层操作
	void enableVectorTools(bool val);
	//! 弹出对话框，用户选择图层，返回true，则加载成功。
	bool askUserForZipItemlayers(QString path);
	//! 弹出对话框，加载用户选择的图层。
	void askUserForGDALSublayers(QgsRasterLayer *layer);
	//! 该方法确认选择的图层是否包括子图层。 
	bool shouldAskUserForGDALSublayers(QgsRasterLayer *layer);
	//! 弹出对话框，加载用户选择的图层。
	void askUserForOGRSublayers(QgsVectorLayer *layer);
	//! 添加栅格图层，不会强制刷新
	bool addRasterLayer(QgsRasterLayer * theRasterLayer);
	#pragma  endregion private member function
};

#define QGIS_ICON_SIZE 24


#endif // QGIS_DEV_H__