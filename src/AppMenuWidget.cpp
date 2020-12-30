/*
    appmenuwidget.cpp  -  The main Qt/KDE window

    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "AppMenuWidget.h"

/* these includes are needed for KDE support */
#include <KLocalizedString>
#include <KActionCollection>
#include <KStandardAction>

/* these includes are needed for Qt support */
#include <QAction>
#include <QLayout>
#include <QPushButton>
#include <QStandardPaths>

#ifdef DEBUG
#include <QDebug>
#endif

#include "ui_taskcolorsbase.h"
#include "ui_taskfontsbase.h"
#include "settingsclass.h"

/* standard C++ library includes */
#include <cstdlib>

/* ----- public member functions ----- */

/* constructor */
AppMenuWidget::AppMenuWidget()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor AppMenuWidget");

#endif
    setupActions();

    QString css =
        QStringLiteral("QPushButton#m_Freestyle {"
        "border: none;"
        "image: url(") +
        QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kbruch/pics/icon_freestyle_1.png")) +
        QStringLiteral(");"
        "}"
        "QPushButton#m_Freestyle:hover {"
        "border: none;"
        "image: url(") +
        QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kbruch/pics/icon_freestyle.png")) +
        QStringLiteral(");"
        "}"
        "QPushButton#m_Learning {"
        "border: none;"
        "image: url(") +
        QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kbruch/pics/icon_learning_1.png")) +
        QStringLiteral(");"
        "}"
        "QPushButton#m_Learning:hover {"
        "border: none;"
        "image: url(") +
        QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kbruch/pics/icon_learning.png")) +
        QStringLiteral(");"
        "}"
        "QLabel#labelInfo, QLabel#labelFreestyle, QLabel#labelLearning"
        "{ font: bold 20px; }");

    setStyleSheet(css);

    QFont defaultFont = SettingsClass::taskFont();
    defaultFont.setBold(true);
    defaultFont.setPointSize(26);

    layout1 = new QHBoxLayout();
    layout1->setObjectName(QStringLiteral("layout1"));
    layout1->setAlignment(Qt::AlignCenter);

    baseWidget = new QWidget();
    baseWidget->setObjectName(QStringLiteral("baseWidget"));

    setCentralWidget(baseWidget);

    interfaceWidget = new QWidget(this);
    interfaceWidget->setObjectName(QStringLiteral("interfaceWidget"));
    interfaceWidget->setFixedSize(QSize(550, 350));

    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QStringLiteral("gridLayout"));

    labelInfo = new QLabel(this);
    labelInfo->setObjectName(QStringLiteral("labelInfo"));
    labelInfo->setText(i18n("KBruch modes:"));
    labelInfo->setFont(defaultFont);
    gridLayout->addWidget(labelInfo, 0, 0, 1, 2);

    // Freestyle mode ----------------------------
    m_Freestyle = new QPushButton(this);
    m_Freestyle->setObjectName(QStringLiteral("m_Freestyle"));
    m_Freestyle->setToolTip(i18n("Open standard KBruch"));
    m_Freestyle->setFixedSize(QSize(150, 197));
    gridLayout->addWidget(m_Freestyle, 1, 0, Qt::AlignCenter);

    labelFreestyle = new QLabel(this);
    labelFreestyle->setObjectName(QStringLiteral("labelFreestyle"));
    labelFreestyle->setText(i18n("Exercise"));
    labelFreestyle->setFont(defaultFont);
    gridLayout->addWidget(labelFreestyle, 2, 0, Qt::AlignCenter);

    QObject::connect(m_Freestyle, &QPushButton::clicked, this, &AppMenuWidget::slotFreestyleClicked);

    // Learning mode ----------------------------
    m_Learning = new QPushButton(this);
    m_Learning->setObjectName(QStringLiteral("m_Learning"));
    m_Learning->setToolTip(i18n("Open learning KBruch"));
    m_Learning->setFixedSize(QSize(150, 197));
    gridLayout->addWidget(m_Learning, 1, 1, Qt::AlignCenter);

    labelLearning = new QLabel(this);
    labelLearning->setObjectName(QStringLiteral("labelLearning"));
    labelLearning->setText(i18n("Learning"));
    labelLearning->setFont(defaultFont);
    gridLayout->addWidget(labelLearning, 2, 1, Qt::AlignCenter);

    QObject::connect(m_Learning, &QPushButton::clicked, this, &AppMenuWidget::slotLearningClicked);

    gridLayout->setRowMinimumHeight(0, 30);
    gridLayout->setRowMinimumHeight(1, 250);
    gridLayout->setRowMinimumHeight(2, 70);


    interfaceWidget->setLayout(gridLayout);
    layout1->addWidget(interfaceWidget);
    baseWidget->setLayout(layout1);

    move(50, 50);
    setFixedSize(QSize(742, 520));
}

/* destructor */
AppMenuWidget::~AppMenuWidget()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("destructor AppMenuWidget()");
#endif

    /* no need to delete any child widgets, Qt does it by itself */
}

void AppMenuWidget::setupActions()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("setupActions FractionRingWidget");
#endif

    // quit action
    KStandardAction::quit(this, SLOT(close()), actionCollection());

    resize(QSize(725, 330).expandedTo(minimumSizeHint()));
    setupGUI(Keys | Create, QStringLiteral("AppMenuWidgetui.rc"));
    setAutoSaveSettings();
}

void AppMenuWidget::slotApplySettings()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("slotApplySettings FractionRingWidget");
#endif
    return;
}

/* ------ private slots ------ */

void AppMenuWidget::slotFreestyleClicked()
{
    kbruchApp = new MainQtWidget();
    kbruchApp->show();
    hide();
    return;
}

void AppMenuWidget::slotLearningClicked()
{
    fractionRing = new FractionRingWidget();
    fractionRing->show();
    hide();
    return;
}
