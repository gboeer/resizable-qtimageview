#include "../include/resizableimageview.h"

#include <QGroupBox>
#include <QScrollBar>
#include <QPushButton>
#include <QGridLayout>
#include <QStyle>
#include <QLayout>
#include <QFileDialog>
#include <QTimer>
#include <QImageReader>

ResizableImageViewDock::ResizableImageViewDock(const QString & title, QWidget* parent) : QDockWidget(title, parent)
{
	_imageReader = new QImageReader;
	_imageReader->setAutoTransform(false);
	setupUI();
}

ResizableImageViewDock::ResizableImageViewDock(QWidget* parent) : QDockWidget(parent)
{
	_imageReader = new QImageReader;
	_imageReader->setAutoTransform(false);
	setupUI();
}

void ResizableImageViewDock::setupUI()
{

	_imageLabel = new QLabel();
	//_imageLabel->setFrameStyle(QFrame::Panel);
	_imageLabel->setMinimumSize(1, 1);
	_imageLabel->setContentsMargins(0, 0, 0, 0);
	_imageLabel->setScaledContents(false);
	
	_imageSequence_ScrollBar = new QScrollBar;
	_imageSequence_ScrollBar->setOrientation(Qt::Horizontal);
	_imageSequence_ScrollBar->setSingleStep(1);
	_imageSequence_ScrollBar->setMinimum(0);
	_imageSequence_ScrollBar->setPageStep(10);
	_imageSequence_ScrollBar->setEnabled(false);
	_imageSequence_ScrollBar->setMinimumWidth(100);
	
	connect(_imageSequence_ScrollBar, &QScrollBar::valueChanged, this, [=]()
	{
		loadImage(_imagePaths[_imageSequence_ScrollBar->value()]);
	});

	_start_Slideshow_Button = new QPushButton;
	_start_Slideshow_Button->setToolTip(tr("Play/Stop the loaded image sequence as slideshow"));
	_start_Slideshow_Button->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
	_start_Slideshow_Button->setMaximumSize(30, 30);
	_start_Slideshow_Button->setStyleSheet("QPushButton{background: transparent;}"); 
	_start_Slideshow_Button->setEnabled(false);
	connect(_start_Slideshow_Button, &QPushButton::pressed, this, [=]()
	{
		if (_slideshow_Timer->isActive())
		{
			_slideshow_Timer->stop();
			_start_Slideshow_Button->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
		}
		else
		{
			_slideshow_Timer->start(10);
			_start_Slideshow_Button->setIcon(this->style()->standardIcon(QStyle::SP_MediaStop));
		}
	});

	_slideshow_Timer = new QTimer;
	connect(_slideshow_Timer, &QTimer::timeout, this, [=]()
	{
		// will toggle loading of image by &QScrollBar::valueChanged signal
		_imageSequence_ScrollBar->setValue(_imageSequence_ScrollBar->value()+1);
	});

	_open_images_Button = new QPushButton;
	_open_images_Button->setToolTip(tr("Open series of images"));
	_open_images_Button->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
	_open_images_Button->setMaximumSize(30, 30);
	_open_images_Button->setStyleSheet("QPushButton{background: transparent;}"); 
	connect(_open_images_Button, &QPushButton::clicked, this, &ResizableImageViewDock::openImages);

	_hide_show_image_Button = new QPushButton;
	_hide_show_image_Button->setToolTip(tr("Push to hide the image"));
	_hide_show_image_Button->setIcon(this->style()->standardIcon(QStyle::SP_ArrowDown)); 
	_hide_show_image_Button->setMinimumSize(30,30);
	_hide_show_image_Button->setStyleSheet("QPushButton{background: transparent;}"); 
	connect(_hide_show_image_Button, &QPushButton::clicked, this, [=](){	
		_imageLabel->isHidden() ? showImage() : hideImage();
	});


	// layout the widgets
	QGridLayout *sublayout = new QGridLayout;
	sublayout->addWidget(_open_images_Button, 0, 0, 1, 1, Qt::AlignLeft);
	sublayout->addWidget(_start_Slideshow_Button, 0, 1, 1, 1, Qt::AlignLeft);
	sublayout->addWidget(_hide_show_image_Button, 0, 2, 1, 1, Qt::AlignLeft);
	sublayout->addWidget(_imageSequence_ScrollBar, 0, 3, 1, 2);
	sublayout->setColumnStretch(3, 2);

	_layout = new QVBoxLayout;
	_layout->setAlignment(Qt::AlignCenter);
	_layout->addWidget(_imageLabel);
	_layout->addLayout(sublayout);

	_control_GroupBox = new QGroupBox;
	_control_GroupBox->setLayout(_layout);
	this->setWidget(_control_GroupBox);
}

QPixmap ResizableImageViewDock::scaledPixmap() const
{
	return _pix.scaled(this->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void ResizableImageViewDock::resizeEvent(QResizeEvent * e)
{
	 if(!_pix.isNull())
        _imageLabel->setPixmap(scaledPixmap());
	
}

void ResizableImageViewDock::setPixmap(const QPixmap & p)
{
	_pix = p;
	_imageLabel->setPixmap(scaledPixmap());
	emit imageChanged();
}

void ResizableImageViewDock::openImages()
{
	QFileDialog dialog(this, tr("Open image sequence"), _lastImagePath.isEmpty() ? QDir::currentPath() : _lastImagePath);

	// get supported image mime types (for file dialogs)
	QStringList mime_ImageTypes;
	foreach (const QByteArray &mimeTypeName, QImageReader::supportedMimeTypes())
    mime_ImageTypes.append(mimeTypeName);
	mime_ImageTypes.sort();

    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setMimeTypeFilters(mime_ImageTypes);
    dialog.selectMimeTypeFilter("image/bmp");
	
	if (dialog.exec())
	{
		_lastImagePath = dialog.directory().absolutePath();

		_imagePaths = dialog.selectedFiles();
		_imageSequence_ScrollBar->setMaximum(_imagePaths.size()-1);
		_imageSequence_ScrollBar->setEnabled(true);
		_imageSequence_ScrollBar->setValue(0);
		loadImage(_imagePaths[_imageSequence_ScrollBar->value()]);	
		_start_Slideshow_Button->setEnabled(true);
	}
}

void ResizableImageViewDock::loadImage(QString path)
{
	_imageReader->setFileName(path);
    QImage image = _imageReader->read();
    // if image could not be loaded reset pixmap
	if (image.isNull()) 
	{
		setPixmap(QPixmap());
    }
	else
	{
		emit imageLoaded(image);
	}
}

void ResizableImageViewDock::hideImage()
{
	if (!_imageLabel->isHidden())
	{
		_imageLabel->hide();
		_hide_show_image_Button->setToolTip(tr("Push to show the image"));
		_hide_show_image_Button->setIcon(this->style()->standardIcon(QStyle::SP_ArrowUp)); 
	}
}

void ResizableImageViewDock::showImage()
{
	if (_imageLabel->isHidden())
	{
		_imageLabel->show();
		_hide_show_image_Button->setToolTip(tr("Push to hide the image"));
		_hide_show_image_Button->setIcon(this->style()->standardIcon(QStyle::SP_ArrowDown)); 
	}
}