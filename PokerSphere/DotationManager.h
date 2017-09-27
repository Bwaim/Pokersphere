#ifndef DOTATION_MANAGER_H
#define DOTATION_MANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QEventLoop>
#include <QComboBox>

#include "networkadapter.h"

class DotationManager : public QObject
{
    Q_OBJECT

private:
    DotationManager();
    ~DotationManager();

signals:
    void isLoaded(void);

public:
    static DotationManager* getInstance();
    static void kill();
    bool addType(QString type, QWidget *caller);
    bool addDescription(QString description, QWidget *caller);
    bool modifyType(QString typeNew, QString typeOld, QWidget *caller);
    bool modifyDescription(QString descriptionNew, QString descriptionOld, QWidget *caller);
    void removeType(QString type, QWidget *caller);
    void removeDescription(QString description, QWidget *caller);
    QStringList getTypes();
    QStringList getDescriptions();
    void fillTypes(QComboBox *comboBox);
    void fillDescriptions(QComboBox *comboBox);

private slots:
    void getTypes(QStringList headers, QList<QStringList> result);
    void getDescriptions(QStringList headers, QList<QStringList> result);

private:
    static DotationManager *m_singleton;

    std::unique_ptr<NetworkAdapter> m_networkAdapter;
    QStringList m_types;
    QStringList m_descriptions;
    QEventLoop m_eventloop;
    bool m_typesLoaded;
    bool m_descriptionsLoaded;
};

#endif // DOTATION_MANAGER_H
