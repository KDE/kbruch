/*
    TaskWidget.cpp  -  paint a task
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "TaskWidget.h"

/* these includes are needed for KDE support */
#include <KLocalizedString>

/* these includes are needed for Qt support */
#include <QPainter>
#include <QPaintEvent>

#ifdef DEBUG
#include <QDebug>
#endif

TaskWidget::TaskWidget(QWidget * parent,
                       const Task &para_task) :
    FractionBaseWidget(parent), m_task(para_task)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor TaskWidget");
#endif
}

TaskWidget::~TaskWidget()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("destructor TaskWidget");
#endif
}

void TaskWidget::setTask(const Task &para_task)
{
    m_task = para_task;
    update();
}

void TaskWidget::setQuestionMixed(bool value)
{
    m_questionMixed = value;
    update();
}

void TaskWidget::paintEvent(QPaintEvent* /* p_paintEvent */)
{
    // our x position, we paint from left to right;
    // we don't want to start directly on the border, so add the margin
    int old_x = _MARGIN_X;
    int old_y = 0;

    // strings holding numerator, denominator and the operation sign
    QString str_operation;

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
    QFontMetrics  fm(paint.fontMetrics());

    // now we can correctly set the height of the widget
    setMinimumHeight(2 * fm.lineSpacing() + 10);
    setMaximumHeight(2 * fm.lineSpacing() + 10);

    // loop through all ratios and paint them
    for (unsigned short tmp_counter = 0; tmp_counter < m_task.getNumberOfRatios(); tmp_counter++) {
        // get the current ratio and paint it
        paintRatio(paint, m_task.get_ratio_n(tmp_counter), old_x, old_y, fm, m_questionMixed);

        // now check if we have another operation to show
        // if not we will stop showing ratios as well
        if (tmp_counter < m_task.getNumberOfOperations()) {
            // get the operation sign
            tmp_operation = m_task.get_op_n(tmp_counter);

            // we have to convert the operation sign into a string
            switch (tmp_operation) {
            case ADD :
                str_operation = i18nc("addition symbol", "+");
                break;
            case SUB :
                str_operation = i18nc("subtraction symbol", "−");
                break;
            case MUL :
                str_operation = i18nc("multiplication symbol", "×");
                break;
            case DIV :
                str_operation = i18nc("division symbol", "/");
                break;
            } /* switch (operation) */

            // add proper spacing around the operator
            str_operation = QStringLiteral(" ") + str_operation + QStringLiteral(" ");

            // paint the operation
            paintMiddle(paint, str_operation, old_x, old_y, fm, m_colorOperation);

        } else {
            // no further operations to show, so we always show the = sign at the
            // end of a task
            paintMiddle(paint, QStringLiteral(" = "), old_x, old_y, fm, m_colorOperation);

            break;
        } // if (tmp_counter < m_task.getNumberOfOperations())
    }

    // stop the painter
    paint.end();

    // the space we needed for painting is the minimum width of the widget
    setMinimumWidth(old_x);

    return;
}
