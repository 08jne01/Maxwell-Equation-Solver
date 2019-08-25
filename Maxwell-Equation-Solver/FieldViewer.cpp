#include "FieldViewer.h"

FieldViewer::FieldViewer(const FileHandler& fileH, Field fieldInit, std::vector<double> drawGeometry): fileHandler(fileH),
displayField(0), modeSet(0), gOn(0), mode(0), field(fieldInit), overlapOn(0), textOn(1)

{
	double ratio = (double)fileHandler.config.pointsY / (double)fileHandler.config.pointsX;
	w = fileHandler.config.screenSize;
	h = (int)((double)w * ratio);
	eigs = fileHandler.config.numModes;
	makeGeometryPoints(drawGeometry);
	getColorScheme();
}

FieldViewer::FieldViewer(const FileHandler& fileH, Field fieldInit, std::vector<double> drawGeometry, std::vector<double> overlaps_) : fileHandler(fileH),
displayField(0), modeSet(0), gOn(0), mode(0), field(fieldInit), overlaps(overlaps_), overlapOn(1),textOn(1)

{
	double ratio = (double)fileHandler.config.pointsY / (double)fileHandler.config.pointsX;
	w = fileHandler.config.screenSize;
	h = (int)((double)w * ratio);
	eigs = fileHandler.config.numModes;
	makeGeometryPoints(drawGeometry);
	getColorScheme();
}

void FieldViewer::getColorScheme()

{
	std::vector<std::vector<double>> buffer;
	fileHandler.readCSV(fileHandler.config.colorMapFileName, buffer, 3);

	for (int i = 0; i < buffer.size(); i++)

	{
		colorMap.push_back(Color(buffer[i][0] * 255, buffer[i][1] * 255, buffer[i][2] * 255));
	}
}

void FieldViewer::makeGeometryPoints(std::vector<double>& drawGeometry)

{
	for (int i = 0; i < w; i++)

	{
		for (int j = 0; j < h; j++)

		{
			//double val = getValue(drawGeometry, fileHandler.config.pointsX, fileHandler.config.pointsY, i, j, w, h);
			//if (val > 10) geometry.append(sf::Vertex(sf::Vector2f(i, (-j + h) % h), sf::Color(val, val, val, 100)));
		}
	}
}

int FieldViewer::mainLoop()

{
	sf::Event events;
	sf::Clock sfClock;
	sf::Text text;
	sf::Font font;
	font.loadFromFile("Resources/arial.ttf");
	text.setFont(font);
	text.setCharacterSize(15);
	text.setPosition(sf::Vector2f(0, 0));
	text.setFillColor(sf::Color::Red);
	windowText = "";
	
	window.create(sf::VideoMode(w, h), "FDFD Maxwell Solver - Field Viewer");

	if (!window.isOpen())

	{
		return EXIT_FAILURE;
	}

	while (window.isOpen())

	{
		while (window.pollEvent(events))

		{
			if (events.type == sf::Event::EventType::Closed)

			{
				window.close();
			}
			//Callback for user commands
			keyCallBack(events);
		}

		if (modeSet != 1)

		{
			setMode(mode);
			text.setString(windowText);
			modeSet = 1;
		}

		window.clear(sf::Color::Black);

		//Draw Here
		draw();
		if (textOn) window.draw(text);
		window.display();

	}

	if (overlapOn == 1) return mode;
	else return EXIT_SUCCESS;
}

void FieldViewer::draw()

{

	window.draw(points);
	if (gOn == 1) window.draw(geometry);
}

void FieldViewer::setMode(int mode)

{
	//Sets the mode
	std::stringstream os;
	os << "Displaying " << field.getFieldName(displayField) << " Component" << std::endl
		<< "Eigen Value: " << field.getFieldName(displayField) << std::endl;

	if (overlapOn == 1) os << "Overlap: " << overlaps[mode] << std::endl;

	os << std::setprecision(10) << "neff: " << sqrt(std::abs((double)field.eigenValues[mode])) / (double)field.k << std::endl
		<< "Current Mode: " << (mode) << std::endl;

	windowText = os.str();

	//std::cout << windowText << std::endl;

	//std::cout << "Displaying " << field.getFieldName(displayField) << " Component" << std::endl;
	//std::cout << "Eigen Value: " << field.eigenValues[mode] << std::endl;
	//if (overlapOn == 1) std::cout << "Overlap: " << overlaps[mode] << std::endl;
	//std::cout << std::setprecision(10) << "neff: " << sqrt(std::abs((double)field.eigenValues[mode])) / (double)field.k << std::endl;

	points.clear();
	std::vector<double> normal;
	Eigen::VectorXd vec = field.getField(displayField).col(mode);
	normalise(vec, normal);
	//Sets color scheme
	for (int i = 0; i < w; i++)

	{
		for (int j = 0; j < h; j++)

		{
			double val = getValue(normal, fileHandler.config.pointsX, fileHandler.config.pointsY, i, j, w, h);
			
			double greyScale = (255.0 / 2.0)*val + (255.0 / 2.0);
			int greyInt = (int)greyScale;
			Color c;
			if (greyInt > colorMap.size()) c = colorMap[colorMap.size() - 1];
			else if (greyInt < 0) c = colorMap[0];
			else c = colorMap[greyInt];
			/*
			int colorR, colorB, colorG;
			if (val < 0.0)

			{
				colorB = 255 * (1 - pow((val + 1), 1));
				colorR = 0;
				colorG = 255 * (1 - 1 * (-(val*val) + 1));
			}

			else

			{
				colorR = 255 * (1 - pow(-(val - 1), 1));
				colorB = 0;
				colorG = 255 * (1 - (-(val*val) + 1));
			}
			*/
			//points.append(sf::Vertex(sf::Vector2f(i, (-j + h) % h), sf::Color(colorR, colorG, colorB)));
			points.append(sf::Vertex(sf::Vector2f(i, (-j + h) % h), sf::Color(c.r, c.g, c.b)));
		}
	}
}

double FieldViewer::interpolate(double d1, double d2, double w)

{
	//Linearly interpolate (there is also bicubic and cosine interpolation, but linear seems best)
	return d1 + w * (d2 - d1);
	//double w2 = (1 - cos(w*PI)) / 2.0;
	//return (d1 * (1 - w2) + d2 * w2);
	/*
	double a = 2 * d1 - 2 * d2;
	double b = -3 * d1 + 3 * d2;
	double d = d1;

	return w*w*w*a + b * w*w + d;
	*/
}

double FieldViewer::getValue(std::vector<double> &gridPoints, int sideLengthX, int sideLengthY, int x, int y, int w, int h)

{
	//Interpolation algorithm (Similar to the last half of a perlin noise algorithm)
	double xVal = ((double)x / (double)w)*(double)sideLengthX;
	double yVal = ((double)y / (double)h)*(double)sideLengthY;

	int xVal0 = (int)xVal;
	int xVal1 = xVal0 + 1;
	int yVal0 = (int)yVal;
	int yVal1 = yVal0 + 1;

	double sX = xVal - double(xVal0);
	double sY = yVal - double(yVal0);
	double ix0, ix1, n0, n1;
	n0 = gridPoints[xVal0 + yVal0 * sideLengthX];
	n1 = gridPoints[xVal1 + yVal0 * sideLengthX];

	ix0 = interpolate(n0, n1, sX);

	n0 = gridPoints[xVal0 + yVal1 * sideLengthX];
	n1 = gridPoints[xVal1 + yVal1 * sideLengthX];

	ix1 = interpolate(n0, n1, sX);
	return interpolate(ix0, ix1, sY);
}

void FieldViewer::normalise(Eigen::VectorXd& vec, std::vector<double>& normalisedVals)

{
	//Normalising algorithm for rendering
	double max = -10.0;

	for (int i = 0; i < vec.size(); i++)

	{
		if (std::abs(vec[i]) > max)

		{
			max = std::abs(vec[i]);
		}
	}

	for (int i = 0; i < vec.size(); i++)

	{
		double val = 1 * vec[i] / max;

		if (val > 1.0)

		{
			val = 1.0;
		}

		else if (val < -1.0)

		{
			val = -1.0;
		}
		normalisedVals.push_back(val);
	}
}

void FieldViewer::writeFields()

{
	//Output fields to a file
	std::ofstream file;
	std::stringstream os;
	std::string s = "Output_Data/Field_Components_Mode_";
	std::string end = ".dat";
	os << s << mode << end;
	file.open(os.str());

	int size = fileHandler.config.pointsX;

	file << "x,y,Ex,Ey,Ez,Hx,Hy,Hz" << std::endl;

	double dx = field.dx;
	double dy = field.dy;

	for (int i = 0; i < fileHandler.config.pointsY; i++)

	{
		for (int j = 0; j < fileHandler.config.pointsX; j++)

		{

			file << i * dx << "," << j * dy << ","
				<< field.Ex.col(mode)[j + size * i] << ","
				<< field.Ey.col(mode)[j + size * i] << ","
				<< field.Ez.col(mode)[j + size * i] << ","
				<< field.Hx.col(mode)[j + size * i] << ","
				<< field.Hy.col(mode)[j + size * i] << ","
				<< field.Hz.col(mode)[j + size * i] << std::endl;
		}
	}

	file.close();
}

void FieldViewer::keyCallBack(sf::Event events)

{	//Callbacks for commands
	if (events.type == sf::Event::EventType::KeyPressed)

	{
		switch (events.key.code)

		{
			case sf::Keyboard::Left:

			{
				displayField--;
				if (displayField < 0) displayField = 5;
				modeSet = 0;
				break;
			}

			case sf::Keyboard::Right:

			{
				displayField++;
				if (displayField > 5) displayField = 0;
				modeSet = 0;
				break;
			}

			case sf::Keyboard::Up:

			{
				mode++;
				if (mode > eigs - 1) mode = 0;
				modeSet = 0;
				break;
			}

			case sf::Keyboard::Down:

			{
				mode--;
				if (mode < 0) mode = eigs - 1;
				modeSet = 0;
				break;
			}

			case sf::Keyboard::Space:

			{
				if (gOn == 1) gOn = 0;
				else gOn = 1;
				break;
			}

			case sf::Keyboard::Return:

			{
				std::cout << "Outputing Current Fields..." << std::endl;
				writeFields();
				break;
			}

			case sf::Keyboard::Escape:

			{
				window.close();
				break;
			}

			case sf::Keyboard::BackSpace:

			{
				window.close();
				mode = -1;
				break;
			}
			case sf::Keyboard::T:

			{
				if (textOn == 1) textOn = 0;
				else textOn = 1;
				break;
			}
		}
	}
}