#ifndef PAIEMENTC_H
#define PAIEMENTC_H
#include<QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include<QDate>
#include <QRegularExpression>
#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextLength>
#include <QSqlQueryModel>
#include <QPrinter>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QDebug>
#include <QFileDialog>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QAxObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>

using namespace QtCharts;



class paiementC
{

private:
    int IDPAIEMENT;
            QString PATIENT;
            QDate DATEPA;
            QString TEL;
            QString TYPE;
            QString MONTANT;
            QString STATUT;
            QList <QString> historyList; // Liste pour stocker l'historique
            QString historiqueFileName = "historique.txt";
public:
    paiementC();
    paiementC(int,QString,QDate,QString,QString,QString,QString);

    bool isValidPhoneNumber( QString TEL) {
        // Vérifier si le numéro de téléphone ne contient que des chiffres
        for (QChar c : TEL) {
            if (!c.isDigit()) {
                // Le numéro de téléphone contient un caractère non numérique
                return false;
            }
        }

        // Vérifier si le numéro de téléphone a une longueur valide
        if (TEL.length() != 8) {
            // Le numéro de téléphone ne comporte pas 8 chiffres
            return false;
        }

        // Si le numéro de téléphone passe les deux vérifications ci-dessus, il est valide
        return true;
    }




    // GETTERS
    int GetIDPAIEMENT() { return IDPAIEMENT; }
    QString GetPATIENT() { return PATIENT; }
    QDate GetDATEPA() { return DATEPA; }
    QString GetTEL() { return TEL; }
    QString GetTYPE() { return TYPE; }
    QString GetMONTANT() { return MONTANT; }
    QString GetSTATUT() { return STATUT; }



    // SETTERS
    void SetIDPAIEMENT(int val) { IDPAIEMENT = val; }
    void SetPATIENT(QString val) { PATIENT = val; }
    void SetDATEPA(QDate val) { DATEPA = val; }
    void SetTEL(QString val) { TEL = val; }
    void SetTYPE(QString val) { TYPE = val; }
    void SetMONTANT(QString val) { MONTANT = val; }
    void SetSTATUT(QString val) { STATUT = val; }




    bool ajouter();
    QSqlQueryModel * afficher_paiement();
    bool supprimer_paiement(int IDPAIEMENT);
    bool modifier_paiement();
    QSqlQueryModel* afficher(int IDPAIEMENT);
    bool recherche_paiement(int IDPAIEMENT, QSqlQueryModel*& model);
    bool PDF_paiement(const QString& fileName, QSqlQueryModel* model);
    QSqlQueryModel* tri_paiement();
    void stat_paiement(QSqlQueryModel* model);
    QSqlQueryModel* afficher(const QString& patientName);
    bool exporterExcel(const QString& fileName, QSqlQueryModel* model);
    void addToHistory_Trierfer(const QString &action);
    void addToHistory_recherchfer(const QString &action,QString PATIENT);
    void addToHistoryfer(const QString &action,int IDPATIENT) ;
    void writeHistoryToFilefer(const QString &fileName);



};







#endif // PAIEMENTC_H
