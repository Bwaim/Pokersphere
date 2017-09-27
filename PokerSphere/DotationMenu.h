#ifndef DOTATIONMENU_H
#define DOTATIONMENU_H

#include <QWidget>

#include "DotationManagement.h"

namespace Ui {
class DotationMenu;
}

class DotationMenu : public QWidget
{
    Q_OBJECT

public:
    explicit DotationMenu(QWidget *parent = 0);
    ~DotationMenu();

private slots:
    void showDotationTypeManagement(const QString&);
    void showDotationDescriptionManagement(const QString&);

private:
    Ui::DotationMenu *ui;

    std::unique_ptr<DotationManagement> m_dotationManagement;
};

#endif // DOTATIONMENU_H
