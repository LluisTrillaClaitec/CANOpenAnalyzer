#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //These things are used by QSettings to set up setting storage
    a.setOrganizationName("LCAD");
    a.setApplicationName("CANOpenAnalyzer");
    a.setOrganizationDomain("lcad.inf.ufes.br");
    QSettings::setDefaultFormat(QSettings::IniFormat);

    MainWindow w;

    QSettings settings;
    int fontSize = settings.value("Main/FontSize", 9).toUInt();
    QFont sysFont = QFont(); //get default font
    sysFont.setPointSize(fontSize);
    a.setFont(sysFont);

    w.show();

    return a.exec();
}
