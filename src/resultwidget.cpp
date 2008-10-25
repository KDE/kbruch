/***************************************************************************
                          resultwidget.h  -  paint the result
                             -------------------
    begin                : 2004/05/30
    copyright            : (C) 2004 by Sebastian Stein
    email                : seb.kde@hpfsc.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "resultwidget.h"
#include "resultwidget.moc"

/* these includes are needed for Qt support */
#include <qpainter.h>
#include <qlabel.h>
#include <qlayout.h>

//Added by qt3to4:
#include <QPaintEvent>

#include "settingsclass.h"
#include <klocale.h>

ResultWidget::ResultWidget(QWidget * parent, const uintList para_factors) :
			FractionBaseWidget(parent), m_factors(para_factors)
{
#ifdef DEBUG
	kDebug() << "constructor ResultWidget";
#endif
	Init();
	m_ExerciseView = 0;
}

ResultWidget::ResultWidget(QWidget * parent = 0,
			const ratio para_result = ratio()) :
			FractionBaseWidget(parent), m_result(para_result)
{
#ifdef DEBUG
	kDebug() << "constructor ResultWidget";
#endif
	Init();
	m_ExerciseView = 1;	
}

ResultWidget::ResultWidget(QWidget * parent) :
			FractionBaseWidget(parent)
{
#ifdef DEBUG
	kDebug() << "constructor ResultWidget";
#endif
	Init();
	m_ExerciseView = 2;
}

void ResultWidget::setFactors(const uintList para_factors)
{
	m_factors = para_factors;
	update();
}

void ResultWidget::Init()
{
#ifdef DEBUG
	kDebug() << "ResultWidget::Init()";
#endif
	setMinimumWidth( 160 ); 
	setMaximumHeight( 213 ); 
	setMinimumHeight( 213 ); 
	
	m_kindView = -1;

	defaultFont = SettingsClass::taskFont();
	defaultFont.setBold( TRUE );
	
	QPalette pal = palette();
	pal.setColor(QPalette::Foreground, Qt::white);
	setPalette(pal);

	m_primaryText = new QLabel("",this);
	m_primaryText->setObjectName("primaryText");
	m_primaryText->setPalette(pal);
	defaultFont.setPointSize(20);
	m_primaryText->setMinimumHeight(75);
	m_primaryText->setAlignment(Qt::AlignCenter);
	m_primaryText->setFont(defaultFont);
	m_primaryText->hide();

	m_secondaryText = new QLabel("Solution:",this);
	m_secondaryText->setObjectName("secondaryText");
	m_secondaryText->setPalette(pal);
	defaultFont.setPointSize(10);
	m_secondaryText->setAlignment(Qt::AlignCenter);
	m_secondaryText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	m_secondaryText->setFont(defaultFont);
	m_secondaryText->hide();

	defaultFont.setPointSize(20);

	layout = new QGridLayout(this);
	layout->addWidget(m_primaryText,0,0);
	layout->addWidget(m_secondaryText,1,0);
	setLayout(layout);
}

ResultWidget::~ResultWidget()
{
#ifdef DEBUG
	kDebug() << "destructor ResultWidget";
#endif
}

int ResultWidget::KindView()
{
	return m_kindView;
}

void ResultWidget::showResult()
{
#ifdef DEBUG
	kDebug() << "ResultWidget::showResult";
#endif
	switch (m_kindView)
	{
		case 1:
			m_primaryText->show();
			m_secondaryText->hide();
			m_primaryText->setText(i18n("Correct!"));
			layout->setRowStretch(2,0);
			layout->setRowStretch(0,1);
			break;
		case 0:
			m_primaryText->show();
			m_secondaryText->show();
			m_primaryText->setText(i18n("Incorrect!"));
			layout->setRowStretch(2,1);
			layout->setRowStretch(0,0);
			break;
		case 2:
			m_primaryText->show();
			m_secondaryText->hide();
			m_primaryText->setText(i18n("Incorrect!"));
			layout->setRowStretch(2,0);
			layout->setRowStretch(0,1);
			break;			
		default:
			setFixedSize( 160, 213 );
			m_primaryText->hide();
			m_secondaryText->hide();
	}
	update();
}

void ResultWidget::setResult(const ratio para_result, int k)
{
#ifdef DEBUG
	kDebug() << "ResultWidget::setResult";
	kDebug() << "m_kindView = " + k;
#endif
	m_kindView = k;
	m_result = para_result;

	showResult();
}

void ResultWidget::setResult(int k)
{
#ifdef DEBUG
	kDebug() << "ResultWidget::setResult";
	kDebug() << "m_kindView = " + k;
#endif
	m_kindView = k;
	showResult();
}

void ResultWidget::paintEvent(QPaintEvent* /* p_paintEvent */)
{
	// our x position, we paint from left to right;
	// we don't want to start directly on the border, so add the margin
	int old_x = 30;
	int old_y = 123;
	int fontWidth; // to store the width of the last thing painted
	
	// start the painter
	QPainter paint(this);

	paint.setRenderHint(QPainter::Antialiasing);
	paint.setPen(QPen(Qt::NoPen));
	switch (m_kindView)
	{
		case 2:
			paint.setBrush( QColor(191, 0, 0) );
			paint.drawRoundedRect(2.0, 2.0, 157.0, 209.0, 10, 10);
			break;	
		case 1:
			paint.setBrush( QColor(0, 191, 0) );
			paint.drawRoundedRect(2.0, 2.0, 157.0, 209.0, 10, 10);
			break;				
		case 0:
			paint.setBrush( QColor(191,0,0) );
			paint.drawRoundedRect(2.0, 2.0, width()-3, 209.0, 10, 10); // 2.0, 2.0, 157.0, 209.0, 10, 10
			paint.setBrush( QColor(255, 255, 255) );
			paint.drawRoundedRect(8.0, 110.0, width()-15, 93.0, 10, 10); // 8.0, 110.0, 145.0, 93.0, 10, 10

			// ratios and operation signs are painted with the same font
			paint.setFont(defaultFont);

			// set the pen for painting
			QPen pen(Qt::SolidLine);
			pen.setWidth(0);
			paint.setPen(pen);

			// get the font height; the font height doesn't change while painting
			QFontMetrics fm(paint.fontMetrics());

			if (m_ExerciseView == 1) {

				if (SettingsClass::showSpecialRatioNotation() == true && qAbs(m_result.numerator()) >= qAbs(m_result.denominator()) && m_result.denominator() != 1)
				{
					paintRatio(paint, m_result, old_x, old_y, fm, false, true, false);
					paintMiddle(paint, QString(" or "), old_x, old_y, fm, m_colorOperation);
					paintRatio(paint, m_result, old_x, old_y, fm, true);
				} else
					paintRatio(paint, m_result, old_x, old_y, fm, false, true, true);
			} else {
				old_y+= 20;
				QString tmpStr;
				int fontHeight = fm.lineSpacing(); // get the font height

				int tmpWidth = 0;
				int tmpHeight = 0;
				for (int tmpInt = 0; tmpInt < m_factors.count(); tmpInt++)
				{
					if (tmpInt != 0)
					{
						tmpWidth += fm.width("*");
					}
					tmpStr.setNum(m_factors[tmpInt]);					
					fontWidth = fm.width(tmpStr);
					tmpWidth += fontWidth;					
				}
				if (tmpWidth <= 100) 
					old_x+= 50 - tmpWidth/2;
					
				for (int tmpInt = 0; tmpInt < m_factors.count(); tmpInt++)
				{
					if (tmpInt != 0)
					{
						fontWidth = fm.width("*");
						paint.drawText(old_x, old_y, fontWidth, fontHeight, Qt::AlignCenter, "*");
						old_x += fontWidth;
					}

					tmpStr.setNum(m_factors[tmpInt]);

					fontWidth = fm.width(tmpStr);
					paint.drawText(old_x, old_y, fontWidth, fontHeight, Qt::AlignCenter, tmpStr);
					old_x += fontWidth;
				}
			}					
			old_x+=30;
			if (  old_x > 160 )
				setMinimumWidth(old_x);						
			break;		
	}
	// stop the painter
	paint.end();

	return;
}
