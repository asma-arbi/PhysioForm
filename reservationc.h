#ifndef RESERVATIONC_H
#define RESERVATIONC_H
#include<QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include<QDateTime>
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
#include <QtCharts/QChart>
#include <QtPrintSupport/QPrinter>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QDebug>
#include <QFileDialog>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
using namespace QtCharts;
class reservationC
{
private:

    QString ID,CIN,TYPRE,MODEC,REGION;
    QDateTime DATER;
public:
  //constructeur
     reservationC();
     reservationC( QString CIN, QDateTime DATER,QString TYPRE , QString MODEC, QString REGION);
     reservationC(QString id ,QString CIN, QDateTime DATER, QString TYPRE, QString MODEC ,QString REGION);



     //getter
     QString getID(){return ID;}
     QString getCIN(){return CIN;}
     QDateTime getDate(){return DATER;}
     QString getTYPRE(){return TYPRE;}
     QString getMODEC(){return MODEC;}
     QString getREGION(){return REGION;}



 //setter
     void setId(QString Id_p){this->ID=Id_p;}
     void setCIN(QString CIN_p){this->CIN=CIN_p;}
     void setDate(QDateTime Date_p){this->DATER=Date_p;}
     void setTYPRE(QString TYPRE_p){this->TYPRE=TYPRE_p;}
     void setMODEC(QString MODEC_p){this->MODEC=MODEC_p;}
     void setREGION(QString REGION_p){this->REGION=REGION_p;}



bool ajouter_reservation();
QSqlQueryModel* affichertous();
bool supprimer_reservation(QString ID);
bool modifier_reservation();
QSqlQueryModel* afficher(QString ID);
bool recherche(QString CIN, QSqlQueryModel*& model);
QSqlQueryModel * recherche(QString);
bool exporterPDF(const QString& fileName, QSqlQueryModel* model);
void genererStatistiquesREGION(QSqlQueryModel* model);
QSqlQueryModel* afficher_tri();

bool validateREGION(QString REGION) {
    return !REGION.isEmpty();
}
bool isValidCin( QString CIN) {
    for (QChar c : CIN) {
        if (!c.isDigit()) {
            return false;
        }
    }
    if (CIN.length() != 8) {
        return false;
    }
    return true;
}

};

#endif // reservationC_H
