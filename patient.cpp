#include "patient.h"
#include "ui_patient.h"
#include <QSqlDatabase>
#include <QDate>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QPixmap>
#include <QItemSelectionModel>
#include <QAudioRecorder>
#include <QTextToSpeech>
#include <QCoreApplication>
#include <QBuffer>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QProcess>
#include <QCamera>
#include <QCameraViewfinder>
#include <QVideoWidget>
#include <QCameraInfo>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QCoreApplication>
#include <QAudioRecorder>
#include <QUrl>
#include <QInputDialog>
#include <QFile>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QEventLoop>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QTextToSpeech>
#include <QDateTime>
#include <QTextStream>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QSslKey>
#include<QSsl>
#include <QtWidgets>
#include <QtNetwork>
#include <QTimer>
#include<QDialog>
#include <QSqlError>
#include <QSerialPort>
#include <QSerialPortInfo>
 using namespace std;


using namespace QtCharts;

patient::patient(QWidget *parent) : QDialog(parent), ui(new Ui::patient) {
    ui->setupUi(this);

    ui->tableView_feriel->setModel(P.afficher());
   connect(ui->tableView_feriel, &QTableView::clicked, this, &patient::onTableRowClickedFER);

    // Dans le constructeur ou l'initialisation
    connect(ui->btn_voice, &QPushButton::clicked, this, &patient::textToSpeech);
    connect(ui->image_fer, &QLineEdit::textChanged, this, &patient::updateImage);
    for(const QCameraInfo &infor : QCameraInfo::availableCameras()){qDebug() <<infor.description();}
    M_camera.reset(new QCamera(QCameraInfo::defaultCamera()));
    M_camera->setViewfinder(ui->widgetcamera_view);
    M_camera_Image.reset(new QCameraImageCapture(M_camera.data()));
    M_camera_Image->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    connect(M_camera_Image.data(), &QCameraImageCapture::imageCaptured,this,&patient::imageCaptured);
    connect(ui->envoyer_certif, &QPushButton::clicked, this, &patient::on_envoyer_certif_clicked);


}


patient::~patient()
{
    delete ui;
}
void patient::on_BtnAffchgFeriel_clicked()
{
      ui->tableView_feriel->setModel(P.afficher());
}
void patient::clearFields() {
    ui->cin_fer->clear();
    ui->tel_fer->clear();
    ui->Nom_fer->clear();
    ui->prenon_fer->clear();
    ui->ville_fer->clear();
    ui->email_fer->clear();
    ui->date_fer->setDate(QDate::currentDate());
    ui->sexe_fer->setCurrentIndex(0);
    ui->description_fer->clear();
    ui->recherche_feriel->clear();
    ui->image_fer->clear();
    ui->label_fer->clear();
    ui->etat->clear();
    ui->record_path->clear();

}
void patient::on_ajout_feriel_clicked()
{
    QString CIN = ui->cin_fer->text();
    QString NUMTEL = ui->tel_fer->text();
    QString NOM= ui->Nom_fer->text();
    QString PRENOM = ui->prenon_fer->text();
    QString ADRESSE = ui->ville_fer->text();
    QString EMAIL= ui->email_fer->text();
    QString SEXE = ui->sexe_fer->currentText();
    QDate DATEN = ui->date_fer->date();
    QString DESCRIPTION = ui->description_fer->toPlainText();
    QString IMAGEPATH = ui->image_fer->text();
    QString RECORD = ui->record_path->text();

QString BATTEMENT;

    patientC P(CIN, NUMTEL, NOM, PRENOM, ADRESSE, EMAIL, DATEN, SEXE, DESCRIPTION, IMAGEPATH, RECORD,BATTEMENT  );

    qDebug()<< CIN;
    qDebug()<< NUMTEL;
    qDebug()<< NOM;
    qDebug()<< PRENOM;
    qDebug()<< ADRESSE;
    qDebug()<< EMAIL;
    qDebug() << "DATEN:" << DATEN.toString("dd-MM-yyyy");
    qDebug()<< SEXE;
    qDebug()<<DESCRIPTION;
    qDebug()<<IMAGEPATH;
    qDebug()<<RECORD;


    bool test = P.ajouter_patient();

    if (test) {
        QTextToSpeech *speech = new QTextToSpeech;
                QString message = "Ajout effectué pour " + NOM + " " + PRENOM;
                speech->say(message);
                clearFields();

    } else {
        QTextToSpeech *speech = new QTextToSpeech;
               speech->say("Ajout non effectué");
    }
    ui->tableView_feriel->setModel(P.afficher());


}
void patient::on_btndelete_ferie_clicked() {


  QModelIndex currentIndex = ui->tableView_feriel->currentIndex();
  if (!currentIndex.isValid()) {
         QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                              QObject::tr("Veuillez sélectionner une ligne à supprimer."), QMessageBox::Ok);
         return;
     }
  QString CIN = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 0)).toString();
  QString NUMTEL = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 1)).toString();
  QString NOM= ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 2)).toString();
  QString PRENOM = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 3)).toString();
  QString ADRESSE = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 4)).toString();
  QString EMAIL= ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 5)).toString();
  QString SEXE = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 7)).toString();
  QDate DATEN = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 6)).toDate();
  QString DESCRIPTION = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 8)).toString();
  QString IMAGEPATH = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 9)).toString();
  QString RECORD = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 10)).toString();
  QString BATTEMENT = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(currentIndex.row(), 11)).toString();

  bool test = Ptmp.supprimer_patient(CIN);
   if (test) {
       QTextToSpeech *speech = new QTextToSpeech;
           QString message = "Suppression effectué de " + NOM + " " + PRENOM;
           speech->say(message);
   } else {
       QTextToSpeech *speech = new QTextToSpeech;
           QString message = "Ajout non effectué  ";
           speech->say(message);
   }
   clearFields();

   ui->tableView_feriel->setModel(P.afficher());

}
void patient::on_update_feriel_clicked()
{
    QString CIN = ui->cin_fer->text();
    QString NUMTEL = ui->tel_fer->text();
    QString NOM= ui->Nom_fer->text();
    QString PRENOM = ui->prenon_fer->text();
    QString ADRESSE = ui->ville_fer->text();
    QString EMAIL= ui->email_fer->text();
    QString SEXE = ui->sexe_fer->currentText();
    QDate DATEN = ui->date_fer->date();
    QString DESCRIPTION = ui->description_fer->toPlainText();
    QString IMAGEPATH = ui->image_fer->text();
    QString RECORD = ui->record_path->text();
QString BATTEMENT;

    QMessageBox::StandardButton confirmation;
        confirmation = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir modifier les données de ce patient ?", QMessageBox::Yes | QMessageBox::No);
        if (confirmation == QMessageBox::Yes) {
   patientC P(CIN, NUMTEL, NOM, PRENOM, ADRESSE, EMAIL, DATEN, SEXE, DESCRIPTION,IMAGEPATH,RECORD,BATTEMENT );
bool test=P.modifier_patient();
if (test)
    {
                QTextToSpeech *speech = new QTextToSpeech(this);
                speech->say("La modification des informations de " + NOM + " " + PRENOM + " a été effectuée avec succès.");

        ui->tableView_feriel->setModel(P.afficher(CIN));
         if (!CIN.isEmpty()) ui->tel_fer->clear();
        if (!NUMTEL.isEmpty()) ui->tel_fer->clear();
        if (!NOM.isEmpty()) ui->Nom_fer->clear();
        if (!PRENOM.isEmpty()) ui->prenon_fer->clear();
        if (!ADRESSE.isEmpty()) ui->ville_fer->clear();
        if (!EMAIL.isEmpty()) ui->email_fer->clear();
         if (!RECORD.isEmpty()) ui->record_path->clear();
        //if (!SEXE.isEmpty()) ui->sexe_fer->clear();
         ui->date_fer->setDate(QDate::currentDate());
         if (!DESCRIPTION.isEmpty()) ui->description_fer->clear();
   }
        }
    else {
                   QTextToSpeech *speech = new QTextToSpeech(this);
                   speech->say("La modification a été annulée.");}
clearFields();
}
void patient::on_btnrechercher_feriel_clicked()
{

    QString CIN = ui->recherche_feriel->text();
    QSqlQueryModel* searchResultModel = nullptr;
    bool searchSuccess = P.recherche(CIN, searchResultModel);

    if (searchSuccess) {
        if (searchResultModel && searchResultModel->rowCount() > 0) {
            ui->tableView_feriel->setModel(searchResultModel);
        } else {
            QMessageBox::information(this, "Information", "Aucun patient trouvé avec ce CIN.");
            ui->tableView_feriel->setModel(nullptr);
        }
    } else {

        QMessageBox::critical(this, "Information", "La recherche du patient a échoué.");
    }
       clearFields();
}
void patient::on_pdf_feriel_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            patientC P;
            QSqlQueryModel* model = P.afficher();
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
                QMessageBox::critical(this, "Exportation PDF", "Échec de l'exportation en PDF.");
            }
        }

}
void patient::on_Trieer_feriel_clicked()
{
      ui->tableView_feriel->setModel(P.afficher_tri());
}
void patient::on_stat_feriel_clicked()
{
    QSqlQueryModel *model = P.afficher();

        P.genererStatistiquesSexe(model);

}
void patient::onTableRowClickedFER(const QModelIndex &index)
{
    if (index.isValid()) {
        QString CIN = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 0)).toString();
        QString NUMTEL = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 1)).toString();
        QString NOM = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 2)).toString();
        QString PRENOM = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 3)).toString();
        QString ADRESSE = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 4)).toString();
        QString EMAIL = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 5)).toString();
        QString SEXE = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 7)).toString();
        QDate DATEN = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 6)).toDate();
        QString DESCRIPTION = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 8)).toString();
        QString IMAGEPATH = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 9)).toString();
        QString RECORD = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 10)).toString();
        QString BATTEMENT = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 11)).toString();

        ui->cin_fer->setText(CIN);
        ui->tel_fer->setText(NUMTEL);
        ui->Nom_fer->setText(NOM);
        ui->prenon_fer->setText(PRENOM);
        ui->ville_fer->setText(ADRESSE);
        ui->email_fer->setText(EMAIL);
        ui->date_fer->setDate(DATEN);

        // Trouver l'index correspondant à la valeur du sexe dans le QComboBox
        int sexeIndex = ui->sexe_fer->findText(SEXE);
        if (sexeIndex != -1) {
            ui->sexe_fer->setCurrentIndex(sexeIndex);
        } else {
            // Si aucune correspondance n'est trouvée, définir une valeur par défaut ou afficher un message d'erreur
            QMessageBox::warning(this, "Erreur", "La valeur du sexe n'est pas valide : " + SEXE);
            // Vous pouvez également définir une valeur par défaut dans votre QComboBox
            // ui->sexe_fer->setCurrentIndex(0); // par exemple, définir la première option comme valeur par défaut
        }

        ui->description_fer->setPlainText(DESCRIPTION);
        ui->image_fer->setText(IMAGEPATH);
        ui->record_path->setText(RECORD);

    }
}
void patient::onTableViewClickedFER2(const QModelIndex &index)
 {

     if (index.isValid()) {

         QString CIN = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 0)).toString();




     }
 }
 /******************************* Insertion  Image **********************/
void patient::on_inserer_image_clicked()
{

        QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image File"), "", tr("Image Files (*.jpg *.jpeg *.png *.bmp *.gif)"));


        if (!filePath.isEmpty()) {

            ui->image_fer->setText(filePath);
        }

}
void patient::on_Afficher_image_clicked()
{
    QModelIndex currentIndex = ui->tableView_feriel->currentIndex();


      if (!currentIndex.isValid()) {
          QMessageBox::warning(this, tr("Erreur"), tr("Veuillez sélectionner une ligne."), QMessageBox::Ok);
          return;
      }


      int videoColumnIndex = -1;
      QAbstractItemModel *model = ui->tableView_feriel->model();
      if (model && currentIndex.row() < model->rowCount() && currentIndex.column() < model->columnCount()) {

          for (int column = 0; column < model->columnCount(); ++column) {
              QString columnName = model->headerData(column, Qt::Horizontal).toString();
              if (columnName == "IMAGEPATH") {
                  videoColumnIndex = column;
                  break;
              }
          }
      }


      if (videoColumnIndex == -1) {
          QMessageBox::warning(this, tr("Erreur"), tr("Impossible de trouver la colonne contenant le chemin de la vidéo."), QMessageBox::Ok);
          return;
      }


      QString videoPath = model->data(model->index(currentIndex.row(), videoColumnIndex)).toString();


      if (videoPath.isEmpty()) {
          QMessageBox::warning(this, tr("Erreur"), tr("Chemin de la photo non trouvé."), QMessageBox::Ok);
          return;
      }


      QDesktopServices::openUrl(QUrl::fromLocalFile(videoPath));
}
void patient::on_tableView_feriel_clicked(const QModelIndex &index)
{
    if (index.isValid()) {
        int imageColumnIndex = -1;
        QAbstractItemModel *model = ui->tableView_feriel->model();

        for (int column = 0; column < model->columnCount(); ++column) {
            QString columnName = model->headerData(column, Qt::Horizontal).toString();
            if (columnName == "IMAGEPATH") {
                imageColumnIndex = column;
                break;
            }
        }

        if (imageColumnIndex != -1) {
            QString imagePath = model->data(model->index(index.row(), imageColumnIndex)).toString();
            if (!imagePath.isEmpty()) {
                QPixmap image(imagePath);
                if (!image.isNull()) {
                    // Afficher l'image dans une QLabel ou une autre widget appropriée
                    ui->label_fer->setPixmap(image.scaled(ui->label_fer->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                    ui->label_fer->setScaledContents(true); // Ajuster l'image à la taille de la QLabel

                }
            }
        }
    }
}
/******************************* txt to speech  **********************/

void patient::textToSpeech() {
    // Récupérer le texte depuis le QTextEdit
    QString text = ui->description_fer->toPlainText();

    // Créer un objet QTextToSpeech
    QTextToSpeech *speech = new QTextToSpeech(this);

    // Vérifier si la synthèse vocale est disponible
    if (speech->availableVoices().isEmpty()) {
        qDebug() << "Aucune voix disponible pour la synthèse vocale.";
        return;
    }

    // Choisir une voix pour la synthèse vocale (optionnel)
    speech->setVoice(speech->availableVoices().first());

    // Lire le texte en synthèse vocale
    speech->say(text);
}
/*******************************  Image **********************/
void patient::updateImage(const QString &imagePath)
{
    // Charger l'image depuis le chemin spécifié
    QPixmap image(imagePath);

    // Vérifier si l'image est valide
    if (!image.isNull()) {
        // Afficher l'image dans label_fer en redimensionnant si nécessaire
        ui->label_fer->setPixmap(image.scaled(ui->label_fer->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // Effacer label_fer si le chemin d'image est vide ou l'image n'est pas valide
        ui->label_fer->clear();
    }
}
void patient::stop_Camera() {
    M_camera->stop();
    ui->widgetcamera_view->setVisible(false); // Masquer la vue de la caméra
}
void patient::Start_Camera() {
    M_camera->start();
    ui->widgetcamera_view->setVisible(true); // Afficher la vue de la caméra
}
void patient::on_camerastart_clicked()
{
Start_Camera();
}
void patient::on_camerastop_clicked()
{
stop_Camera();
}
void patient:: imageCaptured(int reqid,const QImage &img)
{  Q_UNUSED(reqid);
    ImageFromCamera = img.scaled(ui->widgetcamera_view->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    counter = counter + 1;
    QString defaultFileName = QCoreApplication::applicationDirPath() + "/Image_" + QString::number(counter) + ".bmp";
    QString filePath = QFileDialog::getSaveFileName(this, tr("Enregistrer l'image"), defaultFileName, tr("Images (*.bmp)"));
    if (!filePath.isEmpty()) {
        QPixmap::fromImage(ImageFromCamera).save(filePath);
        qDebug() << "Image enregistrée à l'emplacement :" << filePath;
    } else {
        qDebug() << "L'utilisateur a annulé l'enregistrement de l'image.";
    }

}
void patient::on_capture_clicked()
{
    M_camera_Image->capture();
}
/*******************************   Historique **********************/
void   patient::on_pb_historiquefer_clicked()
{

    QString cheminFichier = "C:/Users/PC/Desktop/Crudferiel/patient/historique.txt";
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
void  patient::afficherHistoriquefer()
{
    // Spécifier le chemin complet du fichier historique
    QString cheminFichier = "C:/Users/PC/Desktop/Crudferiel/patient/historique.txt";

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




void patient::on_envoyer_certif_clicked()
{
    QModelIndex index = ui->tableView_feriel->selectionModel()->currentIndex();

    // SMTP server details
        QString host = "smtp.gmail.com";
        int port = 587; // Adjust port number accordingly

        // Sender and recipient details
        QString sender = "ghorbeloussama97@gmail.com";
        QString recipient = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 5)).toString();
        QString url = ui->url->text();
        QRegularExpression urlRegex("^(https?://)?([\\da-z.-]+)\\.([a-z.]{2,6})([/\\w .-]*)*/?$");
        QRegularExpressionMatch match = urlRegex.match(url);
        if (recipient.isEmpty()) {
            // Recipient email is empty, display QMessageBox
            QMessageBox::warning(this, "Destinataire vide", "Veuillez verifier l'@ de destinataire");
            return; // Stop further execution
        }
        if (url.isEmpty()) {
            // URL is empty, display QMessageBox
            QMessageBox::warning(this, "Lien de Certificat vide ", "Veuillez entrer un lien s'il vous plait ");
            return; // Stop further execution
        }
        if(match.hasMatch()) {
            // URL is empty or does not match the pattern, display QMessageBox
            QMessageBox::warning(this, "Erreur de Format", "Veuillez entrer un lien valide s'il vous plait (ex: http://example.com)");
            return; // Stop further execution
        }
        // Message details
        QString subject = "Certificat Medical ";
        QString body = "Voici votre Certificat médical. Veuillez télécharger le fichier en cliquant sur le lien suivant : " + url;

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
        socket.write("RCPT TO:<" + recipient.toUtf8() + ">\r\n");
        if (!socket.waitForBytesWritten() || !socket.waitForReadyRead()) {
            qDebug() << "SMTP server response timeout:" << socket.errorString();
            return;
        }

        QString response = socket.readAll();
        qDebug() << response; // Print the response after RCPT TO

        // Check for error response indicating invalid recipient address
        if (response.startsWith("55") || response.contains("not found")) {
            QMessageBox::critical(this, "Erreur", "Échec de l'envoi de l'email : Adresse e-mail du destinataire non trouvée.");
            return;
        }


        // Close the connection
        socket.close();

        QMessageBox::information(this, "Succès", "L'email a été envoyé avec succès à " + recipient);
        ui->url->clear();

    }


void patient::on_pdf_feriel_2_clicked() {
    // Récupérer l'index de la cellule sélectionnée à partir du TableView
    QModelIndex index = ui->tableView_feriel->selectionModel()->currentIndex();

    // Vérifier si l'index est valide
    if (index.isValid()) {
        // Récupérer les données du patient à partir des colonnes appropriées de l'index
        QString cin= ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 0)).toString();
        QString nom = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 2)).toString();
        QString prenom = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 3)).toString();

        // Générer le contenu du certificat médical
        QString contenuCertificat = "CERTIFICAT MEDICAL\n\n";
        contenuCertificat += "Centre Physioform\n";
        contenuCertificat += "Je soussigné(e), Daagi Feriel, Directrice du Centre Physioform, certifie par la présente que :\n";
        contenuCertificat += "\n";
         contenuCertificat += "Cin du Patient : " + cin + "\n";
          contenuCertificat += "\n";
        contenuCertificat += "Nom du Patient : " + nom + "\n";
         contenuCertificat += "\n";
        contenuCertificat += "Prénom du Patient : " + prenom + "\n";
         contenuCertificat += "\n";
        contenuCertificat += "Est autorisé à s'absenter du travail pour raison médicale le ";
        contenuCertificat += QDate::currentDate().toString("dd/MM/yyyy") + " ";
        contenuCertificat += "à " + QTime::currentTime().toString("hh:mm");

        // Utiliser contenuCertificat selon vos besoins


        // Générer le PDF avec le contenu du certificat médical
        QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            QPdfWriter pdfWriter(fileName);
            pdfWriter.setPageSize(QPageSize(QPageSize::A4));
            pdfWriter.setPageMargins(QMarginsF(30, 30, 30, 30));

            QPainter painter(&pdfWriter);
            painter.setFont(QFont("Arial", 12));
            painter.drawText(QRectF(0, 0, pdfWriter.width(), pdfWriter.height()), Qt::AlignLeft | Qt::AlignTop, contenuCertificat);

            QMessageBox::information(this, "Exportation PDF", "Le certificat médical a été exporté en PDF avec succès !");
        }
    } else {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner un patient dans le TableView.");
    }
}

void patient::on_clearfer_clicked()
{
    clearFields();
}


// Fonction pour récupérer l'index de la vue de sélection actuelle
QModelIndex patient::getCurrentSelectionIndex() {
    return ui->tableView_feriel->selectionModel()->currentIndex();
}

// Fonction pour récupérer les informations du patient à partir de l'index de sélection
QStringList patient::getPatientInformation(const QModelIndex &index) {
    QStringList patientInfo;
    if (index.isValid()) {
        // Récupérer les données du patient à partir des colonnes appropriées de l'index
        QString cin = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 0)).toString();
        QString nom = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 2)).toString();
        QString prenom = ui->tableView_feriel->model()->data(ui->tableView_feriel->model()->index(index.row(), 3)).toString();
        patientInfo << cin << nom << prenom;
    }
    return patientInfo;
}


void patient::on_startrecord_clicked(bool checked) {
    qDebug() << "Start Record button clicked";

    // Vérifier si le bouton est coché
    if (checked) {
        // Vérifier si un patient est sélectionné
        QModelIndex index = getCurrentSelectionIndex();
        if (index.isValid()) {
            qDebug() << "Valid index obtained";
            QStringList patientInfo = getPatientInformation(index);
            QString cin = patientInfo.value(0);
            QString nom = patientInfo.value(1);
            QString prenom = patientInfo.value(2);

            qDebug() << "Record button is checked";

            QString folderPath = "C:/Users/PC/Desktop/Crudferiel/audio_patient";
            QDir dir(folderPath);
            if (!dir.exists()) {
                dir.mkpath(folderPath);
            }

            QString audioFilePath = folderPath + "/audio_" + nom + "_" + prenom + "_" + cin + "_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".wav";
            audioRecorder = new QAudioRecorder(this);
            audioRecorder->setAudioInput("default");
            audioRecorder->setOutputLocation(QUrl::fromLocalFile(audioFilePath));
            ui->record_path->text()=audioFilePath;
            QString queryStr = "UPDATE PATIENTC SET RECORD = :audioFilePath WHERE CIN = :cin";
            QSqlQuery query;
            query.prepare(queryStr);
            query.bindValue(":audioFilePath", audioFilePath);
            query.bindValue(":cin", cin);
            if (!query.exec()) {
                // Gérer l'erreur ici, par exemple afficher un message à l'utilisateur
                QMessageBox::critical(this, tr("Error"), tr("Failed to insert audio file path into database. Please try again."));
            }
            // Utiliser QTextToSpeech pour lire un message audio
            QTextToSpeech *speech = new QTextToSpeech();
            if (speech->state() == QTextToSpeech::Ready) {
                speech->say("Début de l'enregistrement");
            }
            QThread::msleep(3000); // Ajouter une pause de 100 millisecondes

            ui->etat->setText("Enregistrement en cours");
            audioRecorder->record();
            qDebug() << "Recording...";
        } else {
            QMessageBox::warning(this, tr("Warning"), tr("Please select a patient first."), QMessageBox::Ok);
            qDebug() << "No patient selected";
            ui->startrecord->setChecked(false); // Décocher le bouton
            ui->startrecord->setEnabled(true); // Réactiver le bouton
        }
    } else {
        // Le bouton n'est pas coché, arrêter l'enregistrement audio si en cours
        if (audioRecorder && audioRecorder->state() == QMediaRecorder::RecordingState) {
            qDebug() << "Recording stopped";


            // Afficher un message pour indiquer que l'enregistrement est terminé
            QTextToSpeech *speech = new QTextToSpeech();
            if (speech->state() == QTextToSpeech::Ready) {
                speech->say("Enregistrement terminé avec succès");
            }

            audioRecorder->stop();

        }
        qDebug() << "Record button is unchecked";
        ui->etat->clear();
    }

}
QString patient::getAudioFilePathFromDatabase(const QString& cin)
{
    QString audioFilePath;
    // Créer la requête SQL pour récupérer le chemin du fichier audio en fonction du CIN
    QSqlQuery query;
    query.prepare("SELECT RECORD FROM PATIENTC WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    if (query.exec() && query.next()) {
        // Récupérer le chemin du fichier audio depuis le résultat de la requête
        audioFilePath = query.value(0).toString();
    } else {
        qDebug() << "Failed to retrieve audio file path from database for CIN:" << cin;
    }
    return audioFilePath;
}
/*
void patient::on_ListenToRecord_clicked()
{
    QModelIndex index = getCurrentSelectionIndex(); // Obtenez l'index de la sélection actuelle dans le tableau
    if (index.isValid()) {
        QStringList patientInfo = getPatientInformation(index);
        QString cin = patientInfo.value(0);
        // Récupérez le chemin du fichier audio à partir de la base de données en utilisant le CIN ou une autre information unique sur le patient
        QString audioFilePath = getAudioFilePathFromDatabase(cin); // Fonction à implémenter pour récupérer le chemin du fichier audio à partir de la base de données
        if (!audioFilePath.isEmpty()) {
            // Créez un lecteur multimédia pour lire le fichier audio
            QMediaPlayer *player = new QMediaPlayer(this);
            player->setMedia(QUrl::fromLocalFile(audioFilePath));
            // Commencez la lecture du fichier audio
            player->play();
        } else {
            qDebug() << "Audio file path not found for the selected patient.";
            // Gérer le cas où le chemin du fichier audio n'est pas trouvé
        }
    } else {
        qDebug() << "No patient selected.";
        // Gérer le cas où aucun patient n'est sélectionné dans le tableau
    }
}
*/

void patient::on_ListenToRecord_clicked()
{
    QModelIndex index = getCurrentSelectionIndex(); // Obtenez l'index de la sélection actuelle dans le tableau
    if (index.isValid()) {
        QStringList patientInfo = getPatientInformation(index);
        QString cin = patientInfo.value(0);
        // Récupérez le chemin du fichier audio à partir de la base de données en utilisant le CIN ou une autre information unique sur le patient
        QString audioFilePath = getAudioFilePathFromDatabase(cin); // Fonction à implémenter pour récupérer le chemin du fichier audio à partir de la base de données
        if (!audioFilePath.isEmpty()) {
            // Créez un lecteur multimédia pour lire le fichier audio
            player = new QMediaPlayer(this);
            player->setMedia(QUrl::fromLocalFile(audioFilePath));
            // Commencez la lecture du fichier audio
            player->play();

            // Activez les boutons de contrôle de lecture
            ui->playButton->setEnabled(true);
            ui->pauseButton->setEnabled(true);
            ui->stopButton->setEnabled(true);
            ui->fastForwardButton->setEnabled(true);
            ui->rewindButton->setEnabled(true);
        } else {
            qDebug() << "Audio file path not found for the selected patient.";
            // Gérer le cas où le chemin du fichier audio n'est pas trouvé
        }
    } else {
        qDebug() << "No patient selected.";
        // Gérer le cas où aucun patient n'est sélectionné dans le tableau
    }
}

void patient::on_playButton_clicked()
{
    if (player->state() == QMediaPlayer::PausedState)
        player->play();
}

void patient::on_pauseButton_clicked()
{
    if (player->state() == QMediaPlayer::PlayingState)
        player->pause();
}

void patient::on_stopButton_clicked()
{
    player->stop();
}

void patient::on_fastForwardButton_clicked()
{
    player->setPosition(player->position() + 2000); // Avance rapide de 5 secondes (par exemple)
}

void patient::on_rewindButton_clicked()
{
    player->setPosition(player->position() - 2000); // Retour en arrière de 5 secondes (par exemple)
}

