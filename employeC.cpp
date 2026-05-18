#include "employeC.h"
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
employeC::employeC() {
    idemp = "";
    nomemploye = "";
    prenomemploye= "";
    //DATEN = QDate::fromString("00/00/0000", "dd/MM/yyyy");
    emailemploye = "";
    numtele = "";
    mdpemploye = "";
    sexeemploye = "";
    specialiteemploye = "";

}
employeC::employeC(QString idemp, QString nomemploye, QString prenomemploye, QString mdpemploye, QString emailemploye, QString numtele, QString sexeemploye, QString specialiteemploye)
{
    this->idemp = idemp;
    this->nomemploye = nomemploye;
    this->prenomemploye = prenomemploye;
    this->mdpemploye = mdpemploye;
    this->emailemploye = emailemploye;
    this->numtele = numtele;

    this->sexeemploye = sexeemploye;
    this->specialiteemploye = specialiteemploye;
}


bool employeC::ajouter_employe()
{
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYEC(idemp, nomemploye, prenomemploye, mdpemploye, emailemploye, numtele, sexeemploye, specialiteemploye)"
                     "VALUES(:idemp, :nomemploye, :prenomemploye, :mdpemploye, :emailemploye, :numtele,  :sexeemploye, :specialiteemploye )");
    query.bindValue(":idemp",idemp);
    query.bindValue(":nomemploye",nomemploye);
    query.bindValue(":prenomemploye",prenomemploye);
    query.bindValue(":mdpemploye",mdpemploye);
    query.bindValue(":emailemploye",emailemploye);
    query.bindValue(":numtele",numtele);
    //query.bindValue(":adresseemploye",adresseemploye);
    query.bindValue(":sexeemploye",sexeemploye);
    query.bindValue(":specialiteemploye",specialiteemploye);


    return query.exec();

}

QSqlQueryModel * employeC::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employeC ORDER BY idemp");

    return model;
}



bool employeC::idexist(const QString &id)const
{
    QSqlQuery query;
    query.prepare("SELECT IDEMP FROM EMPLOYEC WHERE IDEMP= :IDEMP");
    query.bindValue(":IDEMP",id);
    if (query.exec()&& query.next())
    {
        return true;
    }
    return false;

}


int employeC::verifierAI(const QString &id) const {
    QSqlQuery query;
    query.prepare("SELECT IDEMP FROM EMPLOYEC WHERE IDEMP = :IDEMP");
    query.bindValue(":IDEMP", id);
    if (query.exec() && query.next()) {
        // Record found, ID exists
        return true;
    } else {
        // Record not found, ID does not exist
        return false;
    }
}







bool employeC:: supprimer_employe(QString idemp)
{
    QSqlQuery query;

        query.prepare("Delete from EMPLOYEC where IDEMP=:IDEMP");
        query.bindValue(0, idemp);
   return query.exec();
}

bool employeC::modifier_employe()
{
    if (idemp.isEmpty()) {
            QMessageBox::information(nullptr, "Information", "Le champ idemp est vide.");
            return false;
        } else if (!isValididemp(idemp)) {
            QMessageBox::information(nullptr, "Information", "Le idemp doit être un numéro de 6 chiffres.");
            return false;
        }
    QSqlQuery checkQuery;
      checkQuery.prepare("SELECT COUNT(*) FROM employeC WHERE idemp = :idemp");
      checkQuery.bindValue(":idemp", idemp);
      if (!checkQuery.exec()) {
          QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification du idemp.");
          return false;
      }
      checkQuery.next();
      int count = checkQuery.value(0).toInt();
      if (count == 0) {
          QMessageBox::information(nullptr, "Information", "Le idemp spécifié n'existe pas dans la base de données.");
          return false;
      }
    if (!numtele.isEmpty() && !isValidPhoneNumber(numtele)) {
        QMessageBox::information(nullptr, "Information", "Le numéro de téléphone doit être composé de chiffres uniquement et avoir une longueur valide.");
        return false;
    }
    if (!emailemploye.isEmpty() && !validateemailemploye(emailemploye)) {
        QMessageBox::information(nullptr, "Information", "Veuillez vérifier votre adresseemploye e-mail.");
        return false;
    }
    QSqlQuery query;
    QString queryString = "UPDATE employeC SET ";
    if (!nomemploye.isEmpty()) queryString += "nomemploye = :nomemploye, ";
    if (!prenomemploye.isEmpty()) queryString += "prenomemploye = :prenomemploye, ";
    if (mdpemploye.isEmpty()) queryString += "mdpemploye = :mdpemploye, ";
    if (!emailemploye.isEmpty()) queryString += "emailemploye = :emailemploye, ";
    if (!numtele.isEmpty()) queryString += "numtele = :numtele, ";
    //if (!adresseemploye.isEmpty()) queryString += "adresseemploye = :adresseemploye, ";
    if (!sexeemploye.isEmpty()) queryString += "sexeemploye = :sexeemploye, ";
    if (!specialiteemploye.isEmpty()) queryString += "specialiteemploye = :specialiteemploye, ";
    queryString.chop(2);
    queryString += " WHERE idemp = :idemp";
    query.prepare(queryString);
    if (!nomemploye.isEmpty()) query.bindValue(":nomemploye", nomemploye);
    if (!prenomemploye.isEmpty()) query.bindValue(":prenomemploye", prenomemploye);
    if (mdpemploye.isEmpty()) query.bindValue(":mdpemploye", mdpemploye);
    if (!emailemploye.isEmpty()) query.bindValue(":emailemploye", emailemploye);
    if (!numtele.isEmpty()) query.bindValue(":numtele", numtele);
    //if (!adresseemploye.isEmpty()) query.bindValue(":adresseemploye", adresseemploye);
    if (!sexeemploye.isEmpty()) query.bindValue(":sexeemploye", sexeemploye);
    if (!specialiteemploye.isEmpty()) query.bindValue(":specialiteemploye", specialiteemploye);
    query.bindValue(":idemp", idemp);
    return query.exec();
}

QSqlQueryModel* employeC::afficher(QString idemp)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM employeC WHERE idemp = :idemp");
    query.bindValue(":idemp", idemp);
    if(query.exec())
    {
        model->setQuery(query);
        if (model->rowCount() > 0) {
            model->setHeaderData(0, Qt::Horizontal, QObject::tr("idemp du employe"));
            model->setHeaderData(8, Qt::Horizontal, QObject::tr("nomemploye"));
            model->setHeaderData(3, Qt::Horizontal, QObject::tr("prenomemploye"));
            model->setHeaderData(6, Qt::Horizontal, QObject::tr("mdpemploye"));
            model->setHeaderData(5, Qt::Horizontal, QObject::tr("emailemploye"));
            model->setHeaderData(1, Qt::Horizontal, QObject::tr("numtele"));
            model->setHeaderData(4, Qt::Horizontal, QObject::tr("adresseemploye"));
            model->setHeaderData(7, Qt::Horizontal, QObject::tr("sexeemploye"));
            model->setHeaderData(7, Qt::Horizontal, QObject::tr("specialiteemploye"));
        }
    }
    return model;
}

bool employeC::recherche(QString idemp, QSqlQueryModel*& model)
{
    // Vérification si le idemp est vide
    if (idemp.isEmpty()) {
        QMessageBox::information(nullptr, "Information", "Le champ idemp est vide.");
        return false;
    }
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM employeC WHERE idemp = :idemp");
    checkQuery.bindValue(":idemp", idemp);
    if (!checkQuery.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la vérification du idemp.");
        return false;
    }
    checkQuery.next();
    int count = checkQuery.value(0).toInt();
    if (count == 0) {
        QMessageBox::information(nullptr, "Information", "Le idemp spécifié n'existe pas dans la base de données.");
        return false;
    }
    model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM employeC WHERE idemp = :idemp");
    query.bindValue(":idemp", idemp);
    if (!query.exec()) {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la récupération des données.");
        return false;
    }
    model->setQuery(query);
    return true;
}



bool employeC::exporterPDF(const QString& fileName, QSqlQueryModel* model)//genere une table html a partir des donn du mod sql puis dessine cette table dans un fichi pdf
{
    QPrinter printer(QPrinter::PrinterResolution);//crestion d'une instance a partir du Qprunteer
    printer.setOutputFormat(QPrinter::PdfFormat);//le nom du ficher de sortie
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

void employeC::genererStatistiquessexeemploye(QSqlQueryModel* model)
{
   /* QPieSeries *series = new QPieSeries();//creation d'une nouv ser de donn

        int countHomme = 0;//init des comp
        int countFemme = 0;

        for (int row = 0; row < model->rowCount(); ++row) {
            QString sexeemploye = model->data(model->index(row, 6)).toString(); // Supposons que la colonne du sexeemploye est à l'index 9

            if (sexeemploye.toLower() == "homme") {
                countHomme++;
            } else if (sexeemploye.toLower() == "femme") {
                countFemme++;
            }
        }
        int totalCount = countHomme + countFemme;
        qreal percentHomme = (static_cast<qreal>(countHomme) / totalCount) * 100.0;
        qreal percentFemme = (static_cast<qreal>(countFemme) / totalCount) * 100.0;
        series->append("Hommes (" + QString::number(percentHomme, 'f', 1) + "%)", countHomme);
        series->append("Femmes (" + QString::number(percentFemme, 'f', 1) + "%)", countFemme);
        QChart *chart = new QChart();//presentation du diag
        chart->addSeries(series);
        chart->setTitle("Répartition des employes par sexeemploye");
        chart->legend()->setAlignment(Qt::AlignRight);
        QFont font;
        font.setPixelSize(16); // Taille de police 16 pixels
        chart->setTitleFont(font);
        chart->legend()->setFont(font);
        QChartView *chartView = new QChartView(chart);//afficher le diagra
        chartView->setRenderHint(QPainter::Antialiasing);

        // Affichage du diagramme dans une fenêtre ou un widget
        chartView->resize(800, 600); // Ajustez la taille de la fenêtre selon vos besoins
        chartView->activateWindow();
        chartView->show();
        chartView->raise();*/
}

QSqlQueryModel* employeC::afficher_tri(QString sortingCriteria)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryString = "SELECT * FROM employeC ORDER BY ";

    //model->setQuery("SELECT * FROM employeC ORDER BY LOWER(nomemploye) ASC");
    if(sortingCriteria =="idemp")
    /*model->setHeaderData(0, Qt::Horizontal, QObject::tr("idemp"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("nomemploye"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("prenomemploye"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATEN"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("emailemploye"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("numtele"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("adresseemploye"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("sexeemploye"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("specialiteemploye"));*/
    {
             queryString += "idemp";
         } else if (sortingCriteria == "nomemploye [A-Z]") {
             queryString += "nomemploye";
         } else if (sortingCriteria == "Plus Récent") {
             queryString += "ajoutemploye DESC";

         }

         model->setQuery(queryString);

    return model;
}
bool employeC::mdpCorrect(const QString &id, const QString &mdp)const {
    QSqlQuery query;
    query.prepare("SELECT mdpemploye FROM EMPLOYEC WHERE idemp = :idemp");
    query.bindValue(":idemp", id);

    if (query.exec() && query.next()) {
        QString motDePasseDansLaBase = query.value(0).toString();
        return (motDePasseDansLaBase == mdp);
    } else {
        return false;
    }
}

