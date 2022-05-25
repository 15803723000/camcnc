#ifndef AMDIALOG_H
#define AMDIALOG_H

#include <QDialog>

namespace Ui {
class AmDialog;
}

class AmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AmDialog(QWidget *parent = 0);
    ~AmDialog();

private:
    Ui::AmDialog *ui;
};

#endif // AMDIALOG_H
