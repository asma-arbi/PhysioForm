#ifndef TRAITEMENTC_H
#define TRAITEMENTC_H

#include<QString>
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
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QDebug>
#include <QFileDialog>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QPrinter>
#include <QAxObject>
#include <QListWidget>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

using namespace QtCharts;


class traitementC
{
    QString TITRE,MOTIFCONSULTATION,EXERCICES;
    QDate DEBUTTRAITEMENT, FINTRAITEMENT;
    QString IDTRAITEMENT, VIDEOTRAITEMENT;

public:

    //constructeurs
    traitementC();
    traitementC(QString IDTRAITEMENT , QString TITRE,QDate DEBUTTRAITEMENT,QDate FINTRAITEMENT,QString MOTIFCONSULTATION,QString EXERCICES, QString VIDEOTRAITEMENT);


    //getters
    QString getIdTraitement(){return IDTRAITEMENT;}
    QString getTitre(){return TITRE;}
    QDate getDebutTraitement(){return DEBUTTRAITEMENT;}
    QDate getFinTraitement(){return FINTRAITEMENT;}
    QString getMotifConsultation(){return MOTIFCONSULTATION;}
    QString getExercices(){return EXERCICES;}
    QString getVideoTraitement(){return VIDEOTRAITEMENT;}

    //setters
    void setIdTraitement(QString idtraitement){this->IDTRAITEMENT=idtraitement;}
    void setTitre(QString titre){this->TITRE=titre;}
    void setDebutTraitement(QDate debuttraitement){this->DEBUTTRAITEMENT=debuttraitement;}
    void setFinTraitement(QDate fintraitement){this->FINTRAITEMENT=fintraitement;}
    void setMotifConsultation(QString motifconsultation){this->MOTIFCONSULTATION=motifconsultation;}
    void setExercices(QString exercices){this->EXERCICES=exercices;}
    void setVideoTraitement(QString videoTraitement){this->VIDEOTRAITEMENT=videoTraitement;}

    //fonctionnalités de base relatives à Traitement
    bool ajouterTraitement();
    QSqlQueryModel* afficherTraitement();
    bool supprimerTraitement(QString IDTRAITEMENT);
    bool modifierTraitement();
    bool exporterPDF(const QString& fileName, QSqlQueryModel* model, const QModelIndex& selectedIndex);
    QSqlQueryModel* sortTraitement(QString sortingCriteria);
    QSqlQueryModel* searchTraitement(const QString& searchTerm);
    QStringList getExercicesById(const QString &IDTRAITEMENT);
    bool exporterExcel(const QString& fileName, QSqlQueryModel* model);
    void fetchTreatmentData();







};






#endif // TRAITEMENTC_H
