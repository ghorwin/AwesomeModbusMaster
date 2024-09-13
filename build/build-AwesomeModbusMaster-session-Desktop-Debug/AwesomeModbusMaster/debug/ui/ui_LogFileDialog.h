/********************************************************************************
** Form generated from reading UI file 'LogFileDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGFILEDIALOG_H
#define UI_LOGFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include "LogWidget.h"

QT_BEGIN_NAMESPACE

class Ui_LogFileDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *labelOpenFileError;
    LogWidget *logWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *LogFileDialog)
    {
        if (LogFileDialog->objectName().isEmpty())
            LogFileDialog->setObjectName(QString::fromUtf8("LogFileDialog"));
        LogFileDialog->resize(480, 383);
        verticalLayout = new QVBoxLayout(LogFileDialog);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelOpenFileError = new QLabel(LogFileDialog);
        labelOpenFileError->setObjectName(QString::fromUtf8("labelOpenFileError"));
        labelOpenFileError->setWordWrap(true);

        verticalLayout->addWidget(labelOpenFileError);

        logWidget = new LogWidget(LogFileDialog);
        logWidget->setObjectName(QString::fromUtf8("logWidget"));

        verticalLayout->addWidget(logWidget);

        buttonBox = new QDialogButtonBox(LogFileDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        verticalLayout->setStretch(1, 1);

        retranslateUi(LogFileDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), LogFileDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), LogFileDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(LogFileDialog);
    } // setupUi

    void retranslateUi(QDialog *LogFileDialog)
    {
        LogFileDialog->setWindowTitle(QCoreApplication::translate("LogFileDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LogFileDialog: public Ui_LogFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGFILEDIALOG_H
