#include "paiement.h"
// #include "arduino.h"
#include "ui_paiement.h"
#include "paiementC.h"
#include "connexion.h"
#include <QMessageBox>
#include <QDate>
#include <QString>
#include <QSqlDatabase>
#include <QDebug>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QAxObject>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QFile>
#include <QDebug>
#include <QDesktopServices>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDialog>

//arduino arduinoInstance;
paiement::paiement(QWidget *parent): QDialog(parent), ui(new Ui::paiement){
    ui->setupUi(this);
    QSqlQueryModel *model = P.afficher_paiement();
    ui->tableView_ali->setModel(model);
    //A.connect_arduino();
    connect(ui->tableView_ali, &QTableView::clicked, this, &paiement::on_TableRow_Clicked);
    // connect(ui->sendButton, &QPushButton::clicked, this, &paiement::processIdPaiement);

}

paiement::~paiement()
{
    delete ui;
}

void paiement::clearFields() {
    ui->IDpaiement->clear();
    ui->nom->clear();
    ui->datePai->setDate(QDate::currentDate());
    ui->telephone->clear();
    ui->typePai->setCurrentIndex(0);
    ui->montant_2->clear();
    ui->statut->setCurrentIndex(0);
    ui->idPatient->clear();

}




void paiement::on_ajoutPaiement_clicked()
{
    //recuperation des informations saisies
    int IDPAIEMENT=ui->IDpaiement->text().toInt();
    QString PATIENT=ui->nom->text();
    QDate DATEPA= ui->datePai->date();
    QString TEL = ui->telephone->text();
    QString TYPE= ui->typePai->currentText();
    QString MONTANT = ui->montant_2->text();
    QString STATUT = ui->statut->currentText();

    paiementC P (IDPAIEMENT, PATIENT, DATEPA, TEL, TYPE, MONTANT, STATUT);



    bool test = P.ajouter();
    if(test)
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                QObject::tr("Ajout Effectué\n"
                            "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                QObject::tr("Ajout non Effectué\n"
                            "Click Cancel to exit."), QMessageBox::Cancel);
ui->tableView_ali->setModel(P.afficher_paiement());
clearFields();
}



void paiement::on_tableauSupprimer_clicked()
{
    QModelIndex currentIndex = ui->tableView_ali->currentIndex();

    if (!currentIndex.isValid()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("Veuillez sélectionner une ligne à supprimer."), QMessageBox::Ok);
        return;
    }

    int IDPAIEMENT = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(currentIndex.row(), 0)).toInt();
    QString PATIENT = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(currentIndex.row(), 1)).toString();
    //QDate DATEPA = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(currentIndex.row(), 2)).toDate();
    QString TEL = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(currentIndex.row(), 3)).toString();
    QString TYPE = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(currentIndex.row(), 4)).toString();
    QString MONTANT = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(currentIndex.row(), 5)).toString();
    QString STATUT = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(currentIndex.row(), 6)).toString();

    bool test = Ptmp.supprimer_paiement(IDPAIEMENT);
    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Suppression effectuée\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not Ok"),
                              QObject::tr("Suppression non effectuée.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }
    clearFields();

    ui->tableView_ali->setModel(P.afficher_paiement());
}





void paiement::on_supprimerPai_clicked()
{
    int IDPAIEMENT=ui->idPatient->text().toInt();
    bool test = Ptmp.supprimer_paiement(IDPAIEMENT);
    if (test)
       {
           QMessageBox::information(nullptr,QObject::tr("ok"),
                                    QObject::tr("Suppression effectué\n""click cancel to exit."),QMessageBox::Cancel);



    }
       else
           QMessageBox::critical(nullptr,QObject::tr("not ok"),
                                 QObject::tr("Suppressio non effectué\n""click cancel to exit."),QMessageBox::Cancel);

    ui->tableView_ali->setModel(P.afficher_paiement());
   clearFields();
}


void paiement::on_majPaiement_clicked()
{
    int IDPAIEMENT=ui->IDpaiement->text().toInt();
    QString PATIENT=ui->nom->text();
    QDate DATEPA= ui->datePai->date();
    QString TEL = ui->telephone->text();
    QString TYPE= ui->typePai->currentText();
    QString MONTANT = ui->montant_2->text();
    QString STATUT = ui->statut->currentText();

    paiementC P (IDPAIEMENT, PATIENT, DATEPA, TEL, TYPE, MONTANT, STATUT);
bool test=P.modifier_paiement();
if (test)
    {
        QMessageBox::information(nullptr,QObject::tr("ok"),
                                 QObject::tr("modify effectué\n""click cancel to exit."),QMessageBox::Cancel);
        ui->tableView_ali->setModel(P.afficher_paiement());
        if (IDPAIEMENT < 0) ui->IDpaiement->clear();
        if (!PATIENT.isEmpty()) ui->nom->clear();
        if (!TEL.isEmpty()) ui->telephone->clear();
        if (!TYPE.isEmpty()) ui->typePai->clear();
        if (!MONTANT.isEmpty()) ui->montant_2->clear();
        if (!STATUT.isEmpty()) ui->statut->clear();
   }
    else
        QMessageBox::critical(nullptr,QObject::tr("not ok"),
                              QObject::tr("modify  non effectué\n""click cancel to exit."),QMessageBox::Cancel);
clearFields();
}

void paiement::on_recherchePaiement_clicked()
{
    int IDPAIEMENT=ui->idPatient->text().toInt();
    QSqlQueryModel* searchResultModel = nullptr;
    bool searchSuccess = P.recherche_paiement(IDPAIEMENT, searchResultModel);

    if (searchSuccess && searchResultModel && searchResultModel->rowCount() > 0) {
        ui->tableView_ali->setModel(searchResultModel);
        ui->idPatient->clear();
    } else {
        if (!searchSuccess) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche du paiement.");
        } else {
            QMessageBox::information(this, "Information", "Aucun patient trouvé avec ce ID.");
        }
        // Nettoyer la vue de la table si aucun résultat n'est trouvé
        ui->tableView_ali->setModel(nullptr);
    }

    clearFields();
}

void paiement::on_pdfPai_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            paiementC P;
            QSqlQueryModel* model = P.afficher_paiement();
            // Ajoutez des instructions de débogage pour afficher le contenu du modèle
            qDebug() << "Nombre de lignes dans le modèle :" << model->rowCount();
            qDebug() << "Nombre de colonnes dans le modèle :" << model->columnCount();
            for (int row = 0; row < model->rowCount(); ++row) {
                QString rowData;
                for (int column = 0; column < model->columnCount(); ++column) {
                    QModelIndex index = model->index(row, column);
                    rowData += model->data(index).toString() + "\t";
                }
                qDebug() << "Ligne " << row << ":" << rowData;
            }
            if (P.PDF_paiement(fileName, model)) {
                QMessageBox::information(this, "Exportation PDF", "Les données ont été exportées en PDF avec succès !");
            } else {
                QMessageBox::warning(this, "Exportation PDF", "Échec de l'exportation en PDF.");
            }
        }

}

void paiement::on_TRIpai_clicked()
{
      ui->tableView_ali->setModel(P.tri_paiement());
}


void paiement::on_statPai_clicked()
{
    QSqlQueryModel *model = P.afficher_paiement();

        P.stat_paiement(model);

}


void paiement::on_historique_clicked()
{

    QString patientName = ui->nom->text();
    QSqlQueryModel* historyModel = P.afficher(patientName);
    ui->tableView_ali->setModel(historyModel);
}


void paiement::on_TableRow_Clicked(const QModelIndex &index)
{

    if (index.isValid()) {

        QString IDPAIEMENT = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(index.row(), 0)).toString();
        QString PATIENT = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(index.row(), 1)).toString();
        QDate DATEPA = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(index.row(), 2)).toDate();
        QString TEL = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(index.row(), 3)).toString();
        QString TYPE = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(index.row(), 4)).toString();
        QString MONTANT = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(index.row(), 5)).toString();
        QString STATUT = ui->tableView_ali->model()->data(ui->tableView_ali->model()->index(index.row(), 5)).toString();


        ui->IDpaiement->setText(IDPAIEMENT);
        ui->nom->setText(PATIENT);
        ui->datePai->setDate(DATEPA);
        ui->telephone->setText(TEL);
        ui->typePai->currentText();
        ui->montant_2->setText(MONTANT);
        ui->statut->currentText();
    }
}


void paiement::on_excelPai_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en Excel", "", "Fichiers Excel (*.xlsx)");
    if (!fileName.isEmpty()) {
        QSqlQueryModel* model = P.afficher_paiement();

        if (P.exporterExcel(fileName, model)) {
            QMessageBox::information(this, "Exportation Excel", "Les données ont été exportées en Excel avec succès !");
        } else {
            QMessageBox::warning(this, "Exportation Excel", "Échec de l'exportation en Excel.");
        }
    }
}


void paiement::replyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        // Afficher un message d'erreur
        QMessageBox::warning(this, "Erreur", "Une erreur est survenue lors de l'envoi du SMS : " + reply->errorString());
    }
    // Libérer la mémoire
    reply->deleteLater();
}
void paiement::envoyerSMS(const QString &destinataire, const QString &message)
{
    // SID et auth token de Twilio
    QString sid = "YOUR_TWILIO_ACCOUNT_SID";
    QString authToken = "YOUR_TWILIO_AUTH_TOKEN";

    // Construire l'URL de l'API Twilio
    QString url = "https://api.twilio.com/2010-04-01/Accounts/" + sid + "/Messages.json";

    // Créer le gestionnaire de réseau
    QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager(this);
    networkAccessManager->setNetworkAccessible(QNetworkAccessManager::Accessible); // Définir l'accessibilité du réseau
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    // Construire les données de la requête
    QByteArray postData;
    postData.append("To=" + destinataire.toUtf8());
    postData.append("&From=+12056273550");
    postData.append("&Body=" + message.toUtf8());

    // Créer la requête HTTP
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(sid).arg(authToken).toUtf8()).toBase64());


    // Envoyer la requête
    networkAccessManager->post(request, postData);

    // Afficher un message de succès
    QMessageBox::information(this, "Envoi SMS", "Le SMS a été envoyé avec succès.");
}

void paiement::on_SMSpai_clicked()
{
    QString destinataire = "+21654338469"; // Numéro de téléphone du destinataire
    QString message = ui->smscontenu->text();

      envoyerSMS(destinataire, message);
}

void   paiement::on_pb_historiqueAli_clicked()
{

    QString cheminFichier = "C:/Users/alita/Desktop/paiement/historique.txt";
    QFile file( cheminFichier);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Afficher un message d'erreur si le fichier ne peut pas être ouvert
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier d'historique.");
        return;
    }

    // Lire le contenu du fichier
    QTextStream in(&file);
    QString historique = in.readAll();

    // Fermer le fichier
    file.close();

    // Afficher le contenu de l'historique dans une boîte de dialogue
    QMessageBox msgBox;
    msgBox.setWindowTitle("Historique");
    msgBox.setText(historique);
    msgBox.exec();
}
void  paiement::afficherHistoriquefer()
{
    // Spécifier le chemin complet du fichier historique
    QString cheminFichier = "C:/Users/alita/Desktop/paiement/historique.txt";

    // Ouvrir le fichier en écriture (mode Append)
    QFile file(cheminFichier);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qDebug() << "Erreur lors de l'ouverture du fichier historique.";
        return;
    }

    // Créer un flux texte pour écrire dans le fichier
    QTextStream out(&file);

    // Obtenir la date et l'heure actuelles
    QDateTime currentDateTime = QDateTime::currentDateTime();

    // Écrire dans le fichier
    out << "Actions enregistrées à " << currentDateTime.toString() << "\n";

    // Fermer le fichier
    file.close();

    // Afficher un message pour indiquer que les actions ont été enregistrées dans l'historique
    QMessageBox::information(this, "Historique", "Les actions ont été enregistrées dans l'historique." );
    QDesktopServices::openUrl(QUrl::fromLocalFile(cheminFichier));
}


/*
void paiement::processIdPaiement() {
    // Read IDPAIEMENT from Arduino
    QByteArray receivedData = A.read_from_arduino();
    QString receivedString = QString::fromUtf8(receivedData).trimmed();
    qDebug() << "Le ID est "<< receivedString;

    // Perform database query to check if IDPAIEMENT exists
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PAIEMENT WHERE IDPAIEMENT = :IDPAIEMENT");
    query.bindValue(":IDPAIEMENT", receivedString);

    if (!query.exec()) {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    // Read query result
    query.next();
    int count = query.value(0).toInt();

    // Send result to Arduino
    if (count > 0) {
        A.write_to_arduino("Valid");
    } else {
        A.write_to_arduino("Not Valid");
    }
}
*/




