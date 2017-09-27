#include "PhotoChange.h"

#include "requetes.h"
#include "Transform.h"
#include <QMessageBox>
#include <Qbuffer>

PhotoChange::PhotoChange(std::shared_ptr<Member> member, QWidget *parent)
	: QDialog(parent), m_member(member), m_networkAdapter(nullptr)
{
	ui.setupUi(this);

	QStringList args;
	args.append(QString::number(m_member->getId()));
	m_networkAdapter.reset(new NetworkAdapter(PHOTO,GET_FILE,args,this));
    connect(m_networkAdapter.get(), SIGNAL(finished(QStringList,QString)), this ,SLOT(updatePhoto(QStringList,QString)));
	connect(ui.shootHimLabel,SIGNAL(linkActivated(QString)),this,SLOT(savePhoto(QString)));
	connect(ui.annulerLabel,SIGNAL(linkActivated(QString)),this,SLOT(cancelChangePhoto(QString)));
	connect(ui.enregistrerLabel,SIGNAL(linkActivated(QString)),this,SLOT(changePhoto(QString)));

    m_camera = new cv::VideoCapture(CV_CAP_ANY);
    if (!m_camera->isOpened())
	{
		QMessageBox::warning(this,"Webcam","Problème avec la webcam");
		m_camera = nullptr;
	}
	else
	{
		connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateCamera()));
		m_timer.start(15);
	}
}

PhotoChange::~PhotoChange()
{
	if (m_camera)
        m_camera->release();
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void PhotoChange::updatePhoto(QStringList infos, QString data)
{
    if (!data.contains(QString(NO_PHOTO)))
	{
		QByteArray ba(data.toStdString().c_str());
		ba = QByteArray::fromHex(ba);
		QPixmap pixmap;
		pixmap.loadFromData(ba);
        //float ratioHeight = pixmap.height() / ui.imageLabel->height();
        //Transform::addFranchiseToPhoto(&pixmap,m_member->getFranchise(), ratioHeight);
		ui.imageLabel->setPixmap(pixmap);
	}
	else
	{
        QPixmap pixmap(320,240);
        pixmap.fill(Qt::black);
        //float ratioHeight = pixmap.height() / ui.imageLabel->height();
        //Transform::addFranchiseToPhoto(&pixmap,m_member->getFranchise(), ratioHeight);
        ui.imageLabel->setPixmap(pixmap);
		//ui.imageLabel->setPixmap(QPixmap());
    }
}
#pragma GCC diagnostic pop

void PhotoChange::updateCamera()
{
    cv::Mat mat;
    *m_camera >> mat;
    QImage image = Transform::cvMatToQImage(mat).convertToFormat(QImage::Format_Indexed8,Qt::MonoOnly);
    ui.cameraLabel->setPixmap(QPixmap::fromImage(Transform::coloredQImageToMonoQImage(image)));
}

void PhotoChange::savePhoto(const QString&)
{
	if (m_camera)
	{
        cv::Mat mat;
        *m_camera >> mat;
        QImage image = Transform::cvMatToQImage(mat).convertToFormat(QImage::Format_Indexed8,Qt::MonoOnly);
        ui.imageLabel->setPixmap(QPixmap::fromImage(Transform::coloredQImageToMonoQImage(image)));
	}
}

void PhotoChange::cancelChangePhoto(const QString&)
{
	reject();
}

void PhotoChange::changePhoto(const QString&)
{
	QStringList args;
	args.append(QString::number(m_member->getId()));

	const QPixmap *pixmap = ui.imageLabel->pixmap();
	QByteArray ba;
	if (pixmap)
	{
		
		QBuffer buffer(&ba);
		buffer.open(QIODevice::WriteOnly);
		if (pixmap->save(&buffer,JPG))
		{
			ba = ba.toHex();
			args.append(QString(ba));
		}
	}

	m_networkAdapter.reset(new NetworkAdapter(PHOTO,SET_FILE,args,this));

	connect(m_networkAdapter.get(), SIGNAL(finished()), this ,SLOT(accept()));
}
