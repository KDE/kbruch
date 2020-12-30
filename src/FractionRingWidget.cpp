/*
    SPDX-FileCopyrightText: 2008 Danilo Balzaque <danilo.balzaque@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tadeu Araujo <tadeu.araujo@ltia.fc.unesp.br>
    SPDX-FileCopyrightText: 2008 Tiago Porangaba <tiago.porangaba@ltia.fc.unesp.br>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "FractionRingWidget.h"

/* these includes are needed for KDE support */
#include <KActionCollection>
#include <KConfigDialog>
#include <KLocalizedString>
#include <KStandardAction>
#include <KStandardShortcut>

/* these includes are needed for Qt support */
#include <QAction>
#include <QApplication>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QRectF>
#include <QSpinBox>
#include <QString>
#include <QTextEdit>
#include <QWidgetAction>

#ifdef DEBUG
#include <QDebug>
#endif

#include "ui_taskfontsbase.h"
#include "settingsclass.h"
#include "TaskColors.h"

#define MARG_LEFT 10
#define MARG_TOP 70

/* constructor */
FractionRingWidget::FractionRingWidget()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("constructor FractionRingWidget()");
#endif
    // creating KActions, used by the FractionRingWidgetui.rc file
    setupActions();
    createGUI(QStringLiteral("FractionRingWidgetui.rc"));

    setCaption(i18n("Learning"));

    layout1 = new QGridLayout();
    layout1->setObjectName(QStringLiteral("layout1"));

    baseWidget = new QWidget();
    baseWidget->setObjectName(QStringLiteral("baseWidget"));

    setCentralWidget(baseWidget);

    interfaceWidget = new QWidget(this);
    interfaceWidget->setObjectName(QStringLiteral("interfaceWidget"));
    interfaceWidget->setFixedSize(QSize(270, 300));

    gridLayout = new QGridLayout();
    gridLayout->setObjectName(QStringLiteral("gridLayout"));

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
    fractionWidget->setObjectName(QStringLiteral("fractionWidget"));

    gridLayout->addWidget(fractionWidget, 0, 0, 1, 2, Qt::AlignBottom | Qt::AlignHCenter);

    // Info Labels -------------------------------------
    leftInfoLabel = new QLabel(this);
    leftInfoLabel->setObjectName(QStringLiteral("leftInfoLabel"));
    leftInfoLabel->setText(i18nc("Expands the ratio shown on the left by the given factor. Please note, expanding is not the same as multiplying. For example, expanding a ratio by 2 basically means multiplying the ratio with 2/2 and not with 2/1!", "Expand"));
    gridLayout->addWidget(leftInfoLabel, 1, 0, Qt::AlignCenter);

    rightInfoLabel = new QLabel(this);
    rightInfoLabel->setObjectName(QStringLiteral("rightInfoLabel"));
    rightInfoLabel->setText(i18nc("Expands the ratio shown on the left by the given factor. Please note, expanding is not the same as multiplying. For example, expanding a ratio by 2 basically means multiplying the ratio with 2/2 and not with 2/1!", "Expand"));
    gridLayout->addWidget(rightInfoLabel, 1, 1, Qt::AlignCenter);

    // SpinBox -----------------------------------------
    leftSpinBox = new QSpinBox(this);
    leftSpinBox->setObjectName(QStringLiteral("leftSpinBox"));
    leftSpinBox->setRange(1, 6);
    leftSpinBox->setMaximumWidth(50);
    leftSpinBox->setMinimumWidth(50);
    gridLayout->addWidget(leftSpinBox, 2, 0, Qt::AlignTop | Qt::AlignHCenter);

    rightSpinBox = new QSpinBox(this);
    rightSpinBox->setObjectName(QStringLiteral("rightSpinBox"));
    rightSpinBox->setRange(1, 6);
    rightSpinBox->setMaximumWidth(50);
    rightSpinBox->setMinimumWidth(50);
    gridLayout->addWidget(rightSpinBox, 2, 1, Qt::AlignTop | Qt::AlignHCenter);

    QObject::connect(leftSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &FractionRingWidget::slotLeftSpinBoxValueChanged);
    QObject::connect(rightSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &FractionRingWidget::slotRightSpinBoxValueChanged);

    // Reset Button ------------------------------------
    resetButton = new QPushButton(this);
    resetButton->setObjectName(QStringLiteral("resetButton"));
    resetButton->setText(i18n("New"));
    resetButton->setMaximumWidth(70);
    gridLayout->addWidget(resetButton, 3, 0, 1, 2, Qt::AlignCenter);

    QObject::connect(resetButton, &QPushButton::clicked, this, &FractionRingWidget::NewTask);

    // grid row settings --------------------------------
    gridLayout->setRowMinimumHeight(0, 185);
    gridLayout->setRowMinimumHeight(1, 10);
    gridLayout->setRowMinimumHeight(2, 15);
    gridLayout->setRowMinimumHeight(3, 85);

    interfaceWidget->setLayout(gridLayout);
    layout1->addWidget(interfaceWidget, 1, 0, Qt::AlignTop | Qt::AlignHCenter);

    // textedit -----------------------------------------
    textMsg = new QTextEdit(this);
    textMsg->setObjectName(QStringLiteral("textMsg"));
    textMsg->setFixedSize(QSize(250 - MARG_LEFT, 300));
    textMsg->setReadOnly(true);
    textMsg->setPalette(QPalette(QColor(255, 255, 255)));
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
    qDebug() << QStringLiteral("destructor StatisticsBarWidget()");
#endif
}

void FractionRingWidget::setupActions()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("setupActions FractionRingWidget");
#endif
    // new task action
    m_NewTaskAction = new QWidgetAction(this);
    m_NewTaskAction->setText(i18nc("@action opens a new question", "&New"));
    m_NewTaskAction->setIcon(QIcon::fromTheme(QStringLiteral("document-new")));
    actionCollection()->addAction(QStringLiteral("NewTask"), m_NewTaskAction);
    actionCollection()->setDefaultShortcuts(m_NewTaskAction, KStandardShortcut::shortcut(KStandardShortcut::New));
    connect(m_NewTaskAction, &QAction::triggered, this, &FractionRingWidget::NewTask);

    // back action
    m_BackAction  = new QWidgetAction(this);
    m_BackAction->setText(i18nc("@action go to the main screen", "Back"));
    m_BackAction->setIcon(QIcon::fromTheme(QStringLiteral("go-previous")));
    actionCollection()->addAction(QStringLiteral("Back"), m_BackAction);
    actionCollection()->setDefaultShortcuts(m_BackAction, KStandardShortcut::shortcut(KStandardShortcut::Back));
    connect(m_BackAction, &QAction::triggered, this, &FractionRingWidget::GoBack);

    // hint action (hide it as it doesn't exist here)
    m_HintAction  = new QAction(QIcon::fromTheme(QStringLiteral("games-hint")), i18nc("@action opens hint", "Hint"), this);
    actionCollection()->addAction(QStringLiteral("Hint"), m_HintAction);
    connect(m_HintAction, &QAction::triggered, this, &FractionRingWidget::Hint);

    // quit action
    KStandardAction::quit(this, SLOT(close()), actionCollection());

    KStandardAction::preferences(this,  SLOT(slotPrefs()), actionCollection());

    resize(QSize(725, 330).expandedTo(minimumSizeHint()));
    setupGUI(ToolBar | Keys);

    setAutoSaveSettings();
}

void FractionRingWidget::slotPrefs()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("slotPrefs FractionRingWidget");
#endif
    // do not show dialog twice
    if (KConfigDialog::showDialog(QStringLiteral("settings")))
        return;

    //KConfigDialog didn't find an instance of this dialog, so lets create it :
    KConfigDialog* configDialog = new KConfigDialog(this, QStringLiteral("settings"), SettingsClass::self());

    TaskColors * taskColors = new TaskColors(nullptr);
    configDialog->addPage(taskColors, i18n("Colors"), QStringLiteral("preferences-desktop-color"));

    // User edited the configuration - update your local copies of the
    // configuration data
    connect(configDialog, &KConfigDialog::settingsChanged, this, &FractionRingWidget::slotApplySettings);
    configDialog->show();

    return;
}

void FractionRingWidget::slotApplySettings()
{
#ifdef DEBUG
    qDebug() << QStringLiteral("slotApplySettings FractionRingWidget");
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

    for (int i = 0; i < rLeft.numerator(); i++) {
        painter.setBrush(colorListLeft[i]);
        spanAngle = ((16 * 360) << 8) / rLeft.denominator();
        painter.drawPie(rect, startAngle + ((i * spanAngle) >> 8), (spanAngle >> 8));
    }

    painter.setPen(penBorder);
    painter.setBrush(Qt::transparent);
    for (int i = 0; i < rLeft.denominator() * multLeft; i++) {
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

    for (int i = 0; i < rRight.numerator(); i++) {
        painter.setBrush(colorListRight[i]);
        spanAngle = ((16 * 360) << 8) / rRight.denominator();
        painter.drawPie(rect, startAngle + ((i * spanAngle) >> 8), (spanAngle >> 8));
    }

    painter.setPen(penBorder);
    painter.setBrush(Qt::transparent);
    for (int i = 0; i < rRight.denominator() * multRight; i++) {
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
    if (checkTask()) {
        msg = i18n("Now the parts of the inside ring and outside ring have "
                   "the same size, and we can add the fractions!\n\n"
                   "This is called finding the main denominator. "
                   "The main denominator divides the rings into equal parts.");
        if (rLeft.denominator() * value != MCM(rLeft.denominator(), rRight.denominator())) {
            msg += QStringLiteral("\n\n");
            msg += i18n("There is a smaller main denominator. Can you find it?");
        }

        textMsg->setText(msg);
    } else {
        msg = i18n("You have just expanded the first fraction by %1. "
                   "Our main goal is to make all parts be the same size. "
                   "Try expanding the first fraction by %2 and the second fraction by %3.",
                   value, rRight.denominator(), rLeft.denominator());
        textMsg->setText(msg);
    }

    update();
    fractionWidget->paintFraction(QStringLiteral("+"), rLeft, multLeft, rRight, multRight);
}

void FractionRingWidget::slotRightSpinBoxValueChanged(int value)
{
    multRight = value;

    QString msg;
    if (checkTask()) {
        msg = i18n("Now the parts of the inside ring and outside ring have "
                   "the same size, and we can add the fractions!\n\n"
                   "This is called finding the main denominator. "
                   "The main denominator divides the rings into equal parts.");
        if (rRight.denominator() * value != MCM(rLeft.denominator(), rRight.denominator())) {
            msg += QStringLiteral("\n\n");
            msg += i18n("There is a smaller main denominator. Can you find it?");
        }

        textMsg->setText(msg);
    } else {
        msg = i18n("You have just expanded the second fraction by %1. "
                   "Our main goal is to make all parts be the same size. "
                   "Try expanding the first fraction by %2 and the second fraction by %3.",
                   value, rRight.denominator(), rLeft.denominator());
        textMsg->setText(msg);
    }

    update();
    fractionWidget->paintFraction(QStringLiteral("+"), rLeft, multLeft, rRight, multRight);
}

void FractionRingWidget::NewTask()
{
    resetFraction(true);
}

void FractionRingWidget::resetFraction(bool flag = true)
{
    int denominator = int ((double(qrand()) / RAND_MAX) * 5) + 2;
    int numerator = int ((double(qrand()) / RAND_MAX) * (denominator - 1)) + 1;

    rLeft = Ratio(numerator, denominator, false);

    int denominator2 = int ((double(qrand()) / RAND_MAX) * 5) + 2;
    while (denominator2 == denominator) {
        denominator2 = int ((double(qrand()) / RAND_MAX) * 5) + 2;
    }
    int numerator2 = int ((double(qrand()) / RAND_MAX) * (denominator2 - 1)) + 1;
    rRight = Ratio(numerator2, denominator2, false);

    multLeft = 1;
    multRight = 1;
    leftSpinBox->setValue(1);
    rightSpinBox->setValue(1);

    // NOTE: Inserting denominator as %1, as first number is used to determine plural form,
    // and in these messages that should be the number next to "...painted parts".
    QString insert1 = i18ncp("Inserted as %1 in the message below.",
                             "%2 of 1 painted part.", "%2 of %1 painted parts.",
                             rLeft.denominator(), rLeft.numerator());
    QString insert2 = i18ncp("Inserted as %2 in the message below.",
                             "%2 of 1 painted part.", "%2 of %1 painted parts.",
                             rRight.denominator(), rRight.numerator());
    QString msg = i18nc("%1 and %2 are the two messages translated above.",
                        "The outside ring represents the left fraction. %1\n\n"
                        "The inside ring represents the right fraction. %2\n\n"
                        "The 'Expand' spinboxes expand each fraction. Try to change one of the spinbox values!",
                        insert1, insert2);

    textMsg->setText(msg);

    if (flag) {
        update();
        fractionWidget->paintFraction(QStringLiteral("+"), rLeft, multLeft, rRight, multRight);
    }
}

int FractionRingWidget::MCM(int a, int b)
{
    return a * b / GCF(a, b);
}

int FractionRingWidget::GCF(int a, int b)
{
    int tmp;
    if (a < b) {
        tmp = b;
        b = a;
        a = tmp;
    }
    tmp = a % b;
    while (tmp != 0) {
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
