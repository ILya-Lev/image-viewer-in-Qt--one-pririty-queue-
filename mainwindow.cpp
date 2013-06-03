#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtCore>
#include <QFileDialog>
#include <QDebug>
#include <QListView>
#include "worker.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    zoom = 4;
    setWindowTitle(tr("Image viewer"));

    itemModel = new QStandardItemModel(this);
    listView = new QListView;

    listView->setViewMode(QListView::IconMode);
    listView->setResizeMode(QListView::Adjust);
    listView->setDragDropMode(QListView::NoDragDrop);
    listView->setModel(itemModel);
    setCentralWidget(listView);

  myWorker = new Worker;
	thread = new QThread(this);
	myWorker->moveToThread(thread);

	connect(this, SIGNAL(imgsListCreated(QString, QStringList)), myWorker, SLOT(openedDirList(QString, QStringList)));
    connect(myWorker, SIGNAL(openedImg(QImage, QString)), this, SLOT(newImageOpened(QImage, QString)));

	connect(thread, SIGNAL(finished()), myWorker, SLOT(deleteLater()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionE_xit_triggered()
{
    close();
}
void MainWindow::closeEvent(QCloseEvent *e)
{
	thread->quit();
}

void MainWindow::on_action_Dir_triggered()
{
    if(path.isEmpty()) path = ".";

	QString tempPath = QFileDialog::getExistingDirectory(this, tr("Select Directory"), path,
                                             QFileDialog::ShowDirsOnly);
	if(!tempPath.isEmpty()){			//in case of "cancel" the path isn't changed
		path = tempPath;
		path.replace("\\", "/");
		path += "/";
	    itemModel->clear();

		dirSelectedHandler();
	}
}
void MainWindow::dirSelectedHandler()
{
    setWindowTitle(tr("Image viewer - %1").arg(path));
    QDir dir(path);
    imgs = dir.entryList(QStringList() << "*.jpg" << "*.jpeg" << "*.bmp" << "*.png");

	for(int i = 0; i < imgs.count(); ++i){
		QStandardItem *item = new QStandardItem(QIcon("cross.png"), imgs.at(i));
		itemModel->appendRow(item);
	}

	thread->start();
	emit imgsListCreated(path, imgs);
}
void MainWindow::newImageOpened(const QImage &image, const QString &name)
{
	QList<QStandardItem *> listItem = itemModel->findItems(name);
	if(!listItem.empty()){
		QModelIndex indx = itemModel->indexFromItem(listItem[0]);
		itemModel->setItem(indx.row(), new QStandardItem(QIcon(QPixmap::fromImage(image)), name));
	}
}
void MainWindow::resizeEvent(QResizeEvent *e)
{
    listView->setIconSize(QSize(size().width()/zoom, size().height()/zoom));
}
