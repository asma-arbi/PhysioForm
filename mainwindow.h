#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include "employe.h"
#include "patient.h"
#include "traitement.h"
#include "paiement.h"
#include "reservation.h"
//#include "arduino.h"
#include <QProcess>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // void  carteLue();



private slots:
    void on_seconnecter_login_clicked();

     void on_seconnecter2_clicked();


    void on_creer_compte_clicked();

    void on_annuler_login_clicked();

    void on_annuler_compte_clicked();




private:
    Ui::MainWindow *ui;
    employe *ma;
    patient *pa;
    traitement *ta;
    paiement *pai;
    reservation *res;
        QSqlDatabase mydb;
       // Arduino A;
        QByteArray data;
        //Arduino arduino;
            //QSerialPort* serialPort;
            QString uid;
};

#endif // MAINWINDOW_H
