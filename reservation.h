#ifndef RESERVATION_H
#define RESERVATION_H
#include <QVariant>

#include <QWidget>
#include <QDialog>
#include "reservationc.h"

QT_BEGIN_NAMESPACE
namespace Ui { class reservation; }
QT_END_NAMESPACE

class reservation : public QDialog
{
    Q_OBJECT

public:
    explicit  reservation(QWidget *parent = nullptr);
    ~reservation();
    void clearFields();
    void showEvent(QShowEvent *event) ;



private slots:
    void on_ajout_abdo_clicked();

    void on_btnrechercher_abdo_clicked();

    void on_pdf_abdo_clicked();

    void on_trieer_abdo_clicked();

    void on_stat_abdo_clicked();


    void on_tableView_abdo_doubleClicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_modifier_clicked();

    void on_pushButton_2_clicked();

    void on_sendMail_clicked();

    void on_mailsent_clicked();

    void moveMapCenter(double lat, double lng);
    void placeMarker(double lat, double lng);

 // void on_region_clicked(QString);

private:
   Ui::reservation *ui;
    reservationC Ptmp;
    reservationC P;
signals:
    void setCenter(QVariant lat, QVariant lng);
    void addMarker(QVariant lat, QVariant lng);
};
#endif // reservation_H
