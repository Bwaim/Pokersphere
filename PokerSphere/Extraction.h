#ifndef EXTRACTION_H
#define EXTRACTION_H

#include <QStringList>

class Extraction
{
private:
    Extraction(void);
    ~Extraction(void);

public:
    static void extractToCSV(QString &filename, QStringList headers, QList<QStringList> result);
};

#endif // EXTRACTION_H