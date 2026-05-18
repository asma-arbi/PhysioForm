#ifndef TRAITEMENT_H
#define TRAITEMENT_H

#include<QString>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include<QDate>
#include <QRegularExpression>
#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableCell>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextLength>
#include <QSqlQueryModel>
#include <QPrinter>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QAxObject>
#include <QListWidget>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QMimeData>
#include <QDrag>
#include <QDropEvent>
#include <QLabel>
#include <QFormLayout>
#include <QMainWindow>
//Pour métier insertion vidéo
#include <QMediaPlayer>
#include <QtMultimedia>
#include <QtMultimediaWidgets>
#include <QtCore>
#include <QtGui>
#include <QObject>
#include "traitementC.h"
#include <QTranslator>
#include <QComboBox>
#include <QCoreApplication>
#include <QVideoWidget>
#include <QDesktopServices>
#include <QVideoWidget>
#include <QMap>
#include <QDateTime>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
//detecteur de motion
#include <QProcess>
#include <QDialog>
#include <QVBoxLayout>


//integration
#include "patient.h"
#include "employe.h"
#include "paiement.h"
// #include "traitement.h"
#include "reservation.h"

using namespace QtCharts;

QT_BEGIN_NAMESPACE
namespace Ui { class traitement; }
QT_END_NAMESPACE

class traitement : public QDialog
{
    Q_OBJECT

public:
    explicit traitement(QWidget *parent = nullptr);
    ~traitement();
    void clearFields();
    void toggleDarkMode(bool checked);


private slots:
    void on_ajouterTraitement_clicked();
    void on_supprimerTraitement_clicked();
    void on_majTraitement_clicked();
    void on_BoutonPDF_clicked();
    void onSortingCriteriaChanged(int index);
    void on_BoutonRecherche_clicked();
    void onTableViewClicked(const QModelIndex &index);
    void on_BoutonExcel_clicked();

    //metier to do list
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void deleteSelectedItem(QListWidgetItem *item);
    void statProgres();
    void closeTodolistTab(int index);

    void onTableRowClicked(const QModelIndex &index);

    // metier insertion video
    void onVoirVideoClicked();
    void on_uploadVideo_clicked();
    void onTableRowDoubleClicked(const QModelIndex &index);
    void addTreatmentToFavorites(const QModelIndex &index);
    void on_favoris_clicked();
    void populateTableViewWithFavorites();

    void openSpotify();


    //detecteur emotion
     void runMotionDetection();







private:
    Ui::traitement *ui;
    traitementC Ttmp;
    QListWidget *listWidget;
    QListWidget *listWidgetDone;

    QPoint startPos;
    bool isDragging;
    QListWidget *sourceListWidget;










private:
    traitementC T;


     QLabel *labelErrorIDTraitement;
      QLabel *labelErrorTitre;
      QLabel *labelErrorDebut;
      QLabel *labelErrorMotif;
      QLabel *labelErrorExercices;

      QVector<QString> favoriteTreatments;











};
#endif // TRAITEMENT_H
