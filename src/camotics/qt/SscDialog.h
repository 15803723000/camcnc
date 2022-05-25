#pragma once
#include <cbang/SmartPointer.h>

#include <QDialog>

namespace Ui {class SscDialog;}

namespace CAMotics {
class SscDialog : public QDialog
{
    Q_OBJECT;
    cb::SmartPointer<Ui::SscDialog> ui;

public:
    SscDialog(QWidget *parent);
 
private:
    

private slots:
    void SendSlot();

signals:
    void SendData(double str);


};

}
