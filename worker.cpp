#include "worker.h"
#include <QtGui>
#include <QtCore>
#include <QImage>

Worker::Worker(QObject *parent) : QObject(parent)
{
}
Worker::~Worker()
{
	killTimer(timerId);				//is it necessary ?
}
void Worker::timerEvent(QTimerEvent *e)
{
	if(e->timerId() == timerId){							                      //to avoid killing something in the system
		if(i >= 0 && i < imgsList.count()){					                  //to avoid being out of range
			QImage img = QImage(path + imgsList.at(i)).scaledToHeight(120);
			if(!img.isNull()){								                          //to avoid emitting signals w null image
				emit openedImg( img , imgsList.at(i) );
			}
			++i;
			timerId = startTimer(0);
		}
	}
	else{
		__super::timerEvent(e);								//to expand the event further if it isn't corrspond to mine timer
	}
}
void Worker::openedDirList(const QString &newPath, const QStringList &newList)
{
	i = 0;
	path = newPath;
	imgsList = newList;
	timerId = startTimer(0);				//null delay tells to wait for next event in a queue~~~
}
