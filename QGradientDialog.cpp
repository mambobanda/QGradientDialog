#include "QGradientDialog.h"

namespace deepfx {
	
	QGradientDialog::QGradientDialog( QWidget *parent): QDialog(parent) {
		_mainLayout = new QHBoxLayout;

		_controlsLayout = new QFormLayout;

		_remove = new QPushButton("Remove");
		_reset = new QPushButton("Reset");
		_color  = new QPushButton();
		_range = new QDoubleSpinBox;
		_alpha = new QDoubleSpinBox;

		_gradientWidget = new QGradientWidget;

		_range->setMinimum(0.0);
		_range->setMaximum(1.0);
		_range->setSingleStep(0.01);

		_controlsLayout->addRow("", _remove);
		_controlsLayout->addRow("Color", _color);
		_controlsLayout->addRow("Range", _range);
		_controlsLayout->addRow("", _reset);

		_mainLayout->addWidget(_gradientWidget);
		_mainLayout->addLayout(_controlsLayout);

		setLayout(_mainLayout);

		QObject::connect(_gradientWidget, &QGradientWidget::stepUpdated, this, &QGradientDialog::update);
		QObject::connect(_color, &QPushButton::clicked, this, &QGradientDialog::colorButtonPushed);
		QObject::connect(_range, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &QGradientDialog::rangeChanged);
		QObject::connect(_remove, &QPushButton::clicked, this, &QGradientDialog::remove);
		QObject::connect(_reset, &QPushButton::clicked, this, &QGradientDialog::reset);

	}

	void QGradientDialog::editGradient(QString title, std::vector<std::pair<float, QColor>> &gradient) {
		QGradientDialog dialog;
		dialog._gradientWidget->setGradient(gradient);
		dialog.exec();

		gradient = dialog._gradientWidget->getGradient();
	}

	void QGradientDialog::update(float step, QColor color) {
		_color->setStyleSheet(_color->styleSheet() + "background-color:" + color.name() + ";");
		_range->setValue(step);
	}

	void QGradientDialog::colorButtonPushed() {
		QColorDialog dialog;
		auto color = dialog.getColor(Qt::white, nullptr, "Color");

		_gradientWidget->updateActiveColor(color); 
	}

	void QGradientDialog::rangeChanged(double d) {
		_gradientWidget->updateActiveStep(d);
	}

	void QGradientDialog::remove() {
		_gradientWidget->removeActiveStep();
	}

	void QGradientDialog::reset() {
		_gradientWidget->resetGradient();
	}
}
