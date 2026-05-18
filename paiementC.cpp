#include "paiement.h"
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QString>
#include <QDate>
#include <QDateEdit>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QSqlQuery>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QObject>
#include <QtWidgets>
#include <QTextStream>
#include <QAxObject>
#include <QStandardItemModel>
#include <QNetworkReply>
#include <QNetworkAccessManager>

paiementC::paiementC()
{
    IDPAIEMENT = 0;
    PATIENT = "";
    DATEPA = QDate::currentDate();
    TEL = "";
    TYPE = "";
    MONTANT = "" ;
    STATUT = "";
}


paiementC::paiementC(int IDPAIEMENT,QString PATIENT,QDate DATEPA,QString TEL ,QString TYPE,QString MONTANT,QString STATUT)
{
    this->IDPAIEMENT=IDPAIEMENT;
    this->PATIENT=PATIENT;
    this->DATEPA=DATEPA;
    this->TEL=TEL;
    this->TYPE=TYPE;
    this->MONTANT=MONTANT;
    this->STATUT=STATUT;

}


bool paiementC::ajouter()
{
    if (PATIENT.isEmpty() || TEL.isEmpty() || MONTANT.isEmpty() || TYPE.isEmpty() || STATUT.isEmpty()||IDPAIEMENT == 0) {
           QMessageBox::information(nullptr, "Information", "Veuillez remplir tous les champs requis.");
           return false;
       }
    // Vérification si le IDPAIEMENT existe déjà dans la base de données
        QSqlQuery checkQuery;
        checkQuery.prepare("SELECT COUNT(*) FROM PAIEMENT WHERE IDPAIEMENT = :IDPAIEMENT");
        checkQuery.bindValue(":IDPAIEMENT", IDPAIEMENT);
        if (!checkQuery.exec()) {
            QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification de l'existence du IDPAIEMENT dans la base de données.");
            return false;
        }
        checkQuery.next();
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            QMessageBox::information(nullptr, "Information", "L' IDPAIEMENT spécifié existe déjà dans la base de données.");
            return false;
        }
        //Tel
        if (!isValidPhoneNumber(TEL)) {
            QMessageBox::information(nullptr, "Information", "Le numéro de téléphone doit être composé de chiffres uniquement et avoir une longueur valide.");
            return false;
        }

        addToHistoryfer("Ajout du Patient  ",IDPAIEMENT);


    QSqlQuery query;
    QString res = QString::number(IDPAIEMENT);

    query.prepare("insert into PAIEMENT (IDPAIEMENT, PATIENT, DATEPA, TEL, TYPE, MONTANT, STATUT)"
                  "VALUES (:IDPAIEMENT, :PATIENT, :DATEPA, :TEL, :TYPE, :MONTANT, :STATUT)");

    query.bindValue(":IDPAIEMENT",IDPAIEMENT);
    query.bindValue(":PATIENT",PATIENT);
    query.bindValue(":DATEPA",DATEPA);
    query.bindValue(":TEL",TEL);
    query.bindValue(":TYPE",TYPE);
    query.bindValue(":MONTANT",MONTANT);
    query.bindValue(":STATUT",STATUT);

    return query.exec();
}


QSqlQueryModel * paiementC::afficher_paiement()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT* FROM PAIEMENT order by IDPAIEMENT");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDPAIEMENT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PATIENT"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATEPA"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TEL"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("TYPE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("MONTANT"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("STATUT"));


 return model;

}


bool paiementC:: supprimer_paiement(int IDPAIEMENT)
{

    // Vérification si l' IDPAIEMENT existe dans la base de données
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM PAIEMENT WHERE IDPAIEMENT = :IDPAIEMENT");
    checkQuery.bindValue(":IDPAIEMENT", IDPAIEMENT);
    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification du IDPAIEMENT.");
        return false;
    }
    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::information(nullptr, "Information", "L' IDPAIEMENT spécifié n'existe pas dans la base de données.");
        return false;
    }
    addToHistoryfer("Supression du Patient DE IDPAIEMENT = ",IDPAIEMENT);

    // Requête de suppression
    QSqlQuery query;
    query.prepare("DELETE FROM PAIEMENT WHERE IDPAIEMENT = :IDPAIEMENT");
    query.bindValue(":IDPAIEMENT", IDPAIEMENT);

    return query.exec();

}


bool paiementC::modifier_paiement()
{
    if (IDPAIEMENT < 0) {
            QMessageBox::information(nullptr, "Information", "Le champ IDPAIEMENT est vide.");
            return false;
        }
    QSqlQuery checkQuery;
      checkQuery.prepare("SELECT COUNT(*) FROM PAIEMENT WHERE IDPAIEMENT = :IDPAIEMENT");
      checkQuery.bindValue(":IDPAIEMENT", IDPAIEMENT);
      if (!checkQuery.exec()) {
          QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification du IDPAIEMENT.");
          return false;
      }
      checkQuery.next();
      int count = checkQuery.value(0).toInt();
      if (count == 0) {
          QMessageBox::information(nullptr, "Information", "L' IDPAIEMENT spécifié n'existe pas dans la base de données.");
          return false;
      }


    if (PATIENT.isEmpty()) {
        QMessageBox::information(nullptr, "Information", "Entrer un Patient.");
        return false;
    }
    addToHistoryfer("Modification du Patient DE IDPAIEMENT = ",IDPAIEMENT);



    // Requête de mise à jour
    QSqlQuery query;
    QString queryString = "UPDATE PAIEMENT SET ";

    // Ajoutez les colonnes à mettre à jour
    if (!PATIENT.isEmpty()) queryString += "PATIENT = :PATIENT, ";
    if (!TEL.isEmpty()) queryString += "TEL = :TEL, ";
    if (!TYPE.isEmpty()) queryString += "TYPE = :TYPE, ";
    if (!MONTANT.isEmpty()) queryString += "MONTANT = :MONTANT, ";
    if (!STATUT.isEmpty()) queryString += "STATUT = :STATUT, ";


    // Supprimer la dernière virgule et l'espace
    queryString.chop(2);

    // Ajouter la clause WHERE
    queryString += " WHERE IDPAIEMENT = :IDPAIEMENT";

    // Préparer la requête
    query.prepare(queryString);

    // Lier les valeurs aux paramètres de la requête
    if (!PATIENT.isEmpty()) query.bindValue(":PATIENT", PATIENT);
    if (!TEL.isEmpty()) query.bindValue(":TEL", TEL);
    if (!TYPE.isEmpty()) query.bindValue(":TYPE", TYPE);
    if (!MONTANT.isEmpty()) query.bindValue(":MONTANT", MONTANT);
    if (!STATUT.isEmpty()) query.bindValue(":STATUT", STATUT);
    //if (DATEN.isValid()) query.bindValue(":DATEN", DATEN);


    // Lier le CIN pour la clause WHERE
    query.bindValue(":IDPAIEMENT", IDPAIEMENT);

    // Exécuter la requête
    return query.exec();
}


QSqlQueryModel* paiementC::afficher(int IDPAIEMENT)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM PAIEMENT WHERE IDPAIEMENT = :IDPAIEMENT");
    query.bindValue(":IDPAIEMENT", IDPAIEMENT);
    if(query.exec())
    {
        addToHistory_recherchfer("Recherche du patient :",PATIENT);
        model->setQuery(query);
        if (model->rowCount() > 0) {
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDPAIEMENT"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("PATIENT"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("TEL"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("TYPE"));
            model->setHeaderData(4, Qt::Horizontal, QObject::tr("MONTANT"));
            model->setHeaderData(5, Qt::Horizontal, QObject::tr("STATUT"));
        }
    }
    return model;
}

bool paiementC::recherche_paiement(int IDPAIEMENT, QSqlQueryModel*& model)
{
    // Vérification si le IDPAIEMENT est vide
    if (IDPAIEMENT< 0) {
        QMessageBox::information(nullptr, "Information", "Le champ IDPAIEMENT est vide.");
        return false;
    }

    // Vérification si le IDPAIEMENT existe dans la base de données
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM PAIEMENT WHERE IDPAIEMENT = :IDPAIEMENT");
    checkQuery.bindValue(":IDPAIEMENT", IDPAIEMENT);
    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification du CIN.");
        return false;
    }
    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::information(nullptr, "Information", "L' IDPAIEMENT spécifié n'existe pas dans la base de données.");
        return false;
    }

    // Requête de recherche
    model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM PAIEMENT WHERE IDPAIEMENT = :IDPAIEMENT");
    query.bindValue(":IDPAIEMENT", IDPAIEMENT);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la récupération des données.");
        return false;
    }
    model->setQuery(query);
    return true;
}


bool paiementC::PDF_paiement(const QString& fileName, QSqlQueryModel* model)
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

        // Ajouter les données horizontalement
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

        // Définir le HTML dans le document
        doc.setHtml(html);

        doc.drawContents(&painter);

        return true;
    }


QSqlQueryModel* paiementC::tri_paiement()
{
    addToHistory_Trierfer("Sort ASC");
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM PAIEMENT ORDER BY LOWER(PATIENT) ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDPAIEMENT"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PATIENT"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATEP"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TEL"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("TYPE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("MONTANT"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("STATUT"));

    return model;
}

void paiementC::stat_paiement(QSqlQueryModel* model)
{
    QPieSeries *series = new QPieSeries();

            int countEspece = 0;
            int countCheque = 0;

            for (int row = 0; row < model->rowCount(); ++row) {
                QString type = model->data(model->index(row, 4)).toString(); // Supposons que la colonne du type est à l'index 7

                if (type.toLower() == "espece") {
                    countEspece++;
                } else if (type.toLower() == "cheque") {
                    countCheque++;
                }
            }
            int totalCount = countEspece + countCheque;
            qreal percentEspece = (static_cast<qreal>(countEspece) / totalCount) * 100.0;
            qreal percentCheque = (static_cast<qreal>(countCheque) / totalCount) * 100.0;
            series->append("Espece (" + QString::number(percentEspece, 'f', 1) + "%)", percentEspece);
            series->append("Cheque (" + QString::number(percentCheque, 'f', 1) + "%)", percentCheque);
            QChart *chart = new QChart();
            chart->addSeries(series);
            chart->setTitle("Répartition des paiements par type");
            chart->legend()->setAlignment(Qt::AlignRight);
            QFont font;
            font.setPixelSize(16); // Taille de police 16 pixels
            chart->setTitleFont(font);
            chart->legend()->setFont(font);
            QChartView *chartView = new QChartView(chart);
            chartView->setRenderHint(QPainter::Antialiasing);

            // Affichage du diagramme dans une fenêtre ou un widget
            chartView->resize(800, 600); // Ajustez la taille de la fenêtre selon vos besoins
            chartView->show();
}



QSqlQueryModel* paiementC::afficher(const QString& patientName)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM PAIEMENT WHERE PATIENT = :patientName");
    query.bindValue(":patientName", patientName);
    if(query.exec())
    {
        model->setQuery(query);
        if (model->rowCount() > 0) {
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDPAIEMENT"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("PATIENT"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATEP"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("TEL"));
            model->setHeaderData(4, Qt::Horizontal, QObject::tr("TYPE"));
            model->setHeaderData(5, Qt::Horizontal, QObject::tr("MONTANT"));
            model->setHeaderData(6, Qt::Horizontal, QObject::tr("STATUT"));
        }
    }
    return model;
}

bool paiementC::exporterExcel(const QString& fileName, QSqlQueryModel* model)
{
    QAxObject* excelApp = new QAxObject("Excel.Application");
    if (!excelApp) {
        qDebug() << "Could not create Excel.Application";
        return false;
    }

    excelApp->dynamicCall("SetVisible(bool)", false); // Don't show Excel

    QAxObject* workbooks = excelApp->querySubObject("Workbooks");
    QAxObject* workbook = workbooks->querySubObject("Add");

    QAxObject* sheets = workbook->querySubObject("Worksheets");
    QAxObject* sheet = sheets->querySubObject("Item(int)", 1);


    for (int col = 0; col < model->columnCount(); ++col) {
        QAxObject* cell = sheet->querySubObject("Cells(int,int)", 1, col + 1);
        cell->dynamicCall("SetValue(const QVariant&)", model->headerData(col, Qt::Horizontal));
    }


    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QAxObject* cell = sheet->querySubObject("Cells(int,int)", row + 2, col + 1);
            cell->dynamicCall("SetValue(const QVariant&)", model->data(model->index(row, col)).toString());
        }
    }


    workbook->dynamicCall("SaveAs(const QString&)", fileName);
    workbook->dynamicCall("Close()");


    excelApp->dynamicCall("Quit()");

    delete excelApp;

    return true;
}


void paiementC::addToHistoryfer(const QString &action,INT IDPATIENT)
{
    // Ouvrir le fichier en écriture
    QString cheminFichier = "C:/Users/alita/Desktop/paiement/historique.txt";
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
    if (IDPATIENT != 0) {
        out << " de ID " << IDPATIENT;
    }
    out << "\n";

    // Fermer le fichier
    file.close();
}
void paiementC::addToHistory_recherchfer(const QString &action,QString PATIENT) {
    // Ouvrir le fichier en écriture
    QString cheminFichier ="C:/Users/alita/Desktop/paiement/historique.txt";
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
    if (PATIENT != 0) {
        out << " " << PATIENT;
    }
    out << "\n";

    // Fermer le fichier
    file.close();
}
void paiementC::addToHistory_Trierfer(const QString &action) {
    // Ouvrir le fichier en écriture
    QString cheminFichier = "C:/Users/alita/Desktop/paiement/historique.txt";
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
void paiementC::writeHistoryToFilefer(const QString &fileName) {
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

    qDebug() << "Historique enregistré dans le fichier historique.txt.";
}








