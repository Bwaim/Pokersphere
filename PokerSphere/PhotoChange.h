#ifndef PHOTOCHANGE_H
#define PHOTOCHANGE_H

#include <QDialog>
#include <QTimer>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include "ui_PhotoChange.h"
#include "Member.h"
#include "networkadapter.h"

class PhotoChange : public QDialog
{
	Q_OBJECT

public:
	PhotoChange(std::shared_ptr<Member> member,QWidget *parent = 0);
	~PhotoChange();

private slots:
    void updatePhoto(QStringList infos, QString);
	void updateCamera();
	void savePhoto(const QString&);
	void cancelChangePhoto(const QString&);
	void changePhoto(const QString&);

private:
	Ui::PhotoChangeClass ui;

	std::shared_ptr<Member> m_member;
	std::unique_ptr<NetworkAdapter> m_networkAdapter;
    cv::VideoCapture *m_camera;
	QTimer m_timer;
};

#endif // PHOTOCHANGE_H
