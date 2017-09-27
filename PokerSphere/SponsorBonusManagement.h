#ifndef SPONSORBONUSMANAGEMENT_H
#define SPONSORBONUSMANAGEMENT_H

#include <QDialog>
#include <QHash>
#include "ui_SponsorBonusManagement.h"
#include "networkadapter.h"
#include "SponsorBonus.h"

class SponsorBonusManagement : public QDialog
{
    Q_OBJECT

public:
    SponsorBonusManagement(QWidget *parent = 0);
    ~SponsorBonusManagement();

    static const int LAUNCHING_SIZE = 2;
    static const char *LAUNCHING[LAUNCHING_SIZE];
    static const int BONUS_TYPE_SIZE = 3;
    static const char *BONUS_TYPE[BONUS_TYPE_SIZE];
    static const int NB_MONTH_SIZE = 5;
    static const int NB_MONTH[NB_MONTH_SIZE];

private slots:
	void getBonus(QStringList headers, QList<QStringList> result);
	void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
	void updateBonus(const QString&);
	void typeLaunchingChanged(int index);

private:
    void displayBonus(std::shared_ptr<SponsorBonus> bonus);

    Ui::SponsorBonusManagementClass ui;

    std::shared_ptr<SponsorBonus> m_selectedBonus;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
	QHash<int,std::shared_ptr<SponsorBonus>> m_bonus;
};

#endif // SPONSORBONUSMANAGEMENT_H
