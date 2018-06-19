/********************************************************************************
** Form generated from reading UI file 'qgis_dev.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGIS_DEV_H
#define UI_QGIS_DEV_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qgis_devClass
{
public:
    QAction *actionadd_vector;
    QAction *actionadd_raster;
    QAction *actionNew_Project;
    QAction *actionOpen_Project;
    QAction *actionSave_Project;
    QAction *actionExit;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *qgis_devClass)
    {
        if (qgis_devClass->objectName().isEmpty())
            qgis_devClass->setObjectName(QStringLiteral("qgis_devClass"));
        qgis_devClass->resize(954, 668);
        actionadd_vector = new QAction(qgis_devClass);
        actionadd_vector->setObjectName(QStringLiteral("actionadd_vector"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/themes/default/mActionNewVectorLayer.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionadd_vector->setIcon(icon);
        actionadd_vector->setIconVisibleInMenu(true);
        actionadd_raster = new QAction(qgis_devClass);
        actionadd_raster->setObjectName(QStringLiteral("actionadd_raster"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/themes/default/mActionAddRasterLayer.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionadd_raster->setIcon(icon1);
        actionNew_Project = new QAction(qgis_devClass);
        actionNew_Project->setObjectName(QStringLiteral("actionNew_Project"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/themes/default/mActionFileNew.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Project->setIcon(icon2);
        actionOpen_Project = new QAction(qgis_devClass);
        actionOpen_Project->setObjectName(QStringLiteral("actionOpen_Project"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/themes/default/mActionFileOpen.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Project->setIcon(icon3);
        actionSave_Project = new QAction(qgis_devClass);
        actionSave_Project->setObjectName(QStringLiteral("actionSave_Project"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/images/themes/default/mActionFileSave.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Project->setIcon(icon4);
        actionExit = new QAction(qgis_devClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/images/themes_old/nkids/mActionFileExit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon5);
        centralWidget = new QWidget(qgis_devClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        qgis_devClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(qgis_devClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 954, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        qgis_devClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(qgis_devClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        qgis_devClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(qgis_devClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        qgis_devClass->setStatusBar(statusBar);
        toolBar = new QToolBar(qgis_devClass);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        qgis_devClass->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(actionNew_Project);
        menu->addAction(actionOpen_Project);
        menu->addAction(actionSave_Project);
        menu->addSeparator();
        menu->addAction(actionExit);
        menu_2->addAction(actionadd_vector);
        menu_3->addAction(actionadd_raster);
        mainToolBar->addAction(actionNew_Project);
        mainToolBar->addAction(actionOpen_Project);
        mainToolBar->addAction(actionSave_Project);
        mainToolBar->addAction(actionadd_vector);
        mainToolBar->addAction(actionadd_raster);

        retranslateUi(qgis_devClass);

        QMetaObject::connectSlotsByName(qgis_devClass);
    } // setupUi

    void retranslateUi(QMainWindow *qgis_devClass)
    {
        qgis_devClass->setWindowTitle(QApplication::translate("qgis_devClass", "qgis_dev", Q_NULLPTR));
        actionadd_vector->setText(QApplication::translate("qgis_devClass", "add vector", Q_NULLPTR));
        actionadd_raster->setText(QApplication::translate("qgis_devClass", "add raster", Q_NULLPTR));
        actionNew_Project->setText(QApplication::translate("qgis_devClass", "New Project", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionNew_Project->setShortcut(QApplication::translate("qgis_devClass", "Ctrl+N", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionOpen_Project->setText(QApplication::translate("qgis_devClass", "Open Project", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionOpen_Project->setShortcut(QApplication::translate("qgis_devClass", "Ctrl+O", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionSave_Project->setText(QApplication::translate("qgis_devClass", "Save Project", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionSave_Project->setShortcut(QApplication::translate("qgis_devClass", "Ctrl+S", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionExit->setText(QApplication::translate("qgis_devClass", "Exit", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionExit->setShortcut(QApplication::translate("qgis_devClass", "Ctrl+Q", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        menu->setTitle(QApplication::translate("qgis_devClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("qgis_devClass", "\347\237\242\351\207\217", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("qgis_devClass", "\346\240\205\346\240\274", Q_NULLPTR));
        toolBar->setWindowTitle(QApplication::translate("qgis_devClass", "toolBar", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class qgis_devClass: public Ui_qgis_devClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGIS_DEV_H
