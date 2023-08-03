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
#include "Contrller.h"
#include <QFileInfo>
#include "FFmpegSerialKey.h"

MainWindow::MainWindow(QApplication *app, QWidget *parent)
    : QMainWindow(parent){
	_app = app;
	LOGI("current working directory {}", GlobalConfig::workDir().toStdString().c_str());
	GlobalConfig::translater().update(GlobalConfig::langFile(langFile).toStdString());
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
	
	_openExportAction = new QAction(_openMenu);
	_openExportJsonAction = new QAction(_openExportAction);
	_openExportAction->setMenu(new QMenu(_menuBar));
	_openExportAction->menu()->addAction(_openExportJsonAction);

	_openExportAction->setText("Export");
	_openExportJsonAction->setText("json");

	_openMenu->addAction(_openOpenAction);
	_openMenu->addAction(_openExportAction);
	_openMenu->addSeparator();
	_openMenu->addAction(_openCloseAction);
	_openMenu->addAction(_openExitAction);

	_openMenu->setTitle("Open");
	_openOpenAction->setText("Open");
	_openCloseAction->setText("Close");
	_openExitAction->setText("Exit");

	_viewMenu = new QMenu(_menuBar);
	_showFramesAction = new QAction(_viewMenu);
	_viewMenu->addAction(_showFramesAction);

	_viewMenu->setTitle("Views");
	_showFramesAction->setText("Show Frames");
	_showFramesAction->setCheckable(true);

	_menuBar->addMenu(_openMenu);
	_menuBar->addMenu(_viewMenu);
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
	_tabWin = new QTabWidget(this);
	_fileInfoGroup->setLayout(new QVBoxLayout(this));
	_fileInfoGroup->layout()->addWidget(_tabWin);
}

void MainWindow::setupFileCombox() {
	_fileCombox->addItem("...");
	_fileCombox->installEventFilter(this);
}

void MainWindow::setupConnections() {
	connect(_openExitAction, SIGNAL(triggered()), _app, SLOT(quit()));
	connect(_openOpenAction, SIGNAL(triggered()), this, SLOT(openNewFile()));
	connect(_fileCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(onFileListChanged(int)));
	connect(_showFramesAction, &QAction::triggered, this, &MainWindow::onShowViewChecked);
	connect(_openExportJsonAction, SIGNAL(triggered()), this, SLOT(onExportJson()));
}

json::value jsonTranslate(const json::value &j) {
	json::value jj;
	for (auto i = j.begin(); i != j.end(); i++) {
		if (i->is_object()) {
			jj[TRANS_FETCH(i.key())] = jsonTranslate(j[i.key()]);
		}else {
			jj[TRANS_FETCH(i.key())] = j[i.key()];
		}
	}
	
	return jj;
}

void MainWindow::onExportJson() {
	auto str = _fileCombox->currentText();
	if (str == "...") return;
	if (_tabMaps.find(str.toStdString()) != _tabMaps.end()) {
		auto j = _controller->info(str.toStdString());
		const std::string str = json::dump((jsonTranslate(j)));
		QString filename = QFileDialog::getSaveFileName(this, "Select a File to Export", _lastExportDirectory);
		if (filename.isEmpty()) return;

		_lastExportDirectory = QFileInfo(filename).absolutePath();
		filename += ".json";
		QFile file(filename);
		file.open(QIODevice::WriteOnly);
		if (file.isOpen()) {
			file.write(str.c_str());
			file.close();
		}
	}
}

void MainWindow::clearLayout(QLayout* layout){
	while (QLayoutItem* item = layout->takeAt(0)){
		if (QWidget* widget = item->widget())
			widget->deleteLater();

		if (QLayout* childLayout = item->layout())
			clearLayout(childLayout);

		if (QSpacerItem* spaerItem = item->spacerItem())
			layout->removeItem(spaerItem);

		delete item;
	}
}

void MainWindow::onShowViewChecked(bool checked) {
	LOGI("show frame action checked {}", static_cast<int>(checked));
	auto str = _fileCombox->currentText();
	if (str == "...") return;
	if (_tabMaps.find(str.toStdString()) != _tabMaps.end()) {
		clearLayout(_tabMaps[str.toStdString()]->layout());
		delete _tabMaps[str.toStdString()]->layout();
		_tabMaps[str.toStdString()]->update();
		updateInfo(str);
	}
	
}

void MainWindow::onFileListChanged(int idx) {
	auto str = _fileCombox->currentText();
	if (str == "...") {
		openNewFile();
	}

	_tabWin->setCurrentWidget(_tabMaps[str.toStdString()]);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e){
	if (obj == _fileCombox && e->type() == QEvent::MouseButtonPress) {
		onFileListChanged(_fileCombox->currentIndex());
	}
	
	return QMainWindow::eventFilter(obj, e);
}

void MainWindow::parseUIFromJson(QTreeWidgetItem *win, const json::value &j) {
	for (auto && i = j.begin(); i != j.end(); i++) {
		LOGI("key is {}", i.key().c_str());
		auto item = new QTreeWidgetItem({ TRANS_FETCH(i.key()).c_str() });
		if (j[i.key()].is_object()) {
			if (i.key() != kStreamFrames || _showFramesAction->isChecked()) {
				parseUIFromJson(item, j[i.key()]);
			}
		}else {
			auto v = j[i.key()];
			auto vd = json::dump(v);
			if (v.is_string()) {
				vd = std::string(vd.begin() + 1, vd.end() - 1);
			}
			item->setText(0, (TRANS_FETCH(i.key()) + " : " + vd).c_str());
		}

		if (i.key() != kStreamFrames || _showFramesAction->isChecked()) {
			win->addChild(item);
		}
	}
}

QTreeWidget* MainWindow::paresTreeWidgetFromJson(const QString &filename, const json::value &j){
	QTreeWidget *treeWin = new QTreeWidget(_tabMaps[filename.toStdString()]);
	treeWin->headerItem()->setText(0, QFileInfo(filename).baseName());
	treeWin->setStyle(QStyleFactory::create("windows"));
	treeWin->setHeaderHidden(true);
	for (auto && i = j.begin(); i != j.end(); i++) {
		auto item = new QTreeWidgetItem({ i.key().c_str() });
		if (j[i.key()].is_object()) {
			parseUIFromJson(item, j[i.key()]);
		}
		treeWin->addTopLevelItem(item);
	}

	treeWin->expandToDepth(0);
	return treeWin;
}

void MainWindow::updateInfo(const QString &filename) {
	if (filename == "...") return;
	QVBoxLayout *l = new QVBoxLayout(this);
	_tabMaps[filename.toStdString()]->setLayout(l);
	//setupUIFromJson(_tabMaps[filename.toStdString()], ));
	json::value j = _controller->info(filename.toStdString());
	
	//treeWin->setSortingEnabled(true);
	//treeWin->header()->setSortIndicator(0, Qt::AscendingOrder);
	l->addWidget(paresTreeWidgetFromJson(filename, j));
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
		_tabMaps[filename.toStdString()] = new QWidget(_tabWin);
		_tabWin->addTab(_tabMaps[filename.toStdString()], QFileInfo(filename).baseName());
		updateInfo(filename);
	}

	_fileCombox->setCurrentIndex(_fileCombox->findText(filename));
}

MainWindow::~MainWindow(){

}

