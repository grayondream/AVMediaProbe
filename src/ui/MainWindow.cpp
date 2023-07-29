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
#include <Contrller.h>

MainWindow::MainWindow(QApplication *app, QWidget *parent)
    : QMainWindow(parent){
	_app = app;
	LOGI("current working directory {}", GlobalConfig::workDir().toStdString().c_str());

	setupUI();
	setupSize();
	setupConnections();
	_controller = std::make_shared<Controller>();
}

void MainWindow::setupSize() {
	setMinimumSize(480, 720);
	_fileInfoGroup->setMinimumHeight(40);
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
	_mainViewLayout = new QVBoxLayout(_centerWin);
	setStatusBar(_infoStatusBar);

	_fileCombox = new QComboBox(this);
	_fileInfoGroup = new QGroupBox(this);

#if DEBUG
	//_centerWin->setStyleSheet("background-color:rgb(255,0,255);");
#endif //DEBUG

	_mainViewLayout->addWidget(_fileCombox);
	_mainViewLayout->addWidget(_fileInfoGroup);
	_mainViewLayout->setStretchFactor(_fileCombox, 1);
	_mainViewLayout->setStretchFactor(_fileInfoGroup, 100);

	setupFileCombox();
}

void MainWindow::setupFileCombox() {
	_fileCombox->addItem("...");
	_fileCombox->installEventFilter(this);
}

void MainWindow::setupConnections() {
	connect(_openExitAction, SIGNAL(triggered()), _app, SLOT(quit()));
	connect(_openOpenAction, SIGNAL(triggered()), this, SLOT(openNewFile()));
	connect(_fileCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFileListChanged(int)));
}

void MainWindow::onFileListChanged(int idx) {
	auto str = _fileCombox->currentText();
	if (str == "...") {
		openNewFile();
	}
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e){
	if (obj == _fileCombox && e->type() == QEvent::MouseButtonPress) {
		onFileListChanged(_fileCombox->currentIndex());
	}
	
	return QMainWindow::eventFilter(obj, e);
}


void MainWindow::openNewFile() {
	QString filename = QFileDialog::getOpenFileName(this, "Select a File", _lastOpenDirectory);
	if (filename.isEmpty()) return;

	_lastOpenDirectory = QFileInfo(filename).absolutePath();

	LOGI("open file {}", filename.toStdString().c_str());
	LOGI("select last directory is {}", _lastOpenDirectory.toStdString().c_str());
	if (!_controller->contain(filename.toStdString())) {
		_controller->insert(filename.toStdString());
		_fileCombox->addItem(filename);
	}

	_fileCombox->setCurrentIndex(_fileCombox->findText(filename));
}

MainWindow::~MainWindow(){

}

