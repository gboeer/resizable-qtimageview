#include <QApplication>


#include "mainwindow.h"


/**
@brief main function of optimus software
*/
int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
    app.setApplicationName("Resizable image view");
  
	MainWindow mainWindow;

    return app.exec();

}