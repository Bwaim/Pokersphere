#ifndef SPONSOR_BONUS_H
#define SPONSOR_BONUS_H

class SponsorBonus
{
public:
    SponsorBonus(void);
    SponsorBonus(SponsorBonus const& bonus);
    ~SponsorBonus(void);

    SponsorBonus& operator=(SponsorBonus const& bonus);

    int getId() const;
	void setId(int id);
    int getLaunching() const;
	void setLaunching(int launching);
    int getTypeGodsonGain() const;
	void setTypeGodsonGain(int typeGodsonGain);
    int getGodsonGain() const;
	void setGodsonGain(int godsonGain);
    int getTypeSponsorGain() const;
	void setTypeSponsorGain(int typeSponsorGain);
    int getSponsorGain() const;
	void setSponsorGain(int sponsorGain);
    int getNbMonthFirstMembership() const;
	void setNbMonthFirstMembership(int nbMonthFirstMembership);

private:
    int m_id;
    int m_launching;
    int m_typeGodsonGain;
    int m_godsonGain;
    int m_typeSponsorGain;
    int m_sponsorGain;
    int m_nbMonthFirstMembership;
};

#endif //SPONSOR_BONUS_H