/***************************************************************************
                          taskwidget.cpp  -  paint a task
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

#include "taskwidget.h"
#include "taskwidget.moc"

/* these includes are needed for KDE support */
#include <klocale.h>

/* these includes are needed for Qt support */
#include <qpainter.h>

TaskWidget::TaskWidget(QWidget * parent = 0, const char * name = 0,
													const task para_task = *new task()) :
			FractionBaseWidget(parent, name), m_task(para_task)
{
#ifdef DEBUG
	kdDebug() << "constructor TaskWidget" << endl;
#endif
}

TaskWidget::~TaskWidget()
{
#ifdef DEBUG
	kdDebug() << "destructor TaskWidget" << endl;
#endif
}

void TaskWidget::setTask(const task para_task)
{
	m_task = para_task;
	update();
}

void TaskWidget::paintEvent(QPaintEvent* /* p_paintEvent */)
{
	// our x position, we paint from left to right;
	// we don't want to start directly on the border, so add the margin
	int old_x = _MARGIN_X;

	// strings holding numerator, denominator and the operation sign
	QString str_numerator, str_denominator, str_operation;

	// operation sign as number
	short tmp_operation;

	// start the painter
	QPainter paint(this);

	// ratios and operation signs are painted with the same font
	paint.setFont(m_font);

	// set the pen for painting
	QPen pen(Qt::SolidLine);
	pen.setWidth(0);
	paint.setPen(pen);

	// get the font height; the font height doesn't change while painting
	QFontMetrics fm(paint.fontMetrics());

	// now we can correctly set the height of the widget
	setMinimumHeight(2 * fm.lineSpacing() + 10);
	setMaximumHeight(2 * fm.lineSpacing() + 10);

	// loop through all ratios and paint them
	for (unsigned short tmp_counter = 0; tmp_counter < m_task.getNumberOfRatios(); tmp_counter++)
	{
		// get the current ratio and paint it
		paintRatio(paint, m_task.get_ratio_n(tmp_counter), old_x, fm, false);

		// now check if we have another operation to show
		// if not we will stop showing ratios as well
		if (tmp_counter < m_task.getNumberOfOperations())
		{
			// get the operation sign
			tmp_operation = m_task.get_op_n(tmp_counter);

			// we have to convert the operation sign into a string
			switch (tmp_operation)
			{
				case ADD :
					str_operation = "+";
					break;
				case SUB :
					str_operation = "-";
					break;
				case MUL :
					str_operation = "x";
					break;
				case DIV :
					// there seems to be different division signs around the world
					// so please translate it to the right one for your country
					str_operation = i18n("division symbol", "/");
					break;
			} /* switch (operation) */

			// paint the operation
			paintMiddle(paint, str_operation, old_x, fm, m_colorOperation);

		} else {
			// no further operations to show, so we always show the = sign at the
			// end of a task
			paintMiddle(paint, "=", old_x, fm, m_colorOperation);

			break;
		} // if (tmp_counter < m_task.getNumberOfOperations())
	}

	// stop the painter
	paint.end();

	// the space we needed for painting is the minimum width of the widget
	setMinimumWidth(old_x);

	return;
}
