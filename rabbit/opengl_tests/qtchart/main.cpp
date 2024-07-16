#include <QtWidgets>
#include <rabbit/chart/qtchart.h>

int main(int argc, char ** argv) 
{
	QApplication app(argc, argv);

	rabbit::qtchart chart;
	chart.show();

	chart.add_data(0,0);
	chart.add_data(1,10);

	app.exec();
}