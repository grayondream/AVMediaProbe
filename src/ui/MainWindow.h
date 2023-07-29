#pragma once

#include <QMainWindow>
#include <qapplication.h>
#include <QStatusBar>
#include <QHBoxLayout>

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

private:
	QApplication *_app{};

private slots:
	void openNewFile();

private:
	QHBoxLayout			*_mainViewLayout{};
	QWidget *_centerWin{};
	QStatusBar			*_infoStatusBar{};

private:
	QMenuBar *_menuBar{};
	
	QMenu *_openMenu{};
	QAction *_openOpenAction{};
	QAction *_openCloseAction{};
	QAction *_openExitAction{};

private:
	QString _lastOpenDirectory;
};


