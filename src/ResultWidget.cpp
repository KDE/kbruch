/*
    RatioWidget.cpp  -  paint a ratio
    SPDX-FileCopyrightText: 2004 Sebastian Stein <seb.kde@hpfsc.de>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "ResultWidget.h"

/* these includes are needed for KDE support */
#include <KLocalizedString>

/* these includes are needed for Qt support */
#include <QLayout>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>

#ifdef DEBUG
#include <QDebug>
#endif

#include "settingsclass.h"

ResultWidget::ResultWidget(QWidget * parent, const uintList &para_factors) :
    FractionBaseWidget(parent), m_factors(para_factors)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor ResultWidget");
#endif
    Init();
    m_ExerciseView = 0;
}

ResultWidget::ResultWidget(QWidget * parent,
                           const Ratio &para_result) :
    FractionBaseWidget(parent), m_result(para_result)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor ResultWidget");
#endif
    Init();
    m_ExerciseView = 1;
}

ResultWidget::ResultWidget(QWidget * parent) :
    FractionBaseWidget(parent)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor ResultWidget");
#endif
    Init();
    m_ExerciseView = 2;
}

void ResultWidget::setFactors(const uintList &para_factors)
{
    m_factors = para_factors;
    update();
}

void ResultWidget::Init()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("ResultWidget::Init()");
#endif
    setMinimumWidth(160);
    setMaximumHeight(213);
    setMinimumHeight(213);

    m_answerMixed = true;

    m_kindView = -1;

    defaultFont = SettingsClass::taskFont();
    defaultFont.setBold(true);

    QPalette pal = palette();
    pal.setColor(QPalette::WindowText, Qt::white);
    setPalette(pal);

    m_primaryText = new QLabel(this);
    m_primaryText->setObjectName(QStringLiteral("primaryText"));
    m_primaryText->setPalette(pal);
    defaultFont.setPointSize(20);
    m_primaryText->setMinimumHeight(75);
    m_primaryText->setAlignment(Qt::AlignCenter);
    m_primaryText->setFont(defaultFont);
    m_primaryText->hide();

    m_secondaryText = new QLabel(i18n("Solution:"), this);
    m_secondaryText->setObjectName(QStringLiteral("secondaryText"));
    m_secondaryText->setPalette(pal);
    defaultFont.setPointSize(10);
    m_secondaryText->setAlignment(Qt::AlignCenter);
    m_secondaryText->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_secondaryText->setFont(defaultFont);
    m_secondaryText->hide();

    defaultFont.setPointSize(20);

    layout = new QGridLayout(this);
    layout->addWidget(m_primaryText, 0, 0);
    layout->addWidget(m_secondaryText, 1, 0);
    setLayout(layout);
}

ResultWidget::~ResultWidget()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("destructor ResultWidget");
#endif
}

int ResultWidget::KindView()
{
    return m_kindView;
}

void ResultWidget::showResult()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("ResultWidget::showResult");
#endif
    switch (m_kindView) {
    case 1:
        m_primaryText->show();
        m_secondaryText->hide();
        m_primaryText->setText(i18n("Correct!"));
        layout->setRowStretch(2, 0);
        layout->setRowStretch(0, 1);
        break;
    case 0:
        m_primaryText->show();
        m_secondaryText->show();
        m_primaryText->setText(i18n("Incorrect!"));
        layout->setRowStretch(2, 1);
        layout->setRowStretch(0, 0);
        break;
    case 2:
        m_primaryText->show();
        m_secondaryText->hide();
        m_primaryText->setText(i18n("Incorrect!"));
        layout->setRowStretch(2, 0);
        layout->setRowStretch(0, 1);
        break;
    default:
        setFixedSize(160, 213);
        m_primaryText->hide();
        m_secondaryText->hide();
    }
    update();
}

void ResultWidget::setResult(const Ratio &para_result, int k)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("ResultWidget::setResult");
    qDebug() << ("m_kindView = ") + k;
#endif
    m_kindView = k;
    m_result = para_result;

    showResult();
}

void ResultWidget::setAnswerMixed(bool value)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("ResultWidget::setAnswerMixed");
#endif
    m_answerMixed = value;
}

void ResultWidget::setResult(int k)
{
#ifdef DEBUG
    qDebug() << QStringLiteral("ResultWidget::setResult");
    qDebug() << QStringLiteral("m_kindView = ") + k;
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
    switch (m_kindView) {
    case 2:
        paint.setBrush(QColor(191, 0, 0));
        paint.drawRoundedRect(2.0, 2.0, 157.0, 209.0, 10, 10);
        break;
    case 1:
        paint.setBrush(QColor(0, 191, 0));
        paint.drawRoundedRect(2.0, 2.0, 157.0, 209.0, 10, 10);
        break;
    case 0:
        paint.setBrush(QColor(191, 0, 0));
        paint.drawRoundedRect(2.0, 2.0, width() - 3, 209.0, 10, 10);  // 2.0, 2.0, 157.0, 209.0, 10, 10
        paint.setBrush(QColor(255, 255, 255));
        paint.drawRoundedRect(8.0, 110.0, width() - 15, 93.0, 10, 10);  // 8.0, 110.0, 145.0, 93.0, 10, 10

        // ratios and operation signs are painted with the same font
        paint.setFont(defaultFont);

        // set the pen for painting
        QPen pen(Qt::SolidLine);
        pen.setWidth(0);
        paint.setPen(pen);

        // get the font height; the font height doesn't change while painting
        QFontMetrics fm(paint.fontMetrics());

        // m_ExerciseView != 1 only for exercise factorize!
        if (m_ExerciseView == 1) {
            if (SettingsClass::showSpecialRatioNotation() == true &&
                    qAbs(m_result.numerator()) >= qAbs(m_result.denominator()) &&
                    m_result.denominator() != 1 &&
                    m_answerMixed == true) {
                paintRatio(paint, m_result, old_x, old_y, fm, true, true, true);
            } else {
                paintRatio(paint, m_result, old_x, old_y, fm, false, true, true);
            }
        } else {
            // show solution of a factorization exercise
            old_y += 20;
            QString tmpStr;
            int fontHeight = fm.lineSpacing(); // get the font height

            int tmpWidth = 0;
            for (int tmpInt = 0; tmpInt < m_factors.count(); tmpInt++) {
                if (tmpInt != 0) {
                    tmpWidth += fm.boundingRect(QStringLiteral("*")).width();
                }
                tmpStr.setNum(m_factors[tmpInt]);
                fontWidth = fm.boundingRect(tmpStr).width();
                tmpWidth += fontWidth;
            }
            if (tmpWidth <= 100)
                old_x += 50 - tmpWidth / 2;

            for (int tmpInt = 0; tmpInt < m_factors.count(); tmpInt++) {
                if (tmpInt != 0) {
                    fontWidth = fm.boundingRect(QStringLiteral("*")).width();
                    paint.drawText(old_x, old_y, fontWidth, fontHeight, Qt::AlignCenter, QStringLiteral("*"));
                    old_x += fontWidth;
                }

                tmpStr.setNum(m_factors[tmpInt]);

                fontWidth = fm.boundingRect(tmpStr).width();
                paint.drawText(old_x, old_y, fontWidth, fontHeight, Qt::AlignCenter, tmpStr);
                old_x += fontWidth;
            }
        }
        old_x += 30;
        if (old_x > 160)
            setMinimumWidth(old_x);
        break;
    }
    // stop the painter
    paint.end();

    return;
}
