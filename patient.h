#ifndef PATIENT_H
#define PATIENT_H

#include <QWidget>
#include "patientC.h"
#include <QByteArray>

#include <QMediaPlayer>
#include <QtMultimediaWidgets>
#include <QtMultimedia>
#include <QObject>
#include <QTranslator>
#include <QComboBox>
#include <QCoreApplication>
#include <QVideoWidget>
#include <QDesktopServices>
#include <QDateTime>
#include <QList>
#include <QAudioRecorder>
#include <QtGui>
#include <QtCore>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QSqlDatabase>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QDialog>
#include <QVBoxLayout>


QT_BEGIN_NAMESPACE



namespace Ui { class patient; }
QT_END_NAMESPACE

class patient : public QDialog
{
    Q_OBJECT

public:
    explicit  patient(QWidget *parent = nullptr);
    ~patient();
     void clearFields();

private slots:
    void on_ajout_feriel_clicked();
    void on_btndelete_ferie_clicked();
    void on_update_feriel_clicked();
    void on_btnrechercher_feriel_clicked();
    void on_pdf_feriel_clicked();
    void on_Trieer_feriel_clicked();
    void on_stat_feriel_clicked();
//AMELIOARTION AFFICHAGE
    void on_BtnAffchgFeriel_clicked();
    void onTableRowClickedFER(const QModelIndex &index);
    void onTableViewClickedFER2(const QModelIndex &index);
    void on_tableView_feriel_clicked(const QModelIndex &index);
//IMAGE
    void on_inserer_image_clicked();
     void updateImage(const QString &imagePath);
   void on_Afficher_image_clicked();

    void textToSpeech();
    void imageCaptured(int reqid,const QImage &img);
    void on_camerastart_clicked();
    void on_camerastop_clicked();
    void on_capture_clicked();
    void on_pb_historiquefer_clicked();
    void afficherHistoriquefer();
   //void on_voicerecord_toggled(bool checked);
   // QString transcribeAudio(const QString& audioFilePath);


    void on_envoyer_certif_clicked();
   // void printMessageToFile(const QString &message, const QString &fileName);
   //    void sendEmailWithFileAttachment(const QString &attachmentFileName);
    //   void sendEmailAfterEncryption();
      // void handleSocketError(QAbstractSocket::SocketError socketError);
       // void attachFileToEmail(const QString &fileName, const QString &content, QSslSocket &socket);
    void on_pdf_feriel_2_clicked();
    void on_clearfer_clicked();


    void on_startrecord_clicked(bool checked);

    void on_ListenToRecord_clicked();
    QString getAudioFilePathFromDatabase(const QString& cin);
    void on_playButton_clicked();
       void on_pauseButton_clicked();
       void on_stopButton_clicked();
       void on_fastForwardButton_clicked();
       void on_rewindButton_clicked();


private :
    void Start_Camera();
    void stop_Camera();

private:
   Ui::patient *ui;
    patientC Ptmp;

private:
    patientC P;
    QScopedPointer<QCamera> M_camera;
    QScopedPointer<QCameraImageCapture> M_camera_Image;
    int counter =0;
    QImage ImageFromCamera;
    //VoiceIA
    QMediaPlayer *player;
       QAudioRecorder *audioRecorder;
       QAudioRecorder *audioRecorderAI;
       QString audioFilePath;
       QModelIndex getCurrentSelectionIndex();
        QStringList getPatientInformation(const QModelIndex &index);
  ;
};
#endif // PATIENT_H
