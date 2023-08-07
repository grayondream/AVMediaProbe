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
#include "configor/json.hpp"

using configor::json;
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
	void onExportJson();

	void onFileListChanged(int idx);
	void onShowViewChecked(bool checked);

	void updateInfo(const QString &filename);
	QTreeWidget* paresTreeWidgetFromJson(const QString &filena, const json::value &j);
	void parseUIFromJson(QTreeWidgetItem *win, const json::value &j, const int count);
	void clearLayout(QLayout* layout);
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

	QAction *_openExportAction{};
	QAction *_openExportJsonAction{};

	QMenu *_viewMenu{};
	QAction *_showFramesAction{};

	QComboBox *_fileCombox;
	QGroupBox *_fileInfoGroup;
private:
	QString _lastOpenDirectory;
	QString _lastExportDirectory;
	QTabWidget *_tabWin;
	std::unordered_map<std::string, QWidget*> _tabMaps;
	std::shared_ptr<Controller> _controller;
};


