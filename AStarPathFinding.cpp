#include <iostream>
#include <SFML\Graphics.hpp>
#include "Board.h"
using namespace std;

int main()
{
	Board board;
	int xQuads = board.getWidth();//num of quads in the y axis
	int yQuads = board.getHeight();//num of quads in the x axis
	int width = 14;
	int space = 2;
	sf::RenderWindow window(sf::VideoMode(xQuads*(width+space), yQuads*(width+space)), "A* Pathfinder");
	sf::VertexArray Squares(sf::Quads, yQuads*xQuads * 4);//64*64 squares, 4 verts each. This is the visible board.

	cout << "Welcome" << endl;
	cout << "G to place a starting point, Right click to place an end, and Middle click to place obstacles" << endl;
	cout << "Yellow is the quickest path red is searched and purple is considered searching" << endl;
	bool needed = true;


	while (window.isOpen()) {
		sf::Event evnt;
		while (window.pollEvent(evnt)) {

		}
		//Set Vertex locations for visible board
		for (int i = 0; i < yQuads; i++) {
			for (int j = 0; j < xQuads; j++) {
				Squares[4 * (i * xQuads + j)].position = sf::Vector2f(j *(width + space), i * (width + space));
				Squares[4 * (i * xQuads + j) + 1].position = sf::Vector2f(j *(width + space) + width, i * (width + space));
				Squares[4 * (i * xQuads + j) + 2].position = sf::Vector2f(j *(width + space) + width, i * (width + space) + width);
				Squares[4 * (i * xQuads + j) + 3].position = sf::Vector2f(j *(width + space), i * (width + space) + width);

				if (board.isStart(j, i)) {
					Squares[4 * (i * xQuads + j)].color = sf::Color::White;
					Squares[4 * (i * xQuads + j) + 1].color = sf::Color::White;
					Squares[4 * (i * xQuads + j) + 2].color = sf::Color::White;
					Squares[4 * (i * xQuads + j) + 3].color = sf::Color::White;
				}
				else if (board.isEnd(j, i)) {
					Squares[4 * (i * xQuads + j)].color = sf::Color::Blue;
					Squares[4 * (i * xQuads + j) + 1].color = sf::Color::Blue;
					Squares[4 * (i * xQuads + j) + 2].color = sf::Color::Blue;
					Squares[4 * (i * xQuads + j) + 3].color = sf::Color::Blue;
				}
				else if (board.isObstacle(j, i)) {
					Squares[4 * (i * xQuads + j)].color = sf::Color(100, 100, 100, 225);
					Squares[4 * (i * xQuads + j) + 1].color = sf::Color(100, 100, 100, 225);
					Squares[4 * (i * xQuads + j) + 2].color = sf::Color(100, 100, 100, 225);
					Squares[4 * (i * xQuads + j) + 3].color = sf::Color(100, 100, 100, 225);
				}
				else if (board.isPath(j, i)) {
					sf::Color lightGreen = sf::Color::Yellow;
					Squares[4 * (i * xQuads + j)].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 1].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 2].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 3].color = lightGreen;
				}
				else if (board.isVisited(j, i)) {
					sf::Color lightGreen = sf::Color::Red;
					Squares[4 * (i * xQuads + j)].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 1].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 2].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 3].color = lightGreen;
				}
				else if (board.isConsidered(j, i)) {
					sf::Color lightGreen = sf::Color::Magenta;
					Squares[4 * (i * xQuads + j)].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 1].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 2].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 3].color = lightGreen;
				}
				else {
					sf::Color lightGreen = sf::Color(100, 255, 100, 255);
					Squares[4 * (i * xQuads + j)].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 1].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 2].color = lightGreen;
					Squares[4 * (i * xQuads + j) + 3].color = lightGreen;
				}
			}

		}

		//get mouse position in grid space
		int x = sf::Mouse::getPosition(window).x / (width + space);
		int y = sf::Mouse::getPosition(window).y / (width + space);

		//if left mouse pressed place start, needed is true;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && x < xQuads && x >= 0 && y < yQuads && y >= 0) {
			board.setStart(x, y);
			needed = true;
		}
		
		//if middle mouse pressed place obstacle, needed is true;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Middle) && x < xQuads && x >= 0 && y < yQuads && y >= 0) {
			board.setObstacle(x, y);
			needed = true;
		}
		
		//if right mouse pressed place end, needed is true;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && x < xQuads && x >= 0 && y < yQuads && y >= 0) {
			board.setEnd(x, y);
			needed = true;
		}

		//find the path is needed is true, else skip it
		board.findPath(needed);
		needed = false;

		//clear the screen, draw to squares to buffer, display buffer
		window.clear(sf::Color::Black);
		window.draw(Squares);
		window.display();
	}
	cin.get();
	return 0;
}
