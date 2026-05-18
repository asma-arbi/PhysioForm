#ifndef PATIENTC_H
#define PATIENTC_H
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
#include <QtQuickWidgets/QtQuickWidgetsVersion>

using namespace QtCharts;

class patientC
{

private:

    QString CIN,NUMTEL,NOM,PRENOM,ADRESSE,EMAIL,DESCRIPTION,IMAGEPATH,RECORD,BATTEMENT;
    QDate DATEN;
    QString SEXE;
    QList<QString> historyList; // Liste pour stocker l'historique
    QString historiqueFileName = "historique.txt";
public:


  //constructeur
     patientC();
     patientC(QString CIN, QString NUMTEL, QString NOM, QString PRENOM, QString ADRESSE, QString EMAIL, QDate DATEN, QString SEXE, QString DESCRIPTION ,QString IMAGEPATH,QString RECORD,QString BATTEMENT);


     //getter
     QString getCin(){return CIN;}
     QString getNumTel(){return NUMTEL;}
     QString getNom(){return NOM;}
     QString getPrenom(){return PRENOM;}
     QString getAdresse(){return ADRESSE;}
     QString getEmail(){return EMAIL;}
     QDate getDate(){return DATEN;}
     QString getSexe(){return SEXE;}
     QString getDescription(){return DESCRIPTION;}
     QString getIMAGEPATH(){return IMAGEPATH;}

     QString getRECORD(){return RECORD;}
     QString getBATTEMENT(){return  BATTEMENT;}



 //setter
     void setCin(QString Cin_p){this->CIN=Cin_p;}
     void setNumTel(QString NumTel_p){this->NUMTEL=NumTel_p;}
     void setNom(QString Nom_p){this->NOM=Nom_p;}
     void setPrenom(QString Prenom_p){this->PRENOM=Prenom_p;}
     void setAdresse(QString Adresse_p){this->ADRESSE=Adresse_p;}
     void setEmail(QString Email_p){this->EMAIL=Email_p;}
     void setDate(QDate Date_p){this->DATEN=Date_p;}
     void setSexe(QString Sexe_p){this->SEXE=Sexe_p;}
     void setDescription(QString Description_p){this->DESCRIPTION=Description_p;}
     void setIMAGEPATH(QString IMAGEPATH_p){this->IMAGEPATH=IMAGEPATH_p;}
     void setRECORD(QString RECORD_p){this->RECORD=RECORD_p;}
void setBATTEMENT(QString BATTEMENT_p){this->BATTEMENT=BATTEMENT_p;}


     void addToHistory_Trierfer(const QString &action);
     void addToHistory_recherchfer(const QString &action,QString NUMT);
     void addToHistoryfer(const QString &action,QString IDP) ;
     void writeHistoryToFilefer(const QString &fileName);
bool ajouter_patient();
QSqlQueryModel* afficher();
bool supprimer_patient(QString CIN);
bool modifier_patient();
QSqlQueryModel* afficher(QString CIN);
bool recherche(QString CIN, QSqlQueryModel*& model);
bool exporterPDF(const QString& fileName, QSqlQueryModel* model);
void genererStatistiquesSexe(QSqlQueryModel* model);
QSqlQueryModel* afficher_tri();
bool isValidCIN( QString CIN)
{
    return CIN.length() == 8 && CIN.toInt();
}
bool validateEmail(QString email) {
       QRegularExpression re("^[a-zA-Z0-9+_.-]+@[a-zA-Z0-9.-]+$");
       QRegularExpressionMatch match = re.match(email);
       return match.hasMatch();
   }
bool isValidPhoneNumber( QString NUMTEL) {
    for (QChar c : NUMTEL) {
        if (!c.isDigit()) {
            return false;
        }
    }
    if (NUMTEL.length() != 8) {
        return false;
    }
    return true;
}
bool isValidNom(QString nom) {
    QRegularExpression regex("^[a-zA-Z ]+$");
    return regex.match(nom.trimmed()).hasMatch();
}

bool isValidPrenom(QString prenom) {
    QRegularExpression regex("^[a-zA-Z ]+$");
    return regex.match(prenom.trimmed()).hasMatch();
}
};

#endif // PATIENTC_H
