/***************************************************************************
                          kbruch.cpp  -  Main File of KBruch
                             -------------------
    begin                : Tue Nov 27 16:40:42 CET 2001
    copyright            : (C) 2001 by Sebastian Stein
    email                : seb.stein@hpfsc.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kbruch.h"

/* show the main menu for a text gui */
char show_main_txt()
{
	char chrInput;
	
	CLEAR; /* clear the console */
	cout << "\tKBruch " << VERSION << " " << DATE << endl;
	cout << "\n\n\tWhat do you want to do:" << endl;
	cout << "\n\n\t\t[a]ddition/substraction" << endl;
	cout << "\n\t\t[m]ultiplication/division" << endl;
	cout << "\n\t\t[t]ask with all operations (default)" << endl;
	cout << "\n\t\t[r]ead about the program" << endl;
	cout << "\n\t\t[e]xit program" << endl;
	cout << "\n\n\tInput: ";

	/* check, if the user entered a valid input */
	if (!(cin >> chrInput).good())
		chrInput = 'e';
	cin.clear();
	cin.ignore(INT_MAX, '\n');

	return chrInput;
}

/* ask for the other parameters needed to generate a task */
void ask_parameters_txt(short & nr_ratios, unsigned int & max_md)
{
	CLEAR;
	short tmp_short;
	unsigned int tmp_ui;

	cout << "\tKBruch " << VERSION << " " << DATE << endl;
	cout << "\n\n\tEnter number of ratios (between 2 and 5): ";

	/* get the input and check for errors */
	if (!(cin >> tmp_short).good())
		tmp_short = 2;
	cin.clear();
	cin.ignore(INT_MAX, '\n');

	/* we only allow 2 to 5 ratios per task, we are not able to print more
	 * on a console in a good form */
	if (tmp_short < 2)
		tmp_short = 2;
	if (tmp_short > 5)
		tmp_short = 5;
	nr_ratios = tmp_short; /* save the input */
	
	cout << "\n\n\tEnter the maximum size of the main denominator!";
	cout << "\n\tIt must be at least " << high(2, nr_ratios) << ": ";

	/* get the input and check for errors */
	if (!(cin >> tmp_ui).good())
		tmp_ui = 2;
	cin.clear();
	cin.ignore(INT_MAX, '\n');

	/* we only allow 2 to 5 ratios per task, we are not able to print more
	 * on a console in a good form */
	if (tmp_ui < high(2, nr_ratios))
		tmp_ui = high(2, nr_ratios);

	/* we do not allow the maximum denominator to be more then 4 times
	 * of the required minimum */
	if (tmp_ui > 4 * high(2, nr_ratios))
		tmp_ui = 4 * high(2, nr_ratios);
	max_md = tmp_ui; /* save the input */

	return;
}

/* generates the next task, asks the user for the result */
short run_task_txt(short add_sub, short mul_div,
						short nr_ratios, unsigned int max_md)
{
	task klausur;
	ratio result;
	int int_num, int_deno;
	short wrong = FALSE;

	CLEAR;
	cout << "\tKBruch " << VERSION << " " << DATE << endl;
	cout << "\n\n\tSolve the given task: \n" << endl;

	/* create a new task with the given parameters */
	klausur.create_task(max_md, nr_ratios, add_sub, mul_div);

	/* we do not want a numerator bigger than 200 for the result */
	do
	{
		klausur.create_task(max_md, nr_ratios, add_sub, mul_div);
		result = klausur.solve(); /* solve the task */
if (abs(result.get_zaehler() > 200))
	cout << "DEBUG MESSAGE zaehler: " << result.get_zaehler() << endl;
	} while (abs(result.get_zaehler()) > 200);

	cout << setw(8) << klausur << endl; /* display the task */

	cout << "\n\n\tEnter the numerator: ";

	/* check, if the user entered a valid input */
	if (!(cin >> int_num).good())
		int_num = 0;
	cin.clear();
	cin.ignore(INT_MAX, '\n');
	
	cout << "\n\tEnter the denominator: ";

	/* check, if the user entered a valid input */
	if (!(cin >> int_deno).good())
		int_deno = 1;
	cin.clear();
	cin.ignore(INT_MAX, '\n');

	/* if the result is smaller than 0, the minus must be given
	 * to the numerator */
	if (int_deno < 0)
	{
		int_deno *= -1;
		int_num *= -1;
	}

	/* check if the user entered the right result */
	if (int_num != result.get_zaehler() || int_deno != result.get_nenner())
		wrong = TRUE;	

	/* we accept the result, if it zero, but we do not accept 0/0 */
	if (int_num == 0 && result.get_zaehler() == 0 && int_deno != 0)
		wrong = FALSE;

	/* inform the user, if he entered the right value */
	if (wrong == TRUE)
	{
		CLEAR;
		cout << "\tKBruch " << VERSION << " " << DATE << endl;
		cout << "\n\n\tYou entered the wrong result for the task:\n" << endl;
		cout << setw(8) << klausur << endl; /* display the task */
		cout << "\n\n\tThe correct result is:\n" << endl;
		cout << setw(8) << result << endl;
		cout << "\n\n\tpress ENTER to go on: ";
		cin.ignore(INT_MAX, '\n');
		cin.clear();
		return 0;
	} else {
		cout << "\n\n\tYes, you entered the correct result." << endl;
		cout << "\tPress ENTER to go on: ";
		cin.ignore(INT_MAX, '\n');
		cin.clear();
		return 1;
	}
}

/* show some more information about the program */
void show_about_txt()
{
	CLEAR;
	cout << "\tKBruch " << VERSION << " " << DATE << endl;
	cout << "\n\t\tDescription: task generator for calculations with fractions";
	cout << endl << "\n\t\tHomepage: " << HOMEPAGE << endl;
	cout << "\n\t\tMaintainer: " << MAINTAINER << " <" << MAINTAINER_EMAIL;
	cout << ">" << endl << "\n\n\tPress ENTER to go on: ";
	cin.ignore(INT_MAX, '\n');
	cin.clear();
	return;
}

/* to show a very simple text interface for console users */
int gui_konsole(void)
{
	task klausur;
	short add_sub, mul_div;
	short & nr_ratios = * new short;
	unsigned int & max_md = * new unsigned int;
	char chrInput1, chrInput2;
	unsigned int correct, count;

	/* print the main menu and ask what to do */
	while ((chrInput1 = show_main_txt()) != 'e')
	{
		if (chrInput1 == 'r' || chrInput1 == 'R')
		{
			(void) show_about_txt();
		/* check what kind of operations are wanted for the task */
		} else {
			 if (chrInput1 == 'a' || chrInput1 == 'A')
			{
				/* only add/sub */
				add_sub = TRUE;
				mul_div = FALSE;
			} else if (chrInput1 == 'm' || chrInput1 == 'M')
			{
				/* only mul/div */
				add_sub = FALSE;
				mul_div = TRUE;
			} else {
				/* add/sub and mul/div */
				add_sub = TRUE;
				mul_div = TRUE;
			}
			(void) ask_parameters_txt(nr_ratios, max_md);
		
			correct = 0;
			count = 0;
			do
			{
				correct += run_task_txt(add_sub, mul_div, nr_ratios, max_md);
				count++;
	
				CLEAR;
				cout << "\tKBruch " << VERSION << " " << DATE << endl;
				cout << "\n\n\ttasks so far: \t" << count << endl;
				cout << "\tcorrect: \t" << correct;
				cout << "  " << int(double(correct) / count * 100) << "%" << endl;
				cout << "\tincorrect: \t" << (count - correct);
				cout << "  " << int(double(count - correct) / count * 100) << "%";
				cout << "\n\n\t\t[n]ext task (default)" << endl;
				cout << "\t\t[c]hange parameters" << endl;
				cout << "\t\t[e]xit program" << endl;
				cout << "\n\tInput: ";
			
				/* check, if the user entered a valid input */
				if (!(cin >> chrInput2).good())
					chrInput2 = 'n';
				cin.clear();
				cin.ignore(INT_MAX, '\n');

				/* check, if the user wants to change the task parameter */
				if (chrInput2 == 'c' || chrInput2 == 'C')
					break;

				/* check, if the user wants to quit the program */
				if (chrInput2 == 'e' || chrInput2 == 'E')
					return 0;

				/* user do not want to quit or change parameters, so go on
				 * with the next task */
			} while(1);
		}
	}
	return 0;
}

/* to show a Qt GUI */
int gui_qt_ini(int argc, char * argv[])
{
	KAboutData aboutData( "kbruch", I18N_NOOP("KBruch"),
		VERSION, description, KAboutData::License_GPL,
    		"(c) 2002, Sebastian Stein", 0, 0, "bruch@hpfsc.de");
	aboutData.addAuthor("Sebastian Stein",0, "bruch@hpfsc.de");
 	KCmdLineArgs::init( argc, argv, &aboutData );
	KCmdLineArgs::addCmdLineOptions( options ); // Add our own options.

	KApplication bruch;
	bruch.setFont(QFont("helvetica", 12));

	gui_qt * bruchApp = new gui_qt();
	bruch.setMainWidget(bruchApp);
	bruchApp->show();

	return bruch.exec();
}

/* the main program */
int main(int argc, char * argv[])
{
	/* check, if we have some command line parameters */
	for (int count = 1; count < argc; count++)
	{
		if (! strcmp(argv[count], "--version") || ! strcmp(argv[count], "-v"))
		{
			/* print version information */
			cout << "KBruch " << VERSION << " " << DATE << endl;
			cout << "by " << MAINTAINER << " <" << MAINTAINER_EMAIL << ">" << endl;
			cout <<  HOMEPAGE << endl;
			cout << "task generator for calculations with fractions" << endl;
			cout << "\nThis is free software; see the source for ";
			cout << "copying conditions. There is NO" << endl;
			cout << "warranty; not even for MERCHANTABILITY ";
			cout << "or FITNESS FOR A PARTICULAR PURPOSE." << endl;
		} else if (! strcmp(argv[count], "--help") || !strcmp(argv[count], "-h")
						|| ! strcmp(argv[count], "-?"))
		{
			/* print help information */
			cout << "usage: kbruch [options]" << endl;
			cout << "options:" << endl;
			cout << "\t-h, -?, --help\t\tShow help about options" << endl;
			cout << "\t-v, --version\t\tShow version information" << endl;
			cout << "\t-g, --generate\t\tgenerates tasks and solutions ";
			cout << "without stopping" << endl;
			cout << "\t-t, --text\t\tstart text mode user interface" << endl;
		} else if (! strcmp(argv[count], "--text") || !strcmp(argv[count], "-t"))
		{
			/* start the kbruch in text mode */
			return gui_konsole();
		} else if (! strcmp(argv[count], "-g")
						|| ! strcmp(argv[count], "--generate"))
		{
			task klausur;
			ratio ergebnis;

			do
			{
				do
				{
					klausur.create_task(100, 5, YES, YES);
					ergebnis = klausur.solve();
				} while (abs(ergebnis.get_zaehler()) > 200);

				cout << "Task:" << endl;
				cout << setw(8) << klausur << endl;
				cout << "Solution:" << endl;
				cout << setw(8) << ergebnis << endl;
				cout << "Press ENTER to go on or stop program ";
				cout << "by pressing Ctrl+C" << endl;
				cin.get();
				cin.clear();
			} while (1);
		} else { /* if ... --debug */
			/* an unknown command line option */
			cout << "\nunknown command line option: " << argv[count];
			cout << "\n" << endl;
		} /* if with cmd parameters */
	} /* for (...) -> loop through all cmd parameters */

	/* if we got command line options, we do not start the program */
	if (argc > 1)
		return 0;

	/* lets call the Qt GUI */
	return gui_qt_ini(argc, argv);
}
