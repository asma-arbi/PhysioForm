#include "patientC.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QString>
#include <QDate>
#include <QDateEdit>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QTextStream>
/*******************************    CRUD **********************/
patientC::patientC() {
    CIN = "";
    NUMTEL = "";
    NOM = "";
    PRENOM= "";
    ADRESSE = "";
    EMAIL = "";
    DATEN = QDate::currentDate();
    SEXE = "";
    DESCRIPTION = "";
    IMAGEPATH = "";
    RECORD="";
    BATTEMENT="";
}
patientC::patientC(QString CIN, QString NUMTEL, QString NOM, QString PRENOM, QString ADRESSE, QString EMAIL, QDate DATEN, QString SEXE, QString DESCRIPTION ,QString IMAGEPATH,QString RECORD,QString BATTEMENT)
{
    this->CIN = CIN;
    this->NUMTEL = NUMTEL;
    this->NOM = NOM;
    this->PRENOM = PRENOM;
    this->ADRESSE = ADRESSE;
    this->EMAIL = EMAIL;
    this->DATEN = DATEN;
    this->SEXE = SEXE;
    this->DESCRIPTION = DESCRIPTION;
    this->IMAGEPATH = IMAGEPATH;
    this->RECORD=RECORD;
    this->BATTEMENT=BATTEMENT;
}
bool patientC::ajouter_patient()
{
    if (CIN.isEmpty() || NUMTEL.isEmpty() || NOM.isEmpty() || PRENOM.isEmpty() || ADRESSE.isEmpty() || EMAIL.isEmpty() || SEXE.isEmpty() || DESCRIPTION.isEmpty() ) {
           QMessageBox::critical(nullptr, "Information", "Veuillez remplir tous les champs requis.");
           return false;
       }

    if (!isValidCIN(CIN)) {
            QMessageBox::critical(nullptr, "Information", "Le CIN doit être un numéro de 8 chiffres.");
            return false;

    }
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM PATIENTC WHERE CIN = :CIN");
        checkQuery.bindValue(":CIN", CIN);
        if (!checkQuery.exec()) {
            QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification de l'existence du CIN dans la base de données.");
            return false;
        }
        checkQuery.next();
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            QMessageBox::critical(nullptr, "Information", "Le CIN spécifié existe déjà dans la base de données.");
            return false;
        }
        if(!isValidNom(NOM)){
            QMessageBox::critical(nullptr, "Information", "Le NOM doit être des Alphabet seulement ");
            return false;

        }
        if(!isValidNom(PRENOM)){
           QMessageBox::critical(nullptr, "Information", "Le PRENOM doit être des Alphabet seulement ");
            return false;

        }

    //Tel
    if (!isValidPhoneNumber(NUMTEL)) {
        QMessageBox::critical(nullptr, "Information", "Le numéro de téléphone doit être composé de chiffres uniquement et avoir une longueur valide.");
        return false;
    }
    //mail
    if (!validateEmail(EMAIL)) {
       QMessageBox::critical(nullptr, "Information", "Verifiez votre adresse mail");
        return false;
    }
    if (DATEN.isValid() && DATEN == QDate::currentDate())
{
        QMessageBox::critical(nullptr, "Information", "Veuillez verifier la date de naissance ");
         return false;

    }


    addToHistoryfer("Modification du Patient  ",CIN);


    QSqlQuery query;
    query.prepare("INSERT INTO PATIENTC(CIN, NUMTEL, NOM, PRENOM, ADRESSE, EMAIL, DATEN, SEXE,DESCRIPTION,IMAGEPATH,RECORD,BATTEMENT)"
                     "VALUES(:CIN, :NUMTEL, :NOM, :PRENOM, :ADRESSE, :EMAIL, :DATEN, :SEXE, :DESCRIPTION, :IMAGEPATH, :RECORD, :BATTEMENT)");
    query.bindValue(":CIN",CIN);
    query.bindValue(":NUMTEL",NUMTEL);
    query.bindValue(":NOM",NOM);
    query.bindValue(":PRENOM",PRENOM);
    query.bindValue(":ADRESSE",ADRESSE);
    query.bindValue(":EMAIL",EMAIL);
    query.bindValue(":DATEN",DATEN);
    query.bindValue(":SEXE",SEXE);
    query.bindValue(":DESCRIPTION",DESCRIPTION);
    query.bindValue(":IMAGEPATH",IMAGEPATH);
    query.bindValue(":IMAGEPATH",IMAGEPATH);
    query.bindValue(":RECORD", RECORD);
    query.bindValue(":BATTEMENT", BATTEMENT);




    return query.exec();

}
QSqlQueryModel * patientC::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT CIN, NUMTEL, NOM, PRENOM, ADRESSE, EMAIL, DATEN, SEXE, DESCRIPTION, IMAGEPATH,RECORD,BATTEMENT FROM PATIENTC ORDER BY CIN");

    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex dateIndex = model->index(i, 6);
        QString dateString = dateIndex.data().toDate().toString("yyyy-MM-dd");
        model->setData(dateIndex, dateString); }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NUMTEL"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("DATEN"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("SEXE"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("DESCRIPTION"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("IMAGEPATH"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("RECORD"));
    model->setHeaderData(11, Qt::Horizontal, QObject::tr("BATTEMENT"));


    return model;
}
bool patientC:: supprimer_patient(QString CIN)
{


        if (!isValidCIN(CIN)) {
                QMessageBox::critical(nullptr, "Information", "Le CIN doit être un numéro de 8 chiffres.");
                return false;

        }
        else
        {
            addToHistoryfer("Supression du Patient DE CIN = ",CIN);
            QSqlQuery checkQuery;
            checkQuery.prepare("SELECT COUNT(*) FROM PATIENTC WHERE CIN = :CIN");
            checkQuery.bindValue(":CIN", CIN);
            if (!checkQuery.exec()) {
                QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification du CIN.");
                return false;
            }
            checkQuery.next();
            int count = checkQuery.value(0).toInt();
            if (count == 0) {
                QMessageBox::information(nullptr, "Information", "Le CIN spécifié n'existe pas dans la base de données.");
                return false;
            }

            QSqlQuery query;
            query.prepare("DELETE FROM PATIENTC WHERE CIN = :CIN");
            query.bindValue(":CIN", CIN);

            return query.exec();
        }






}
bool patientC::modifier_patient()
{
    if (!isValidCIN(CIN)) {
            QMessageBox::critical(nullptr, "Information", "Le CIN doit être un numéro de 8 chiffres.");
            return false;
        }
    QSqlQuery checkQuery;
      checkQuery.prepare("SELECT COUNT(*) FROM PATIENTC WHERE CIN = :CIN");
      checkQuery.bindValue(":CIN", CIN);
      if (!checkQuery.exec()) {
          QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification du CIN.");
          return false;
      }
      checkQuery.next();
      int count = checkQuery.value(0).toInt();
      if (count == 0) {
          QMessageBox::information(nullptr, "Information", "Le CIN spécifié n'existe pas dans la base de données.");
          return false;
      }
    if (!NUMTEL.isEmpty() && !isValidPhoneNumber(NUMTEL)) {
        QMessageBox::critical(nullptr, "Information", "Le numéro de téléphone doit être composé de chiffres uniquement et avoir une longueur valide.");
        return false;
    }
    if (!EMAIL.isEmpty() && !validateEmail(EMAIL)) {
        QMessageBox::critical(nullptr, "Information", "Veuillez vérifier votre adresse e-mail.");
        return false;
    }
    if(!NOM.isEmpty() && !isValidNom(NOM) ){

            QMessageBox::critical(nullptr, "Information", "Le NOM doit être des Alphabet seulement ");
            return false;
    }
    if(!PRENOM.isEmpty() && !isValidNom(PRENOM) ){

            QMessageBox::critical(nullptr, "Information", "Le PRENOM doit être des Alphabet seulement ");
            return false;
    }

      addToHistoryfer("Modification du Patient  ",CIN);

    QSqlQuery query;
    QString queryString = "UPDATE PATIENTC SET ";
    if (!NOM.isEmpty()) queryString += "NOM = :NOM, ";
    if (!PRENOM.isEmpty()) queryString += "PRENOM = :PRENOM, ";
    if (!ADRESSE.isEmpty()) queryString += "ADRESSE = :ADRESSE, ";
    if (!EMAIL.isEmpty()) queryString += "EMAIL = :EMAIL, ";
    if (!NUMTEL.isEmpty()) queryString += "NUMTEL = :NUMTEL, ";

    if (DATEN.isValid() && DATEN != QDate::currentDate())
        queryString += "DATEN = :DATEN, ";

    if (!SEXE.isEmpty()) queryString += "SEXE = :SEXE, ";
    if (!DESCRIPTION.isEmpty()) queryString += "DESCRIPTION = :DESCRIPTION, ";
    if (!IMAGEPATH.isEmpty()) queryString += "IMAGEPATH = :IMAGEPATH, ";
    if (!RECORD.isEmpty()) queryString += "RECORD = :RECORD, ";
    if (!RECORD.isEmpty()) queryString += "BATTEMENT = :BATTEMENT, ";

    queryString.chop(2);
    queryString += " WHERE CIN = :CIN";
    query.prepare(queryString);
    if (!NOM.isEmpty()) query.bindValue(":NOM", NOM);
    if (!PRENOM.isEmpty()) query.bindValue(":PRENOM", PRENOM);
    if (!ADRESSE.isEmpty()) query.bindValue(":ADRESSE", ADRESSE);
    if (!EMAIL.isEmpty()) query.bindValue(":EMAIL", EMAIL);
    if (!NUMTEL.isEmpty()) query.bindValue(":NUMTEL", NUMTEL);
    if (DATEN.isValid() && DATEN != QDate::currentDate())
        query.bindValue(":DATEN", DATEN);
    if (!SEXE.isEmpty()) query.bindValue(":SEXE", SEXE);
    if (!DESCRIPTION.isEmpty()) query.bindValue(":DESCRIPTION", DESCRIPTION);
     if (!IMAGEPATH.isEmpty()) query.bindValue(":IMAGEPATH", IMAGEPATH);
     if (!RECORD.isEmpty()) query.bindValue(":RECORD", RECORD);
     if (!BATTEMENT.isEmpty()) query.bindValue(":BATTEMENT", BATTEMENT);

    query.bindValue(":CIN", CIN);
    return query.exec();

}
QSqlQueryModel* patientC::afficher(QString CIN)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM PATIENTC WHERE CIN = :CIN");
    query.bindValue(":CIN", CIN);
    if(query.exec())
    {
        model->setQuery(query);
        if (model->rowCount() > 0) {
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN du Patient"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("NUMTEL"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOM"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRENOM"));
            model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));
            model->setHeaderData(5, Qt::Horizontal, QObject::tr("EMAIL"));
            model->setHeaderData(6, Qt::Horizontal, QObject::tr("DATEN"));
            model->setHeaderData(7, Qt::Horizontal, QObject::tr("SEXE"));
            model->setHeaderData(8, Qt::Horizontal, QObject::tr("DESCRIPTION"));
                model->setHeaderData(9, Qt::Horizontal, QObject::tr("IMAGEPATH"));
                    model->setHeaderData(10, Qt::Horizontal, QObject::tr("RECORD"));
                    model->setHeaderData(10, Qt::Horizontal, QObject::tr("BATTEMENT"));

        }
    }
    return model;
}
/*******************************  Metier simple **********************/

bool patientC::recherche(QString CIN, QSqlQueryModel*& model)
{
    if (CIN.isEmpty()) {
        QMessageBox::information(nullptr, "Information", "Le champ CIN est vide.");
        return false;
    }
    else {

        if (!isValidCIN(CIN)) {
                QMessageBox::information(nullptr, "Information", "Le CIN doit être un numéro de 8 chiffres.");
                return false;

        }
        else
        {
            addToHistory_recherchfer("Recherche du patient de CIN:",CIN);
            QSqlQuery checkQuery;
            checkQuery.prepare("SELECT COUNT(*) FROM PATIENTC WHERE CIN = :CIN");
            checkQuery.bindValue(":CIN", CIN);
            if (!checkQuery.exec()) {
                QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification du CIN.");
                return false;
            }
            checkQuery.next();
            int count = checkQuery.value(0).toInt();
            if (count == 0) {
                QMessageBox::critical(nullptr, "Information", "Le CIN spécifié n'existe pas dans la base de données.");
                return false;
            }
            model = new QSqlQueryModel();
            QSqlQuery query;
            query.prepare("SELECT * FROM PATIENTC WHERE CIN = :CIN");
            query.bindValue(":CIN", CIN);
            if (!query.exec()) {
                QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la récupération des données.");
                return false;
            }
            model->setQuery(query);
            return true;
        }

    }

}
bool patientC::exporterPDF(const QString& fileName, QSqlQueryModel* model)
{
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A3);
    printer.setOutputFileName(fileName);

    QPainter painter(&printer);
    if (!painter.isActive()) {
        QMessageBox::warning(nullptr, "Erreur", "Impossible de dessiner sur l'imprimante.");
        return false;
    }

    QTextDocument doc;

    QString html = "<html><head><style>"
                   "table { border-collapse: collapse; width: 20%; }"
                   "th, td { border: 1px solid #dddddd; text-align: left; padding: 8px; font-size: 50%; }"
                   "th { background-color: #f2f2f2; }"
                   "</style></head><body>"
                   "<table>";
    html += "<tr>";
        for (int col = 0; col < model->columnCount(); ++col) {
            html += "<th>" + model->headerData(col, Qt::Horizontal).toString() + "</th>";
        }
        html += "</tr>";
        for (int row = 0; row < model->rowCount(); ++row) {
            html += "<tr>";
            for (int col = 0; col < model->columnCount(); ++col) {
                if (col == 6) {
                    QDateTime dateTime = model->data(model->index(row, col)).toDateTime();
                    QString formattedDate = dateTime.toString("yyyy-MM-dd");
                    html += "<td>" + formattedDate + "</td>";
                } else {
                    html += "<td>" + model->data(model->index(row, col)).toString() + "</td>";
                }
            }
            html += "</tr>";
        }
        html += "</table></body></html>";
        doc.setHtml(html);
        doc.drawContents(&painter);
        return true;
}
void patientC::genererStatistiquesSexe(QSqlQueryModel* model)
{
    QPieSeries *series = new QPieSeries();

        int countHomme = 0;
        int countFemme = 0;

        for (int row = 0; row < model->rowCount(); ++row) {
            QString sexe = model->data(model->index(row, 7)).toString();

            if (sexe.toLower() == "homme") {
                countHomme++;
            } else if (sexe.toLower() == "femme") {
                countFemme++;
            }
        }
        int totalCount = countHomme + countFemme;
        qreal percentHomme = (static_cast<qreal>(countHomme) / totalCount) * 100.0;
        qreal percentFemme = (static_cast<qreal>(countFemme) / totalCount) * 100.0;
        series->append("Hommes (" + QString::number(percentHomme, 'f', 1) + "%)", countHomme);
        series->append("Femmes (" + QString::number(percentFemme, 'f', 1) + "%)", countFemme);
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition des patients par sexe");
        chart->legend()->setAlignment(Qt::AlignRight);
        QFont font;
        font.setPixelSize(16);
        chart->setTitleFont(font);
        chart->legend()->setFont(font);
        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->resize(800, 600);
        chartView->show();
}
QSqlQueryModel* patientC::afficher_tri()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    addToHistory_Trierfer("Trier A-Z ");

    model->setQuery("SELECT CIN, NUMTEL, NOM, PRENOM, ADRESSE, EMAIL, DATEN, SEXE, DESCRIPTION,IMAGEPATH,RECORD, BATTEMENT FROM PATIENTC ORDER BY LOWER(NOM) ASC");
    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex dateIndex = model->index(i, 6);
        QString dateString = dateIndex.data().toDate().toString("yyyy-MM-dd");
        model->setData(dateIndex, dateString);
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NUMTEL"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("PRENOM"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("DATEN"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("SEXE"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("DESCRIPTION"));
        model->setHeaderData(9, Qt::Horizontal, QObject::tr("IMAGEPATH"));
        model->setHeaderData(10, Qt::Horizontal, QObject::tr("RECORD"));
        model->setHeaderData(11, Qt::Horizontal, QObject::tr("BATTEMENT"));

    return model;
}
/*******************************  Historique **********************/
void patientC::addToHistoryfer(const QString &action,QString IDP)
{
    // Ouvrir le fichier en écriture
    QString cheminFichier = "C:/Users/PC/Desktop/Crudferiel/patient/historique.txt";
    QFile file(cheminFichier);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier historique.";
        return;

    }

    // Créer un flux texte pour écrire dans le fichier
    QTextStream out(&file);

    // Obtenir la date et l'heure actuelles
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Écrire l'action dans le fichier historique avec la date et l'heure
    out << currentDateTime.toString("yyyy-MM-dd hh:mm:ss") << " - " << action;
    if (IDP != 0) {
        out << " de CIN " << IDP;
    }
    out << "\n";

    // Fermer le fichier
    file.close();
}
void patientC::addToHistory_recherchfer(const QString &action,QString NUMT) {
    // Ouvrir le fichier en écriture
    QString cheminFichier ="C:/Users/PC/Desktop/Crudferiel/patient/historique.txt";
    QFile file(cheminFichier);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier historique.";
        return;
    }

    // Créer un flux texte pour écrire dans le fichier
    QTextStream out(&file);

    // Obtenir la date et l'heure actuelles
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Écrire l'action dans le fichier historique avec la date et l'heure
    out << currentDateTime.toString("yyyy-MM-dd hh:mm:ss") << " - " << action;
    if (NUMT != 0) {
        out << " " << NUMT;
    }
    out << "\n";

    // Fermer le fichier
    file.close();
}
void patientC::addToHistory_Trierfer(const QString &action) {
    // Ouvrir le fichier en écriture
    QString cheminFichier = "C:/Users/PC/Desktop/Crudferiel/patient/historique.txt";
    QFile file(cheminFichier);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier historique.";
        return;
    }

    // Créer un flux texte pour écrire dans le fichier
    QTextStream out(&file);

    // Obtenir la date et l'heure actuelles
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Écrire l'action dans le fichier historique avec la date et l'heure
    out << currentDateTime.toString("yyyy-MM-dd hh:mm:ss") << " - " << action;

    out << " " <<endl;


    // Fermer le fichier
    file.close();
}
void patientC::writeHistoryToFilefer(const QString &fileName) {
    // Ouvrir le fichier en écriture
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier historique.";
        return;
    }

    // Créer un flux texte pour écrire dans le fichier
    QTextStream out(&file);

    // Écrire chaque entrée d'historique dans le fichier
    for (const QString &entry : historyList) {
        out << entry << "\n";
    }

    // Fermer le fichier
    file.close();

    qDebug() << "Historique enregistré dans le fichier historique_patients.txt.";
}
