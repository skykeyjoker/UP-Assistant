#include <QApplication>

#include "UpAssistantWidget.h"

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);

	UpAssistantWidget widget;
	widget.show();

	return app.exec();
}
