#ifndef CHAT_H
#define CHAT_H

#include <QDialog>
#include <QWidget>
#include "employeC.h"
#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class chat;
}

class chat : public QDialog
{
    Q_OBJECT

public:
    explicit chat(QWidget *parent = nullptr);
    ~chat();
private slots:
    void sendMessage();
    void on_SendButton_clicked();
    void fetchTriviaQuestion(const QString& apiUrl);

private:
    Ui::chat *ui;
    QString generatechatReply(const QString& message);
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
};

#endif // CHAT_H
