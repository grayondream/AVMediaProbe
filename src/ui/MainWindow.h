#pragma once

#include <QMainWindow>
#include <qapplication.h>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QtWidgets>
#include <vector>
#include <set>
#include <QMouseEvent>
#include <memory>

class Controller;
class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QApplication *app, QWidget *parent = nullptr);
    ~MainWindow();

private:
	void setupSize();
	void setupUI();

    void setupMenuBar();
    void setupWidgets();
    
	void setupOpenMenu();
	void setupConnections();
	void setupFileCombox();

	bool eventFilter(QObject *obj, QEvent *e);
private:
	QApplication *_app{};

private slots:
	void openNewFile();
	void onFileListChanged(int idx);

private:
	QVBoxLayout			*_mainViewLayout{};
	QWidget *_centerWin{};
	QStatusBar			*_infoStatusBar{};

private:
	QMenuBar *_menuBar{};
	
	QMenu *_openMenu{};
	QAction *_openOpenAction{};
	QAction *_openCloseAction{};
	QAction *_openExitAction{};

	QComboBox *_fileCombox;
	QGroupBox *_fileInfoGroup;
private:
	QString _lastOpenDirectory;
	std::shared_ptr<Controller> _controller;
};


