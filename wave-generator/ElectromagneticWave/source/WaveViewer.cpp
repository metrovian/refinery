#include "WaveViewer.h"

WaveViewer::WaveViewer(unsigned int _width, unsigned int _height)
{
	width = _width;
	height = _height;
}

bool WaveViewer::onMoveSliderKnob(const TimeField& _data)
{
	sf::Vector2i mpos = sf::Mouse::getPosition(window);

	float spos = knob.getPosition().x + knob.getRadius();

	size_t mdx = _data.size() - 1;

	if (std::fabs(mpos.x - spos) <= 10)
	{
		while (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i nmpos = sf::Mouse::getPosition(window);

			kpos = nmpos.x - knob.getRadius();

			if (kpos < minpos - rad) kpos = minpos - rad;
			else if (kpos > maxpos - rad) kpos = maxpos - rad;

			time = static_cast<size_t>((kpos - minpos + rad) / (maxpos - minpos) * mdx);

			draw(_data);
		}
	}

	return true;
}

bool WaveViewer::draw(const TimeField& _data)
{
	window.clear();

	drawField(_data[time]);
	drawSlider();

	window.display();

	return true;
}

bool WaveViewer::draw(const WaveField& _data)
{
	window.clear();

	drawField(_data);

	window.display();

	return true;
}

bool WaveViewer::drawField(const WaveField& _field)
{
	sf::Vector2u size = window.getSize();

	size_t rows = _field.getNX();
	size_t cols = _field.getNY();

	if (rows <= 0) return false;
	if (cols <= 0) return false;

	float cx = (float)size.x / (float)rows;
	float cy = (float)size.y / (float)cols;

	sf::RectangleShape cell(sf::Vector2f(cx, cy));

	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; ++j)
		{
			cell.setPosition((float)i * cx, (float)j * cy);
			cell.setFillColor(calcColorGradient(calcColorScale(_field.getField(i, j).z())));

			window.draw(cell);
		}
	}

	return true;
}

bool WaveViewer::drawSlider()
{
	sf::Vector2u size = window.getSize();

	box = (float)size.x / 1.5;
	boy = (float)size.y / 350.0;
	rad = (float)size.y / 150.0;

	minpos = (size.x - box) / 2.0;
	maxpos = (size.x + box) / 2.0;

	slider = sf::RectangleShape(sf::Vector2f(box, boy));
	slider.setPosition((size.x - box) / 2.0, size.y - boy * 15.0);
	slider.setFillColor(sf::Color::White);

	if (kpos < minpos - rad) kpos = minpos - rad;
	else if (kpos > maxpos - rad) kpos = maxpos - rad;

	knob = sf::CircleShape(rad);
	knob.setFillColor(sf::Color::Red);
	knob.setPosition(kpos, size.y - boy * 14.5 - rad);

	window.draw(slider);
	window.draw(knob);

	return true;
}

bool WaveViewer::setColorScale(const TimeField& _data)
{
	scale = 0.0;

	if (_data.size() == 0) return false;
	size_t rows = _data[0].getNX();
	size_t cols = _data[0].getNY();

	for (size_t t = 0; t < _data.size(); ++t)
	{
		for (size_t i = 0; i < rows; ++i)
		{
			for (size_t j = 0; j < cols; ++j)
			{
				if (scale < fabs(_data[t].getField(i, j).z()))
				{
					scale = fabs(_data[t].getField(i, j).z());
				}
			}
		}
	}

	return true;
}

bool WaveViewer::setColorScale(const WaveField& _data)
{
	scale = 0.0;

	size_t rows = _data.getNX();
	size_t cols = _data.getNY();

	for (size_t i = 0; i < rows; ++i)
	{
		for (size_t j = 0; j < cols; ++j)
		{
			if (scale < fabs(_data.getField(i, j).z()))
			{
				scale = fabs(_data.getField(i, j).z());
			}
		}
	}

	return true;
}

double WaveViewer::calcColorScale(double _rval) const
{
	if (scale > 0)
	{
		if (_rval > 0) return sqrt(_rval / scale);
		return -sqrt(-_rval / scale);
	}

	else
	{
		return 0.0;
	}
}

sf::Color WaveViewer::calcColorGradient(double _nval) const
{
	if (_nval > 0) return sf::Color((sf::Uint8)(255.0 * _nval), 0, 0);
	else return sf::Color(0, 0, (sf::Uint8)(255.0 * (-_nval)));
}

bool WaveViewer::display(const TimeField& _data)
{
	if (!setColorScale(_data)) return false;

	window.create(sf::VideoMode(width, height), "Electromagnetic Field Distribution Viewer", sf::Style::Titlebar | sf::Style::Close);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::Resized) window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) onMoveSliderKnob(_data);
		}

		draw(_data);
	}

	return true;
}

bool WaveViewer::display(const WaveField& _data)
{
	if (!setColorScale(_data)) return false;

	window.create(sf::VideoMode(width, height), "Electromagnetic Field Distribution Viewer", sf::Style::Titlebar | sf::Style::Close);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) window.close();
			if (event.type == sf::Event::Resized) window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
		}

		draw(_data);
	}

	return true;
}
