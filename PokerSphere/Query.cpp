#include "Query.h"

#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QList>

//Initialization
const char *Query::INFOS[Query::SIZE_INFOS] = {"Prénom", "Nom", "Adresse Mail", "Téléphone"};
QMap<QString,QString> Query::MAP_SELECT = QMap<QString,QString>();

Query::Query(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    static int init = 0;

    if (init == 0)
    {
        Query::init();
        init++;
    }

    QStandardItemModel*  model = new QStandardItemModel();
    for (int i=0; i < Query::SIZE_INFOS; i++)
        model->setItem(i,new QStandardItem(Query::INFOS[i]));

    ui.globalListView->setModel(model);

    QStandardItemModel*  modelSelect = new QStandardItemModel();
    ui.selectListView->setModel(modelSelect);

    connect(ui.addPushButton,SIGNAL(clicked(bool)),this,SLOT(add(bool)));
    connect(ui.addAllPushButton,SIGNAL(clicked(bool)),this,SLOT(addAll(bool)));
    connect(ui.removePushButton,SIGNAL(clicked(bool)),this,SLOT(remove(bool)));
    connect(ui.removeAllPushButton,SIGNAL(clicked(bool)),this,SLOT(removeAll(bool)));
}

Query::~Query()
{

}

void Query::init()
{
    Query::MAP_SELECT.insert("Prénom", "toto");
    Query::MAP_SELECT.insert("Nom", "toto");
    Query::MAP_SELECT.insert("Adresse Mail", "toto");
    Query::MAP_SELECT.insert("Téléphone", "toto");
}

void Query::add(bool)
{
    add(*ui.globalListView, *ui.selectListView);
}

void Query::addAll(bool)
{
    addAll(*ui.globalListView, *ui.selectListView);
}

void Query::remove(bool)
{
    add(*ui.selectListView, *ui.globalListView);
}

void Query::removeAll(bool)
{
    addAll(*ui.selectListView, *ui.globalListView);
}

void Query::add(QListView &from, QListView &to)
{
    QModelIndexList selection = from.selectionModel()->selectedIndexes();

    QList<QStandardItem*> list = QList<QStandardItem*>();
    for (int i=selection.size()-1; i >= 0 ; i--)
    {
        list.append(new QStandardItem(QString(selection[i].data().toString())));
        from.model()->removeRow(selection[i].row());
    }

    if (!list.isEmpty())
        for (int i = list.size()-1; i >= 0 ; i--)
            static_cast<QStandardItemModel*>(to.model())->appendRow(list[i]);
}

void Query::addAll(QListView &from, QListView &to)
{
    QStandardItemModel *model = static_cast<QStandardItemModel*>(from.model());

    QList<QStandardItem*> list = QList<QStandardItem*>();
     for (int i=0; i < model->rowCount() ; i++)
    {
        static_cast<QStandardItemModel*>(to.model())->appendRow(model->takeItem(i));
    }

    model->removeRows(0,model->rowCount());
}