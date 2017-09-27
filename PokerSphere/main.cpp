#include "pokersphere.h"
#include "LogWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>
#include <QCoreApplication>

#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);

	QString locale = QLocale::system().name().section('_', 0, 0);
	QTranslator translator;
	translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
	a.installTranslator(&translator);

    LogWindow logWindow;

	PokerSphere w(logWindow);
	if (w.isLogged())
		w.show();
	else
		return EXIT_SUCCESS;	
	return a.exec();
}
