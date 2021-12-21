#include "QGradientDialog.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	std::vector<std::pair<float, QColor>> gradient;
	gradient.push_back(std::pair<float, QColor>(0.f, QColor(Qt::black)));
	gradient.push_back(std::pair<float, QColor>(0.5f, QColor(Qt::red)));
	gradient.push_back(std::pair<float, QColor>(1.f, QColor(Qt::white)));

	deepfx::QGradientDialog::editGradient("Gradient Editor", gradient);

    return a.exec(); 
}
