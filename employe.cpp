#include "employe.h"
#include "ui_employe.h"
#include <QSqlDatabase>
#include <QDate>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include "employeC.h"
#include<random>
#include<cstdlib>
#include<ctime>
#include<QJsonArray>
#include <QStandardItemModel>
#include "chat.h"
using namespace QtCharts;

employe::employe(QWidget *parent):  QDialog(parent), ui(new Ui::employe){

    ui->setupUi(this);
       ui->tableView->setModel(P.afficher());
       connect(ui->tableView, &QTableView::clicked, this, &employe::selectinfo2);
       QRegExp regex("[a-zA-Z]+[' ']+[a-zA-Z]+");
       ui->nom_halima->setValidator(new QRegExpValidator(regex));
       ui->prenom_halima->setValidator(new QRegExpValidator(regex));
       //ui->adresse_halima->setValidator(new QRegExpValidator(regex));
       ui->id_halima->setValidator(new QIntValidator(100000, 999999, this));
       ui->recherche_halima->setValidator(new QIntValidator(100000, 999999, this));
       ui->tel_halima->setValidator(new QIntValidator(10000000, 99999999, this));
       QRegExp regex1("\\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}\\b");
       // Créez un validateur basé sur l'expression régulière
       QValidator *validator = new QRegExpValidator(regex1, this);

       // Appliquez le validateur au QLineEdit
       ui->email_halima->setValidator(validator);


}

employe::~employe()
{
    delete ui;
}
void employe:: selectinfo2(const QModelIndex &index1){
if (index1.isValid()) {
    int row = index1.row();
    QString id2 = ui->tableView->model()->data(ui->tableView->model()->index(row, 0)).toString();
    ui->id_halima->setText(id2);
    ui->deletebnthalima->setText(id2);
    QString nom2 = ui->tableView->model()->data(ui->tableView->model()->index(row, 1)).toString();
    ui->nom_halima->setText(nom2);
    QString prenom = ui->tableView->model()->data(ui->tableView->model()->index(row, 2)).toString();
    ui->prenom_halima->setText(prenom);
   // QModelIndex index = ui->tableView->model()->index(row, 3); // Obtenez l'index pour la cellule
   // QString dateString = ui->tableView->model()->data(index).toString(); // Obtenez la chaîne de caractères à partir des données de la cellule
    //QDate date = QDate::fromString(dateString, "dd/MM/yyyy"); // Convertissez la chaîne de caractères en objet QDate
   QString mdp = ui->tableView->model()->data(ui->tableView->model()->index(row, 3)).toString();
    ui->mdp_halima->setText(mdp);

    QString email2 = ui->tableView->model()->data(ui->tableView->model()->index(row, 4)).toString();
    ui->email_halima->setText(email2);
    QString numtel = ui->tableView->model()->data(ui->tableView->model()->index(row, 5)).toString();
    ui->tel_halima->setText(numtel);
    //QString adresse = ui->tableView->model()->data(ui->tableView->model()->index(row, 6)).toString();
    //ui->adresse_halima->setText(adresse);
    QString sexe2 = ui->tableView->model()->data(ui->tableView->model()->index(row, 6)).toString();
    if (sexe2 == "Homme") {
        ui->sexe_halima->setCurrentIndex(0); // Sélectionner "Homme" (si c'est le premier élément)
    } else if (sexe2 == "Femme") {
        ui->sexe_halima->setCurrentIndex(1); // Sélectionner "Femme" (si c'est le deuxième élément)
    }
    QString specialite = ui->tableView->model()->data(ui->tableView->model()->index(row, 7)).toString();
    if (specialite == "Employe") {
        ui->specialite_halima->setCurrentIndex(ui->specialite_halima->findText("Employe"));
    } else if (specialite == "Patient") {
        ui->specialite_halima->setCurrentIndex(ui->specialite_halima->findText("Patient"));
    } else if(specialite == "Reservation") {
        ui->specialite_halima->setCurrentIndex(ui->specialite_halima->findText("Reservation"));
    }
    else if(specialite == "Paiement"){
        ui->specialite_halima->setCurrentIndex(ui->specialite_halima->findText("Paiement"));
    }
    else {ui->specialite_halima->setCurrentIndex(ui->specialite_halima->findText("Suivie de Traitement"));}


}
}
void employe::clearFields() {
    ui->id_halima->clear();
    ui->nom_halima->clear();
    ui->prenom_halima->clear();
    //ui->date_halima->setDate(QDate::currentDate());
    ui->mdp_halima->clear();
    ui->email_halima->clear();
    ui->tel_halima->clear();
   // ui->adresse_halima->clear();
    ui->sexe_halima->setCurrentIndex(0);
    ui->specialite_halima->setCurrentIndex(0);
    ui->recherche_halima->clear();
    ui->deletebnthalima->clear();
    ui->tableView->setModel(P.afficher());
}

void employe::on_ajout_halima_clicked()
{
    ui->tableView->setModel(P.afficher());
    QString idemp = ui->id_halima->text();
    QString nomemploye = ui->nom_halima->text();
    QString prenomemploye = ui->prenom_halima->text();
    QString mdpemploye = ui->mdp_halima->text();
    QString emailemploye= ui->email_halima->text();
    QString numtele = ui->tel_halima->text();
    //QString adresseemploye = ui->adresse_halima->text();
    QString sexeemploye = ui->sexe_halima->currentText();
    QString specialiteemploye = ui->specialite_halima->currentText();



    employeC P(idemp, nomemploye, prenomemploye, mdpemploye, emailemploye, numtele, sexeemploye, specialiteemploye);


    bool test = P.ajouter_employe();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\n"
                                             "Click Cancel to exit ."), QMessageBox::Cancel);
        ui->tableView->setModel(P.afficher());
    } else {
        QMessageBox::critical(nullptr,QObject::tr("NOT OK"),
                              QObject::tr("Ajout non effectué\n"
                                          "Click Cancel to exit ."), QMessageBox::Cancel);
    }

    ui->tableView->setModel(P.afficher());
    clearFields();
}




void employe::on_btndelete_halima_clicked()
{
    QString idemp = ui->deletebnthalima->text();
    bool test = Ptmp.supprimer_employe(idemp);
    if (test)
       {
           QMessageBox::information(nullptr,QObject::tr("ok"),QObject::tr("Suppression effectué\n""click cancel to exit."),QMessageBox::Cancel);
          // ui->tableView->setModel(P.afficher());
           ui->deletebnthalima->clear();
    }
       else
           QMessageBox::critical(nullptr,QObject::tr("not ok"),QObject::tr("Suppression non effectué\n""click cancel to exit."),QMessageBox::Cancel);
    ui->tableView->setModel(P.afficher());
   clearFields();
}


void employe::on_update_halima_clicked()
{
    QString idemp = ui->id_halima->text();
    QString nomemploye = ui->nom_halima->text();
    QString prenomemploye= ui->prenom_halima->text();
    //QDate DATEN = ui->date_halima->date();
    QString mdpemploye= ui->mdp_halima->text();
    QString emailemploye= ui->email_halima->text();
    QString numtele= ui->tel_halima->text();
    //QString adresseemploye = ui->adresse_halima->text();
    QString sexeemploye = ui->sexe_halima->currentText();
    QString specialiteemploye = ui->specialite_halima->currentText();

   employeC P(idemp, nomemploye, prenomemploye, mdpemploye, emailemploye, numtele, sexeemploye, specialiteemploye );
bool test=P.modifier_employe();
if (test)
    {
        QMessageBox::information(nullptr,QObject::tr("ok"),QObject::tr("modification effectué\n""click cancel to exit."),QMessageBox::Cancel);
        ui->tableView->setModel(P.afficher(idemp));
         if (!idemp.isEmpty()) ui->id_halima->clear();
        if (!nomemploye.isEmpty()) ui->nom_halima->clear();
        if (!prenomemploye.isEmpty()) ui->prenom_halima->clear();
        //ui->date_halima->setDate(QDate::currentDate());
        if (!mdpemploye.isEmpty()) ui->mdp_halima->clear();
        if (!emailemploye.isEmpty()) ui->email_halima->clear();
      //  if (!adresseemploye.isEmpty()) ui->adresse_halima->clear();
        ui->tableView->setModel(P.afficher());

   }
    else
        QMessageBox::critical(nullptr,QObject::tr("not ok"),QObject::tr("modify  non effectué\n""click cancel to exit."),QMessageBox::Cancel);
clearFields();
}

void employe::on_btnrechercher_halima_clicked()
{
    /*QString idemp = ui->recherche_halima->text();
    QSqlQueryModel* searchResultModel = nullptr;
    bool searchSuccess = P.recherche(idemp, searchResultModel);

    if (searchSuccess && searchResultModel && searchResultModel->rowCount() > 0) {
        ui->tableView->setModel(searchResultModel);
        ui->recherche_halima->clear();
    } else {
        if (!searchSuccess) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la recherche du employe.");
        } else {
            QMessageBox::information(this, "Information", "Aucun employe trouvé avec ce idemp.");
        }
        // Nettoyer la vue de la table si aucun résultat n'est trouvé
        ui->tableView->setModel(nullptr);
    }

    clearFields();*/
    QString searchId = ui->recherche_halima->text(); // Retrieve the ID from the QLineEdit

    // Retrieve the ID from the QLineEdit

        if (searchId.isEmpty()) {
            // If searchId is empty, display the full table view
           ui->tableView->setModel(P.afficher());
            return;
        }
    employeC P;
    QSqlQuery query;
    query.prepare("SELECT * FROM employeC WHERE idemp = ?");
    query.addBindValue(searchId.toInt());

    if (query.exec() && query.next()) {
        if (P.idexist(searchId)){
            int id = query.value(0).toInt();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();
            QString mdp = query.value(3).toString();
            QString email = query.value(4).toString();
            QString num = query.value(5).toString();
            //QString adresse = query.value(6).toString();
            QString sexe = query.value(6).toString();
            QString specialite = query.value(7).toString();

            // Update the QTableView with the retrieved information
            QStandardItemModel *model = new QStandardItemModel(0, 9, this);
            model->setHorizontalHeaderItem(0, new QStandardItem(QString("ID")));
            model->setHorizontalHeaderItem(1, new QStandardItem(QString("Nom")));
            model->setHorizontalHeaderItem(2, new QStandardItem(QString("Prenom")));
            model->setHorizontalHeaderItem(3, new QStandardItem(QString("Mot de passe")));
            model->setHorizontalHeaderItem(4, new QStandardItem(QString("Email")));
            model->setHorizontalHeaderItem(5, new QStandardItem(QString("Numéro tel")));
           // model->setHorizontalHeaderItem(6, new QStandardItem(QString("Adresse")));
            model->setHorizontalHeaderItem(6, new QStandardItem(QString("Sexe")));
            model->setHorizontalHeaderItem(7, new QStandardItem(QString("Specialite")));
            model->setItem(0, 0, new QStandardItem(QString::number(id)));
            model->setItem(0, 1, new QStandardItem(nom));
            model->setItem(0, 2, new QStandardItem(prenom));
            model->setItem(0, 3, new QStandardItem(mdp));
            model->setItem(0, 4, new QStandardItem(email));
            model->setItem(0, 5, new QStandardItem(num));
           // model->setItem(0, 6, new QStandardItem(adresse));
            model->setItem(0, 6, new QStandardItem(sexe));
            model->setItem(0, 7, new QStandardItem(specialite));

        ui->tableView->setModel(model);}
    } else {
        // Display a message if the employee is not found
        QMessageBox::critical(nullptr, QObject::tr("ERROR"),
            QObject::tr("cet ID n'existe pas .\n inserrer un ID existant."), QMessageBox::Cancel);
    }
     ui->recherche_halima->clear();
}


void employe::on_pdf_halima_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
        if (!fileName.isEmpty()) {
            employeC P;
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
                QMessageBox::warning(this, "Exportation PDF", "Échec de l'exportation en PDF.");
            }
        }

}

void employe::on_Tri_halima_clicked(int index)
{ QString sortingCriteria = ui->Tri_halima->itemText(index);

    //qDebug() <<"le critere est" ,sortingCriteria;
    QSqlQueryModel *sortedModel = P.afficher_tri(sortingCriteria);

      ui->tableView->setModel(sortedModel);
}



void employe::on_stat_halima_clicked()
{
    QSqlQueryModel *model = P.afficher();


       // P.genererStatistiquessexeemploye(model);
    QPieSeries *series = new QPieSeries();//creation d'une nouv ser de donn

        int countHomme = 0;//init des comp
        int countFemme = 0;

        for (int row = 0; row < model->rowCount(); ++row) {
            QString sexeemploye = model->data(model->index(row, 6)).toString(); // Supposons que la colonne du sexeemploye est à l'index 6

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
        chartView->raise();

}
void employe::on_chatbot_halima_clicked()
{


        //P.genererStatistiquessexeemploye(model);
        chat chatDialog;
        chatDialog.setModal(true);
        chatDialog.exec();

}

QSqlQueryModel* employe::on_Tri_halima_currentIndexChanged()
{
    QString sortingCriteria = ui->Tri_halima->currentText();
    qDebug() << "Le critère de tri est : " << sortingCriteria;

    QSqlQueryModel *model = new QSqlQueryModel();

    if (sortingCriteria == "idemp")
    {
        model->setQuery("SELECT * FROM employeC ORDER BY idemp");
    }
    else if (sortingCriteria == "Plus récent")
    {
        model->setQuery("SELECT * FROM employeC ORDER BY idemp DESC");

    }
    else
    {
        model->setQuery("SELECT * FROM employeC ORDER BY NOMEMPLOYE");
    }

    // Mettre à jour le modèle de vue tableView
    ui->tableView->setModel(model);

    return model;
}


void employe::on_btquitter_clicked()
{
    this->close();
}
