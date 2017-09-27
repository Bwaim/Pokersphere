#include "DotationManager.h"

#include "requetes.h"
#include "Transform.h"

#include <QTimer>

//Initialisation du singleton à NULL
DotationManager *DotationManager::m_singleton = nullptr;

DotationManager::DotationManager()
    : QObject(), m_networkAdapter(nullptr), m_types(QStringList()), m_descriptions(QStringList()), m_typesLoaded(false)
    , m_descriptionsLoaded(false)
{

    m_networkAdapter.reset(new NetworkAdapter(SELECT_DOTATION_TYPE,SELECT,QStringList()));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this
        ,SLOT(getTypes(QStringList,QList<QStringList>)));

    m_eventloop.exec();

    m_networkAdapter.reset(new NetworkAdapter(SELECT_DOTATION_DESCRIPTION,SELECT,QStringList()));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QList<QStringList>)), this
        ,SLOT(getDescriptions(QStringList,QList<QStringList>)));
}

DotationManager::~DotationManager(void)
{

}

DotationManager* DotationManager::getInstance()
{
    if (!m_singleton)
    {
        kill();
        m_singleton = new DotationManager();
    }

    return m_singleton;
}

void DotationManager::kill()
{
    if (m_singleton)
    {
        delete m_singleton;
        m_singleton = nullptr;
    }
}

void DotationManager::getTypes(QStringList headers, QList<QStringList> result)
{
    m_types = Transform::toDotations(headers,result);
    m_typesLoaded = true;
    m_eventloop.quit();
}

void DotationManager::getDescriptions(QStringList headers, QList<QStringList> result)
{
    m_descriptions = Transform::toDotations(headers,result);
    m_descriptionsLoaded = true;
    emit isLoaded();
}

bool DotationManager::addType(QString type, QWidget *caller)
{
    if (m_types.contains(type))
        return false;

    m_types.append(type);
    m_types.sort();

    QStringList args = QStringList();
    args.append(type);

    m_networkAdapter.reset(new NetworkAdapter(CREATE_DOT_TYPE,INSERT,args));

    connect(m_networkAdapter.get(), SIGNAL(finished()), caller ,SLOT(accept()));

    return true;
}

bool DotationManager::addDescription(QString description, QWidget *caller)
{
    if (m_descriptions.contains(description))
        return false;

    m_descriptions.append(description);
    m_descriptions.sort();

    QStringList args = QStringList();
    args.append(description);

    m_networkAdapter.reset(new NetworkAdapter(CREATE_DOT_DESCRIPTION,INSERT,args));

    connect(m_networkAdapter.get(), SIGNAL(finished()), caller ,SLOT(accept()));

    return true;
}

QStringList DotationManager::getTypes()
{
    return m_types;
}

QStringList DotationManager::getDescriptions()
{
    return m_descriptions;
}

bool DotationManager::modifyType(QString typeNew, QString typeOld, QWidget *caller)
{
    if (typeNew.compare(typeOld) != 0
        && m_types.contains(typeNew))
        return false;

    m_types.removeOne(typeOld);
    m_types.append(typeNew);

    QStringList args = QStringList();

    args.append(typeOld);
    args.append(typeNew);

    m_networkAdapter.reset(new NetworkAdapter(UPDATE_DOT_TYPE,UPDATE,args,caller));

    return true;
}

bool DotationManager::modifyDescription(QString descriptionNew, QString descriptionOld, QWidget *caller)
{
    if (descriptionNew.compare(descriptionOld) != 0
        && m_descriptions.contains(descriptionNew))
        return false;

    m_descriptions.removeOne(descriptionOld);
    m_descriptions.append(descriptionNew);

    QStringList args = QStringList();

    args.append(descriptionOld);
    args.append(descriptionNew);

    m_networkAdapter.reset(new NetworkAdapter(UPDATE_DOT_DESCRIPTION,UPDATE,args,caller));

    return true;
}

void DotationManager::removeType(QString type, QWidget *caller)
{
    m_types.removeOne(type);

    QStringList args = QStringList();
    args.append(type);

    m_networkAdapter.reset(new NetworkAdapter(DELETE_DOT_TYPE,DELETE_REQ,args,caller));
}

void DotationManager::removeDescription(QString description, QWidget *caller)
{
    m_descriptions.removeOne(description);

    QStringList args = QStringList();
    args.append(description);

    m_networkAdapter.reset(new NetworkAdapter(DELETE_DOT_DESCRIPTION,DELETE_REQ,args,caller));
}

void DotationManager::fillTypes(QComboBox *comboBox)
{
    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    connect(this,  SIGNAL(isLoaded()), &loop, SLOT(quit()) );
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(10);

    loop.exec();

    while (!m_typesLoaded)
    {
        timer.start(10);
        loop.exec();
    }

    comboBox->clear();
    comboBox->addItem(QString(""));
    comboBox->addItems(m_types);
}

void DotationManager::fillDescriptions(QComboBox *comboBox)
{
    QTimer timer;
    timer.setSingleShot(true);
    QEventLoop loop;

    connect(this,  SIGNAL(isLoaded()), &loop, SLOT(quit()) );
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(10);

    loop.exec();

    while (!m_descriptionsLoaded)
    {
        timer.start(10);
        loop.exec();
    }

    comboBox->clear();
    comboBox->addItem(QString(""));
    comboBox->addItems(m_descriptions);
}
