#include "Member.h"


Member::Member(void)
    : m_sponsor(nullptr)
{
}

Member::Member(Member const& member)
	: m_id(member.m_id), m_name(member.m_name), m_firstname(member.m_firstname), m_pseudo(member.m_pseudo)
	, m_birthDate(member.m_birthDate), m_adress(member.m_adress), m_zipCode(member.m_zipCode), m_city(member.m_city)
	, m_email(member.m_email), m_phone(member.m_phone), m_kosmos(member.m_kosmos), m_club(member.m_club)
	, m_anciennete(member.m_anciennete), m_echeance(member.m_echeance), m_rc(member.m_rc)
	, m_kosmosDisponible(member.m_kosmosDisponible), m_kosmosDisponibleEnchere(member.m_kosmosDisponibleEnchere)
	, m_kosmosResultatNet(member.m_kosmosResultatNet), m_kosmosResultatMoisCourant(member.m_kosmosResultatMoisCourant)
	, m_kosmosResultatMoisPrecedent(member.m_kosmosResultatMoisPrecedent), m_sponsor(member.m_sponsor), m_franchise(member.m_franchise)
    , m_fidStatut(member.m_fidStatut), m_fidPoints(member.m_fidPoints),m_fidStatutPoints(member.m_fidStatutPoints)
    , m_plaque_immatriculation(member.m_plaque_immatriculation), m_id_PMU(member.m_id_PMU)
{
}

Member::~Member(void)
{
}

Member& Member::operator=(Member const& member)
{
	if (this != &member)
	{
		m_id = member.m_id;
		m_name = member.m_name;
		m_firstname = member.m_firstname;
		m_pseudo = member.m_pseudo;
		m_birthDate = member.m_birthDate;
		m_adress = member.m_adress;
		m_zipCode = member.m_zipCode;
		m_city = member.m_city;
		m_email = member.m_email;
		m_phone = member.m_phone;
		m_kosmos = member.m_kosmos;
		m_club = member.m_club;
		m_anciennete = member.m_anciennete;
		m_echeance = member.m_echeance;
		m_rc = member.m_rc;
		m_kosmosDisponible = member.m_kosmosDisponible;
		m_kosmosDisponibleEnchere = member.m_kosmosDisponibleEnchere;
		m_kosmosResultatNet = member.m_kosmosResultatNet;
		m_kosmosResultatMoisCourant = member.m_kosmosResultatMoisCourant;
		m_kosmosResultatMoisPrecedent = member.m_kosmosResultatMoisPrecedent;
        m_sponsor = member.m_sponsor;
        m_franchise = member.m_franchise;
        m_fidStatut = member.m_fidStatut;
        m_fidPoints = member.m_fidPoints;
        m_fidStatutPoints = member.m_fidStatutPoints;
        m_plaque_immatriculation = member.m_plaque_immatriculation;
        m_id_PMU = member.m_id_PMU;
	}

	return *this;
}

int Member::getId() const
{
	return m_id;
}

void Member::setId(int id)
{
	m_id = id;
}

QString Member::getName() const
{
	return m_name;
}

void Member::setName(QString name)
{
	m_name = name;
}

QString Member::getFirstname() const
{
	return m_firstname;
}

void Member::setFirstname(QString firstname)
{
	m_firstname = firstname;
}

QString Member::getPseudo() const
{
	return m_pseudo;
}

void Member::setPseudo(QString pseudo)
{
	m_pseudo = pseudo;
}

QDate Member::getBirthDate() const
{
	return m_birthDate;
}

void Member::setBirthDate(QDate birthDate)
{
	m_birthDate = birthDate;
}

QString Member::getAdress() const
{
	return m_adress;
}

void Member::setAdress(QString adress)
{
	m_adress = adress;
}

QString Member::getZipCode() const
{
	return m_zipCode;
}

void Member::setZipCode(QString zipCode)
{
	m_zipCode = zipCode;
}

QString Member::getCity() const
{
	return m_city;
}

void Member::setCity(QString city)
{
	m_city = city;
}

QString Member::getEmail() const
{
	return m_email;
}

void Member::setEmail(QString email)
{
	m_email = email;
}

QString Member::getPhone() const
{
	return m_phone;
}

void Member::setPhone(QString phone)
{
	m_phone = phone;
}

int Member::getKosmos() const
{
	return m_kosmos;
}

void Member::setKosmos(int kosmos)
{
	m_kosmos = kosmos;
}

QString Member::getClub() const
{
	return m_club;
}

void Member::setClub(QString club)
{
	m_club = club;
}

QDate Member::getAnciennete() const
{
	return m_anciennete;
}

void Member::setAnciennete(QDate anciennete)
{
	m_anciennete = anciennete;
}

QDate Member::getEcheance() const
{
	return m_echeance;
}

void Member::setEcheance(QDate echeance)
{
	m_echeance = echeance;
}

bool Member::getRc() const
{
	return m_rc;
}

void Member::setRc(bool rc)
{
	m_rc = rc;
}

int Member::getKosmosDisponible() const
{
	return m_kosmosDisponible;
}

void Member::setKosmosDisponible(int kosmos)
{
	m_kosmosDisponible = kosmos;
}

int Member::getKosmosDisponibleEnchere() const
{
	return m_kosmosDisponibleEnchere;
}

void Member::setKosmosDisponibleEnchere(int kosmos)
{
	m_kosmosDisponibleEnchere = kosmos;
}

int Member::getKosmosResultatNet() const
{
	return m_kosmosResultatNet;
}

void Member::setKosmosResultatNet(int kosmos)
{
	m_kosmosResultatNet = kosmos;
}

int Member::getKosmosResultatMoisPrecedent() const
{
	return m_kosmosResultatMoisPrecedent;
}

void Member::setKosmosResultatMoisPrecedent(int kosmos)
{
	m_kosmosResultatMoisPrecedent = kosmos;
}

int Member::getKosmosResultatMoisCourant() const
{
	return m_kosmosResultatMoisCourant;
}

void Member::setKosmosResultatMoisCourant(int kosmos)
{
	m_kosmosResultatMoisCourant = kosmos;
}

QString Member::getCardNumber() const
{
    return m_cardNumber;
}

void Member::setCardNumber(QString cardNumber)
{
    m_cardNumber = cardNumber;
}

QString Member::getLogin() const
{
    return m_login;
}

void Member::setLogin(QString login)
{
    m_login = login;
}

std::shared_ptr<Member> Member::getSponsor() const
{
    return m_sponsor;
}

void Member::setSponsor(std::shared_ptr<Member> sponsor)
{
    m_sponsor = sponsor;
}

QString Member::getFranchise() const
{
    return m_franchise;
}
	
void Member::setFranchise(QString franchise)
{
    m_franchise = franchise;
}

QString Member::getFidStatut() const
{
    return m_fidStatut;
}

void Member::setFidStatut(QString fidStatut)
{
    m_fidStatut = fidStatut;
}

void Member::setFidPoints(int fidPoints)
{
    m_fidPoints = fidPoints;
}

int Member::getFidPoints() const
{
    return m_fidPoints;
}

void Member::setFidStatutPoints(int fidStatutPoints)
{
    m_fidStatutPoints = fidStatutPoints;
}

int Member::getFidStatutPoints() const
{
    return m_fidStatutPoints;
}

QString Member::getPlaqueImmatriculation() const
{
    return m_plaque_immatriculation;
}

void Member::setPlaqueImmatriculation(QString plaqueImmatriculation)
{
    m_plaque_immatriculation = plaqueImmatriculation;
}

QString Member::getIdPMU() const
{
    return m_id_PMU;
}

void Member::setIdPMU(QString idPMU)
{
    m_id_PMU = idPMU;
}
