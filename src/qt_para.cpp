/***************************************************************************
                          qt_para.cpp  -  The parameter window
                             -------------------
    begin                : Tue Jan 31 17:23:00 CET 2002
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

#include "qt_para.h"

/* constructor */
qt_para::qt_para(QWidget * parent, const char * name,
															short padd_sub, short pmul_div)
: QDialog(parent, name, FALSE, 0), add_sub(padd_sub), mul_div(pmul_div),
  pparent(parent), pname(name)
{
#ifdef DEBUG
	cout << "constructor qt_para()" << endl;
#endif

	setFont(QFont("helvetica", 12));

	/* add a layout as a base */
	layout1 = new QVBoxLayout(this);
	layout1->setSpacing(6);
	layout1->setMargin(0);

	/* the label above the number of ratios combo box */
	termLabelHBox = new QHBoxLayout(layout1);
	termLabel = new QLabel(i18n("Select number of ratios for the task"), this);
	termLabel->setFont(QFont("helvetica", 10));
	termLabelHBox->addWidget(termLabel);
	termLabelHBox->addStretch(10);

	/* insert a ComboBox where the user can select the number of ratios */
	termComHBox = new QHBoxLayout(layout1);
	termComBox = new QComboBox(this);
	termComBox->setFont(QFont("helvetica", 10));
	termComBox->insertItem("2", -1);
	termComBox->insertItem("3", -1);
	termComBox->insertItem("4", -1);
	termComBox->insertItem("5", -1);
	termComBox->setEditable(NO);
	termComHBox->addWidget(termComBox);
	termComHBox->addStretch(10);
	QObject::connect(termComBox, SIGNAL(activated(int)),
													this, SLOT(term_nr_changed(int)));
	
	/* the label above the max. main denominator combo box */
	denoLabelHBox = new QHBoxLayout(layout1);
	denoLabel = new QLabel(i18n("Select maximum size of the main denominator"),
																								this);
	denoLabel->setFont(QFont("helvetica", 10));
	denoLabelHBox->addWidget(denoLabel);
	denoLabelHBox->addStretch(10);

	/* insert a ComboBox where the user can select the max. size of the
	 * main denominator */
	denoComHBox = new QHBoxLayout(layout1);
	denoComBox = new QComboBox(this);
	denoComBox->setFont(QFont("helvetica", 10));
	setdenoComBox(0); /* calls a function which adds the items to the box */
	denoComBox->setEditable(NO);
	denoComHBox->addWidget(denoComBox);
	denoComHBox->addStretch(10);
	
	/* insert a horizontal grid to arange the buttons side by side */
	buttonHBox = new QHBoxLayout(layout1);

	/* ok button */
	okBtn = new QPushButton(i18n("&Ok"), this);
	okBtn->setFont(QFont("helvetica", 10));
	buttonHBox->addWidget(okBtn);
	QObject::connect(okBtn, SIGNAL(clicked()), this, SLOT(showQt_Task()));

	/* back button */
	backBtn= new QPushButton(i18n("&Cancel"), this);
	backBtn->setFont(QFont("helvetica", 10));
	buttonHBox->addWidget(backBtn);
	QObject::connect(backBtn, SIGNAL(clicked()), this, SLOT(close()));

	/* the next button should be on the right of the window */
	buttonHBox->addStretch(10);

	/* help button */
	helpBtn = new QPushButton(i18n("&Help"), this);
	helpBtn->setFont(QFont("helvetica", 10));
	buttonHBox->addWidget(helpBtn);

	/* show KBruch - selected operations */
	if (add_sub == YES && mul_div == NO)
		setCaption(i18n("set up task with add and sub"));
	if (add_sub == NO && mul_div == YES)
		setCaption(i18n("set up task with mul and div"));
	if (add_sub == YES && mul_div == YES)
		setCaption(i18n("set up task with all operations"));

	okBtn->setFocus();
}

/* destructor */
qt_para::~qt_para()
{
#ifdef DEBUG
	cout << "destructor qt_para()" << endl;
#endif

	/* no need to delete any child widgets, Qt does it by itself */
}

/* if the window is closed, we emit the ev_close event so other classes can
 * use a slot on it */
void qt_para::closeEvent(QCloseEvent * ev_close)
{
	ev_close->accept();
	emit closed();
}


/* ------ private member functions ------ */

/* sets the items of the denoComBox */
void qt_para::setdenoComBox(int index)
{
	denoComBox->clear(); /* delete the given items */

	/* if we have mul_div, we must ensure a minimum size for the main
	 * denominator */
	if (index <= 1 || mul_div == NO)
	{
		denoComBox->insertItem("10", -1);
		denoComBox->insertItem("15", -1);
	}
	if (index <= 2 || mul_div == NO)
	{
		denoComBox->insertItem("20", -1);
		denoComBox->insertItem("30", -1);
	}
	denoComBox->insertItem("40", -1);
	denoComBox->insertItem("50", -1);

	return;
}


/* ------ private slots ------ */

/* shows the main menu and deletes the parameter and task window */
void qt_para::closeQt_Task()
{
	delete task_window; /* delete the task window */
	close(); /* close the para window */

	return;
}

/* shows the parameter window */
void qt_para::showQt_Task()
{
	hide(); /* hides the para window */

	/* we build a new task window */
	task_window = new qt_task(pparent, pname, add_sub, mul_div,
		termComBox->currentItem() + 2, (denoComBox->currentText()).toUInt());

	/* we connect a slot to the close signal of the window, so we get to know
	 * when the other window was closed */
	QObject::connect(task_window, SIGNAL(closed()), this, SLOT(closeQt_Task()));
	task_window->show(); /* show the new window */
	task_window->set_position();

	return;
}

/* function is called if something was selected in the nr_ratios combo box */
void qt_para::term_nr_changed(int index)
{
#ifdef DEBUG
	cout << "qt_para::term_nr_changed(int index)" << endl;
#endif

	setdenoComBox(index);

	return;
}
