#include <QApplication>

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")// Hide app

#include "UpAssistantWidget.h"

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	app.setWindowIcon(QIcon(":/Res/icon.png"));
	app.setApplicationName("UP主小助手");
	app.setApplicationDisplayName("UP主小助手");
	app.setApplicationVersion("1.0.0");
	app.setOrganizationName("Skykey");
	app.setOrganizationDomain("Diana");

	UpAssistantWidget widget;
	widget.show();

	return app.exec();
}
