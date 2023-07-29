#include "MainWindow.h"
#include <QHBoxLayout>
#include <qmenubar.h>
#include <QMenu>
#include <QAction>
#include <qdebug.h>
#include <QDir>
#include "GlobalConfig.h"
#include "Log.h"
#include <QFileDialog>

MainWindow::MainWindow(QApplication *app, QWidget *parent)
    : QMainWindow(parent){
	_app = app;
	LOGI("current working directory {}", GlobalConfig::workDir().toStdString().c_str());

	setupSize();
	setupUI();
	setupConnections();
}

void MainWindow::setupSize() {
	
}

void MainWindow::setupUI() {
	setupMenuBar();
	setupWidgets();
}

void MainWindow::setupMenuBar() {
	_menuBar = new QMenuBar(this);
	setMenuBar(_menuBar);
	setupOpenMenu();
}

void MainWindow::setupOpenMenu() {
	_openMenu = new QMenu(_menuBar);
	
	_openOpenAction = new QAction(_openMenu);
	_openCloseAction = new QAction(_openMenu);
	_openExitAction = new QAction(_openMenu);

	_openMenu->addAction(_openOpenAction);
	_openMenu->addSeparator();
	_openMenu->addAction(_openCloseAction);
	_openMenu->addAction(_openExitAction);

	_openMenu->setTitle("Open");
	_openOpenAction->setText("Open");
	_openCloseAction->setText("Close");
	_openExitAction->setText("Exit");

	_menuBar->addMenu(_openMenu);
}

void MainWindow::setupWidgets() {
	setWindowTitle("AVMediaProbe");

	_centerWin = new QWidget(this);
	setCentralWidget(_centerWin);
	setWindowTitle("AVMediaProbe");

	_infoStatusBar = new QStatusBar(this);
	_mainViewLayout = new QHBoxLayout(_centerWin);
	setStatusBar(_infoStatusBar);

#if DEBUG
	_centerWin->setStyleSheet("background-color:rgb(255,0,255);");
#endif //DEBUG
}

void MainWindow::setupConnections() {
	connect(_openExitAction, SIGNAL(triggered()), _app, SLOT(quit()));
	connect(_openOpenAction, SIGNAL(triggered()), this, SLOT(openNewFile()));
}

void MainWindow::openNewFile() {
	QString filename = QFileDialog::getOpenFileName(this, "Select a File", _lastOpenDirectory);
	if (filename.isEmpty()) return;

	_lastOpenDirectory = QFileInfo(filename).absolutePath();

	LOGI("open file {}", filename.toStdString().c_str());
	LOGI("select last directory is {}", _lastOpenDirectory.toStdString().c_str());
}

MainWindow::~MainWindow(){

}

