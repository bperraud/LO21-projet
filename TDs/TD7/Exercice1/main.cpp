#include <QApplication>
#include <QPushButton>

int main(int argc, char* argv[]){

    QApplication app(argc, argv);

    QPushButton button("Quitter");
    button.show();
    QObject::connect(&button, SIGNAL(clicked()), &app, SLOT(quit()));

    QPushButton button2("coucou");
    button2.show();

    return app.exec();
}
