#pragma once
#include <string>
#include <QDir>

class ViewController;
class GlobalConfig{
public:
	static QString workDir() {
		QString workingDir = QDir::currentPath();
#if DEBUG
		workingDir += "/Debug";
#endif//DEBUG
		return workingDir;
	}
	static QString logPath() {
		return workDir() + "/log";
	}
	static QString logFile() {
		return logPath() + "/log.txt";
	}
};

