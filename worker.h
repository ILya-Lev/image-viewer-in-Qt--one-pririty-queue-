#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QImage>

class Worker : public QObject
{
  Q_OBJECT
public:
	explicit Worker(QObject *parent = 0);
	~Worker();
protected:
	virtual void timerEvent(QTimerEvent *);
signals:
	void openedImg(const QImage &, const QString &);
public slots:
	void openedDirList(const QString &, const QStringList &);
private:
	QString path;
	QStringList imgsList;
	int i;
	int timerId;
};

#endif //WORKER_H
