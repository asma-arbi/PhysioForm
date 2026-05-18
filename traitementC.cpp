#include "traitementC.h"
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
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QtWidgets/QTableWidget>
#include <QTableWidgetItem>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>


#include <QSqlError>


traitementC::traitementC()
{
    IDTRAITEMENT ="";
    TITRE ="";
    DEBUTTRAITEMENT =QDate::currentDate();
    FINTRAITEMENT =QDate::currentDate();
    MOTIFCONSULTATION ="";
    EXERCICES ="";
    VIDEOTRAITEMENT ="";

}


traitementC::traitementC(QString IDTRAITEMENT, QString TITRE ,QDate DEBUTTRAITEMENT ,QDate FINTRAITEMENT,QString MOTIFCONSULTATION,QString EXERCICES, QString VIDEOTRAITEMENT)
{
    this->IDTRAITEMENT=IDTRAITEMENT;
    this->TITRE=TITRE;
    this->DEBUTTRAITEMENT=DEBUTTRAITEMENT;
    this->FINTRAITEMENT=FINTRAITEMENT;
    this->MOTIFCONSULTATION=MOTIFCONSULTATION;
    this->EXERCICES=EXERCICES;
    this->VIDEOTRAITEMENT=VIDEOTRAITEMENT;
}






bool traitementC::ajouterTraitement()
{
    QSqlQuery query;
    query.prepare("insert into TRAITEMENT(IDTRAITEMENT, TITRE, DEBUTTRAITEMENT, FINTRAITEMENT,MOTIFCONSULTATION,EXERCICES,VIDEOTRAITEMENT)"
                  "values(:IDTRAITEMENT, :TITRE, :DEBUTTRAITEMENT, :FINTRAITEMENT, :MOTIFCONSULTATION, :EXERCICES, :VIDEOTRAITEMENT)");

    query.bindValue(":IDTRAITEMENT",IDTRAITEMENT);
    query.bindValue(":TITRE", TITRE);
    query.bindValue(":DEBUTTRAITEMENT", DEBUTTRAITEMENT);
    query.bindValue(":FINTRAITEMENT",FINTRAITEMENT );
    query.bindValue(":MOTIFCONSULTATION",MOTIFCONSULTATION );
    query.bindValue(":EXERCICES",EXERCICES );
    query.bindValue(":VIDEOTRAITEMENT",VIDEOTRAITEMENT );

    return query.exec(); //exec() envoie la requete pour l'exécuter
 }




QSqlQueryModel* traitementC::afficherTraitement(){
    QSqlQueryModel *model = new QSqlQueryModel();
        model->setQuery("SELECT * FROM TRAITEMENT ORDER BY IDTRAITEMENT");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDTRAITEMENT"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("TITRE"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("DEBUTTRAITEMENT"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("FINTRAITEMENT"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("MOTIFCONSULTATION"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("EXERCICES"));
        model->setHeaderData(6, Qt::Horizontal, QObject::tr("VIDEOTRAITEMENT"));

     return model;

}



 bool traitementC::supprimerTraitement(QString IDTRAITEMENT)
{
    QSqlQuery query;

    query.prepare("Delete from TRAITEMENT where IDTRAITEMENT= :IDTRAITEMENT");
    query.bindValue(":IDTRAITEMENT", IDTRAITEMENT);



    return query.exec();

}


 bool traitementC::modifierTraitement()
 {

        // Requête de mise à jour
        QSqlQuery query;
        QString queryString = "UPDATE TRAITEMENT SET ";

        // Ajoutez les colonnes à mettre à jour
        if (!IDTRAITEMENT.isEmpty()) queryString += "IDTRAITEMENT = :IDTRAITEMENT, ";
        if (!TITRE.isEmpty()) queryString += "TITRE = :TITRE, ";
        if (!DEBUTTRAITEMENT.isValid()) queryString += "DEBUTTRAITEMENT = :DEBUTTRAITEMENT, ";
        if (!FINTRAITEMENT.isValid()) queryString += "FINTRAITEMENT = :FINTRAITEMENT, ";
        if (!MOTIFCONSULTATION.isEmpty()) queryString += "MOTIFCONSULTATION = :MOTIFCONSULTATION, ";
        if (!EXERCICES.isEmpty()) queryString += "EXERCICES = :EXERCICES, ";
        if (!VIDEOTRAITEMENT.isEmpty()) queryString += "VIDEOTRAITEMENT = :VIDEOTRAITEMENT, ";


        // Supprimer la dernière virgule et l'espace
        queryString.chop(2);

        // Ajouter la clause WHERE
        queryString += " WHERE IDTRAITEMENT = :IDTRAITEMENT";

        // Préparer la requête
        query.prepare(queryString);

        // Lier les valeurs aux paramètres de la requête
        if (!IDTRAITEMENT.isEmpty()) query.bindValue(":IDTRAITEMENT", IDTRAITEMENT);
        if (!TITRE.isEmpty()) query.bindValue(":TITRE", TITRE);
        if (!DEBUTTRAITEMENT.isValid()) query.bindValue(":DEBUTTRAITEMENT", DEBUTTRAITEMENT);
        if (!FINTRAITEMENT.isValid()) query.bindValue(":FINTRAITEMENT", FINTRAITEMENT);
        if (!MOTIFCONSULTATION.isEmpty()) query.bindValue(":MOTIFCONSULTATION", MOTIFCONSULTATION);
        if (!EXERCICES.isEmpty()) query.bindValue(":EXERCICES", EXERCICES);
        if (!VIDEOTRAITEMENT.isEmpty()) query.bindValue(":VIDEOTRAITEMENT", VIDEOTRAITEMENT);

        // Lier le IDTRAITEMENT pour la clause WHERE
        query.bindValue(":IDTRAITEMENT", IDTRAITEMENT);

        // Exécuter la requête
        return query.exec();

 }





//exportation PDF
 bool traitementC::exporterPDF(const QString& fileName, QSqlQueryModel* model, const QModelIndex& selectedIndex)
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

     // Create HTML for the selected row
     QString html = "<html><head><style>"
                    "table { border-collapse: collapse; width: 100%; }"
                    "th, td { border: 1px solid #dddddd; text-align: left; padding: 4px; font-size: 80%; }"
                    "th { background-color: #31a4b4; color: white; width: 30%; }"
                    "td { background-color: #5bab99; color: black; width: 70%; }"
                    "</style></head><body>";

     // Add the logo and title
     html += "<div style='position: fixed; top: 10px; left: 150px;'>";
     html += "<img src='C:/Users/malek/Desktop/traitement/logoPhysioform.png' alt='logo' style='width: 50px; height: auto;'>";
     html += "</div>";
     html += "<div style='position: fixed; top: 10px; right: 10px;'>";
     html += "<h1 style='font-size: 24px; text-align: left; color: #31a4b4;'>Traitement</h1>";
     html += "</div>";

     // Add table header
     html += "<table style='margin-left: 200px;'>"; // Adjust margin to move content to the right
     html += "<tr><th>Champ</th><th>Valeur</th></tr>";

     // Add selected row data
     for (int col = 0; col < model->columnCount(); ++col) {
         QString fieldName = model->headerData(col, Qt::Horizontal).toString();
         QString fieldValue = model->data(selectedIndex.sibling(selectedIndex.row(), col)).toString();
         html += "<tr><td>" + fieldName + "</td><td>" + fieldValue + "</td></tr>";
     }

     html += "</table></body></html>";

     // Set the HTML content in the document
     doc.setHtml(html);

     // Draw the document contents
     QRect rect = QRect(100, 100, 600, 800); // Example rectangle for drawing contents
     doc.drawContents(&painter, rect);

     return true;
 }




 QSqlQueryModel* traitementC::sortTraitement(QString sortingCriteria)
 {
     QSqlQueryModel *model = new QSqlQueryModel();
     QString queryString = "SELECT * FROM TRAITEMENT ORDER BY ";

     if (sortingCriteria == "ID") {
         queryString += "IDTRAITEMENT";
     } else if (sortingCriteria == "Titre [A-Z]") {
         queryString += "TITRE";
     } else if (sortingCriteria == "Plus Récent") {
         queryString += "DEBUTTRAITEMENT DESC";
     } else if (sortingCriteria == "Plus Ancien") {
         queryString += "DEBUTTRAITEMENT";
     } else if (sortingCriteria == "Motif de consultation [A-Z]") {
         queryString += "MOTIFCONSULTATION";
     }

     model->setQuery(queryString);
     return model;
 }



 QSqlQueryModel* traitementC::searchTraitement(const QString& searchTerm)
 {
     QSqlQueryModel *model = new QSqlQueryModel();


     QString queryString = "SELECT * FROM TRAITEMENT WHERE "
                           "IDTRAITEMENT LIKE :searchTerm OR "
                           "TITRE LIKE :searchTerm OR "
                           "DEBUTTRAITEMENT LIKE :searchTerm OR "
                           "FINTRAITEMENT LIKE :searchTerm OR "
                           "MOTIFCONSULTATION LIKE :searchTerm OR "
                           "EXERCICES LIKE :searchTerm";

     QSqlQuery query;
     query.prepare(queryString);
     query.bindValue(":searchTerm", "%" + searchTerm + "%");
     query.exec();

     model->setQuery(query);

     return model;
 }


 QStringList traitementC::getExercicesById(const QString &IDTRAITEMENT)
 {
     QSqlQuery query;
     query.prepare("SELECT EXERCICES FROM TRAITEMENT WHERE IDTRAITEMENT = :IDTRAITEMENT");
     query.bindValue(":IDTRAITEMENT", IDTRAITEMENT);
     query.exec();

     QStringList exercises;
     if (query.next()) {
         QString exercicesString = query.value(0).toString();
         exercises = exercicesString.split(',');

         // Remove empty parts from the list
         exercises.removeAll(QString());
     }

     return exercises;
 }



bool traitementC::exporterExcel(const QString& fileName, QSqlQueryModel* model)
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





void traitementC::fetchTreatmentData()
{



    QSqlQuery query;
    query.prepare("SELECT * FROM TRAITEMENT WHERE IDTRAITEMENT = ?");
    query.addBindValue(IDTRAITEMENT); // Assuming IDTRAITEMENT is a member variable
    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }


    if (query.next()) {

        TITRE = query.value("TITRE").toString();
        DEBUTTRAITEMENT = query.value("DEBUTTRAITEMENT").toDate();
        FINTRAITEMENT = query.value("FINTRAITEMENT").toDate();
        MOTIFCONSULTATION = query.value("MOTIFCONSULTATION").toString();
        EXERCICES = query.value("EXERCICES").toString();
        VIDEOTRAITEMENT = query.value("VIDEOTRAITEMENT").toString();
    } else {
        qDebug() << "No record found for IDTRAITEMENT:" << IDTRAITEMENT;

    }
}




