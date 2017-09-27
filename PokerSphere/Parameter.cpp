#include "Parameter.h"

//Initialisation du singleton à NULL
Parameter *Parameter::m_singleton = nullptr;

static const char * PASSERELLE_FILE = "passerelle.php";
static const char * VERSION_FILE = "version.txt";

Parameter::Parameter(void)
    :m_franchise(0), m_title(QString("France"))
{
#ifdef LOCAL 
    m_url = QString("http://localhost:8080/Fabien/");
#else
#ifdef FREE
    //m_url = QString("http://arkanepoker.free.fr/pokersphere_re7/logiciel/");
    m_url = QString("http://localhost:8080/Pokersphere/");
#else
#endif
#endif

    m_franchiseLabels.insert(0,"France");
    m_franchiseLabels.insert(1,"Bordeaux");
    m_franchiseLabels.insert(2,"Toulouse");
    m_franchiseLabels.insert(3,"Biarritz");
    m_franchiseLabels.insert(4,"Biscarrosse");
    m_franchiseLabels.insert(5,"Lyon");

#ifdef FRANCE
    m_title = QString("France");
    m_franchise = 0;
#else
#ifdef BORDEAUX
    m_title = QString("Bordeaux");
    m_franchise = 1;
#else
#ifdef TOULOUSE
    m_title = QString("Toulouse");
    m_franchise = 2;
#else
#ifdef BIARRITZ
    m_title = QString("Biarritz");
    m_franchise = 3;
#else
#ifdef BISCARROSSE
    m_title = QString("Biscarrosse");
    m_franchise = 4;
#else
#ifdef LYON
    m_title = QString("Lyon");
    m_franchise = 4;
#endif
#endif
#endif
#endif
#endif
#endif
}


Parameter::~Parameter(void)
{
}

Parameter* Parameter::getInstance()
{
    if (!m_singleton)
	{
		m_singleton = new Parameter;
	}

	return m_singleton;
}

void Parameter::kill()
{
	if (m_singleton)
	{
		delete m_singleton;
		m_singleton = nullptr;
	}
}

QString Parameter::getUrl()
{
    return m_url + PASSERELLE_FILE;
}

int Parameter::getFranchise()
{
    return m_franchise;
}

QString Parameter::getTitle()
{
    return m_title;
}

QString Parameter::getVersionFile()
{
    return m_url + VERSION_FILE;
}

QString Parameter::getFranchiseLabel(int id)
{
    return m_franchiseLabels.value(id);
}
