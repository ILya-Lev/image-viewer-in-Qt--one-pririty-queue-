#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QStringList>

class QListView;
class QStandardItemModel;
class Worker;
class QThread;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
  virtual void closeEvent(QCloseEvent *);
    virtual void resizeEvent(QResizeEvent *);
private slots:
    void on_actionE_xit_triggered();
    void on_action_Dir_triggered();
    void newImageOpened(const QImage &, const QString &);
signals:
    void imgsListCreated(const QString &, const QStringList &);
private:
    Ui::MainWindow *ui;
    QString path;
    QStringList imgs;
    QListView *listView;
    QStandardItemModel *itemModel;
	Worker *myWorker;
	QThread *thread;
    int zoom;

	void dirSelectedHandler();
};

#endif // MAINWINDOW_H
