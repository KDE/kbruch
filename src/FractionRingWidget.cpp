/***************************************************************************
                              fractionringwidget.cpp
                             -------------------
    begin                : 2008/09/16
    copyright            : (C) 2008 by Danilo Balzaque
    email                : danilo.balzaque@ltia.fc.unesp.br

    copyright            : (C) 2008 by Tadeu Araujo, tadeu.araujo@ltia.fc.unesp.br
    copyright            : (C) 2008 by Tiago Porangaba, tiago.porangaba@ltia.fc.unesp.br
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "FractionRingWidget.h"
#include "FractionRingWidget.moc"

/* these includes are needed for KDE support */
#include <klocale.h>
#include <KTextEdit>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <kconfigdialog.h>
#include <KAction>
#include <KStandardShortcut>
#include <KToolBar>

/* these includes are needed for Qt support */
#include <qapplication.h>

//Added by qt3to4:
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QRectF>
#include <QString>
#include <QPushButton>

#include "ratio.h"
#include "FractionPainter.h"
#include "ui_taskcolorsbase.h"
#include "ui_taskfontsbase.h"
#include "settingsclass.h"

#define MARG_LEFT 10
#define MARG_TOP 70

class TaskColors : public QWidget, public Ui::TaskColorsBase
{
	public:
		TaskColors( QWidget * parent ) : QWidget( parent )
		{
			setupUi(this);
		}
};

/* constructor */
FractionRingWidget::FractionRingWidget()
{
#ifdef DEBUG
	kDebug() << "constructor FractionRingWidget()";
#endif
	// creating KActions, used by the kbruchui.rc file
	setupActions();
	createGUI("FractionRingWidgetui.rc");

	layout1 = new QGridLayout();
	layout1->setObjectName( "layout1" );

	baseWidget = new QWidget();
	baseWidget->setObjectName( "baseWidget" );

	setCentralWidget(baseWidget);

	interfaceWidget = new QWidget(this);
	interfaceWidget->setObjectName( "interfaceWidget" );
	interfaceWidget->setFixedSize(QSize(270, 300));

	gridLayout = new QGridLayout();
	gridLayout->setObjectName( "gridLayout" );

	// setup colors ---------------------------------------
	bgOutsideRing = QColor(242, 242, 242);
	bgOutsideMold = QColor(51, 51, 153);
	bgInsideRing = QColor(242, 242, 242);
	bgInsideMold = QColor(184, 18, 0);
	bgColor = QColor(240, 240, 250);

	colorListLeft[0] = QColor(0, 122, 209);
	colorListLeft[1] = QColor(70, 156, 207);
	colorListLeft[2] = QColor(121, 205, 241);
	colorListLeft[3] = QColor(174, 231, 255);
	colorListLeft[4] = QColor(117, 241, 255);

	colorListRight[0] = QColor(186, 18, 24);
	colorListRight[1] = QColor(217, 74, 81);
	colorListRight[2] = QColor(255, 114, 122);
	colorListRight[3] = QColor(230, 147, 152);
	colorListRight[4] = QColor(255, 186, 190);

	// 742, 520
	setFixedSize(QSize(742, 520));

	setPalette(QPalette(bgColor));
        setAutoFillBackground(true);

	move(50, 50);

	fractionWidget = new FractionPainter(this);
	fractionWidget->setObjectName( "fractionWidget" );

	gridLayout->addWidget(fractionWidget, 0, 0, 1, 2, Qt::AlignBottom | Qt::AlignHCenter);

	// Info Labels -------------------------------------
	leftInfoLabel = new QLabel(this);
	leftInfoLabel->setObjectName( "leftInfoLabel" );
	leftInfoLabel->setText(i18n("Multiply"));
	gridLayout->addWidget(leftInfoLabel, 1, 0, Qt::AlignCenter);

	rightInfoLabel = new QLabel(this);
	rightInfoLabel->setObjectName( "rightInfoLabel" );
	rightInfoLabel->setText(i18n("Multiply"));
	gridLayout->addWidget(rightInfoLabel, 1, 1, Qt::AlignCenter);

	// SpinBox -----------------------------------------
	leftSpinBox = new QSpinBox(this);
	leftSpinBox->setObjectName( "leftSpinBox" );
	leftSpinBox->setRange(1, 6);
	leftSpinBox->setMaximumWidth(50);
	leftSpinBox->setMinimumWidth(50);
	gridLayout->addWidget(leftSpinBox, 2, 0, Qt::AlignTop | Qt::AlignHCenter);

	rightSpinBox = new QSpinBox(this);
	rightSpinBox->setObjectName( "rightSpinBox" );
	rightSpinBox->setRange(1, 6);
	rightSpinBox->setMaximumWidth(50);
	rightSpinBox->setMinimumWidth(50);
	gridLayout->addWidget(rightSpinBox, 2, 1, Qt::AlignTop | Qt::AlignHCenter);

	QObject::connect(leftSpinBox, SIGNAL(valueChanged(int)), this,
		SLOT(slotLeftSpinBoxValueChanged(int)));
	QObject::connect(rightSpinBox, SIGNAL(valueChanged(int)), this, 
		SLOT(slotRightSpinBoxValueChanged(int)));

	// Reset Button ------------------------------------
	resetButton = new QPushButton(this);
	resetButton->setObjectName( "resetButton" );
	resetButton->setText(i18n("New"));
	resetButton->setMaximumWidth(70);
	gridLayout->addWidget(resetButton, 3, 0, 1, 2, Qt::AlignCenter);

	QObject::connect(resetButton, SIGNAL(clicked()), this,
		SLOT(NewTask()));

	// grid row settings --------------------------------
	gridLayout->setRowMinimumHeight(0, 185);
	gridLayout->setRowMinimumHeight(1, 10);
	gridLayout->setRowMinimumHeight(2, 15);
	gridLayout->setRowMinimumHeight(3, 85);

	interfaceWidget->setLayout(gridLayout);
	layout1->addWidget(interfaceWidget, 1, 0, Qt::AlignTop | Qt::AlignHCenter);

	// textedit -----------------------------------------
	textMsg = new KTextEdit(this);
	textMsg->setObjectName("textMsg");
	textMsg->setFixedSize(QSize(250 - MARG_LEFT, 300));
	textMsg->setReadOnly(true);
	textMsg->setPalette(QPalette( QColor(255,255,255)));
  	textMsg->setAutoFillBackground(true);
	layout1->addWidget(textMsg, 0, 1, 2, 1, Qt::AlignCenter);

	// grid row settings --------------------------------
	layout1->setRowMinimumHeight(0, MARG_TOP - 20);
	layout1->setRowMinimumHeight(1, 370 - MARG_TOP);

	// grid column settings -----------------------------
	layout1->setColumnMinimumWidth(0, 460 + MARG_LEFT);
	layout1->setColumnMinimumWidth(1, 282 - MARG_LEFT);

	baseWidget->setLayout(layout1);

	// Ratio ----------------------------------------------
	resetFraction(true);
}

/* destructor */
FractionRingWidget::~FractionRingWidget()
{
#ifdef DEBUG
	kDebug() << "destructor StatisticsBarWidget()";
#endif
}

void FractionRingWidget::setupActions()
{
#ifdef DEBUG
	kDebug() << "setupActions FractionRingWidget";
#endif
	// new task action
   	m_NewTaskAction  = new KAction(KIcon("document-new"), i18nc("@action opens a new question", "&New"), this);
   	actionCollection()->addAction("NewTask", m_NewTaskAction );
	connect(m_NewTaskAction, SIGNAL(triggered(bool) ), SLOT(NewTask()));
	m_NewTaskAction->setShortcut(KStandardShortcut::shortcut(KStandardShortcut::New));

	// back action
   	m_BackAction  = new KAction(KIcon("go-previous"), i18nc("@action go to the main screen", "Back"), this);
   	actionCollection()->addAction("Back", m_BackAction );
	connect(m_BackAction, SIGNAL(triggered(bool) ), SLOT(GoBack()));

	// hint action (hide it as it doesn't exist here)
   	m_HintAction  = new KAction(KIcon("games-hint"), i18nc("@action opens hint", "Hint"), this);
   	actionCollection()->addAction("Hint", m_HintAction );
	connect(m_HintAction, SIGNAL(triggered(bool) ), SLOT(Hint()));

	// quit action
	KStandardAction::quit(kapp, SLOT(quit()), actionCollection());

	KStandardAction::preferences(this,  SLOT( slotPrefs() ), actionCollection());

	if (!initialGeometrySet())
		resize(QSize(725, 330).expandedTo(minimumSizeHint()));
	setupGUI(ToolBar | Keys | StatusBar | Create);
	setAutoSaveSettings();
}

void FractionRingWidget::slotPrefs()
{
#ifdef DEBUG
	kDebug() << "slotPrefs FractionRingWidget";
#endif
	// do not show dialog twice
	if (KConfigDialog::showDialog("settings"))
		return;

	//KConfigDialog didn't find an instance of this dialog, so lets create it :
	KConfigDialog* configDialog = new KConfigDialog( this, "settings", SettingsClass::self() );

	TaskColors * taskColors = new TaskColors(0);
	configDialog->addPage(taskColors, i18n("Colors"), "preferences-desktop-color");

	// User edited the configuration - update your local copies of the
	// configuration data
	connect(configDialog, SIGNAL(settingsChanged( const QString &)), this, SLOT(slotApplySettings()) );
        configDialog->setHelp("kbruch/index.html");
	configDialog->show();

	return;
}

void FractionRingWidget::slotApplySettings()
{
#ifdef DEBUG
	kDebug() << "slotApplySettings FractionRingWidget";
#endif
	fractionWidget->update();
	return;
}

void FractionRingWidget::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QPen penBorder = QPen(Qt::white);
	penBorder.setWidth(2);

	painter.setPen(penBorder);
	painter.setBrush(bgOutsideRing);

	QRectF rect(MARG_LEFT, MARG_TOP, 440, 440);

	int startAngle = 90 * 16;
	int spanAngle = 0;

	// Outside Ring -------------------------------
	painter.drawEllipse(rect);

	painter.setPen(Qt::NoPen);

	int div = 255 / (rLeft.denominator() + 1);
	for (int i = 0; i < rLeft.numerator(); i++)
	{
		painter.setBrush(colorListLeft[i]);
		spanAngle = ((16 * 360) << 8) / rLeft.denominator();
		painter.drawPie(rect, startAngle + ((i * spanAngle) >> 8), (spanAngle >> 8));
	}

	painter.setPen(penBorder);
	painter.setBrush(Qt::transparent);
	for (int i = 0; i < rLeft.denominator() * multLeft; i++)
	{
		spanAngle = ((16 * 360) << 8) / (rLeft.denominator() * multLeft);
		painter.drawPie(rect, startAngle + ((i * spanAngle) >> 8), (spanAngle >> 8));
	}

	// Spacer Ring -------------------------------
	painter.setPen(penBorder);
	painter.setBrush(bgColor);
	rect.setRect(MARG_LEFT + 27, MARG_TOP + 27, 386, 386);
	painter.drawEllipse(rect);

	// Inside Ring -------------------------------
	painter.setPen(penBorder);
	painter.setBrush(bgInsideRing);
	rect.setRect(MARG_LEFT + 33, MARG_TOP + 33, 374, 374);
	painter.drawEllipse(rect);

	painter.setPen(penBorder);

	div = 255 / (rRight.denominator() + 1);
	for (int i = 0; i < rRight.numerator(); i++)
	{
		painter.setBrush(colorListRight[i]);
		spanAngle = ((16 * 360) << 8) / rRight.denominator();
		painter.drawPie(rect, startAngle + ((i * spanAngle) >> 8), (spanAngle >> 8));
	}

	painter.setPen(penBorder);
	painter.setBrush(Qt::transparent);
	for (int i = 0; i < rRight.denominator() * multRight; i++)
	{
		spanAngle = ((16 * 360) << 8) / ((rRight.denominator() * multRight));
		painter.drawPie(rect, startAngle + ((i * spanAngle) >> 8), (spanAngle >> 8));
	}

	// Inside Area -------------------------------
	painter.setPen(penBorder);
	painter.setBrush(QColor(230, 230, 240));
	rect.setRect(MARG_LEFT + 60, MARG_TOP + 60, 320, 320);
	painter.drawEllipse(rect);

	// left mold ---------------------------------
	QPen pen = QPen(bgOutsideMold);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.setBrush(QColor(205, 225, 240, 150));
	rect.setRect(MARG_LEFT + 118, MARG_TOP + 120, 80, 160);
	painter.drawRect(rect);

	// right mold --------------------------------
	pen.setColor(bgInsideMold);
	painter.setPen(pen);
	painter.setBrush(QColor(240, 210, 160));
	rect.setRect(MARG_LEFT + 252, MARG_TOP + 120, 80, 160);
	painter.drawRect(rect);
}

void FractionRingWidget::GoBack()
{
	kbruchApp = new AppMenuWidget();
	kbruchApp->show();
	close();
}

void FractionRingWidget::Hint()
{
	textMsg->setVisible(!textMsg->isVisible());
}

void FractionRingWidget::slotLeftSpinBoxValueChanged(int value)
{
	multLeft = value;

	QString msg;
	if (checkTask())
	{
		msg = i18n("Now the parts of the inside ring and outside ring have "
		           "the same size, and we can add the fractions!\n\n"
		           "This is called finding the main denominator. "
		           "The main denominator divides the rings into equal parts.");
		if (rLeft.denominator() * value != MCM(rLeft.denominator(), rRight.denominator()))
		{
			msg += QString("\n\n");
			msg += i18n("There is a smaller main denominator. Can you find it?");
		}

		textMsg->setText(msg);
	}
	else
	{
		msg = i18n("You have just multiplied the first fraction by %1. "
		           "Our main goal is to make all parts be the same size. "
		           "Try multiplying the first fraction by %2 and the second fraction by %3.",
		           value, rRight.denominator(), rLeft.denominator());
		textMsg->setText(msg);
	}

	update();
	fractionWidget->paintFraction("+", rLeft, multLeft, rRight, multRight);
}

void FractionRingWidget::slotRightSpinBoxValueChanged(int value)
{
	multRight = value;

	QString msg;
	if (checkTask())
	{
		msg = i18n("Now the parts of the inside ring and outside ring have "
		           "the same size, and we can add the fractions!\n\n"
		           "This is called finding the main denominator. "
		           "The main denominator divides the rings into equal parts.");
		if (rRight.denominator() * value != MCM(rLeft.denominator(), rRight.denominator()))
		{
			msg += QString("\n\n");
			msg += i18n("There is a smaller main denominator. Can you find it?");
		}

		textMsg->setText(msg);
	}
	else
	{
		msg = i18n("You have just multiplied the second fraction by %1. "
		           "Our main goal is to make all parts be the same size. "
		           "Try multiplying the first fraction by %2 and the second fraction by %3.",
		           value, rRight.denominator(), rLeft.denominator());
		textMsg->setText(msg);
	}

	update();
	fractionWidget->paintFraction("+", rLeft, multLeft, rRight, multRight);
}

void FractionRingWidget::NewTask()
{
	resetFraction(true);
}

void FractionRingWidget::resetFraction(bool flag = true)
{
	int denominator = int((double(rand()) / RAND_MAX) * 5) + 2;
	int numerator = int((double(rand()) / RAND_MAX) * (denominator - 1)) + 1;

	rLeft = ratio(numerator, denominator, false);

	int denominator2 = int((double(rand()) / RAND_MAX) * 5) + 2;
	while (denominator2 == denominator)
	{
		denominator2 = int((double(rand()) / RAND_MAX) * 5) + 2;
	}
	int numerator2 = int((double(rand()) / RAND_MAX) * (denominator2 - 1)) + 1;
	rRight = ratio(numerator2, denominator2, false);

	multLeft = 1;
	multRight = 1;
	leftSpinBox->setValue(1);
	rightSpinBox->setValue(1);

	// NOTE: Inserting denominator as %1, as first number is used to determine plural form,
	// and in these messages that should be the number next to "...painted parts".
	QString insert1 = i18nc("Inserted as %1 in the message below.",
	                         "%2 of %1 painted parts.", rLeft.denominator(), rLeft.numerator());
	QString insert2 = i18nc("Inserted as %2 in the message below.",
	                         "%2 of %1 painted parts.", rRight.denominator(), rRight.numerator());
	QString msg = i18nc("%1 and %2 are the two messages translated above.",
	                    "The outside ring represents the left fraction. %1\n\n"
	                    "The inside ring represents the right fraction. %2\n\n"
	                    "The 'Multiply' spinboxes multiplies each fraction. Try to change one of the spinbox values!",
	                    insert1, insert2);

	textMsg->setText(msg);

	if (flag)
	{
		update();
		fractionWidget->paintFraction("+", rLeft, multLeft, rRight, multRight);
	}
}

int FractionRingWidget::MCM(int a, int b)
{
	return a * b / GCF(a, b);
}

int FractionRingWidget::GCF(int a, int b)
{
	int tmp;
	if (a < b)
	{
		tmp = b;
		b = a;
		a = tmp;
	}
	tmp = a % b;
	while (tmp != 0)
	{
		a = b;
		b = tmp;
		tmp = a % b;
	}
	return b;
}

bool FractionRingWidget::checkTask()
{
	return (rLeft.denominator() * multLeft == rRight.denominator() * multRight);
}
