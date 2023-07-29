#include <QtWidgets/qmainwindow.h>
#include <QApplication>
#include <mainwindow.h>
#include "Log.h"
#include "GlobalConfig.h"
#include "FileSystem.h"

int main(int argc, char **argv){
	FileSystem::makedir(GlobalConfig::logPath().toStdString());
	LOGSETFILE(GlobalConfig::logFile().toStdString());
#if DEBUG
	LOGSETLEVEL(LogLevel::Log_Debug);
#else
	LOGSETLEVEL(LogLevel::Log_Error);
#endif

	LOGI("Prgram start...");
	QApplication app(argc, argv);
	MainWindow win(&app);
	win.show();
	return app.exec();
}