#include <QMainWindow>

#include "../include/resizableimageview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

	ResizableImageViewDock imageView;

public:

    MainWindow() : imageView("Image", this)
	{

		setCentralWidget(&imageView);
		connect(&imageView, &ResizableImageViewDock::imageLoaded, this, [=](QImage image){
			imageView.setPixmap(QPixmap::fromImage(image));
		});

		show();
	}

};