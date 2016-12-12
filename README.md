# resizable-qtimageview
A QT5 based Widget used to load, display and slideshow-play a collection of images which can be freely resized while keeping the right aspect ratio.

This is a simple custom widget which can be used to display an image. The displayed image will be resized if the size of the window changes, keeping the right aspect ratio. 
The widget is based on a [QDockWidget](http://doc.qt.io/qt-5/qdockwidget.html) and therefor can be used as a docking widget for the main window.

The **usage** is simple. Suppose you have implemented a [QMainWindow](http://doc.qt.io/qt-5/qmainwindow.html) called MainWindow.  

```cpp
#include "resizableimageview.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
  // create the image view ...
  ResizableImageViewDock imageView("Image", this);
  // and set it as central widget
  setCentralWidget(&imageView);
  
  // to display an image use the setPixmap method as usual for a QLabel
  QImage myImage("cow.jpg");
  imageView.setPixmap(QPixmap::fromImage(myImage));
  
  show();
}
```
This would result in a window as illustrated below:

![imageview_resize](https://cloud.githubusercontent.com/assets/1067159/21003489/ea78f92c-bd2b-11e6-9df4-819f3501f1e5.gif)

## Using a sequence of images

The widget comes with the possibility to load a whole series of images from the disk. 
One can browse through the image sequence manually by using a simple slider or by playing the images as a a slideshow.
Allthough the Widget handles the loading of the images itself they have to be displayed explicitly.
This however can be realised very simple using a provided signal and connection as can be seen in the following example.

```cpp
#include "resizableimageview.h"
#include "mainwindow.h"

MainWindow::MainWindow()
{
  // create the image view ...
  ResizableImageViewDock imageView("Image", this);
  // and set it as central widget
  setCentralWidget(&imageView);
  
  // use the imageLoaded signal to display a loaded image of a loaded image sequence
  connect(&imageView, &ResizableImageViewDock::imageLoaded, this, [&](QImage image)
  {
			imageView.setPixmap(QPixmap::fromImage(image));
  });
  
  show();
}
```

Using this configuration one the widget can be used as displayed below.
Open an image sequence:

![opensequence](https://cloud.githubusercontent.com/assets/1067159/21004206/7be120d0-bd2f-11e6-88f5-71921d8e77e5.gif)

Browse the images:

![slideimages](https://cloud.githubusercontent.com/assets/1067159/21003985/7e67bbbc-bd2e-11e6-9f1d-e2ef44b38c72.gif)

Play a slideshow:

![slideshow](https://cloud.githubusercontent.com/assets/1067159/21003984/7e6652a4-bd2e-11e6-89ae-89ec54f350d1.gif)


## Multiple image views

Since the Widget is actually a docking widget it can be easily added to exisiting windows as docks.
![multiview1](https://cloud.githubusercontent.com/assets/1067159/21006362/df1f3f16-bd38-11e6-96ba-996db764390e.gif)

The whole code needed to implement this example is shown below (for readability shown in one header file).

```cpp
#include <QMainWindow>

#include "resizableimageview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

	ResizableImageViewDock imageView;
	ResizableImageViewDock imageViewDockLeft;
	ResizableImageViewDock imageViewDockRight;

public:

    MainWindow() : imageView("Central Image", this), imageViewDockLeft("Left image", this), imageViewDockRight("Right image", this)
	{

		resize(600, 600);

		setCentralWidget(&imageView);
		connect(&imageView, &ResizableImageViewDock::imageLoaded, this, [=](QImage image){
			imageView.setPixmap(QPixmap::fromImage(image));
		});

		
		imageViewDockLeft.setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
		imageViewDockLeft.setAllowedAreas(Qt::LeftDockWidgetArea);
		addDockWidget(Qt::LeftDockWidgetArea, &imageViewDockLeft);
		connect(&imageViewDockLeft, &ResizableImageViewDock::imageLoaded, this, [=](QImage image){
			imageViewDockLeft.setPixmap(QPixmap::fromImage(image));
		});

		imageViewDockRight.setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
		imageViewDockRight.setAllowedAreas(Qt::RightDockWidgetArea);
		addDockWidget(Qt::RightDockWidgetArea, &imageViewDockRight);
		connect(&imageViewDockRight, &ResizableImageViewDock::imageLoaded, this, [=](QImage image){
			imageViewDockRight.setPixmap(QPixmap::fromImage(image));
		});

		show();
	}

};
```

