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
	 * @brief  ��ָ���Ĺ����ļ������б�Ҫ����ʾ����֮ǰ�Ĺ���
	 * @return void  
	 * @param fileName  
	 * @note  
	*/
	void openProject(const QString &fileName);
	/**   
	 * @brief  ��qgis�����ļ�
	 * @return bool  
	 * @param projectFile  
	 * @note  
	*/
	bool addProject(const QString& projectFile);
	//! ���dock widget
	void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget);
	//!Overloaded version of the private function with same name that takes the imagename as a parameter
	void saveMapAsImage(const QString&, QPixmap *);
	//! ���ֵ�һʵ��
	static qgis_dev *instance() { return sInstance; };

	//! ��ʼ���������
	void namSetup();

	//! ��������
	void masterPasswordSetup();

	//! �������ƻ�ȡ����ͼ��
	static QIcon getThemeIcon(const QString &theName);
	
	void setIconSizes(int size);

	//! ��ȡ��ǰmapcanvas
	QgsMapCanvas *mapCanvas() { return pMapCanvas; }

	//! ��ȡ��ǰ��Ϣչʾ��
	QgsMessageBar *messageBar() { return mInfoBar; }

	/** Open the message log dock widget **/
	void openMessageLog();

	//! ������Ϣ��ʾ����ʾ��ʱ��
	int messageTimeout();
	
    //! �����ʼ������ź�
    void completeInitialization();

	//! ��ʾ��ǰ�����������иı䣬��Ҫ����
	void markDirty();


public slots:
	/**   
	 * @brief  ���ʸ��ͼ��
	 * @date 2018/06/11
	 * @return void  
	 * @note  
	*/
	void addVectorLayers();

	/**   
	 * @brief  ���դ��ͼ��
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

	// ��ͼ����
	class Tools
	{
	public:
		Tools() = default;
		QgsMapTool *mZoomIn = nullptr;
		QgsMapTool *mZoomOut = nullptr;
		QgsMapTool *mPan = nullptr;
		QgsMapToolIdentify *mIdentify = nullptr;	
	} mMapTools;

	QgsMapCanvas *pMapCanvas = nullptr; //����
	QgsMapOverviewCanvas *mOverviewCanvas = nullptr;
	QStackedWidget *mStackedWidget;//����mapCanvas��mapComposor���л�
	QStackedWidget *mCentralContainer = nullptr;//
	QgsLayerTreeView *mLayerTreeView = nullptr;//ͼ�������
	QgsLayerTreeMapCanvasBridge *mLayerTreeCanvasBridge = nullptr; //����ͼ��������͵�ͼ����������
	QgsMessageBarItem *mLastMapToolMessage = nullptr;
	QgsMessageLogViewer *mLogViewer = nullptr;//��Ϣ��ͼ
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



	QLabel *mScaleLabel = nullptr; //״̬������ʾscale
	QgsScaleComboBox *mScaleEdit = nullptr; //��״̬������ʾ������
	QProgressBar *mProgressBar = nullptr;
	QLabel *mCoordsLabel = nullptr;//��״̬������ʾ Coordinate / Extent
	QLineEdit *mCoordsEdit = nullptr;//��״̬������ʾ��������

	QList<QgsMapCanvasLayer> mListMapCanvesLayers;//����������ͼ��ļ���

	QCursor *mOverviewMapCursor = nullptr;//ӥ��ͼ�����ָ��
	QRect *mMapWindow = nullptr; //��ǰ��ͼ���ڷ�Χ��ʵ�������ʾ

	QString mFullPathName; //��ǰ��ͼ�ļ�������·�������ѱ������أ�

	unsigned int mMousePrecisionDecimalPlaces;//ָ���������С����λ��
	QDateTime mProjectLastModified;
	
	QTimer *mDizzyTimer;//
	int mProjOpen = 0;

	
	


	QgsMessageBar *mInfoBar = nullptr;//��������ʽ����Ϣ��
	QWidget *mMacroshWarn = nullptr;
	QToolButton *mMessageButton;
	QgsUserInputDockWidget *mUserInputDockWidget = nullptr;


	//! ���ڻ�ȡimages��Դ�⵱�е�Icon·��
	static const QString activeThemePath();
	static const QString defaultThemePath();

	/************************************************************************/
	/* Private Member Function                                                                     */
	/************************************************************************/
	#pragma region private member function
	//! ��ʼ��ͼ�������
	void initLayerTreeView();
	//! ����״̬��
	void createStatusBar();
	//! ��ʼ����ͼ����
	void createCanvasTools();
	//! ������ӡ��ͼ��ͼ
	void createComposer();
	//! ֱ��ͼ����
	void histogramStretch(bool visibleAreaOnly = false, QgsRaster::ContrastEnhancementLimits theLimits = QgsRaster::ContrastEnhancementMinMax);
	//! �Աȶȵ�����updateBrightness�������Ƶ����Աȶ�ʱ�Ƿ���Ҫͬʱ��������
	void adjustBrightnessContrast(int delta, bool updateBrightness = true);
	//! ����դ��ͼ�����
	void enableRasterTools(bool val);
	//! ����ʸ��ͼ�����
	void enableVectorTools(bool val);
	//! �����Ի����û�ѡ��ͼ�㣬����true������سɹ���
	bool askUserForZipItemlayers(QString path);
	//! �����Ի��򣬼����û�ѡ���ͼ�㡣
	void askUserForGDALSublayers(QgsRasterLayer *layer);
	//! �÷���ȷ��ѡ���ͼ���Ƿ������ͼ�㡣 
	bool shouldAskUserForGDALSublayers(QgsRasterLayer *layer);
	//! �����Ի��򣬼����û�ѡ���ͼ�㡣
	void askUserForOGRSublayers(QgsVectorLayer *layer);
	//! ���դ��ͼ�㣬����ǿ��ˢ��
	bool addRasterLayer(QgsRasterLayer * theRasterLayer);
	#pragma  endregion private member function
};

#define QGIS_ICON_SIZE 24


#endif // QGIS_DEV_H__