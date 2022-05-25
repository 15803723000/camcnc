#include "Amdialog.h"
#include "ui_Amdialog.h"

AmDialog::AmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AmDialog)
{
    ui->setupUi(this);
}

AmDialog::~AmDialog()
{
    delete ui;
}
