//
/***************************************************************************
                          appmenuwidget.h  -  The main Qt/KDE window
                             -------------------
    begin                : Tue Sep 16 00:00:00 CET 2008
    copyright            : (C) 2008 by LTIA
    email                : danilo.balzaque@ltia.fc.unesp.br
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "AppMenuWidget.h"
#include "AppMenuWidget.moc"

/* these includes are needed for KDE support */
#include <kdebug.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kicon.h>
#include <kapplication.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <kconfigdialog.h>
#include <KAction>
#include <KStandardShortcut>
#include <KToolBar>

/* these includes are needed for Qt support */
#include <qapplication.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <KStandardDirs>

#include "ui_taskgeneralbase.h"
#include "ui_taskcolorsbase.h"
#include "ui_taskfontsbase.h"
#include "settingsclass.h"

//Added by qt3to4:
#include <QHBoxLayout>

/* standard C++ library includes */
#include <stdlib.h>

class TaskGeneral : public QWidget, public Ui::TaskGeneralBase
{
	public:
		TaskGeneral( QWidget * parent ) : QWidget( parent )
		{
			setupUi(this);
		}
};

class TaskColors : public QWidget, public Ui::TaskColorsBase
{
	public:
		TaskColors( QWidget * parent ) : QWidget( parent )
		{
			setupUi(this);
		}
};

class TaskFonts : public QWidget, public Ui::TaskFontsBase
{
	public:
		TaskFonts( QWidget * parent ) : QWidget( parent )
		{
			setupUi(this);
		}
};

/* ----- public member functions ----- */

/* constructor */
AppMenuWidget::AppMenuWidget()
{
#ifdef DEBUG
	kDebug() << "constructor AppMenuWidget";
#endif
	setupActions();
	createGUI("AppMenuWidgetui.rc");

	QString css;
	css += "QPushButton#m_Freestyle {";
	css +=		"border: none;";
	css +=		"image: url(";
	css +=		KStandardDirs::locate("data", "kbruch/pics/icon_freestyle_1.png");
	css +=		");";
	css +=	"}";		
	css += "QPushButton#m_Freestyle:hover {";
	css +=		"border: none;";
	css +=		"image: url(";
	css +=		KStandardDirs::locate("data", "kbruch/pics/icon_freestyle.png");
	css +=		");";
	css +=	"}";
	css += "QPushButton#m_Learning {";
	css +=		"border: none;";
	css +=		"image: url(";
	css +=		KStandardDirs::locate("data", "kbruch/pics/icon_learning_1.png");
	css +=		");";
	css +=	"}";
	css += "QPushButton#m_Learning:hover {";
	css +=		"border: none;";
	css +=		"image: url(";
	css +=		KStandardDirs::locate("data", "kbruch/pics/icon_learning.png");
	css +=		");";
	css +=	"}";
	css +=	"QLabel#labelInfo, QLabel#labelFreestyle, QLabel#labelLearning";
	css +=	"{ font: bold 20px; }";

	setStyleSheet(css);

	layout1 = new QHBoxLayout();
	layout1->setObjectName( "layout1" );
	layout1->setAlignment(Qt::AlignCenter);

	baseWidget = new QWidget();
	baseWidget->setObjectName( "baseWidget" );

	setCentralWidget(baseWidget);

	interfaceWidget = new QWidget(this);
	interfaceWidget->setObjectName( "interfaceWidget" );
	interfaceWidget->setFixedSize(QSize(550, 350));

	gridLayout = new QGridLayout();
	gridLayout->setObjectName( "gridLayout" );

	labelInfo = new QLabel(this);
	labelInfo->setObjectName( "labelInfo" );
	labelInfo->setText(i18n("KBruch modes:"));
	gridLayout->addWidget(labelInfo, 0, 0, 1, 2);

	// Freestyle mode ----------------------------
	m_Freestyle = new QPushButton(this);
	m_Freestyle->setObjectName( "m_Freestyle" );
	//m_Freestyle->setToolTip(i18n("Open standard kbruch."));
	m_Freestyle->setFixedSize(QSize(150, 197));
	gridLayout->addWidget(m_Freestyle, 1, 0, Qt::AlignCenter);

	labelFreestyle = new QLabel(this);
	labelFreestyle->setObjectName( "labelFreestyle" );
	labelFreestyle->setText(i18n("Freestyle"));
	gridLayout->addWidget(labelFreestyle, 2, 0, Qt::AlignCenter);

	QObject::connect(m_Freestyle, SIGNAL(clicked()), this, SLOT(slotFreestyleClicked()));

	// Learning mode ----------------------------
	m_Learning = new QPushButton(this);
	m_Learning->setObjectName( "m_Learning" );
	//m_Learning->setToolTip(i18n("Open learning kbruch."));
	m_Learning->setFixedSize(QSize(150, 197));
	gridLayout->addWidget(m_Learning, 1, 1, Qt::AlignCenter);

	labelLearning = new QLabel(this);
	labelLearning->setObjectName( "labelLearning" );
	labelLearning->setText(i18n("Learning"));
	gridLayout->addWidget(labelLearning, 2, 1, Qt::AlignCenter);

	QObject::connect(m_Learning, SIGNAL(clicked()), this, SLOT(slotLearningClicked()));

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
	kDebug() << "destructor AppMenuWidget()";
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

void AppMenuWidget::setupActions()
{
#ifdef DEBUG
	kDebug() << "setupActions FractionRingWidget";
#endif

	// quit action
	KStandardAction::quit(kapp, SLOT(quit()), actionCollection());

	KStandardAction::preferences(this,  SLOT( slotPrefs() ), actionCollection());

	if (!initialGeometrySet())
		resize(QSize(725, 330).expandedTo(minimumSizeHint()));
	setupGUI(Keys | Create);
	setAutoSaveSettings();
}

void AppMenuWidget::slotPrefs()
{
#ifdef DEBUG
	kDebug() << "slotPrefs FractionRingWidget";
#endif
	// do not show dialog twice
	if (KConfigDialog::showDialog("settings"))
		return;

	//KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog* configDialog = new KConfigDialog( this, "settings", SettingsClass::self() );

	TaskGeneral * taskGeneral = new TaskGeneral(0);
	configDialog->addPage(taskGeneral, i18n("General"), "preferences-desktop-other");

	TaskColors * taskColors = new TaskColors(0);
	configDialog->addPage(taskColors, i18n("Colors"), "preferences-desktop-color");

	TaskFonts * taskFonts = new TaskFonts(0);
	configDialog->addPage(taskFonts, i18n("Fonts"), "preferences-desktop-font");
	// User edited the configuration - update your local copies of the
	// configuration data
	connect(configDialog, SIGNAL(settingsChanged( const QString &)), this, SLOT(slotApplySettings()) );
        configDialog->setHelp("kbruch/index.html");
	configDialog->show();

	return;
}

void AppMenuWidget::slotApplySettings()
{
#ifdef DEBUG
	kDebug() << "slotApplySettings FractionRingWidget";
#endif
	return;
}

/* ------ private slots ------ */

void AppMenuWidget::slotFreestyleClicked()
{
	kbruchApp = new MainQtWidget();
	kbruchApp->show();
	close();
	return;
}

void AppMenuWidget::slotLearningClicked()
{
	fractionRing = new FractionRingWidget();
	fractionRing->show();
	close();
	return;
}