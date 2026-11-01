#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QApplication::setApplicationName("Campus Esports Arena");
    QApplication::setOrganizationName("Campus Esports Arena");

    MainWindow window;
    window.show();

    return app.exec();
}
