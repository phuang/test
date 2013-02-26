#include <QApplication>
#include <QPushButton>
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QPushButton helloButton("Hello World");
  helloButton.resize(80, 20);
  helloButton.show();
  return app.exec();
}
