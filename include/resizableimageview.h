#ifndef _RESIZABLE_IMAGE_VIEW_DOCK_HEADER
#define _RESIZABLE_IMAGE_VIEW_DOCK_HEADER

#include <QDockWidget>
#include <QLabel>
#include <QString>
#include <QScrollBar>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QImageReader>

class ResizableImageViewDock : public QDockWidget
{
	Q_OBJECT

	private:

		//! The central layout
		QVBoxLayout* _layout;
		//! Groups the widgets
		QGroupBox* _control_GroupBox;
		//! Pixmap used to scale image content
		QPixmap _pix;
		//! Label to show an image
		QLabel* _imageLabel;
		//! Scrollbar to scroll image sequence
		QScrollBar* _imageSequence_ScrollBar;
		//! Push to open images 
		QPushButton* _open_images_Button;
		//! Push to hide or unhide the image view
		QPushButton* _hide_show_image_Button;
		//! Push to start a slideshow of the loaded image list
		QPushButton* _start_Slideshow_Button;
		//! Controls the slideshow 
		QTimer* _slideshow_Timer;

		//! The last path where images where loaded from
		/*
			For convenience, to ease browsing for files.
		*/
		QString _lastImagePath;
		
		//! List holding paths of images which should be read from the hdd
		QStringList _imagePaths;

		//! Open images from the harddrive
		QImageReader* _imageReader;

	public:

		ResizableImageViewDock(const QString & title, QWidget* parent);
		ResizableImageViewDock(QWidget* parent);

		QPixmap scaledPixmap() const;


	public slots:
			
		void resizeEvent(QResizeEvent *);

		void setPixmap(const QPixmap & p);

		//! Start a dialog to choose a series of images
		void openImages();

		//! Hide the image (keep only the scrollbar)
		void hideImage();

		//! Show the image again (if hidden)
		void showImage();

		//! Load an image from the harddisk and display on image label
		/*
			\param path The full path to an image file.
		*/
		void loadImage(QString path);

	private slots:

		void setupUI();

	signals:

		//! This signal will be emitted after the displayed image has changed
		void imageChanged();

		//! This signal will be emitted after an image has been loaded from the harddrive
		/*
			This signal is usefull if the widget is used, to process an image sequence.
			E.g. it will be emitted if a new image was loaded from disk by dragging the slider.
			Provided is a full copy of the loaded image as QImage.
		*/
		void imageLoaded(QImage image);

};


#endif