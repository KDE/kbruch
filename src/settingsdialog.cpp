/***************************************************************************
                          settingsdialog.cpp  -  settings dialog
                             -------------------
    begin                : 2004/06/01
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

#include "settingsdialog.h"
#include "settingsdialog.moc"

#include <kcolorbutton.h>
#include <kfontdialog.h>
#include <kiconloader.h>

#include "settingsclass.h"

SettingsDialog::SettingsDialog(QWidget * parent) :
	KDialogBase(IconList, i18n("KBruch Setup"),
		Help | Default | Apply | Ok | Cancel, Ok, parent, "SettingsDialog", true, true)
{
#ifdef DEBUG
	kdDebug() << "constructor SettingsDialog" << endl;
#endif

	// set the help file
	setHelp( "kbruch/index.html", QString::null );

	// set up task viewer options page
	// it has page index 0 (page_taskviewer -> see enum Page
	QVBox * page = addVBoxPage(i18n("Task Viewer"), i18n("Task Viewer Settings"), DesktopIcon("misc"));
	m_taskViewerOptions = new TaskViewerOptionsBase((QWidget *) page, "TaskViewerOptionsBase");


	// now set the current values
	setValues();
}

SettingsDialog::~SettingsDialog()
{
#ifdef DEBUG
	kdDebug() << "destructor SettingsDialog" << endl;
#endif
}

void SettingsDialog::setValues()
{
	// for the task viewer page
	m_taskViewerOptions->kcfg_numberColor->setColor(SettingsClass::numberColor());
	m_taskViewerOptions->operationColorButton->setColor(SettingsClass::operationColor());
	m_taskViewerOptions->fractionBarColorButton->setColor(SettingsClass::fractionBarColor());
	m_taskViewerOptions->fontChooser->setFont(SettingsClass::taskFont());

	return;
}

void SettingsDialog::slotApply()
{
	applyPage((Page) activePageIndex());
	return;
}

void SettingsDialog::slotDefault()
{
	// reset settings to default values
	SettingsClass::self()->setDefaults();

	// show the default settings in this dialog
	setValues();

	return;
}

void SettingsDialog::slotOk()
{
	// apply all pages
	applyPage(page_taskviewer);

	// close window
	accept();
	return;
}

void SettingsDialog::applyPage(Page page)
{
	switch (page)
	{
		case page_taskviewer:
				SettingsClass::setNumberColor(m_taskViewerOptions->kcfg_numberColor->color());
				SettingsClass::setOperationColor(m_taskViewerOptions->operationColorButton->color());
				SettingsClass::setFractionBarColor(m_taskViewerOptions->fractionBarColorButton->color());
				SettingsClass::setTaskFont(m_taskViewerOptions->fontChooser->font());
				break;
	}

	// the values change
	emit configChanged();

	return;
}
