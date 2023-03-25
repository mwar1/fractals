#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>
#include <thread>

const double boundary = 3;
const int maxIterations = 150;
std::complex<double> c{-0.812, 0.168};

const sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
const unsigned int width = desktop.width;
const unsigned int height = desktop.height;
const unsigned int numThreads = 32;
const float zoomFactor = 1.005f;
const float fadeTime = 2.0f;
float zoom = height / 2;

sf::Vector2<double> centre{0, 0};

const std::vector<sf::Color> colours{sf::Color{10, 10, 10}, sf::Color{0, 0, 100}, sf::Color{255, 150, 150}};

int blendChannel(int chan1, int chan2, float t)  {
	return (int) sqrt((1 - t) * chan1*chan1 + t * chan2*chan2);
}

sf::Color blendColours(sf::Color colour1, sf::Color colour2, float t) {
	sf::Color result;
	result.r = blendChannel(colour1.r, colour2.r, t);
	result.g = blendChannel(colour1.g, colour2.g, t);
	result.b = blendChannel(colour1.b, colour2.b, t);
	return result;
}

sf::Color getColour(float percentage) {
	int lowerBound = floor(percentage * (colours.size() - 1));

	float interval = 1.f / (colours.size() - 1);
	float scaledPerc = (percentage - interval*lowerBound) / interval;

	sf::Color output = blendColours(colours[lowerBound], colours[lowerBound+1], scaledPerc);

	return output;
}

float max(float a, float b) {
	if (a > b) {
		return a;
	}
	return b;
}

// Returns the number of iterations before z is outisde the boundary, where
// z = x + yi
float performFractalCalc(sf::Vector2<double> coords) {
	std::complex<double> z{coords.x, coords.y};
	double mod = std::abs(z);

	int iter = 0;
	while (iter < maxIterations && mod < boundary) {
		z = z*z + c;
		mod = std::abs(z);

		iter++;
	}
	return float(iter) - log2(max(1.0f, log2(mod)));
}

void threadJob(unsigned int id, sf::Image* img) {
	double yDiff = height / numThreads;
	for (double x = 0; x < width; x++) {
		for (double y = id * yDiff; y < (id + 1) * yDiff; y++) {
			sf::Vector2<double> coords{(x - width/2 + centre.x) / zoom, (y - height/2 + centre.y) / zoom};

			float iterations = performFractalCalc(coords);

			float r = iterations / maxIterations;
			img->setPixel(x, y, getColour(r));
		}
	}
}

int main() {	
	sf::RenderWindow window(sf::VideoMode(width, height, desktop.bitsPerPixel), "Fractals", sf::Style::Fullscreen);
	window.setFramerateLimit(60);

	sf::Clock clock;
	float lastTime = 0;

	sf::Image img;
	img.create(width, height);

	sf::Texture tex;
	sf::Sprite sprite;

    while (window.isOpen()) {
		float offset = 500 / zoom;

		// Check events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				zoom *= zoomFactor;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				zoom /= zoomFactor;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				centre += sf::Vector2<double> {-offset, 0};
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				centre += sf::Vector2<double> {offset, 0};
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				centre += sf::Vector2<double> {0, -offset};
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				centre += sf::Vector2<double> {0, offset};
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				c += std::complex<double>{0.01, 0};
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				c += std::complex<double>{-0.01, 0};
        }

		// Logic
		std::vector<std::thread> threads;
		for (int i = 0; i < numThreads; i++) {
			threads.push_back(std::thread(threadJob, i, &img));
		}
		
		for (auto &th : threads) {
			th.join();
		}

		tex.loadFromImage(img);
		sprite.setTexture(tex, true);

		// Draw
        window.clear();
		window.draw(sprite);
        window.display();

		float currentTime = clock.restart().asSeconds();
		float fps = 1.f / (currentTime);
		lastTime = currentTime;
		std::cout << std::to_string(fps) << std::endl;
    }

    return 0;
}