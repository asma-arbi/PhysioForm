#include "mainwindow.h" // Assuming your main window class is named MainWindow
#include "employe.h"
#include "traitement.h"
#include "paiement.h"
#include "patient.h"
#include "reservation.h"
#include <QApplication>
#include "connexion.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create instances of your main window and other widgets
    MainWindow w;
    employe e;
    traitement t;
    paiement pai;
    patient pat;
    reservation res;




    // Establish the database connection
    Connexion connexion;
    if (!connexion.create()) {
        QMessageBox::critical(nullptr, QObject::tr("Database Error"),
                              QObject::tr("Failed to connect to database. Please check your database settings and try again."),
                              QMessageBox::Ok);
        return -1; // Exit the application if connection fails
    }

    // If the database connection is successful, show the main window
    w.show();

    return a.exec();
}


