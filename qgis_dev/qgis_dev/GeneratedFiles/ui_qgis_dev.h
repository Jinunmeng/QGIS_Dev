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
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

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
        actionadd_raster = new QAction(qgis_devClass);
        actionadd_raster->setObjectName(QStringLiteral("actionadd_raster"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/themes/default/mActionAddRasterLayer.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionadd_raster->setIcon(icon1);
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

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu_2->addAction(actionadd_vector);
        menu_3->addAction(actionadd_raster);

        retranslateUi(qgis_devClass);

        QMetaObject::connectSlotsByName(qgis_devClass);
    } // setupUi

    void retranslateUi(QMainWindow *qgis_devClass)
    {
        qgis_devClass->setWindowTitle(QApplication::translate("qgis_devClass", "qgis_dev", Q_NULLPTR));
        actionadd_vector->setText(QApplication::translate("qgis_devClass", "add vector", Q_NULLPTR));
        actionadd_raster->setText(QApplication::translate("qgis_devClass", "add raster", Q_NULLPTR));
        menu->setTitle(QApplication::translate("qgis_devClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("qgis_devClass", "\347\237\242\351\207\217", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("qgis_devClass", "\346\240\205\346\240\274", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class qgis_devClass: public Ui_qgis_devClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGIS_DEV_H
