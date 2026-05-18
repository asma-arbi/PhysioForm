#include "reservation.h"
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
#include <QDateTime>
#include <QDateEdit>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
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
reservationC::reservationC() {
    ID = "";
    CIN = "";
    DATER = QDateTime::currentDateTime();//REGION
    TYPRE = "";
    MODEC = "";
    REGION = "";
}
reservationC::reservationC(QString CIN, QDateTime DATER, QString TYPRE, QString MODEC ,QString REGION)
{
    this->CIN = CIN;
    this->DATER = DATER;// Obtenir la date du QDateEdit
    this->TYPRE = TYPRE;
    this->MODEC = MODEC;
    this->REGION = REGION;
}
reservationC::reservationC(QString id ,QString CIN, QDateTime DATER, QString TYPRE, QString MODEC ,QString REGION)
{
    this->ID=id;
    this->CIN = CIN;
    this->DATER = DATER;// Obtenir la date du QDateEdit
    this->TYPRE = TYPRE;
    this->MODEC = MODEC;
    this->REGION = REGION;
}


bool reservationC::ajouter_reservation()
{
    QString cin = getCIN();
        if(cin.length() != 8 || !cin.toInt()) { // Vérifie la longueur et si la conversion en entier est valide
            qDebug() << "Le CIN doit être une chaîne de 8 chiffres.";
            return false;
        }
    QSqlQuery query;
    query.prepare("INSERT INTO RESERVATIONC(CIN, DATER, TYPRE, MODEC , REGION)"
                     "VALUES( :CIN, :DATER, :TYPRE, :MODEC, :REGION)");
    query.bindValue(":CIN",getCIN());
    query.bindValue(":DATER",getDate());
    query.bindValue(":TYPRE",getTYPRE());
    query.bindValue(":MODEC",getMODEC());
    query.bindValue(":REGION",getREGION());


    return query.exec();

}


QSqlQueryModel * reservationC::affichertous()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID, CIN, DATER, TYPRE, MODEC, REGION FROM reservationC ");

    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex dateIndex = model->index(i, 6); // Index de la colonne contenant la date
        QString dateString = dateIndex.data().toDate().toString("ddd MMMM d yy"); // Extraction et formatage de la date
        model->setData(dateIndex, dateString); }

    // Configuration des en-têtes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATER"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TYPRE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("MODEC"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("REGION")); // Assuming 6 is the column index of Loca

    return model;
}


bool reservationC:: supprimer_reservation(QString ID)
{   // Vérification si le ID est vide
    if (ID.isEmpty()) {
        QMessageBox::information(nullptr, "Information", "Le champ ID est vide.");
        return false;
    }

    // Vérification si le ID existe dans la base de données
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM reservationC WHERE ID = :ID");
    checkQuery.bindValue(":ID", ID);
    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification du ID.");
        return false;
    }
    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::information(nullptr, "Information", "Le ID spécifié n'existe pas dans la base de données.");
        return false;
    }

    // Requête de suppression
    QSqlQuery query;
    query.prepare("DELETE FROM reservationC WHERE ID = :ID");
    query.bindValue(":ID", ID);

    return query.exec();

}

bool reservationC::modifier_reservation()
{
    QString cin = getCIN();
        if(cin.length() != 8 || !cin.toInt()) { // Vérifie la longueur et si la conversion en entier est valide
            qDebug() << "Le CIN doit être une chaîne de 8 chiffres.";
            return false;
        }

    QSqlQuery query;
    query.prepare("UPDATE RESERVATIONC SET CIN = :CIN, DATER = :DATER, TYPRE = :TYPRE, MODEC = :MODEC, REGION = :REGION WHERE ID = :id");
    query.bindValue(":CIN", getCIN()); // Conversion en QString
    query.bindValue(":DATER", getDate()); // Conversion en QString
    query.bindValue(":TYPRE", getTYPRE());
    query.bindValue(":MODEC", getMODEC());
    query.bindValue(":REGION", getREGION());

    query.bindValue(":id", getID());
    if (query.exec()) {
        qDebug() << "Modification de la réservation sans projet réussie.";
        return true;
    } else {
        qDebug() << "Erreur lors de la modification dans la base de données :" ;
        return false;
    }
}

QSqlQueryModel* reservationC::afficher(QString ID)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM reservationC WHERE ID = :ID");
    query.bindValue(":ID", ID);
    if(query.exec())
    {
        model->setQuery(query);
        if (model->rowCount() > 0) {
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
            model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATER"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("TYPRE"));
            model->setHeaderData(4, Qt::Horizontal, QObject::tr("MODEC"));
            model->setHeaderData(5, Qt::Horizontal, QObject::tr("REGION"));
        }
    }
    return model;
}

bool reservationC::recherche(QString CIN, QSqlQueryModel*& model)
{
    // Vérification si le CIN est vide
    if (CIN.isEmpty()) {
        QMessageBox::information(nullptr, "Information", "Le champ CIN est vide.");
        return false;
    }
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM reservationC WHERE CIN = :CIN");
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
    model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM reservationC WHERE CIN = :CIN");
    query.bindValue(":CIN", CIN);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la récupération des données.");
        return false;
    }
    model->setQuery(query);
    return true;
}
QSqlQueryModel * reservationC::recherche(QString recherche)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM reservationC WHERE CIN LIKE '%"+recherche+"%' ");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATER"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TYPRE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("MODEC"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("REGION"));
    return model;
}


bool reservationC::exporterPDF(const QString& fileName, QSqlQueryModel* model)
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
                    QString formattedDate = dateTime.toString("ddd MMMM d yy");
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

void reservationC::genererStatistiquesREGION(QSqlQueryModel* model)
{
    QPieSeries *series = new QPieSeries();

    QMap<QString, int> regionCount;

    // Initialisation des compteurs pour chaque région
    regionCount["Bizerte"] = 0;
    regionCount["Ariana"] = 0;
    regionCount["Ben Arous"] = 0;
    regionCount["Nabeul"] = 0;
    regionCount["Zaghouan"] = 0;
    regionCount["Sousse"] = 0;

    for (int row = 0; row < model->rowCount(); ++row) {
        QString region = model->data(model->index(row, 5)).toString(); // Supposons que la colonne de la région est à l'index 3
        regionCount[region]++;
    }

    int totalCount = model->rowCount();

    for (auto it = regionCount.begin(); it != regionCount.end(); ++it) {
        qreal percent = (static_cast<qreal>(it.value()) / totalCount) * 100.0;
        series->append(it.key() + " (" + QString::number(percent, 'f', 1) + "%)", it.value());
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des reservations par REGION");
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

QSqlQueryModel* reservationC::afficher_tri()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT * FROM reservationC ORDER BY CIN ASC");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATER"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("TYPRE"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("MODEC"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("REGION"));

    return model;
}
