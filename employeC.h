#ifndef EMPLOYEC_H
#define EMPLOYEC_H
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
using namespace QtCharts;
class employeC
{
private:

    QString idemp,nomemploye,prenomemploye,emailemploye,numtele,adresseemploye;
    QDate DATEN;
    QString sexeemploye,specialiteemploye,mdpemploye;
public:
  //constructeur
     employeC();
     employeC(QString idemp, QString nomemploye, QString prenomemploye, QString mdpemploye, QString emailemploye, QString numtele, QString sexeemploye, QString specialiteemploye );


     //getter
     QString getidemp(){return idemp;}
     QString getnomemploye(){return nomemploye;}
     QString getprenomemploye(){return prenomemploye;}
     QDate getDate(){return DATEN;}
     QString getemailemploye(){return emailemploye;}
      QString getnumtele(){return numtele;}
      QString getadresseemploye(){return adresseemploye;}
     QString getsexeemploye(){return sexeemploye;}
      QString getspecialiteemploye(){return specialiteemploye;}
      QString getmdpemploye(){return mdpemploye;}


 //setter
     void setidemp(QString idemp_p){this->idemp=idemp_p;}
     void setnomemploye(QString nomemploye_p){this->nomemploye=nomemploye_p;}
     void setprenomemploye(QString prenomemploye_p){this->prenomemploye=prenomemploye_p;}
     void setDate(QDate Date_p){this->DATEN=Date_p;}
     void setemailemploye(QString emailemploye_p){this->emailemploye=emailemploye_p;}
     void setnumtele(QString numtele_p){this->numtele=numtele_p;}
     void setadresseemploye(QString adresseemploye_p){this->adresseemploye=adresseemploye_p;}
     void setsexeemploye(QString sexeemploye_p){this->sexeemploye=sexeemploye_p;}
     void setspecialiteemploye(QString specialiteemploye_p){this->specialiteemploye=specialiteemploye_p;}
     void setmdpempolye(QString mdp){this->mdpemploye=mdp;}

bool mdpCorrect(const QString &id, const QString &mdp)const;
bool idexist(const QString &id)const;
int verifierAI(const QString &id)const;
bool ajouter_employe();
QSqlQueryModel* afficher();
bool supprimer_employe(QString idemp);
bool modifier_employe();
QSqlQueryModel* afficher(QString idemp);
bool recherche(QString idemp, QSqlQueryModel*& model);
bool exporterPDF(const QString& fileName, QSqlQueryModel* model);
void genererStatistiquessexeemploye(QSqlQueryModel* model);
void genererStatistiquesspecialiteemploye(QSqlQueryModel* model);
QSqlQueryModel* afficher_tri(QString sortingCreteria);
bool isValididemp( QString idemp)
{
    return idemp.length() == 6 && idemp.toInt();
}
bool validateemailemploye(QString emailemploye) {
       QRegularExpression re("^[a-zA-Z0-9+_.-]+@[a-zA-Z0-9.-]+$");
       QRegularExpressionMatch match = re.match(emailemploye);//re:recherche une corr dans la chaine email et la stocke dans l'objet match
       return match.hasMatch();//retoune true si une correspondance a été trouvé
   }
bool isValidPhoneNumber( QString nomemploye) {
    for (QChar c : nomemploye) {
        if (!c.isDigit()) {
            return false;
        }
    }
    if (nomemploye.length() != 8) {
        return false;
    }
    return true;
}

};

#endif // EMPLOYEC_H
