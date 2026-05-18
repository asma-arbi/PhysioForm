#ifndef PAIEMENT_H
#define PAIEMENT_H

#include <QWidget>
#include <QNetworkReply>
#include "paiementC.h"
//#include "arduino.h"
#include <QDesktopServices>
#include <QSerialPort>
#include <QSerialPortInfo>
QT_BEGIN_NAMESPACE
namespace Ui { class paiement; }
QT_END_NAMESPACE



class paiement : public QDialog
{
    Q_OBJECT


public:
    explicit  paiement(QWidget *parent = nullptr);
    ~paiement();
    void clearFields();
    void envoyerSMS(const QString &destinataire, const QString &message);




private slots:
    void on_ajoutPaiement_clicked();
    void on_supprimerPai_clicked();
    void on_tableauSupprimer_clicked();
    void on_majPaiement_clicked();
    void on_recherchePaiement_clicked();
    void on_pdfPai_clicked();
    void on_TRIpai_clicked();
    void on_statPai_clicked();
    void on_historique_clicked();
    void on_TableRow_Clicked(const QModelIndex &index);
    void on_excelPai_clicked();
    void on_SMSpai_clicked();
    void on_pb_historiqueAli_clicked();
    void afficherHistoriquefer();
    // void processIdPaiement();




public slots:
    void replyFinished(QNetworkReply *reply);





private:
    Ui::paiement *ui;
    paiementC Ptmp;
    //arduino A;
    QByteArray data;


private:
    paiementC P;


};
#endif // PAIEMENT_H
