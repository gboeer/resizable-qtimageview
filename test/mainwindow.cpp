#include "mainwindow.h"

 MainWindow::MainWindow() : imageView("Image", this)
{
	setCentralWidget(&imageView);
	connect(&imageView, &ResizableImageViewDock::imageLoaded, this, [=](QImage image){
		imageView.setPixmap(QPixmap::fromImage(image));
	});

	show();
}