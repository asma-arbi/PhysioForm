#include "reservation.h"
#include "ui_reservation.h"
#include <QSqlDatabase>
#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include "reservationc.h"
#include "connexion.h"
#include <QShowEvent>
#include <QFileDialog>
#include <QDebug>
#include <QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include <QtCharts>
#include <QtCharts/QBarSet>
#include <QSslSocket>
#include <QGeoPositionInfoSource>
#include <QGeoServiceProvider>
#include <QDebug>
#include <QSslError>
#include <QQuickItem>
#include <QtQuickWidgets/QQuickWidget>
#include <QDialog>
#include <QFileDialog>
using namespace QtCharts;

reservation::reservation(QWidget *parent): QDialog(parent), ui(new Ui::reservation){
    ui->setupUi(this);

  //  QSqlQueryModel *model = P.afficher();
  //     ui->tableView_abdo->setModel(model);
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->quickWidget->show();

    auto obj = ui->quickWidget->rootObject();
    connect(this, SIGNAL(setCenter(QVariant, QVariant)), obj, SLOT(setCenter(QVariant, QVariant)));
    connect(this, SIGNAL(addMarker(QVariant, QVariant)), obj, SLOT(addMarker(QVariant, QVariant)));
    connect(ui->quickWidget->rootObject(), SIGNAL(getLocationDetails(double, double)), this, SLOT(getLocationDetails(double, double)));

        emit setCenter(36.8663, 10.1633);
        emit addMarker(36.8663, 10.1633);

        emit setCenter(37.2744, 9.8581);
        emit addMarker(37.2744, 9.8581);

        emit setCenter(36.7489, 10.2490);
        emit addMarker(36.7489, 10.2490);

        emit setCenter(36.4560, 10.7326);
        emit addMarker(36.4560, 10.7326);

        emit setCenter(35.8254, 10.6412);
        emit addMarker(35.8254, 10.6412);

        emit setCenter(36.4007, 10.1440);
        emit addMarker(36.4007, 10.1440);

}

reservation::~reservation()
{
    delete ui;
}
void reservation::clearFields() {
    ui->cin_abdo->clear();
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->type_abdo->setCurrentIndex(0);
     ui->mode_abdo->setCurrentIndex(0);
    ui->region_abdo->setCurrentIndex(0);
    ui->recherche_abdo->clear();
}

void reservation::on_ajout_abdo_clicked()
{
    QString CIN = ui->cin_abdo->text();
    QDateTime DATER = ui->dateTimeEdit->dateTime();
    QString TYPRE = ui->type_abdo->currentText();
    QString MODEC = ui->mode_abdo->currentText();
    QString REGION = ui->region_abdo->currentText();


    reservationC P( CIN, DATER, TYPRE, MODEC, REGION);

    qDebug()<< CIN;
    qDebug() << "DATER:" << DATER.toString("ddd MMMM d yy");
    qDebug()<< TYPRE;
    qDebug()<< MODEC;
    qDebug()<<REGION;

    bool test = P.ajouter_reservation();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\n"
                                             "Click Cancel to exit ."), QMessageBox::Cancel);
       // on_region_clicked(REGION);
    } else {
        QMessageBox::critical(nullptr,QObject::tr("NOT OK"),
                              QObject::tr("Ajout non effectué\n"
                                          "Click Cancel to exit ."), QMessageBox::Cancel);
    }
     ui->tableView_abdo->setModel(P.affichertous());
    clearFields();
}

void reservation::showEvent(QShowEvent *event) {

    QWidget::showEvent(event);

    // Créer une instance de la classe res pour les réservations
reservationC c;    // Créer une instance de la classe evenement pour les événements

    ui->tableView_abdo->setModel(c.affichertous());


}






void reservation::on_btnrechercher_abdo_clicked()
{
    QString CIN = ui->recherche_abdo->text();
    QSqlQueryModel* searchResultModel = nullptr;
    bool searchSuccess = P.recherche(CIN, searchResultModel);

    if (searchSuccess && searchResultModel && searchResultModel->rowCount() > 0) {
        ui->tableView_abdo->setModel(searchResultModel);
        ui->recherche_abdo->clear();
    } else {
        if (!searchSuccess) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche du reservation.");
        } else {
            QMessageBox::information(this, "Information", "Aucun reservation trouvé avec ce CIN.");
        }
        // Nettoyer la vue de la table si aucun résultat n'est trouvé
        ui->tableView_abdo->setModel(nullptr);
    }

    clearFields();
}


void reservation::on_pdf_abdo_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            reservationC P;
            QSqlQueryModel* model = P.affichertous();
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
            if (P.exporterPDF(fileName, model)) {
                QMessageBox::information(this, "Exportation PDF", "Les données ont été exportées en PDF avec succès !");
            } else {
                QMessageBox::warning(this, "Exportation PDF", "Échec de l'exportation en PDF.");
            }
        }

}

void reservation::on_trieer_abdo_clicked()
{
      ui->tableView_abdo->setModel(P.afficher_tri());
}



void reservation::on_stat_abdo_clicked()
{
    QSqlQueryModel *model = P.affichertous();

        P.genererStatistiquesREGION(model);

}


void reservation::on_tableView_abdo_doubleClicked(const QModelIndex &index)
{
    if (index.isValid()) {
          reservation c ;


          int row = index.row();
          QString cin = ui->tableView_abdo->model()->index(row, 1).data().toString();
          QDateTime date = ui->tableView_abdo->model()->index(row, 2).data().toDateTime();
          QString type = ui->tableView_abdo->model()->index(row, 3).data().toString();
          QString modec = ui->tableView_abdo->model()->index(row, 4).data().toString();
          QString region = ui->tableView_abdo->model()->index(row, 5).data().toString();




          // Afficher un message de confirmation
          QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirmation de sélection",
              "Confirmez-vous la sélection de cette réservation ?", QMessageBox::Yes | QMessageBox::No);

          if (reply == QMessageBox::Yes) {
              ui->ajout_abdo->setEnabled(false);
              ui->pushButton->setEnabled(false);



              ui->cin_abdo->setText(cin);
              ui->dateTimeEdit->setDateTime(date);
              ui->type_abdo->setCurrentText(type);
              ui->mode_abdo->setCurrentText(modec);
              ui->region_abdo->setCurrentText(region);


              // Désactiver les widgets pour éviter la modification accidentelle

              ui->ajout_abdo->setEnabled(false);
              ui->pushButton->setEnabled(false);
          } else {
              qDebug() << "Sélection annulée.";
          }
      } else {
          qDebug() << "Données invalides ou non convertibles";
      }
}
// supprimer

void reservation::on_pushButton_clicked()
{
    reservationC c;
        QModelIndex index = ui->tableView_abdo->currentIndex();

        if (index.isValid()) {
            QString idReservation = ui->tableView_abdo->model()->data(ui->tableView_abdo->model()->index(index.row(), 0)).toString();
            QMessageBox::StandardButton confirmation = QMessageBox::question(this, "Confirmation de suppression",
                                            "Êtes-vous sûr de vouloir supprimer cette réservation ?",
                                            QMessageBox::Yes | QMessageBox::No);
            if (confirmation == QMessageBox::Yes) {
                if (c.supprimer_reservation(idReservation)) {
                    ui->tableView_abdo->setModel(c.affichertous());
                    QMessageBox messageBox;
                    messageBox.setStyleSheet("background-color: rgb(255, 250, 250);");
                    messageBox.setWindowTitle(QObject::tr("Suppression réussie"));
                    messageBox.setText(QObject::tr("La réservation a été supprimée avec succès.\n"
                                                   ""));
                    messageBox.addButton(QMessageBox::Cancel);
                    messageBox.exec();



                } else {
                    qDebug() << "Erreur lors de la suppression de la réservation avec l'ID" << idReservation;
                    QMessageBox messageBox;
                    messageBox.setStyleSheet("background-color: red;");
                    messageBox.critical(this, "Erreur de suppression", "Erreur lors de la suppression de la réservation.");
                }
            } else {
                qDebug() << "Suppression annulée par l'utilisateur.";
            }
        } else {
            qDebug() << "Aucune ligne sélectionnée.";
            QMessageBox messageBox;
            messageBox.setStyleSheet("background-color: yellow;");
            messageBox.warning(this, "Sélection requise", "Veuillez sélectionner une réservation à supprimer.");
        }
}

void reservation::on_modifier_clicked()
{

    QModelIndexList selectedIndexes = ui->tableView_abdo->selectionModel()->selectedIndexes();

    // Vérifier si aucun élément n'est sélectionné dans le tableau
    if (selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner une réservation à modifier.");
        return;
    }

    QModelIndex index = selectedIndexes.first();
    int idReservation = ui->tableView_abdo->model()->data(ui->tableView_abdo->model()->index(index.row(), 0)).toInt();
    qDebug() << "ID de la réservation sélectionnée :" << idReservation;
    QString substring=  QString::number(idReservation);


    QString CIN = ui->cin_abdo->text();
    QDateTime DATER = ui->dateTimeEdit->dateTime();
    QString TYPRE = ui->type_abdo->currentText();
    QString MODEC = ui->mode_abdo->currentText();
    QString REGION = ui->region_abdo->currentText();

    reservationC c(substring, CIN, DATER, TYPRE, MODEC, REGION);


        bool modificationReussie = c.modifier_reservation();

        if (modificationReussie) {
            QMessageBox::information(nullptr, QObject::tr("La modification est validée"),
                                     QObject::tr("bien modifer .\n"
                                                 "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
           clearFields();
           ui->tableView_abdo->setModel(c.affichertous());


        }
        else {
            qDebug()<<" erreur ";
        }
}



//AFFICHER
void reservation::on_pushButton_2_clicked()
{
    reservationC c;
    ui->tableView_abdo->setModel(c.affichertous());
}

void reservation::on_sendMail_clicked()
{
    // SMTP server details
        QString host = "smtp.gmail.com";
        int port = 587; // Adjust port number accordingly

        // Sender and recipient details
        QString sender = "ghorbeloussama97@gmail.com";
        QString recipient = ui->to->text();

        // Message details
        QString subject = ui->subject->text();
        QString body = ui->msg->toPlainText();

        // Establish connection to the SMTP server
        QSslSocket socket;
        socket.connectToHost(host, port);
        if (!socket.waitForConnected()) {
            qDebug() << "Failed to connect to SMTP server:" << socket.errorString();
            return;
        }

        // Wait for server response
        if (!socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }
        qDebug() << socket.readAll(); // Print the initial server response

        // Send EHLO command
        socket.write("EHLO localhost\r\n");
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }
        qDebug() << socket.readAll(); // Print the response after EHLO

        // Send STARTTLS command
        socket.write("STARTTLS\r\n");
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }
        qDebug() << socket.readAll(); // Print the response after STARTTLS

        // Start encryption
        socket.startClientEncryption();
        if (!socket.waitForEncrypted()) {
            qDebug() << "Failed to start encryption:" << socket.errorString();
            return;
        }

        // Send AUTH LOGIN command
        socket.write("AUTH LOGIN\r\n");
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }
        qDebug() << socket.readAll(); // Print the response after AUTH LOGIN

        // Send username in Base64 encoding
        socket.write(QByteArray().append(sender.toUtf8()).toBase64() + "\r\n");
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }
        qDebug() << socket.readAll(); // Print the response after sending username

        // Send password in Base64 encoding
        socket.write(QByteArray().append("qmwo noya vgyz eimm").toBase64() + "\r\n");
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }
        qDebug() << socket.readAll(); // Print the response after sending password

        // Send MAIL FROM command
        socket.write("MAIL FROM:<" + sender.toUtf8() + ">\r\n");
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }
        qDebug() << socket.readAll(); // Print the response after MAIL FROM

        // Send RCPT TO command
        socket.write("RCPT TO:<" + recipient.toUtf8() + ">\r\n");
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }
        qDebug() << socket.readAll(); // Print the response after RCPT TO

        // Send DATA command
        socket.write("DATA\r\n");
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }
        qDebug() << socket.readAll(); // Print the response after DATA

        // Send email headers and body
        socket.write("From: " + sender.toUtf8() + "\r\n");
        socket.write("To: " + recipient.toUtf8() + "\r\n");
        socket.write("Subject: " + subject.toUtf8() + "\r\n");
        socket.write("\r\n");
        socket.write(body.toUtf8() + "\r\n");
        socket.write(".\r\n");
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }
        qDebug() << socket.readAll(); // Print the response after sending email data

        // Send QUIT command
        socket.write("QUIT\r\n");
        if (!socket.waitForBytesWritten()) {
            qDebug() << "Failed to send QUIT command:" << socket.errorString();
            return;
        }

        // Close the connection
        socket.close();
    }


void reservation::on_mailsent_clicked()
{
    reservationC c;
     ui->to->clear();
     ui->msg->clear();
     ui->subject->clear();
}

void reservation::moveMapCenter(double lat, double lng)
{
    emit setCenter(lat, lng);
}

void reservation::placeMarker(double lat, double lng)
{
    emit addMarker(lat, lng);
}
/*
void reservation::on_region_clicked(QString userId)
{
    ui->quickWidget->setSource(QUrl(QStringLiteral("qrc:/map.qml")));
    ui->quickWidget->show();

    auto Obje = ui->quickWidget->rootObject();
    //start position bta3 il map
    connect(this ,SIGNAL(setCenterPosition(QVariant,QVariant)), Obje ,SLOT(setCenterPosition(QVariant,QVariant)));
    //position bta3 il marking
     connect(this ,SIGNAL(setLocationMarking(QVariant,QVariant)), Obje ,SLOT(setLocationMarking(QVariant,QVariant)));


    userId = ui->region_abdo->currentText();

   float longi;
         float  lati;
        //QSqlQuery query( "SELECT * FROM LOCATION WHERE IDL LIKE :userId" );
        //query.bindValue(":userId", userId);
              // int did=query.value(« idl »).toInt();
        //       longi=  query.value( "longitude" ).toFloat();
//                lati=  query.value( "latitude" ).toFloat();
           //    ui->MAPA->setText(QString::number(lati));
         if(userId=="Bizerte")
         {
             longi = 9.8275;  // Longitude of Bizerte, Tunisia
             lati = 37.2953;   // Latitude of Bizerte, Tunisia
         }
         else if (userId == "Ariana") {
             longi = 10.1955; // Longitude of Ariana, Tunisia
             lati = 36.8625;  // Latitude of Ariana, Tunisia
         }
         else if (userId == "Ben Arous")
         {
             longi = 10.1815; // Longitude of Ben Arous, Tunisia
             lati = 36.8065;  // Latitude of Ben Arous, Tunisia
         }
         else if (userId == "Nabeul")
         {
             longi = 10.7341; // Longitude of Nabeul, Tunisia
             lati = 36.4561;  // Latitude of Nabeul, Tunisia
         }
         else if(userId == "Sousse") {
             longi = 10.6085; // Longitude of Sousse, Tunisia
             lati = 35.8272;  // Latitude of Sousse, Tunisia
         }
         else if(userId == "Zaghouan") {
             longi = 9.7892;  // Longitude of Zaghouan, Tunisia
             lati = 36.2056;   // Latitude of Zaghouan, Tunisia
         }


     emit setCenter(longi, lati);
     emit addMarker( longi,lati);

}*/


