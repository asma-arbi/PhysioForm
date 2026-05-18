#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QWidget>
#include "employeC.h"
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QVBoxLayout>
QT_BEGIN_NAMESPACE
namespace Ui { class employe; }
QT_END_NAMESPACE

class employe :  public QDialog
{
    Q_OBJECT

public:
    explicit  employe(QWidget *parent = nullptr);
    ~employe();
    void clearFields();
       void selectinfo2(const QModelIndex &index1);


private slots:
    void on_ajout_halima_clicked();



    void on_btndelete_halima_clicked();

    void on_update_halima_clicked();

    void on_btnrechercher_halima_clicked();

    void on_pdf_halima_clicked();

    void on_Tri_halima_clicked(int index);

    void on_stat_halima_clicked();
    void on_chatbot_halima_clicked();
    //void sendMessage();
    //void on_SendButton_clicked();
    //void fetchTriviaQuestion(const QString& apiUrl);



    QSqlQueryModel* on_Tri_halima_currentIndexChanged();

    void on_btquitter_clicked();

private:
   Ui::employe *ui;
    employeC Ptmp;
    QString generateChatbotReply(const QString& message);
    QString getRandomJoke();
    QString m_lastJoke;
    QNetworkAccessManager* networkManager;
    struct Question {
           QString question;
           QStringList options;
           QString correctAnswer;
           bool isValid;

           Question()
               : isValid(false) {}

           void clear()
           {
               question.clear();
               options.clear();
               correctAnswer.clear();
               isValid = false;
           }
       };

       Question m_currentQuestion;
       QString decodeHtmlEntities(const QString& text);

private:
    employeC P;
};
#endif // employe_H
