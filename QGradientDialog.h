/*
Copyright (c) 2022 Deep FX

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <QWidget>
#include <QDialog>
#include <QBoxLayout>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <vector>
#include <QColorDialog>
#include "QGradientWidget.h"

namespace deepfx {

	class QGradientDialog : public QDialog {
		Q_OBJECT
	private: 

		// Layouts
		QHBoxLayout *_mainLayout;

		QFormLayout *_controlsLayout;

		// Contols
		QPushButton *_reset;
		QPushButton *_remove;
		QPushButton *_color;
		QDoubleSpinBox *_range;
		QDoubleSpinBox *_alpha;

		// Gradient Widget
		QGradientWidget *_gradientWidget;
	public:
		QGradientDialog(QWidget *parent = nullptr);

		static void editGradient(QString title, std::vector<std::pair<float, QColor>> &gradient);

	public slots:
		void update(float step, QColor color);
		void colorButtonPushed();
		void rangeChanged(double d);
		void remove();
		void reset();
	};

}