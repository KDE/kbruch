/***************************************************************************
                          settingsdialog.h  -  settings dialog
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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#ifdef DEBUG
#include <kdebug.h>
#endif

#include <kdialogbase.h>
#include <klocale.h>

#include "taskvieweroptionsbase.h"

class SettingsDialog : public KDialogBase
{
	Q_OBJECT

public:
	SettingsDialog(QWidget * parent);
	~SettingsDialog();

private:
	/** sets the current values, values taken from setting class */
	void setValues();

	/** for each page in the settings dialog there is an entry in the enum */
	enum Page
	{
		page_taskviewer = 0
	};

	/** the page with the settings for the task viewer */
	TaskViewerOptionsBase * m_taskViewerOptions;

signals:
	void configChanged();

private slots:
	void slotApply();
	void slotDefault();
	void slotOk();
	void applyPage(Page page);
};

#endif
