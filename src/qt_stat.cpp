/***************************************************************************
                          qt_stat.cpp  -  the statistic window
                             -------------------
    begin                : Tue Mar 08 17:20:00 CET 2002
    copyright            : (C) 2001 - 2002 by Sebastian Stein
    email                : bruch@hpfsc.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "qt_stat.h"

/* constructor */
qt_stat::qt_stat(QWidget * parent, const char * name):
	QDialog(parent, name, FALSE, 0)
{
	QPalette pal;
	QColorGroup cg;
#ifdef DEBUG
	cout << "constructor qt_stat()" << endl;
#endif

	/* reset the number of solved tasks */
	count = correct = 0;

	/* set a standard font for all members */
	setFont(QFont("helvetica", 12));

	/* set the caption of the window */
	setCaption(i18n("Statistics"));

	/* add a layout as a base */
	layout1 = new QVBoxLayout(this);
	layout1->setSpacing(6);
	layout1->setMargin(0);

	/* create a grid to show the labels */
	labelGrid = new QGridLayout(layout1, 3, 2);

	/* add 6 labels to the grid */
	info1Label = new QLabel(this);
	info1Label->setText(i18n("tasks so far:"));
	labelGrid->addWidget(info1Label, 1, 0);

	result1Label = new QLabel(this);
	labelGrid->addWidget(result1Label, 1, 1);
	QToolTip::add(result1Label,
				i18n("This is the current total number of solved tasks."));

	info2Label = new QLabel(this);
	info2Label->setText(i18n("correct:"));
	labelGrid->addWidget(info2Label, 2, 0);

	result2Label = new QLabel(this);

	/* set green text color for this label */
	pal = result2Label->palette();
	cg = pal.active();
	cg.setColor(QColorGroup::Foreground, QColor(6, 179, 0));
	pal.setActive(cg);
	cg = pal.inactive();
	cg.setColor(QColorGroup::Foreground, QColor(6, 179, 0));
	pal.setInactive(cg);
	result2Label->setPalette(pal);

	labelGrid->addWidget(result2Label, 2, 1);
	QToolTip::add(result2Label,
				i18n("This is the current total number of correctly solved tasks."));

	info3Label = new QLabel(this);
	info3Label->setText(i18n("incorrect:"));
	labelGrid->addWidget(info3Label, 3, 0);

	result3Label = new QLabel(this);

	/* set red text color for this label */
	pal = result3Label->palette();
	cg = pal.active();
	cg.setColor(QColorGroup::Foreground, QColor(red));
	pal.setActive(cg);
	cg = pal.inactive();
	cg.setColor(QColorGroup::Foreground, QColor(red));
	pal.setInactive(cg);
	result3Label->setPalette(pal);

	labelGrid->addWidget(result3Label, 3, 1);
	QToolTip::add(result3Label,
				i18n("This is the current total number of unsolved tasks."));

	/* the Reset button */
	resetBtn = new QPushButton(i18n("&Reset"), this);
	QObject::connect(resetBtn, SIGNAL(clicked()), this, SLOT(reset_stat()));
	layout1->addWidget(resetBtn);
	QToolTip::add(resetBtn, i18n("Press the button to reset the statistics."));

	/* calculate the statistics */
	(void) calc();
}

/* destructor */
qt_stat::~qt_stat()
{
#ifdef DEBUG
	cout << "destructor qt_stat()" << endl;
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

/* called, if a task solved correctly */
void qt_stat::addCorrect()
{
	++count;
	++correct;
	(void) calc(); /* repaint the statistics */

	return;
}

/* called, if a task was solved wrong */
void qt_stat::addWrong()
{
	++count;
	(void) calc(); /* repaint the statistics */

	return;
}


/* ------ private member functions ------ */

/* recalculates the statistics and changes the corresponding labels */
void qt_stat::calc()
{
	QString new_text;
	QString number;

	new_text.sprintf("<b>%d</b>", count);
	result1Label->setText(new_text);

	/* we have to be careful with division by 0 */
	if (count == 0)
	{
		result2Label->setText("- (-%)");
		result3Label->setText("- (-%)");
	} else {
		/* set the correct label */
		new_text.sprintf("%d (%d %%)", correct,
												int(double(correct) / count * 100));
		result2Label->setText(new_text);

		/* set the incorrect label */
		new_text.sprintf("%d (%d %%)", count - correct,
											int(double(count - correct) / count * 100));
		result3Label->setText(new_text);
	}
	return;
}

/* ------ private slots ------ */

/* called by the reset button */
void qt_stat::reset_stat()
{
	count = 0;
	correct = 0;
	(void) calc();
	return;
}
