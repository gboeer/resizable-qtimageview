#include <QMainWindow>

#include "../include/resizableimageview.h"

//! This is a simple GUI to demonstrate the ResizableImageViewDock widget
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

	ResizableImageViewDock imageView;

public:

    MainWindow();

};