#include "Extraction.h"

#include "requetes.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

Extraction::Extraction(void)
{
}


Extraction::~Extraction(void)
{
}


void Extraction::extractToCSV(QString &filename, QStringList headers, QList<QStringList> result)
{
    QFile file(filename);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) 
	{
		QMessageBox::warning(nullptr,EXPORT,QString(PROBLEME_CREATION_FICHIER));
		return;
	}

	QTextStream flux(&file);

	for ( int i = 0 ; i < headers.size() ; i++)
	{
		if (i!= 0)
			flux << ";";
		flux << headers[i];
	}

	for ( int i = 0 ; i< result.size() ; i++)
	{
		QStringList line = result[i];

		flux << "\n";

		for ( int j = 0 ; j < line.size() ; j++)
		{
			if (j!= 0)
				flux << ";";
			flux << line[j];
		}
	}
	QMessageBox::information(nullptr,EXPORT,QString(EXPORT_SUCCES));
}