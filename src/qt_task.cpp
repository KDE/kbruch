/***************************************************************************
                          qt_task.cpp  -  The task window
                             -------------------
    begin                : Tue Feb 08 13:41:00 CET 2002
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

#include "qt_task.h"


/* ----- static members ----- */

/* the statistic window */
qt_stat * qt_task::stats = NULL;


/* ----- public member functions ----- */

/* constructor */
qt_task::qt_task(QWidget * parent, const char * name,	short padd_sub,
			short pmul_div, short pnr_ratios, unsigned int pmax_md):
	QDialog(parent, name, FALSE, 0), add_sub(padd_sub), mul_div(pmul_div),
				nr_ratios(pnr_ratios), max_md(pmax_md), isWaitResult(YES)
{
	ratio curr_ratio; /* to store a ratio of the created task */
	short tmp_counter; /* a counter dummy */
	QString tmp_string; /* use to convert things into strings */
	short operation; /* this is the unconverted number of an operation */

#ifdef DEBUG
	cout << "constructor qt_task()" << endl;
#endif

	setFont(QFont("helvetica", 12));

	/* create a new task */
	QApplication::setOverrideCursor(waitCursor); /* show the sand clock */
	current_task.create_task(max_md, nr_ratios, add_sub, mul_div);
	QApplication::restoreOverrideCursor(); /* show the normal cursor */

	/* add a layout as a base */
	layout1 = new QVBoxLayout(this);
	layout1->setSpacing(6);
	layout1->setMargin(0);

	/* create a grid to show the task */
	taskGrid = new QGridLayout(layout1, 3, 2 * nr_ratios + 9);

	/* add 2 spacers in front and in back of the task */
	spacer_item = new QSpacerItem(25, 1);
	taskGrid->addItem(spacer_item, 2, 0); /* in front */
	spacer_item = new QSpacerItem(25, 1);
	taskGrid->addItem(spacer_item, 2, 2 * nr_ratios + 8); /* at the end */

	/* add the lines within the ratios to the grid */
	for (tmp_counter = 0; tmp_counter < nr_ratios; tmp_counter++)
	{
		ratio_line = new QFrame(this);
		ratio_line->setGeometry(QRect(100, 100, 20, 20));
		ratio_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
		taskGrid->addWidget(ratio_line, 2, 2 * tmp_counter + 1);
	}

	/* add a line between the edit boxes */
	edit_line = new QFrame(this);
	edit_line->setGeometry(QRect(100, 100, 20, 20));
	edit_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	taskGrid->addWidget(edit_line, 2, 2 * nr_ratios + 2);

	/* add 3 labels with the text = */
	eq1_label = new QLabel(this);
	eq1_label->setText("=");
	taskGrid->addWidget(eq1_label, 2, 2 * nr_ratios + 1);

	eq2_label = new QLabel(this);
	eq2_label->setText("=");
	taskGrid->addWidget(eq2_label, 2, 2 * nr_ratios + 3);
	eq2_label->hide();

	eq3_label = new QLabel(this);
	eq3_label->setText("=");
	taskGrid->addWidget(eq3_label, 2, 2 * nr_ratios + 5);
	eq3_label->hide();

	/* add 2 input boxes so the user can enter numerator and denominator */
	numer_edit = new QLineEdit(this);
	taskGrid->addWidget(numer_edit, 1, 2 * nr_ratios + 2);
	QToolTip::add(numer_edit, i18n("Enter the numerator of your result"));
	deno_edit = new QLineEdit(this);
	taskGrid->addWidget(deno_edit, 3, 2 * nr_ratios + 2);
	QToolTip::add(deno_edit, i18n("Enter the denominator of your result"));

	/* add 2 * nr_ratios labels to show the numerators and denominators of
	 * the task */
	numer_lab_vek = new QLabel * [nr_ratios];
	deno_lab_vek = new QLabel * [nr_ratios];
	for (tmp_counter = 0; tmp_counter < nr_ratios; tmp_counter++)
	{
		/* get the current ratio */
		curr_ratio = current_task.get_ratio_n(tmp_counter);

		/* get the numerator of the current ratio and set it as text of a new
		 * label */
		numer_lab_vek[tmp_counter] = new QLabel(this);
		numer_lab_vek[tmp_counter]->setAlignment(Qt::AlignHCenter);
		tmp_string.setNum(curr_ratio.get_zaehler(), 10);
		numer_lab_vek[tmp_counter]->setText(tmp_string);

		/* add this label to the grid */
		taskGrid->addWidget(numer_lab_vek[tmp_counter], 1, 2 * tmp_counter + 1);

		/* get the denominator of the current ratio and set it as text of a new
		 * label */
		deno_lab_vek[tmp_counter] = new QLabel(this);
		deno_lab_vek[tmp_counter]->setAlignment(Qt::AlignHCenter);
		tmp_string.setNum(curr_ratio.get_nenner(), 10);
		deno_lab_vek[tmp_counter]->setText(tmp_string);

		/* add this label to the grid */
		taskGrid->addWidget(deno_lab_vek[tmp_counter], 3, 2 * tmp_counter + 1);
	}

	/* add nr_ratios - 1 labels to show the operations of the task */
	op_lab_vek = new QLabel * [nr_ratios - 1];
	for (tmp_counter = 0; tmp_counter < nr_ratios - 1; tmp_counter++)
	{
		/* create a new label */
		op_lab_vek[tmp_counter] = new QLabel(this);
		op_lab_vek[tmp_counter]->setAlignment(Qt::AlignHCenter);

		/* get the operation */
		operation = current_task.get_op_n(tmp_counter);

		/* we have to convert the operation into a string and set the label
		 * with the fitting sign */
		switch (operation)
		{
			case ADD : op_lab_vek[tmp_counter]->setText("+");
						  break;
			case SUB : op_lab_vek[tmp_counter]->setText("-");
						  break;
			case MUL : op_lab_vek[tmp_counter]->setText("*");
						  break;
			case DIV : op_lab_vek[tmp_counter]->setText("/");
						  break;
		}
		taskGrid->addWidget(op_lab_vek[tmp_counter], 2, 2 * tmp_counter + 2);
	}

	/* 2 labels to show the result reduced; seperated with a line */
	res_num1_label = new QLabel(this);
	taskGrid->addWidget(res_num1_label, 1, 2 * nr_ratios + 4);
	res_num1_label->hide();

	res_deno1_label = new QLabel(this);
	taskGrid->addWidget(res_deno1_label, 3, 2 * nr_ratios + 4);
	res_deno1_label->hide();

	res1_line = new QFrame(this);
	res1_line->setGeometry(QRect(100, 100, 20, 20));
	res1_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	taskGrid->addWidget(res1_line, 2, 2 * nr_ratios + 4);
	res1_line->hide();

	/* 3 labels to show the result as: 4 5/6; and a line */
	res_num2_label = new QLabel(this);
	taskGrid->addWidget(res_num2_label, 1, 2 * nr_ratios + 7);
	res_num2_label->hide();

	res_deno2_label = new QLabel(this);
	taskGrid->addWidget(res_deno2_label, 3, 2 * nr_ratios + 7);
	res_deno2_label->hide();

	res_common_label = new QLabel(this);
	taskGrid->addWidget(res_common_label, 2, 2 * nr_ratios + 6);
	res_common_label->hide();

	res2_line = new QFrame(this);
	res2_line->setGeometry(QRect(100, 100, 20, 20));
	res2_line->setFrameStyle(QFrame::HLine | QFrame::Sunken);
	taskGrid->addWidget(res2_line, 2, 2 * nr_ratios + 7);
	res2_line->hide();

	result_label = new QLabel(this);
	result_label->setText(i18n("WRONG"));
	taskGrid->addWidget(result_label, 2, 2 * nr_ratios + 8);
	result_label->hide();

	/* insert a horizontal box to arange the buttons side by side */
	buttonHBox = new QHBoxLayout(layout1);

	/* ok button */
	okBtn = new QPushButton(i18n("Ch&eck"), this);
	okBtn->setFont(QFont("helvetica", 10));
	buttonHBox->addWidget(okBtn);
	QToolTip::add(okBtn, i18n("Press the button to check your results"));
	okBtn->setDefault(TRUE); /* set it as the default button of the widget */
	QObject::connect(okBtn, SIGNAL(clicked()), this, SLOT(okBtnClicked()));

	/* back button */
	backBtn= new QPushButton(i18n("&Cancel"), this);
	backBtn->setFont(QFont("helvetica", 10));
	buttonHBox->addWidget(backBtn);
	QToolTip::add(backBtn,
							i18n("Press the button to get back to the main menu"));
	QObject::connect(backBtn, SIGNAL(clicked()), this, SLOT(close()));

	/* the next button should be on the right of the window */
	buttonHBox->addStretch(10);

	/* help button */
	helpBtn = new QPushButton(i18n("&Help"), this);
	helpBtn->setFont(QFont("helvetica", 10));
	buttonHBox->addWidget(helpBtn);
	QToolTip::add(helpBtn, i18n("Press the button to open the handbook"));
	QObject::connect(helpBtn, SIGNAL(clicked()), this, SLOT(slotShowBook()));

	/* show selected operations */
	if (add_sub == YES && mul_div == NO)
		setCaption(i18n("addition and subtraction task"));
	if (add_sub == NO && mul_div == YES)
		setCaption(i18n("multiplication and division task"));
	if (add_sub == YES && mul_div == YES)
		setCaption(i18n("task with all 4 operations"));

	/* show the statistic window; check if we have to create the window
	 * first, because it is the first call of qt_task */
	if (stats == NULL)
		stats = new qt_stat(parent, name);

	stats->show(); /* show the statistic window */
	numer_edit->setFocus();
}

/* destructor */
qt_task::~qt_task()
{
#ifdef DEBUG
	cout << "destructor qt_task()" << endl;
#endif

	stats->hide(); /* hide the statistic window */
	/* no need to delete any child widgets, Qt does it by itself */
}

/* align task and statistic window next to each other with the same top;
	we can not do this in the constructor */
void qt_task::set_position()
{
	stats->move(x() + frameGeometry().width() + int(abs((frameGeometry().width() - geometry().width())) / 2) , geometry().y());
	return;
}


/* ------ public slots ------ */

/* show the KBruch handbook section about the task window */
void qt_task::slotShowBook()
{
	kapp->invokeHelp("task-menu");
	return;
}


/* ------ private member functions ------ */


/* ------ private slots ------ */

/* if the window is closed, we emit the ev_close event so other classes can
 * use a slot on it */
void qt_task::closeEvent(QCloseEvent * ev_close)
{
	ev_close->accept();
	emit closed();
	return;
}


/* called, if the OK/Go on button was clicked */
void qt_task::okBtnClicked()
{
	QString tmp_str; /* to build a string for a label */
	QPalette pal;
	QColorGroup cg;
	int tmp_int; /* we calculate numbers before we convert them to a string */
	short tmp_counter; /* a counter dummy */
	ratio curr_ratio; /* to store a ratio of the created task */
	short operation; /* this is the unconverted number of an operation */

	/* check wether to validate the entered result or to generate a new task */
	if (isWaitResult == YES)
	{
		/* check the entered result of the given task */
		isWaitResult = NO;
		okBtn->setText(i18n("N&ext task"));
		QToolTip::add(okBtn, i18n("Press the button to go to the next task."));
		okBtn->setFocus();

		/* solve the task */
		QApplication::setOverrideCursor(waitCursor);
		result = current_task.solve();
		QApplication::restoreOverrideCursor();

		eq2_label->show(); /* show a = at the end */

		/* show the correct result as -16/7 */
		tmp_str.setNum(result.get_zaehler());
		res_num1_label->setText(tmp_str);
		res_num1_label->show();
		tmp_str.setNum(result.get_nenner());
		res_deno1_label->setText(tmp_str);
		res_deno1_label->show();
		res1_line->show();

		/* show the result as -2 2/7 */
		tmp_int = abs((abs(result.get_zaehler()) - abs(result.get_zaehler()
						% result.get_nenner())) / result.get_nenner());
		if ((result.get_zaehler() < 0 && result.get_nenner() > 0) ||
				(result.get_zaehler() > 0 && result.get_nenner() < 0))
			tmp_int *= -1;
		tmp_str.setNum(tmp_int);
		res_common_label->setText(tmp_str);

		/* we do not want 0 4/7 to be seen */
		if (tmp_int != 0)
		{
			res_common_label->show();
		}

		/* to not see a -4/7 for 2 times */
		if (res_common_label->isVisible() == TRUE)
		{
			eq3_label->show();
			tmp_int = abs(result.get_zaehler() % result.get_nenner());
			tmp_str.setNum(tmp_int);
			res_num2_label->setText(tmp_str);

			tmp_str.setNum(abs(result.get_nenner()));
			res_deno2_label->setText(tmp_str);

			/* we do not want 4 0/1 to be seen */
			if (abs(result.get_zaehler() % result.get_nenner()) != 0)
			{
				res_num2_label->show();
				res_deno2_label->show();
				res2_line->show();
			}
		}

		/* store the entered result to check it */
		entered_result.set_zaehler_ohne_k(numer_edit->text().toInt());
		entered_result.set_nenner_ohne_k(deno_edit->text().toInt());

		/* check the entered result; 0/1 == 0/5 -> true,
		 * but 0/1 == 0/0 -> false */
		if ((entered_result == result) ||
			(result.get_zaehler() == 0 && entered_result.get_zaehler() == 0
				&& entered_result.get_nenner() != 0))
		{
			/* yes, the user entered the correct result */
			result_label->setText(i18n("CORRECT"));
			pal = result_label->palette(); /* set green font color */
			cg = pal.active();
			cg.setColor(QColorGroup::Foreground, QColor(6, 179, 0));
			pal.setActive(cg);
			cg = pal.inactive();
			cg.setColor(QColorGroup::Foreground, QColor(6, 179, 0));
			pal.setInactive(cg);
			result_label->setPalette(pal);
			result_label->show(); /* show the result at the end of the task */

			/* change the statistics -> add correctly solved task */
			stats->addCorrect();

		} else {
			/* no, the user entered the wrong result */
			result_label->setText(i18n("WRONG"));
			pal = result_label->palette(); /* set red font color */
			cg = pal.active();
			cg.setColor(QColorGroup::Foreground, QColor(red));
			pal.setActive(cg);
			cg = pal.inactive();
			cg.setColor(QColorGroup::Foreground, QColor(red));
			pal.setInactive(cg);
			result_label->setPalette(pal);

			result_label->show(); /* show the result at the end of the task */

			/* maybe the entered ratio was not reduced */
			entered_result.reduce();
			if (entered_result == result)
				KMessageBox::information(this, 
									i18n("You entered the correct result, but not reduced.\nAlways enter your results as reduced. This task will be counted as not correctly solved."), i18n("KBruch"));
			/* change the statistics -> add wrong solved task */
			stats->addWrong();

		} /* if (entered_result == result) */

	} else {
		/* generate the next task and show it to the user */
		isWaitResult = YES;
		okBtn->setText(i18n("Ch&eck"));
		QToolTip::add(okBtn, i18n("Press the button to check your results"));
		result_label->hide(); /* do not show the result at the end of the task */
		res_num1_label->hide(); /* hide the result labels for the last task */
		res_deno1_label->hide();
		res1_line->hide();
		res_num2_label->hide();
		res_deno2_label->hide();
		res2_line->hide();
		res_common_label->hide();
		eq2_label->hide(); /* hide both = at the end */
		eq3_label->hide();

		/* clear user input */
		deno_edit->setText("");
		numer_edit->setText("");
		numer_edit->setFocus();

		/* create a new task */
		QApplication::setOverrideCursor(waitCursor); /* show the sand clock */
		current_task.create_task(max_md, nr_ratios, add_sub, mul_div);
		QApplication::restoreOverrideCursor(); /* show the normal cursor */

		/* show the new task;
		 * first show all the ratios */
		for (tmp_counter = 0; tmp_counter < nr_ratios; tmp_counter++)
		{
			/* get the current ratio */
			curr_ratio = current_task.get_ratio_n(tmp_counter);

			/* get the numerator of the current ratio and set it as text of a new
			 * label */
			tmp_str.setNum(curr_ratio.get_zaehler(), 10);
			numer_lab_vek[tmp_counter]->setText(tmp_str);

			/* get the denominator of the current ratio and set it as text of a
			 *  new label */
			tmp_str.setNum(curr_ratio.get_nenner(), 10);
			deno_lab_vek[tmp_counter]->setText(tmp_str);
		} /* for (tmp_counter = 0; tmp_counter < nr_ratios; tmp_counter++) */

		/* show all operations */
		for (tmp_counter = 0; tmp_counter < nr_ratios - 1; tmp_counter++)
		{
			/* get the operation */
			operation = current_task.get_op_n(tmp_counter);

			/* we have to convert the operation into a string and set the label
			 * with the fitting sign */
			switch (operation)
			{
				case ADD : op_lab_vek[tmp_counter]->setText("+");
							  break;
				case SUB : op_lab_vek[tmp_counter]->setText("-");
							  break;
				case MUL : op_lab_vek[tmp_counter]->setText("*");
							  break;
				case DIV : op_lab_vek[tmp_counter]->setText("/");
							  break;
			} /* switch (operation) */
		} /* for (tmp_counter = 0; tmp_counter < nr_ratios - 1; tmp_counter++) */
	} /* if (isWaitResult == YES) */

	return;
}
