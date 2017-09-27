#ifndef KOSMOSEXTRACTION_H
#define KOSMOSEXTRACTION_H

#include <QDialog>
#include <QStringList>
#include "ui_KosmosExtraction.h"
#include "Member.h"
#include "networkadapter.h"

class KosmosExtraction : public QDialog
{
    Q_OBJECT

public:
    KosmosExtraction(std::shared_ptr<Member> member, QWidget *parent = 0);
    ~KosmosExtraction();

private slots:
    void cancelExtraction(const QString&);
    void initDate(QStringList headers, QList<QStringList> result);
    void extract(const QString&);
    void exportKosmos(QStringList headers, QList<QStringList> result);

private:
    Ui::KosmosExtraction ui;
    std::shared_ptr<Member> m_member;
    std::unique_ptr<NetworkAdapter> m_networkAdapter;
};

#endif // KOSMOSEXTRACTION_H
