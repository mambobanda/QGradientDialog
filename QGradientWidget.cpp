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

#include <algorithm>
#include <limits>
#include "QGradientWidget.h"


namespace deepfx {

	QGradientWidget::QGradientWidget(int width, QWidget *parent): QWidget(parent) {
		_indicatorWidth = 25;
		_gradientWidth = width;
		_heightPadding = 6;
		_leftMouseButtonDown = false;
		_selectedIndex = -1; 
		_activeIndex = -1; 
	}
	
	QGradientWidget::~QGradientWidget() {

	}

	int QGradientWidget::widgetHeight() const{
		return parentWidget()->height() - 50;
	}

	int QGradientWidget::widgetWidth() const {
		return _indicatorWidth + _gradientWidth;
	}

	void QGradientWidget::setGradient(std::vector<std::pair<float, QColor>> gradient) {
		_gradient = gradient;
		if (!_gradient.empty()) _activeIndex = 0;

		emit stepUpdated(_gradient[_activeIndex].first, _gradient[_activeIndex].second);

	}

	std::vector<std::pair<float, QColor>> QGradientWidget::getGradient() {
		return _gradient;
	}
	
	QSize QGradientWidget::sizeHint() const {
		return QSize(widgetWidth() + 1, widgetHeight() + 1);
	}

	void QGradientWidget::select(QPoint mousePos) {
		if (!_gradient.empty()) {
			
			if (mousePos.x() >= (_gradientWidth - 6) && mousePos.x() <= (_gradientWidth + 8)) {
				
				auto index = 0;
				for (auto step : _gradient) {

					int distance = 0;
					auto stepPos = step.first * widgetHeight();
					
					if (stepPos > mousePos.y()) distance = stepPos - mousePos.y();
					else distance = mousePos.y() - stepPos;

					if (distance <= 12) {
						_selectedIndex = index;
						_activeIndex = index;

						emit stepUpdated(_gradient[_activeIndex].first, _gradient[_activeIndex].second);
	
						update();

						break;
					}

					index++;
				}
			}

		}
		else {
			_selectedIndex = -1;
		}
	}

	void QGradientWidget::mouseMoveEvent(QMouseEvent *event) {
		if (_leftMouseButtonDown && _selectedIndex != -1) {
			auto newStep = float(event->pos().y()) / float(widgetHeight());
			_gradient[_selectedIndex].first = newStep;

			emit stepUpdated(_gradient[_selectedIndex].first, _gradient[_selectedIndex].second);

			update();
		}
	}

	void QGradientWidget::mousePressEvent(QMouseEvent *event) {
		if (event->button() == Qt::LeftButton) {
			_leftMouseButtonDown = true;
			select(event->pos());
		}
	}

	float QGradientWidget::lerp(float v0, float v1, float t) {
		return (1 - t) * v0 + t * v1;
	}

	void QGradientWidget::mouseReleaseEvent(QMouseEvent *event) {
		if (event->button() == Qt::LeftButton) {
			_leftMouseButtonDown = false;
			
			if (event->pos().x() < (_gradientWidth - 6) && _selectedIndex == -1) {
				auto newStep = float(event->pos().y()) / float(widgetHeight());

				bool alreadyExists = false;
				for (auto step : _gradient) {
					if (step.first == newStep) alreadyExists = true;
				}

				if (!alreadyExists) {
					
					std::pair<float, QColor> _newStep(newStep, QColor(Qt::white));
					_gradient.push_back(_newStep);
				
					std::sort(_gradient.begin(), _gradient.end(), [](std::pair<float, QColor> a, std::pair<float, QColor> b) {
						return a.first < b.first;
						});

					update();

					int index = 0;
					for (auto &step : _gradient) {
						if (step.first == newStep) {
							if (index > 0 && index < (_gradient.size() - 1)) {

								auto red00 = _gradient[index - 1].second.red();    auto red01 = _gradient[index + 1].second.red();
								auto green00 = _gradient[index - 1].second.green();  auto green01 = _gradient[index + 1].second.green();
								auto blue00 = _gradient[index - 1].second.blue();   auto blue01 = _gradient[index + 1].second.blue();

								auto fct_diff = _gradient[index + 1].first - newStep;
								auto fct = fct_diff / (_gradient[index + 1].first - _gradient[index - 1].first);
								auto r = lerp(red00, red01, fct);
								auto g = lerp(blue00, blue01, fct);
								auto b = lerp(green00, green01, fct);
								auto newColor = QColor(int(r), int(g), int(b), 255);
								step.second = newColor;
								
							}
						}

						index++;
					}
				}
			}
		
		}

		_selectedIndex = -1;
	}

	void QGradientWidget::paintEvent(QPaintEvent *event) {
		QPainter painter(this);
		painter.setRenderHint(QPainter::Antialiasing);

		if (!_gradient.empty()) {
			QLinearGradient linearGrad(double(_gradientWidth * 0.5), 0, double(_gradientWidth * 0.5), double(widgetHeight()));

			for (auto step: _gradient) {
				qreal range =  std::min(1.0f, std::max(0.f,step.first));
				linearGrad.setColorAt(range, step.second);
			}
			painter.setBrush(QBrush(linearGrad));
			painter.drawRect(0, _heightPadding, _gradientWidth, widgetHeight());

			auto index = 0;
			for (auto step : _gradient) {
				qreal range = std::min(1.0f, std::max(0.f, step.first));
				qreal offset = widgetHeight() * range;
				offset += _heightPadding;

				QPointF points[3] = { 
					QPointF(_gradientWidth - 6, offset),
					QPointF(_gradientWidth + 8, offset - 6),
					QPointF(_gradientWidth + 8, offset + 6),
				};

				if(index == _activeIndex) painter.setBrush(QColor(Qt::yellow));
				else painter.setBrush(QColor(Qt::white));

				painter.drawConvexPolygon(points, 3);

				index++;
			}		

		}
	
	}

	void QGradientWidget::updateActiveColor(QColor color) {
		_gradient[_activeIndex].second = color;
		update();
	}

	void QGradientWidget::updateActiveStep(float step) {
		_gradient[_activeIndex].first = step;
		update();
	}

	void QGradientWidget::removeActiveStep() {
		if (_gradient.size() > 2) {
			_gradient.erase(_gradient.begin() + _activeIndex);
			_activeIndex = 0;
		}

		emit stepUpdated(_gradient[_activeIndex].first, _gradient[_activeIndex].second);

		update();
	}

	void QGradientWidget::resetGradient() {
		_gradient.clear();
		_gradient.push_back(std::pair<float, QColor>(0.f, QColor(Qt::black)));
		_gradient.push_back(std::pair<float, QColor>(1.f, QColor(Qt::white)));

		_activeIndex = 0;

		emit stepUpdated(_gradient[_activeIndex].first, _gradient[_activeIndex].second);

		update();
	}
}