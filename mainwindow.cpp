#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employe.h"
#include "employeC.h"
#include "patient.h"
#include "patientC.h"
#include "paiement.h"
#include "paiementC.h"
#include "reservation.h"
#include "reservationc.h"
#include <QMessageBox>
#include <QDebug>
//#include "arduino.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //animation gif physoioform
        QMovie *movie = new QMovie("C:/Users/malek/Desktop/integration/physioform.gif");
        ui->physiofrom->setMovie(movie);
        movie->start();




    ui->numcompte->setValidator(new QIntValidator(10000000, 99999999, this));
    ui->idcompte->setValidator(new QIntValidator(100000, 999999, this));
    ui->idlogin->setValidator(new QIntValidator(100000, 999999, this));
    QRegExp regex("[a-zA-Z]+[' ']+[a-zA-Z]+");
    ui->nomcompte->setValidator(new QRegExpValidator(regex));
    ui->prenomcompte->setValidator(new QRegExpValidator(regex));

    QRegExp regex1("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b");
    // Créez un validateur basé sur l'expression régulière
    QValidator *validator = new QRegExpValidator(regex1, this);

    connect(ui->creecompte1, &QPushButton::clicked, [=]() {
             // Switch to the second tab (index 1) of the QTabWidget
             ui->tabWidget->setCurrentIndex(1);
         });
    connect(ui->allerseconnecter, &QPushButton::clicked, [=]() {
             // Switch to the second tab (index 1) of the QTabWidget
             ui->tabWidget->setCurrentIndex(0);
         });

    // Appliquez le validateur au QLineEdit
    ui->emailcompte->setValidator(validator);
   /* int result = arduino.connect_arduino();
                if (result == 0) {
                    qDebug() << "Arduino connected successfully.";
                    // Connectez la fonction carteLue au signal readyRead de votre classe Arduino
                    //connect(arduino.getserial(), &QSerialPort::readyRead, this, &MainWindow::carteLue);
                    //carteLue();

                } else if (result == 1) {
                    qDebug() << "Error opening serial port.";
                } else if (result == -1) {
                    qDebug() << "Arduino not found on any available port.";
                }

}
void MainWindow::carteLue() {
    if (arduino.getserial()->isOpen()) {
        // Demander à Arduino les données actuelles
        QByteArray receivedData = arduino.read_from_arduino();

        // Convertir QByteArray en QString pour une comparaison plus facile
        QString receivedString = QString::fromUtf8(receivedData).trimmed();
        qDebug() << "Le REFCARD est "<< receivedString;

        // Requête pour vérifier si le REFCARD est trouvé dans la base de données
        QSqlQuery query;
        query.prepare("SELECT  NOMEMPLOYE , PRENOMEMPLOYE FROM employec WHERE RFID = :RFID1");
        query.bindValue(":RFID1", receivedString);
        bool test=false;
        if (query.exec() && query.next()) {
           QString employenom = query.value(0).toString();
            QString employeprenom = query.value(1).toString();
             test=true;
            qDebug() << "Le REFCARD a été trouvé dans la base de données. nom de l'employé : " << employenom<<"prenom:"<<employeprenom;

            if(test) {
                QString successMessage = "Authentification verifiée\n"
                                         "Nom: " + employenom + "\n"
                                         "Prenom: " + employeprenom + "\n"
                                         "Cliquez sur OK pour quitter";
                QMessageBox::information(this, "Authentification faite", successMessage);
            } else {
                QMessageBox::warning(this, "Authentification échouée", "Le REFCARD n'a pas été trouvé dans la base de données.");
            }
        } else {
            qDebug() << "Le REFCARD n'a pas été trouvé dans la base de données.";
            // Afficher une boîte de dialogue QMessageBox pour l'authentification échouée
            QMessageBox::warning(this, "Authentification échouée", "Le REFCARD n'a pas été trouvé dans la base de données.");
        }
    } else {
        qDebug() << "Le port série n'est pas ouvert. Impossible d'envoyer le message.";
    }

*/

 }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_seconnecter_login_clicked()
{
    employeC P;
    QString  id=ui->idlogin->text();
    QString mdp=ui->mdp_login->text();
    bool test=P.idexist(id);
    if(test) {
        bool test1=P.mdpCorrect(id,mdp);
        if (test1)
        {
            QMessageBox::information(nullptr, "Connexion réussie", "Bienvenu dans votre espace employe !");
            ui->idlogin->clear();
            ui->mdp_login->clear();
            QString option = ui->comboBox->currentText();
            if (option == "Employe") {
                employe employeDialog;
                employeDialog.setModal(true);
                employeDialog.exec();
            } else if (option == "Patient") {
                patient patientDialog;
                patientDialog.setModal(true);
                patientDialog.exec();
            }
         else if (option == "Suivie de Traitement") {
            traitement traitementDialog;
            traitementDialog.setModal(true);
            traitementDialog.exec();
        }
            else if (option == "Paiement") {
               paiement paiementDialog;
               paiementDialog.setModal(true);
               paiementDialog.exec();
           }
            else if (option == "Reservation") {
               reservation reservationDialog;
               reservationDialog.setModal(true);
               reservationDialog.exec();
           }





        }
        else {
            QMessageBox::critical(nullptr, "Erreur de connexion", "Mot de passe incorrect");
            ui->mdp_login->clear();
        }
    }
    else {
        QMessageBox::critical(nullptr, "Erreur de connexion", "Cet ID n'existe pas.");
    }

}




void MainWindow::on_seconnecter2_clicked() {
    employeC P;

    // Path to the Python script for facial recognition
    QString pythonScriptPath = "C:/Users/malek/Desktop/integration/reconnaissance_faciale.py";

    // Start the Python process
    QProcess process;
    process.start("python", QStringList() << pythonScriptPath);
    if (!process.waitForStarted()) {
        qDebug() << "Erreur au démarrage du processus :" << process.errorString();
        return;
    }
    if (!process.waitForFinished()) {
        qDebug() << "Erreur à la fin du processus :" << process.errorString();
        return;
    }

    // Read the output of the Python script
    QString output = QString::fromUtf8(process.readAllStandardOutput());
    qDebug() << output;

    // Verify the ID
    int test = P.verifierAI(ui->idlogin->text());
    if (!test) {
        // Show a success message
        QMessageBox::information(nullptr, "Connexion réussie", "Bienvenue dans votre espace employé !");
        // Get the selected option from the comboBox
        QString option = ui->comboBox->currentText();

        // Show the corresponding dialog based on the selected option
        if (option == "Employe") {
            employe employeDialog;
            employeDialog.setModal(true);
            employeDialog.exec();
        } else if (option == "Patient") {
            patient patientDialog;
            patientDialog.setModal(true);
            patientDialog.exec();
        } else if (option == "Suivie de Traitement") {
            traitement traitementDialog;
            traitementDialog.setModal(true);
            traitementDialog.exec();
        }
        else if (option == "Paiement") {
            paiement paiementDialog;
            paiementDialog.setModal(true);
            paiementDialog.exec();
       }
        else if (option == "Reservation") {
           reservation reservationDialog;
           reservationDialog.setModal(true);
           reservationDialog.exec();
       }



        // Clear login fields after successful login
        ui->idlogin->clear();
        ui->mdp_login->clear();
    } else {
        // Show an error message if the ID is not verified
        QMessageBox::critical(nullptr, "Erreur de connexion", "ID non vérifié. Veuillez réessayer.");
    }
}








void MainWindow::on_creer_compte_clicked()
{
    QString idemp = ui->idcompte->text();
    QString nomemploye = ui->nomcompte->text();
    QString prenomemploye = ui->prenomcompte->text();

    QString emailemploye= ui->emailcompte->text();
    QString numtele = ui->numcompte->text();
    QString secteur = ui->secteurcompte->currentText();
    QString sexeemploye = ui->sexecompte->currentText();
    QString mdp = ui->mdpcompte->text();



    employeC P(idemp, nomemploye, prenomemploye, mdp, emailemploye, numtele,   sexeemploye, secteur);



    bool test = P.ajouter_employe();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\n"
                                             "Click Cancel to exit ."), QMessageBox::Cancel);
        // Construction du message
        QString successMessage = "Compte employé créé\n"
                                 "ID: " + idemp + "\n"
                                 "Nom: " + nomemploye + "\n"
                                   "Prenom: " + prenomemploye + "\n"
                                 "Mot de passe: " + mdp + "\n"
                                 "Poste: " + secteur + "\n"
                                 "Sexe: " + sexeemploye + "\n"
                                 "Email: " + emailemploye + "\n"
                                "Num téléphone: " + numtele + "\n"

                                 "Cliquez sur OK pour quitter";
        ui->idcompte->clear();
        ui->nomcompte->clear();
        ui->prenomcompte->clear();
        ui->mdpcompte->clear();
        ui->emailcompte->clear();
        ui->numcompte->clear();

        // Affichage du QMessageBox
        QMessageBox msgBox;
        msgBox.setText(successMessage);
        msgBox.setWindowTitle("Succès");
        msgBox.setIcon(QMessageBox::Information);
        msgBox.addButton(QMessageBox::Ok);
        msgBox.exec();
    } else {
        QMessageBox::critical(nullptr,QObject::tr("NOT OK"),
                              QObject::tr("Ajout non effectué\n"
                                          "Click Cancel to exit ."), QMessageBox::Cancel);
    }
}

void MainWindow::on_annuler_login_clicked()
{
    ui->idlogin->clear();
    ui->mdp_login->clear();
}

void MainWindow::on_annuler_compte_clicked()
{
    this->close();
}

