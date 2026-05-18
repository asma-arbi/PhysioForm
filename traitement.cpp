 #include "traitement.h"
#include "traitementC.h"
#include "connexion.h"
#include <QMessageBox>
#include "ui_traitement.h"
#include <QSqlDatabase>
#include <QDate>
#include <QString>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QFormLayout>
#include <QMediaPlayer>
#include <QComboBox>
#include <QLocale>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QProcess>





using namespace QtCharts;


traitement::traitement(QWidget *parent): QDialog(parent),ui(new Ui::traitement){
    ui->setupUi(this);


    connect(ui->majTraitement, &QPushButton::clicked, [this]() {
            on_majTraitement_clicked();
        });

     connect(ui->tri, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &traitement::onSortingCriteriaChanged);

     connect(ui->BoutonRecherche, &QPushButton::clicked, this, &traitement::on_BoutonRecherche_clicked);

     connect(ui->tableView_malek, &QTableView::clicked, this, &traitement::onTableViewClicked);

     listWidget = ui->listWidget;
     listWidgetDone = ui->listWidgetDone;


     ui->listWidget->setAcceptDrops(true);
     ui->listWidgetDoing->setAcceptDrops(true);
     ui->listWidgetDone->setAcceptDrops(true);
     ui->listWidget->setDragEnabled(true);
     ui->listWidgetDoing->setDragEnabled(true);
     ui->listWidgetDone->setDragEnabled(true);

     ui->listWidget->setDragDropMode(QAbstractItemView::DragDrop);
      ui->listWidgetDoing->setDragDropMode(QAbstractItemView::DragDrop);
       ui->listWidgetDone->setDragDropMode(QAbstractItemView::DragDrop);

     connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(deleteSelectedItem(QListWidgetItem*)));
     connect(ui->listWidgetDone, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(deleteSelectedItem(QListWidgetItem*)));
     connect(ui->listWidgetDoing, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(deleteSelectedItem(QListWidgetItem*)));


     connect(ui->BoutonStat, &QPushButton::clicked, this, &traitement::statProgres);

     connect(ui->boutonTodolist, &QPushButton::clicked, [=]() {
         // Switch to the second tab (index 1) of the QTabWidget
         ui->GestionTraitement->setCurrentIndex(1);
     });

     connect(ui->GestionTraitement, &QTabWidget::tabCloseRequested, this, &traitement::closeTodolistTab);



      connect(ui->clear, &QPushButton::clicked, this, &traitement::clearFields);

     connect(ui->tableView_malek, &QTableView::clicked, this, &traitement::onTableRowClicked);

     connect(ui->voirVideo, SIGNAL(clicked()), this, SLOT(onVoirVideoClicked()));

     connect(ui->uploadVideo, SIGNAL(clicked()), this, SLOT(on_uploadVideo_clicked()));

     connect(ui->tableView_malek, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onTableRowDoubleClicked(const QModelIndex&)));



        connect(ui->spotify, &QPushButton::clicked, this, &traitement::openSpotify);







        //detecteur de motion
       connect(ui->motion, &QPushButton::clicked, this, &traitement::runMotionDetection);







}


traitement::~traitement()
{
    delete ui;
}

void traitement::clearFields() {
    ui->IDtraitement->clear();
    ui->titre->clear();
    ui->datedebut->minimumDate();
    ui->datefin->minimumDate();
    ui->motifconsultation->clear();
    ui->exercicestext->clear();
    ui->videoTraitement->clear();
    ui->listWidget->clear();
    ui->listWidgetDoing->clear();
    ui->listWidgetDone->clear();
}




//ajout d'un traitement
void traitement::on_ajouterTraitement_clicked()
{
    QString IDTRAITEMENT = ui->IDtraitement->text();
    QString TITRE = ui->titre->text();
    QDate DEBUTTRAITEMENT = ui->datedebut->date();
    QDate FINTRAITEMENT = ui->datefin->date();
    QString MOTIFCONSULTATION = ui->motifconsultation->text();
    QString EXERCICES = ui->exercicestext->text();
    QString VIDEOTRAITEMENT = ui->videoTraitement->text(); // Retrieve video URL


    ui->labelErrorIDTraitement->clear();
    ui->labelErrorTitre->clear();
    ui->labelErrorDebut->clear();
    ui->labelErrorMotif->clear();
    ui->labelErrorExercices->clear();

    // controle de saisie
    if (IDTRAITEMENT.isEmpty() || !IDTRAITEMENT.toInt()) {
        ui->labelErrorIDTraitement->setText("<font color='#3bc4a9'>IDtraitement doit être un nombre valide.</font>");
        return;
    }

    if (TITRE.length() < 3) {
        ui->labelErrorTitre->setText("<font color='#3bc4a9'>TITRE doit contenir au moins 3 lettres.</font>");
        return;
    }

    if (!DEBUTTRAITEMENT.isValid() || !FINTRAITEMENT.isValid() || DEBUTTRAITEMENT > FINTRAITEMENT) {
        ui->labelErrorDebut->setText("<font color='#3bc4a9'>Dates de traitement non valides.</font>");
        return;
    }

    if (MOTIFCONSULTATION.length() < 3) {
        ui->labelErrorMotif->setText("<font color='#3bc4a9'>MOTIFCONSULTATION doit contenir au moins 3 lettres.</font>");
        return;
    }

    if (EXERCICES.length() < 3) {
        ui->labelErrorExercices->setText("<font color='#3bc4a9'>EXERCICES doit contenir au moins 3 lettres.</font>");
        return;
    }


    traitementC T(IDTRAITEMENT, TITRE, DEBUTTRAITEMENT, FINTRAITEMENT, MOTIFCONSULTATION, EXERCICES, VIDEOTRAITEMENT);

    qDebug() << IDTRAITEMENT;
    qDebug() << TITRE;
    qDebug() << "DEBUTTRAITEMENT:" << DEBUTTRAITEMENT.toString("dd-MM-yyyy");
    qDebug() << "FINTRAITEMENT:" << FINTRAITEMENT.toString("dd-MM-yyyy");
    qDebug() << MOTIFCONSULTATION;
    qDebug() << EXERCICES;
    qDebug() << VIDEOTRAITEMENT; // Log video URL

    bool test = T.ajouterTraitement();

    if (test) {
        QStringList exercisesList = EXERCICES.split(",");
        for (const QString &exercise : exercisesList) {
            if (!exercise.trimmed().isEmpty())
                ui->listWidget->addItem(exercise.trimmed());
        }

        ui->exercicestext->clear();
        ui->videoTraitement->clear(); // Clear video URL input
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Not Ok"),
                              QObject::tr("Ajout non effectué.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return;
    }

    ui->tableView_malek->setModel(T.afficherTraitement());
    clearFields();
}


//Suppression d'un traitement après selection d'un traitement sur le tableView_malek
void traitement::on_supprimerTraitement_clicked()
{
    QModelIndex currentIndex = ui->tableView_malek->currentIndex();

    if (!currentIndex.isValid()) {
        QMessageBox::warning(nullptr, QObject::tr("Erreur"),
                             QObject::tr("Veuillez sélectionner une ligne à supprimer."), QMessageBox::Ok);
        return;
    }

    QString IDTRAITEMENT = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(currentIndex.row(), 0)).toString();
    QString TITRE = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(currentIndex.row(), 1)).toString();
    //QDate DEBUTTRAITEMENT = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(currentIndex.row(), 2)).toDate();
    // QDate FINTRAITEMENT = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(currentIndex.row(), 3)).toDate();
    QString MOTIFCONSULTATION = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(currentIndex.row(), 4)).toString();
    QString EXERCICES = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(currentIndex.row(), 5)).toString();
    QString VIDEOTRAITEMENT = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(currentIndex.row(), 6)).toString();

    bool test = Ttmp.supprimerTraitement(IDTRAITEMENT);
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

    ui->tableView_malek->setModel(T.afficherTraitement());
}





//Modifiction d'un traitement après selection d'un traitement sur le tableView_malek
void traitement::on_majTraitement_clicked()
{
    // Extraction de l'information sur les QLineEdits
    QString IDTRAITEMENT = ui->IDtraitement->text();
    QString TITRE = ui->titre->text();
    QDate DEBUTTRAITEMENT = ui->datedebut->date();
    QDate FINTRAITEMENT = ui->datefin->date();
    QString MOTIFCONSULTATION = ui->motifconsultation->text();
    QString EXERCICES = ui->exercicestext->text();
    QString VIDEOTRAITEMENT = ui->videoTraitement->text();

    // appel du fct modifierTraitement de traitementC
    traitementC T(IDTRAITEMENT, TITRE, DEBUTTRAITEMENT, FINTRAITEMENT, MOTIFCONSULTATION, EXERCICES, VIDEOTRAITEMENT);
    bool test = T.modifierTraitement();


    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Ok"), QObject::tr("Modification effectuée\n""Click Cancel to exit."), QMessageBox::Cancel);
        ui->tableView_malek->setModel(T.afficherTraitement());
        clearFields();
    }
}








// extraction PDF
void traitement::on_BoutonPDF_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "Fichiers PDF (*.pdf)");
    if (!fileName.isEmpty()) {
        traitementC T;
        QSqlQueryModel* model = T.afficherTraitement();

        // Get the selected row index from tableView_malek
        QModelIndexList selectedIndexes = ui->tableView_malek->selectionModel()->selectedIndexes();
        if (selectedIndexes.isEmpty()) {
            QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner une ligne.");
            return;
        }
        QModelIndex selectedIndex = selectedIndexes.first(); // Assuming only one row can be selected

        if (T.exporterPDF(fileName, model, selectedIndex)) {
            QMessageBox::information(this, "Exportation PDF", "Les données ont été exportées en PDF avec succès !");
        } else {
            QMessageBox::warning(this, "Exportation PDF", "Échec de l'exportation en PDF.");
        }
    }
}





// tri selon le critère selectioné dans le comboBox
 void traitement::onSortingCriteriaChanged(int index)
 {
     QString sortingCriteria = ui->tri->itemText(index);


     QSqlQueryModel *sortedModel = T.sortTraitement(sortingCriteria);


     ui->tableView_malek->setModel(sortedModel);
 }


//Recherche dans le tableView_malek
 void traitement::on_BoutonRecherche_clicked()
 {
     QString searchTerm = ui->rechercheTableTraitement->text();


     QSqlQueryModel *searchedModel = T.searchTraitement(searchTerm);


     ui->tableView_malek->setModel(searchedModel);
 }


 //affichage des informations après sélection de tableview_malek(méthode de l'index)
 void traitement::onTableViewClicked(const QModelIndex &index)
 {

     if (index.isValid()) {

         QString IDTRAITEMENT = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(index.row(), 0)).toString();


         QStringList exercises = Ttmp.getExercicesById(IDTRAITEMENT);

         // update du To-do list
         ui->listWidget->clear();
         ui->listWidgetDoing->clear();
         ui->listWidgetDone->clear();
         ui->listWidget->addItems(exercises);
     }
 }


// exportation en excel (hyper lente)
 void traitement::on_BoutonExcel_clicked()
 {
     QString fileName = QFileDialog::getSaveFileName(this, "Exporter en Excel", "", "Fichiers Excel (*.xlsx)");
     if (!fileName.isEmpty()) {
         QSqlQueryModel* model = T.afficherTraitement();

         if (T.exporterExcel(fileName, model)) {
             QMessageBox::information(this, "Exportation Excel", "Les données ont été exportées en Excel avec succès !");
         } else {
             QMessageBox::warning(this, "Exportation Excel", "Échec de l'exportation en Excel.");
         }
     }
 }


// ces fcts pour le métier avancé du to-do list



 void traitement::mousePressEvent(QMouseEvent *event)
 {
     if (event->button() == Qt::LeftButton) {
         startPos = event->pos();
         isDragging = true;

         // Find the source list widget under the mouse position
         sourceListWidget = qobject_cast<QListWidget*>(childAt(event->pos()));
     }

     QWidget::mousePressEvent(event);
 }




 void traitement::mouseMoveEvent(QMouseEvent *event)
 {
     if (isDragging) {
         // Determine the distance moved by the mouse
         int distance = (event->pos() - startPos).manhattanLength();

         // Start a drag operation if the distance moved is greater than a threshold
         if (distance >= QApplication::startDragDistance()) {
             // Ensure a valid source list widget and selected item
             if (sourceListWidget && sourceListWidget->currentItem()) {
                 // Find the list widget under the mouse cursor
                 QListWidget *targetListWidget = qobject_cast<QListWidget*>(childAt(event->pos()));

                 // Ensure the target list widget is valid and not the same as the source list widget
                 if (targetListWidget && targetListWidget != sourceListWidget) {
                     // Get the selected item from the source list widget
                     QListWidgetItem *item = sourceListWidget->takeItem(sourceListWidget->row(sourceListWidget->currentItem()));

                     // Add the item to the target list widget
                     targetListWidget->addItem(item);

                     // Remove the item from the source list widget
                     delete item;

                     isDragging = false; // Stop dragging
                 }
             }
         }
     }

     QWidget::mouseMoveEvent(event);
 }








 void traitement::mouseReleaseEvent(QMouseEvent *event)
 {

     if (event->button() == Qt::LeftButton) {
         isDragging = false;
     }


     QWidget::mouseReleaseEvent(event);
 }


 void traitement::deleteSelectedItem(QListWidgetItem *item) {
     // Prompt the user for confirmation
     QMessageBox::StandardButton confirmDelete = QMessageBox::question(this, tr("Confirmation"),
         tr("Vous voulez supprimer cet exercice?"),
         QMessageBox::Yes | QMessageBox::No);

     // If user confirms deletion, remove the item from the listWidget
     if (confirmDelete == QMessageBox::Yes) {
         ui->listWidget->removeItemWidget(item);
         delete item;
     }
 }




// génération du statistiques de progrès à partir du to-do list
 void traitement::statProgres()
 {
     int totalItems = ui->listWidget->count();
     int doingItems = ui->listWidgetDoing->count();
     int doneItems = ui->listWidgetDone->count();

     qreal total = totalItems + doingItems + doneItems;
     qreal todoPercentage = (totalItems / total) * 100.0;
     qreal doingPercentage = (doingItems / total) * 100.0;
     qreal donePercentage = (doneItems / total) * 100.0;

     QPieSeries *series = new QPieSeries();
     series->append("A Faire (" + QString::number(todoPercentage, 'f', 1) + "%)", todoPercentage);
     series->append("En cours (" + QString::number(doingPercentage, 'f', 1) + "%)", doingPercentage);
     series->append("Faites (" + QString::number(donePercentage, 'f', 1) + "%)", donePercentage);

     QChart *chart = new QChart();
     chart->addSeries(series);
     chart->setTitle("Statistique De Progrès");

     QChartView *chartView = new QChartView(chart);
     chartView->setRenderHint(QPainter::Antialiasing);

     chartView->resize(800, 600);
     chartView->show();
 }

 void traitement::closeTodolistTab(int index) {
     // Remove the tab at the specified index
     ui->GestionTraitement->removeTab(index);
 }





// affichage des informations après sélection dans tableView_malek
void traitement::onTableRowClicked(const QModelIndex &index)
{

    if (index.isValid()) {

        QString IDTRAITEMENT = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(index.row(), 0)).toString();
        QString TITRE = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(index.row(), 1)).toString();
        QDate DEBUTTRAITEMENT = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(index.row(), 2)).toDate();
        QDate FINTRAITEMENT = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(index.row(), 3)).toDate();
        QString MOTIFCONSULTATION = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(index.row(), 4)).toString();
        QString EXERCICES = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(index.row(), 5)).toString();
        QString VIDEOTRAITEMENT = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(index.row(), 6)).toString();


        ui->IDtraitement->setText(IDTRAITEMENT);
        ui->titre->setText(TITRE);
        ui->datedebut->setDate(DEBUTTRAITEMENT);
        ui->datefin->setDate(FINTRAITEMENT);
        ui->motifconsultation->setText(MOTIFCONSULTATION);
        ui->exercicestext->setText(EXERCICES);
        ui->videoTraitement->setText(VIDEOTRAITEMENT);
    }
}




//métier avancé; insertion video: c'est pour choisir le fichier et afficher le chemin sur le QLineEdit nommé videoTraitement
void traitement::on_uploadVideo_clicked()
{

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Video File"), "", tr("Video Files (*.mp4 *.avi *.mkv)"));


    if (!filePath.isEmpty()) {

        ui->videoTraitement->setText(filePath);
    }
}







// metier avancé; insetion vidéo: c'est pour voir le vidéo du traitement après sélection de tableView_malek

void traitement::onVoirVideoClicked() {

    QModelIndex currentIndex = ui->tableView_malek->currentIndex();


    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, tr("Erreur"), tr("Veuillez sélectionner une ligne."), QMessageBox::Ok);
        return;
    }


    int videoColumnIndex = -1;
    QAbstractItemModel *model = ui->tableView_malek->model();
    if (model && currentIndex.row() < model->rowCount() && currentIndex.column() < model->columnCount()) {

        for (int column = 0; column < model->columnCount(); ++column) {
            QString columnName = model->headerData(column, Qt::Horizontal).toString();
            if (columnName == "VIDEOTRAITEMENT") {
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
        QMessageBox::warning(this, tr("Erreur"), tr("Chemin de la vidéo non trouvé."), QMessageBox::Ok);
        return;
    }


    QDesktopServices::openUrl(QUrl::fromLocalFile(videoPath));
}








//metier: favoris

//double-clique sur le tableView_malek
void traitement::onTableRowDoubleClicked(const QModelIndex &index)
{

    if (index.isValid()) {

        QMessageBox msgBox;
        msgBox.setText("Voulez-vous ajouter ce traitement au favoris?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int choice = msgBox.exec();


        if (choice == QMessageBox::Yes) {

            addTreatmentToFavorites(index);
        }
    }
}


//ajout aux favoris
void traitement::addTreatmentToFavorites(const QModelIndex &index)
{
    QString IDTRAITEMENT = ui->tableView_malek->model()->data(ui->tableView_malek->model()->index(index.row(), 0)).toString();
    favoriteTreatments.append(IDTRAITEMENT);
}



//consulter favoris
void traitement::on_favoris_clicked()
{

    ui->tableView_malek->setModel(nullptr);


    populateTableViewWithFavorites();
}



//affichage de favoris dans le tableView_malek


void traitement::populateTableViewWithFavorites()
{

    QStandardItemModel *model = new QStandardItemModel(this);


    model->setHorizontalHeaderLabels({"IDTRAITEMENT", "TITRE", "DEBUTTRAITEMENT", "FINTRAITEMENT", "MOTIFCONSULTATION", "EXERCICES", "VIDEOTRAITEMENT"});


    for (const QString &idTrait : favoriteTreatments) {

        traitementC T(idTrait, "", QDate(), QDate(), "", "", "");
        T.fetchTreatmentData();


        if (!T.getTitre().isEmpty() && T.getDebutTraitement().isValid() && T.getFinTraitement().isValid() &&
            !T.getMotifConsultation().isEmpty() && !T.getExercices().isEmpty() && !T.getVideoTraitement().isEmpty()) {

            QList<QStandardItem*> rowItems;
            rowItems << new QStandardItem(idTrait);
            rowItems << new QStandardItem(T.getTitre());
            rowItems << new QStandardItem(T.getDebutTraitement().toString("dd-MM-yyyy"));
            rowItems << new QStandardItem(T.getFinTraitement().toString("dd-MM-yyyy"));
            rowItems << new QStandardItem(T.getMotifConsultation());
            rowItems << new QStandardItem(T.getExercices());
            rowItems << new QStandardItem(T.getVideoTraitement());


            model->appendRow(rowItems);
        }
    }

    ui->tableView_malek->setModel(model);
}







void traitement::openSpotify()
{
    // Open Spotify using QDesktopServices
    QDesktopServices::openUrl(QUrl("spotify:"));
}











//detecteur de motion
void traitement::runMotionDetection() {
    QString pythonScriptPath = "C:/Users/malek/Desktop/traitement/motion_detection.py";

        QProcess process;
        QString command = "python";
        QStringList arguments;
        arguments << pythonScriptPath;

        process.start(command, arguments);
        if (!process.waitForStarted()) {
            qDebug() << "Erreur au démarrage du processus :" << process.errorString();
            return;
        }

        qDebug() << "Processus démarré";

        if (!process.waitForFinished()) {
            qDebug() << "Erreur à la fin du processus :" << process.errorString();
            return;
        }

        qDebug() << "Processus terminé";

        QString output = QString::fromUtf8(process.readAllStandardOutput());
        QString errors = QString::fromUtf8(process.readAllStandardError());
}




