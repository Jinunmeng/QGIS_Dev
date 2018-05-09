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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qgis_devClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *qgis_devClass)
    {
        if (qgis_devClass->objectName().isEmpty())
            qgis_devClass->setObjectName(QStringLiteral("qgis_devClass"));
        qgis_devClass->resize(600, 400);
        menuBar = new QMenuBar(qgis_devClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        qgis_devClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(qgis_devClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        qgis_devClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(qgis_devClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        qgis_devClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(qgis_devClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        qgis_devClass->setStatusBar(statusBar);

        retranslateUi(qgis_devClass);

        QMetaObject::connectSlotsByName(qgis_devClass);
    } // setupUi

    void retranslateUi(QMainWindow *qgis_devClass)
    {
        qgis_devClass->setWindowTitle(QApplication::translate("qgis_devClass", "qgis_dev", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class qgis_devClass: public Ui_qgis_devClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGIS_DEV_H
