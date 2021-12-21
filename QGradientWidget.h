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
#include <QPainter.h>
#include <QMouseEvent>

namespace deepfx {

	class QGradientWidget : public QWidget {
		Q_OBJECT
	private:

		int _indicatorWidth;
		int _gradientWidth;
		int _heightPadding;

		bool _leftMouseButtonDown;

		int _activeIndex;
		int _selectedIndex; 
		
		std::vector<std::pair<float, QColor>> _gradient;

		int widgetHeight() const;
		int widgetWidth() const;
		
		float lerp(float v0, float v1, float t);

		void select(QPoint mousePo);
	protected:

		QSize sizeHint() const override;
		void paintEvent(QPaintEvent *event) override;

		void mouseMoveEvent(QMouseEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void mouseReleaseEvent(QMouseEvent *event) override;

	public:

		QGradientWidget(int width = 76, QWidget *parent = nullptr);
		virtual ~QGradientWidget();

		void setGradient(std::vector<std::pair<float, QColor>> gradient);
		std::vector<std::pair<float, QColor>> getGradient();

	signals:
		void stepUpdated(float step, QColor color);

	public slots:
		void updateActiveColor(QColor color);
		void updateActiveStep(float step);
		void removeActiveStep();
		void resetGradient();
	};

}