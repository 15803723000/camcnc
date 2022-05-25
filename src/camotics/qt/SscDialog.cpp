#include "SscDialog.h"
#include "ui_sscdialog.h"
using namespace CAMotics;
using namespace std;

SscDialog::SscDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SscDialog)
{
    ui->setupUi(this);
    connect(ui->pushButton,&QPushButton::released,this,&SscDialog::SendSlot);
    this->setFixedSize(300,200);

}
void SscDialog::SendSlot()
{
    emit SendData(ui->lineEdit->text().toDouble());
}

