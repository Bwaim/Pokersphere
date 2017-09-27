#ifndef MEMBER_H
#define MEMBER_H

#include <memory>

#include <QString>
#include <QDate>

class Member
{
public:
	Member(void);
	Member(Member const& member);
	~Member(void);

	Member& operator=(Member const& member);

	int getId() const;
	void setId(int id);
	QString getName() const;
	void setName(QString name);
	QString getFirstname() const;
	void setFirstname(QString firstname);
	QString getPseudo() const;
	void setPseudo(QString pseudo);
	QDate getBirthDate() const;
	void setBirthDate(QDate birthDate);
	QString getAdress() const;
	void setAdress(QString adress);
	QString getZipCode() const;
	void setZipCode(QString zipCode);
	QString getCity() const;
	void setCity(QString city);
	QString getEmail() const;
	void setEmail(QString email);
	QString getPhone() const;
	void setPhone(QString phone);
	int getKosmos() const;
	void setKosmos(int kosmos);
	QString getClub() const;
	void setClub(QString club);
	QDate getAnciennete() const;
	void setAnciennete(QDate anciennete);
	QDate getEcheance() const;
	void setEcheance(QDate echeance);
	bool getRc() const;
	void setRc(bool rc);
	int getKosmosDisponible() const;
	void setKosmosDisponible(int kosmos);
	int getKosmosDisponibleEnchere() const;
	void setKosmosDisponibleEnchere(int kosmos);
	int getKosmosResultatNet() const;
	void setKosmosResultatNet(int kosmos);
	int getKosmosResultatMoisPrecedent() const;
	void setKosmosResultatMoisPrecedent(int kosmos);
	int getKosmosResultatMoisCourant() const;
	void setKosmosResultatMoisCourant(int kosmos);
    QString getCardNumber() const;
	void setCardNumber(QString cardNumber);
    QString getLogin() const;
	void setLogin(QString login);
    std::shared_ptr<Member> getSponsor() const;
	void setSponsor(std::shared_ptr<Member> sponsor);
    QString getFranchise() const;
	void setFranchise(QString franchise);
    QString getFidStatut() const;
    void setFidStatut(QString fidStatut);
    void setFidPoints(int fidPoints);
    int getFidPoints() const;
    void setFidStatutPoints(int fidStatutPoints);
    int getFidStatutPoints() const;
    QString getPlaqueImmatriculation() const;
    void setPlaqueImmatriculation(QString plaqueImmatriculation);
    QString getIdPMU() const;
    void setIdPMU(QString idPMU);

private:
	int m_id;
	QString m_name;
	QString m_firstname;
	QString m_pseudo;
	QDate m_birthDate;
	QString m_adress;
	QString m_zipCode;
	QString m_city;
	QString m_email;
	QString m_phone;
	int m_kosmos;
	QString m_club;
	QDate m_anciennete;
	QDate m_echeance;
	bool m_rc;
	int m_kosmosDisponible;
	int m_kosmosDisponibleEnchere;
	int m_kosmosResultatNet;
    int m_kosmosResultatMoisCourant;
    int m_kosmosResultatMoisPrecedent;
    QString m_cardNumber;
    QString m_login;
    std::shared_ptr<Member> m_sponsor;
    QString m_franchise;
    QString m_fidStatut;
    int m_fidPoints;
    int m_fidStatutPoints;
    QString m_plaque_immatriculation;
    QString m_id_PMU;
};

#endif // MEMBER_H
