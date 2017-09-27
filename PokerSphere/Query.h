#ifndef QUERY_H
#define QUERY_H

#include <QDialog>
#include <QMap>
#include "ui_Query.h"

class Query : public QDialog
{
    Q_OBJECT

public:
    Query(QWidget *parent = 0);
    ~Query();

private slots:
    void add(bool);
    void addAll(bool);
    void remove(bool);
    void removeAll(bool);

private:
    Ui::QueryClass ui;

    static void init();
    void add(QListView &from, QListView &to);
    void addAll(QListView &from, QListView &to);

    static const int SIZE_INFOS = 4;
	static const char *INFOS[SIZE_INFOS];
    static QMap<QString,QString> MAP_SELECT;
};

#endif // QUERY_H
