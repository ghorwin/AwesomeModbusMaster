/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionFileOpen;
    QAction *actionFileSave;
    QAction *actionFileSaveAs;
    QAction *actionFileQuit;
    QAction *actionFileNew;
    QAction *actionFileReload;
    QAction *actionFileRecentProjects;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayoutMain;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QRadioButton *radioButtonModbusTCP;
    QWidget *widgetInputTCP;
    QGridLayout *gridLayoutTCP;
    QLineEdit *lineEditPort;
    QLabel *label;
    QLineEdit *lineEditHostname;
    QLabel *label_2;
    QWidget *widgetInputRTU;
    QGridLayout *gridLayoutRTU;
    QComboBox *comboBoxBaudRate;
    QLabel *label_4;
    QLabel *label_6;
    QComboBox *comboBoxStopBits;
    QLineEdit *lineEditCOM;
    QLabel *label_3;
    QLabel *label_5;
    QComboBox *comboBoxParity;
    QRadioButton *radioButtonRTU;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButtonConnect;
    QLabel *labelConnectLED;
    QLabel *labelConnected;
    QSpacerItem *horizontalSpacer_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *tab_2;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menu_Help;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1454, 600);
        actionFileOpen = new QAction(MainWindow);
        actionFileOpen->setObjectName(QString::fromUtf8("actionFileOpen"));
        actionFileSave = new QAction(MainWindow);
        actionFileSave->setObjectName(QString::fromUtf8("actionFileSave"));
        actionFileSaveAs = new QAction(MainWindow);
        actionFileSaveAs->setObjectName(QString::fromUtf8("actionFileSaveAs"));
        actionFileQuit = new QAction(MainWindow);
        actionFileQuit->setObjectName(QString::fromUtf8("actionFileQuit"));
        actionFileNew = new QAction(MainWindow);
        actionFileNew->setObjectName(QString::fromUtf8("actionFileNew"));
        actionFileReload = new QAction(MainWindow);
        actionFileReload->setObjectName(QString::fromUtf8("actionFileReload"));
        actionFileRecentProjects = new QAction(MainWindow);
        actionFileRecentProjects->setObjectName(QString::fromUtf8("actionFileRecentProjects"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayoutMain = new QVBoxLayout(centralwidget);
        verticalLayoutMain->setObjectName(QString::fromUtf8("verticalLayoutMain"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setStyleSheet(QString::fromUtf8("font-size:8pt;font-weight:bold"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        radioButtonModbusTCP = new QRadioButton(groupBox);
        radioButtonModbusTCP->setObjectName(QString::fromUtf8("radioButtonModbusTCP"));
        radioButtonModbusTCP->setChecked(true);

        gridLayout_3->addWidget(radioButtonModbusTCP, 0, 0, 1, 1);

        widgetInputTCP = new QWidget(groupBox);
        widgetInputTCP->setObjectName(QString::fromUtf8("widgetInputTCP"));
        gridLayoutTCP = new QGridLayout(widgetInputTCP);
        gridLayoutTCP->setObjectName(QString::fromUtf8("gridLayoutTCP"));
        lineEditPort = new QLineEdit(widgetInputTCP);
        lineEditPort->setObjectName(QString::fromUtf8("lineEditPort"));
        lineEditPort->setMaximumSize(QSize(130, 16777215));

        gridLayoutTCP->addWidget(lineEditPort, 1, 1, 1, 1);

        label = new QLabel(widgetInputTCP);
        label->setObjectName(QString::fromUtf8("label"));
        label->setStyleSheet(QString::fromUtf8("font-size:8pt"));

        gridLayoutTCP->addWidget(label, 0, 0, 1, 1);

        lineEditHostname = new QLineEdit(widgetInputTCP);
        lineEditHostname->setObjectName(QString::fromUtf8("lineEditHostname"));

        gridLayoutTCP->addWidget(lineEditHostname, 1, 0, 1, 1);

        label_2 = new QLabel(widgetInputTCP);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setStyleSheet(QString::fromUtf8("font-size:8pt"));

        gridLayoutTCP->addWidget(label_2, 0, 1, 1, 1);


        gridLayout_3->addWidget(widgetInputTCP, 0, 1, 2, 1);

        widgetInputRTU = new QWidget(groupBox);
        widgetInputRTU->setObjectName(QString::fromUtf8("widgetInputRTU"));
        gridLayoutRTU = new QGridLayout(widgetInputRTU);
        gridLayoutRTU->setObjectName(QString::fromUtf8("gridLayoutRTU"));
        comboBoxBaudRate = new QComboBox(widgetInputRTU);
        comboBoxBaudRate->setObjectName(QString::fromUtf8("comboBoxBaudRate"));

        gridLayoutRTU->addWidget(comboBoxBaudRate, 1, 1, 1, 1);

        label_4 = new QLabel(widgetInputRTU);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setStyleSheet(QString::fromUtf8("font-size:8pt"));

        gridLayoutRTU->addWidget(label_4, 0, 0, 1, 1);

        label_6 = new QLabel(widgetInputRTU);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setStyleSheet(QString::fromUtf8("font-size:8pt"));

        gridLayoutRTU->addWidget(label_6, 0, 3, 1, 1);

        comboBoxStopBits = new QComboBox(widgetInputRTU);
        comboBoxStopBits->setObjectName(QString::fromUtf8("comboBoxStopBits"));

        gridLayoutRTU->addWidget(comboBoxStopBits, 1, 2, 1, 1);

        lineEditCOM = new QLineEdit(widgetInputRTU);
        lineEditCOM->setObjectName(QString::fromUtf8("lineEditCOM"));

        gridLayoutRTU->addWidget(lineEditCOM, 1, 0, 1, 1);

        label_3 = new QLabel(widgetInputRTU);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setStyleSheet(QString::fromUtf8("font-size:8pt"));

        gridLayoutRTU->addWidget(label_3, 0, 1, 1, 1);

        label_5 = new QLabel(widgetInputRTU);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setStyleSheet(QString::fromUtf8("font-size:8pt"));

        gridLayoutRTU->addWidget(label_5, 0, 2, 1, 1);

        comboBoxParity = new QComboBox(widgetInputRTU);
        comboBoxParity->setObjectName(QString::fromUtf8("comboBoxParity"));

        gridLayoutRTU->addWidget(comboBoxParity, 1, 3, 1, 1);


        gridLayout_3->addWidget(widgetInputRTU, 0, 2, 2, 1);

        radioButtonRTU = new QRadioButton(groupBox);
        radioButtonRTU->setObjectName(QString::fromUtf8("radioButtonRTU"));

        gridLayout_3->addWidget(radioButtonRTU, 1, 0, 1, 1);


        horizontalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer, 1, 1, 1, 1);

        pushButtonConnect = new QPushButton(groupBox_2);
        pushButtonConnect->setObjectName(QString::fromUtf8("pushButtonConnect"));

        gridLayout_4->addWidget(pushButtonConnect, 0, 0, 1, 1);

        labelConnectLED = new QLabel(groupBox_2);
        labelConnectLED->setObjectName(QString::fromUtf8("labelConnectLED"));

        gridLayout_4->addWidget(labelConnectLED, 0, 1, 1, 1);

        labelConnected = new QLabel(groupBox_2);
        labelConnected->setObjectName(QString::fromUtf8("labelConnected"));

        gridLayout_4->addWidget(labelConnected, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 3, 1, 1);


        horizontalLayout_2->addWidget(groupBox_2);


        verticalLayoutMain->addLayout(horizontalLayout_2);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        tabWidget->addTab(tab_2, QString());

        verticalLayoutMain->addWidget(tabWidget);

        verticalLayoutMain->setStretch(1, 1);
        MainWindow->setCentralWidget(centralwidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1454, 22));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QString::fromUtf8("menu_Help"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menu_File->addAction(actionFileNew);
        menu_File->addAction(actionFileOpen);
        menu_File->addAction(actionFileReload);
        menu_File->addAction(actionFileRecentProjects);
        menu_File->addSeparator();
        menu_File->addAction(actionFileSave);
        menu_File->addAction(actionFileSaveAs);
        menu_File->addSeparator();
        menu_File->addAction(actionFileQuit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionFileOpen->setText(QCoreApplication::translate("MainWindow", "&Load configuration...", nullptr));
#if QT_CONFIG(shortcut)
        actionFileOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFileSave->setText(QCoreApplication::translate("MainWindow", "&Save configuration", nullptr));
#if QT_CONFIG(shortcut)
        actionFileSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFileSaveAs->setText(QCoreApplication::translate("MainWindow", "Save configuration as...", nullptr));
#if QT_CONFIG(shortcut)
        actionFileSaveAs->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFileQuit->setText(QCoreApplication::translate("MainWindow", "&Quit", nullptr));
#if QT_CONFIG(shortcut)
        actionFileQuit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFileNew->setText(QCoreApplication::translate("MainWindow", "New configuration", nullptr));
        actionFileReload->setText(QCoreApplication::translate("MainWindow", "Reload configuration", nullptr));
        actionFileRecentProjects->setText(QCoreApplication::translate("MainWindow", "Recently used configurations", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "Connection settings", nullptr));
        radioButtonModbusTCP->setText(QCoreApplication::translate("MainWindow", "Modbus TCP", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Host address", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Port", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "COM port", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Parity", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Baud rate", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Stop bits", nullptr));
        radioButtonRTU->setText(QCoreApplication::translate("MainWindow", "Modbus RTU", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Connection status", nullptr));
        pushButtonConnect->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        labelConnectLED->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        labelConnected->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
        menu_File->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
        menu_Help->setTitle(QCoreApplication::translate("MainWindow", "&Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
