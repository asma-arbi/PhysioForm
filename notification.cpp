#include "notification.h"

void notification::notification_ajout()
{
    QSystemTrayIcon *notifyIcon = new QSystemTrayIcon;
       notifyIcon->setIcon(QIcon("C:/Users/telechargement/new employe.png"));
       //notifyIcon->setVisible(1);
       notifyIcon->show();
       notifyIcon->showMessage("Physioform"," Bienvenue chez nous! ",QSystemTrayIcon::Information,15000);


}
