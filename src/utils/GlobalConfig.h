#pragma once
#include <string>
#include <QDir>
#include "LanauageTranslater.h"

constexpr const char *langPath = "lang";
constexpr const char *langFile = "ch.json";

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

	static QString langFile(const QString &file) {
		return workDir() + "/" + langPath + "/" + file;
	}

	static LangTranslater& translater() { return LangTranslater::instance(); }
};

#define TRANS_FETCH(key) GlobalConfig::translater().get(key)