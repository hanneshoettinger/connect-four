/////////////////////////////////////////////////////////////////////////////////////////////////////
// MiniMax4.cpp : Defines the entry point for the console application.							   //
// SFML version of Connect 4 / Score 4 by Hannes Hoettinger using MiniMax Algorithm                //
/////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SFML\Graphics.hpp"

//#include "global.h"

#include "ScoreMiniMax.h"

// minimax Object
ScoreMiniMax minimax;

void loadParameters(int argc, char *argv[])
{
	for (int i = 1; i<argc; i++)
		if (!strcmp(argv[i], "-debug"))
			g_debug = 1;
		else if (!strcmp(argv[i], "-level"))
			g_maxDepth = atoi(argv[i + 1]);
}

void initGame(Board &board)
{
	// fill board with zeros
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			board._slots[y][x] = Empty;
		}
	}
	// Console Input
	/*
	std::cout << "Which Player to start? CPU [1] or You [2]: ";
	char player;
	bool input = true;
	while (input == true)
	{
		std::cin >> player;
		
		if (player == '1')
		{
			playerswitch = true;
			input = false;
		}
		else if (player == '2')
		{
			playerswitch = false;
			input = false;
			std::cout << std::endl;
			system("cls");
			minimax.printBoard(board);
		}
		else
		{
			std::cout << "Which Player to start? CPU [1] or You [2]: ";
		}
	}

	std::cout << "Skill Level of CPU? [h]: hard | [m]: medium | [e]: easy | [n]: no limit: ";
	char skill;
	input = true;
	while (input == true)
	{
		std::cin >> skill;
		if (skill == 'h')
		{
			g_maxDepth = 9;
			input = false;
		}
		else if (skill == 'm')
		{
			g_maxDepth = 3;
			input = false;
		}
		else if (skill == 'e')
		{
			g_maxDepth = 1;
			input = false;
		}
		else if (skill == 'n')
		{
			g_maxDepth = 41;
			input = false;
		}
		else
		{
			std::cout << "Skill Level of CPU? [h]: hard | [m]: medium | [e]: easy | [n]: no limit: ";
		}
	}

	std::cout << "Which Algorithm? [m]: MiniMax | [n]: NegaMax | [a]: NegaMax Alpha Beta Pruning: ";
	char nm;
	input = true;
	while (input == true)
	{
		std::cin >> nm;
		if (nm == 'm')
		{
			g_selection = 1;
			input = false;
		}
		else if (nm == 'n')
		{
			g_selection = 2;
			input = false;
		}
		else if (nm == 'a')
		{
			g_selection = 3;
			input = false;
		}
		else
		{
			std::cout << "Which Algorithm? [m]: MiniMax | [n]: NegaMax | [a]: NegaMax Alpha Beta Pruning: ";
		}
	}
	*/
}

int main(int argc, char *argv[])
{
	// create Board
	Board board;
	// init Player move
	int mymove = -1;
	// init check for finish
	int check = 0;
	// load game parameters if are some available
	loadParameters(argc, argv);

	// menu
	bool menu = true;
	// init Game -> fill Board with empty coins and ask for game mode
	initGame(board);

	//************************ SFML INIT ******************************************
	sf::RenderWindow window(sf::VideoMode(700, 600), "Minimax 4 Gewinnt");

	// Set the colors of the board
	sf::Color boardColor = sf::Color::Color(0, 102, 153);
	sf::Color redColor = sf::Color::Color(214, 0, 0);
	sf::Color blueColor = sf::Color::Color(0, 0, 214);
	sf::Color emptyColor = sf::Color::Color(224, 224, 224);
	sf::Color lineColor = sf::Color::Color(0, 51, 102);
	sf::Color highlightColor = sf::Color::Color(255, 153, 0);

	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
	{
		std::cout << "Could not load Font \n";
	}

	sf::Text quitText;
	// select the font
	quitText.setFont(font); // font is a sf::Font
	quitText.setString("Quit: press [q]");
	// set the character size
	quitText.setCharacterSize(18); // in pixels, not points!
	// set the color
	quitText.setColor(sf::Color::Black);
	// set pos
	quitText.setPosition(580.f, 5.f);

	sf::Text newgameText;
	// select the font
	newgameText.setFont(font); // font is a sf::Font
	newgameText.setString("New Game: press [n]");
	// set the character size
	newgameText.setCharacterSize(18); // in pixels, not points!
	// set the color
	newgameText.setColor(sf::Color::Black);
	// set pos
	newgameText.setPosition(10.f, 5.f);

	sf::Text winnerText = newgameText;
	winnerText.setPosition(300.f, 5.f);

	// ******* menu ********************
	// Player
	sf::Text PLselection1 = winnerText;
	PLselection1.setPosition(20.f, 100.f);
	PLselection1.setString("YOU");

	sf::CircleShape plSelectCirc1(50.f);
	plSelectCirc1.setOrigin(50.f / 2, 50.f / 2);
	plSelectCirc1.setFillColor(blueColor);
	plSelectCirc1.setPosition(100.f, 100.f);
	plSelectCirc1.setOutlineThickness(8.0f);
	plSelectCirc1.setOutlineColor(sf::Color::Transparent);

	sf::Text PLselection2 = winnerText;
	PLselection2.setPosition(20.f, 250.f);
	PLselection2.setString("CPU");

	sf::CircleShape plSelectCirc2(50.f);
	plSelectCirc2.setOrigin(50.f / 2, 50.f / 2);
	plSelectCirc2.setFillColor(redColor);
	plSelectCirc2.setPosition(100.f, 250.f);
	plSelectCirc2.setOutlineThickness(8.0f);
	plSelectCirc2.setOutlineColor(sf::Color::Black);

	// Algorithm
	sf::Text Algselection1 = winnerText;
	Algselection1.setPosition(220.f, 100.f);
	Algselection1.setString("MM");

	sf::CircleShape algSelectCirc1(50.f);
	algSelectCirc1.setOrigin(50.f / 2, 50.f / 2);
	algSelectCirc1.setFillColor(sf::Color::White);
	algSelectCirc1.setPosition(300.f, 100.f);
	algSelectCirc1.setOutlineThickness(8.0f);
	algSelectCirc1.setOutlineColor(sf::Color::Black);

	sf::Text Algselection2 = winnerText;
	Algselection2.setPosition(220.f, 250.f);
	Algselection2.setString("NM");

	sf::CircleShape algSelectCirc2(50.f);
	algSelectCirc2.setOrigin(50.f / 2, 50.f / 2);
	algSelectCirc2.setFillColor(sf::Color::White);
	algSelectCirc2.setPosition(300.f, 250.f);
	algSelectCirc2.setOutlineThickness(8.0f);
	algSelectCirc2.setOutlineColor(sf::Color::Transparent);

	sf::Text Algselection3 = winnerText;
	Algselection3.setPosition(220.f, 400.f);
	Algselection3.setString("a/b");

	sf::CircleShape algSelectCirc3(50.f);
	algSelectCirc3.setOrigin(50.f / 2, 50.f / 2);
	algSelectCirc3.setFillColor(sf::Color::White);
	algSelectCirc3.setPosition(300.f, 400.f);
	algSelectCirc3.setOutlineThickness(8.0f);
	algSelectCirc3.setOutlineColor(sf::Color::Transparent);

	// Difficulty
	sf::Text Difselection1 = winnerText;
	Difselection1.setPosition(420.f, 100.f);
	Difselection1.setString("Easy");

	sf::CircleShape difSelectCirc1(50.f);
	difSelectCirc1.setOrigin(50.f / 2, 50.f / 2);
	difSelectCirc1.setFillColor(sf::Color::Green);
	difSelectCirc1.setPosition(500.f, 100.f);
	difSelectCirc1.setOutlineThickness(8.0f);
	difSelectCirc1.setOutlineColor(sf::Color::Transparent);

	sf::Text Difselection2 = winnerText;
	Difselection2.setPosition(420.f, 250.f);
	Difselection2.setString("Med");

	sf::CircleShape difSelectCirc2(50.f);
	difSelectCirc2.setOrigin(50.f / 2, 50.f / 2);
	difSelectCirc2.setFillColor(sf::Color::Yellow);
	difSelectCirc2.setPosition(500.f, 250.f);
	difSelectCirc2.setOutlineThickness(8.0f);
	difSelectCirc2.setOutlineColor(sf::Color::Transparent);

	sf::Text Difselection3 = winnerText;
	Difselection3.setPosition(420.f, 400.f);
	Difselection3.setString("Hard");

	sf::CircleShape difSelectCirc3(50.f);
	difSelectCirc3.setOrigin(50.f / 2, 50.f / 2);
	difSelectCirc3.setFillColor(sf::Color::Red);
	difSelectCirc3.setPosition(500.f, 400.f);
	difSelectCirc3.setOutlineThickness(8.0f);
	difSelectCirc3.setOutlineColor(sf::Color::Black);

	// New Game

	sf::Text newGameselection = winnerText;
	newGameselection.setPosition(545.f, 520.f);
	newGameselection.setString("Start");

	sf::CircleShape newSelectCirc(50.f);
	newSelectCirc.setOrigin(50.f / 2, 50.f / 2);
	newSelectCirc.setFillColor(sf::Color::Black);
	newSelectCirc.setPosition(625.f, 520.f);

	// **********************************

	sf::Text playerText = winnerText;
	playerText.setPosition(200.f, 5.f);
	playerText.setString("YOU ");

	sf::CircleShape plRect(10.f);
	plRect.setOrigin(10.f / 2, 10.f / 2);
	plRect.setFillColor(blueColor);
	plRect.setPosition(250.f, 10.f);

	sf::Text cpuText = playerText;
	cpuText.setPosition(500.f, 5.f);
	cpuText.setString("CPU ");

	sf::CircleShape cpuRect = plRect;
	cpuRect.setFillColor(redColor);
	cpuRect.setPosition(550.f, 10.f);

	sf::Clock clock; // starts the clock
	sf::Time elapsed1 = clock.getElapsedTime();

	//*******************************************************************************

	while (window.isOpen())
	{
		// CPU calculation of nextMove only if it is the CPU turn and no finish yet -> minimax
		if (playerswitch == true && check != 1 && check != 2 && menu == false)
		{
			check = minimax.nextMove(board, playerswitch, mymove);
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::MouseButtonPressed)
			{
				// check on which column the player has pressed -> execute the move and switch to CPU
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					// execute player move
					if (menu == false)
					{
						if		(0 <= event.mouseButton.x	&& event.mouseButton.x < 100)	mymove = 0;
						else if (100 <= event.mouseButton.x && event.mouseButton.x < 200)	mymove = 1;
						else if (200 <= event.mouseButton.x && event.mouseButton.x < 300)	mymove = 2;
						else if (300 <= event.mouseButton.x && event.mouseButton.x < 400)	mymove = 3;
						else if (400 <= event.mouseButton.x && event.mouseButton.x < 500)	mymove = 4;
						else if (500 <= event.mouseButton.x && event.mouseButton.x < 600)	mymove = 5;
						else if (600 <= event.mouseButton.x && event.mouseButton.x <= 700)	mymove = 6;

						int check = minimax.nextMove(board, playerswitch, mymove);
					}
					// Game Menu
					else if (menu == true)
					{
						// Player
						if ((75 <= event.mouseButton.x && event.mouseButton.x <= 175) && (75 <= event.mouseButton.y && event.mouseButton.y <= 175))
						{
							playerswitch = false;
							plSelectCirc1.setOutlineColor(sf::Color::Black);
							plSelectCirc2.setOutlineColor(sf::Color::Transparent);
						}
						if ((75 <= event.mouseButton.x && event.mouseButton.x <= 175) && (225 <= event.mouseButton.y && event.mouseButton.y <= 325))
						{
							playerswitch = true;
							plSelectCirc2.setOutlineColor(sf::Color::Black);
							plSelectCirc1.setOutlineColor(sf::Color::Transparent);
						}

						// Algorithm
						if ((275 <= event.mouseButton.x && event.mouseButton.x <= 375) && (75 <= event.mouseButton.y && event.mouseButton.y <= 175))
						{
							g_selection = 1;
							algSelectCirc1.setOutlineColor(sf::Color::Black);
							algSelectCirc2.setOutlineColor(sf::Color::Transparent);
							algSelectCirc3.setOutlineColor(sf::Color::Transparent);
						}
						if ((275 <= event.mouseButton.x && event.mouseButton.x <= 375) && (225 <= event.mouseButton.y && event.mouseButton.y <= 325))
						{
							g_selection = 2;
							algSelectCirc1.setOutlineColor(sf::Color::Transparent);
							algSelectCirc2.setOutlineColor(sf::Color::Black);
							algSelectCirc3.setOutlineColor(sf::Color::Transparent);
						}
						if ((275 <= event.mouseButton.x && event.mouseButton.x <= 375) && (375 <= event.mouseButton.y && event.mouseButton.y <= 475))
						{
							g_selection = 3;
							algSelectCirc1.setOutlineColor(sf::Color::Transparent);
							algSelectCirc2.setOutlineColor(sf::Color::Transparent);
							algSelectCirc3.setOutlineColor(sf::Color::Black);
						}

						// Difficulty
						if ((475 <= event.mouseButton.x && event.mouseButton.x <= 575) && (75 <= event.mouseButton.y && event.mouseButton.y <= 175))
						{
							g_maxDepth = 1;
							difSelectCirc1.setOutlineColor(sf::Color::Black);
							difSelectCirc2.setOutlineColor(sf::Color::Transparent);
							difSelectCirc3.setOutlineColor(sf::Color::Transparent);
						}
						if ((475 <= event.mouseButton.x && event.mouseButton.x <= 575) && (225 <= event.mouseButton.y && event.mouseButton.y <= 325))
						{
							g_maxDepth = 5;
							difSelectCirc1.setOutlineColor(sf::Color::Transparent);
							difSelectCirc2.setOutlineColor(sf::Color::Black);
							difSelectCirc3.setOutlineColor(sf::Color::Transparent);
						}
						if ((475 <= event.mouseButton.x && event.mouseButton.x <= 575) && (375 <= event.mouseButton.y && event.mouseButton.y <= 475))
						{
							g_maxDepth = 9;
							difSelectCirc1.setOutlineColor(sf::Color::Transparent);
							difSelectCirc2.setOutlineColor(sf::Color::Transparent);
							difSelectCirc3.setOutlineColor(sf::Color::Black);
						}

						// Start Game
						if ((600 <= event.mouseButton.x && event.mouseButton.x <= 700) && (495 <= event.mouseButton.y && event.mouseButton.y <= 595))
						{
							menu = false;
							initGame(board);
							check = 0;
						}
					}
				}
			}
			// Game menu 
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Q)
				{
					exit(-1);
				}
				else if (event.key.code == sf::Keyboard::N)
				{
					menu = true;
					//window.setVisible(false);
					initGame(board);
					//window.setVisible(true);
					check = 0;
				}
				// in game switch between algorithms
				else if (event.key.code == sf::Keyboard::Num1)
				{
					g_selection = 1;
					algSelectCirc1.setOutlineColor(sf::Color::Black);
					algSelectCirc2.setOutlineColor(sf::Color::Transparent);
					algSelectCirc3.setOutlineColor(sf::Color::Transparent);
				}
				else if (event.key.code == sf::Keyboard::Num2)
				{
					g_selection = 2;
					algSelectCirc1.setOutlineColor(sf::Color::Transparent);
					algSelectCirc2.setOutlineColor(sf::Color::Black);
					algSelectCirc3.setOutlineColor(sf::Color::Transparent);
				}
				else if (event.key.code == sf::Keyboard::Num3)
				{
					g_selection = 3;
					algSelectCirc1.setOutlineColor(sf::Color::Transparent);
					algSelectCirc2.setOutlineColor(sf::Color::Transparent);
					algSelectCirc3.setOutlineColor(sf::Color::Black);
				}
				else if (event.key.code == sf::Keyboard::Num4)
				{
					g_selection = 2;
					g_maxDepth = 41;

					algSelectCirc1.setOutlineColor(sf::Color::Transparent);
					algSelectCirc2.setOutlineColor(sf::Color::Black);
					algSelectCirc3.setOutlineColor(sf::Color::Transparent);

					difSelectCirc1.setOutlineColor(sf::Color::Transparent);
					difSelectCirc2.setOutlineColor(sf::Color::Transparent);
					difSelectCirc3.setOutlineColor(sf::Color::Transparent);
				}
			}
		}

		window.clear(sf::Color::Color(40,130,120,0));

		if (menu == false)
		{
			//****************************** Draw Board ********************************************

			float radius = window.getSize().y / (height*2.0);
			float spaceY = 1.0 / 2.0 * radius;
			float outline = 1.0 / 15.0 * radius;
			radius = radius - spaceY / 2.0 - outline;
			float spaceX = (window.getSize().x - (width*2.0)*radius) / (width + 2.0);

			for (int y = 1; y <= height; y++)
			{
				// Get the center of the next row of circles
				float centerY = (spaceY + radius)*y + radius*(y - 1);
				for (int x = 1; x <= width; x++)
				{
					// Set the color of the tokens outline
					sf::Color outlineColor;

					// Get the center of the next column of circles
					float centerX = (spaceX + radius)*x + radius*(x - 1);
					// Set the color of the circle
					sf::Color fillColor;

					//******************** check for fill color and if finished -> blink mode ******************
					if (board._slots[y - 1][x - 1] == Empty)
						fillColor = emptyColor;
					else if (board._slots[y - 1][x - 1] == Red)
					{
						fillColor = redColor;
						if (check == 1)
						{
							elapsed1 = clock.getElapsedTime();
							if (elapsed1.asSeconds() < 0.5) { outlineColor = sf::Color::Green; }
							else if (elapsed1.asSeconds() > 0.5 && elapsed1.asSeconds() < 1.0) { outlineColor = sf::Color::Black; }
							else if (elapsed1.asSeconds() > 1.0) { clock.restart(); }
						}
					}
					else
					{
						fillColor = blueColor;
						if (check == 2)
						{
							elapsed1 = clock.getElapsedTime();
							if (elapsed1.asSeconds() < 0.5) { outlineColor = sf::Color::Green; }
							else if (elapsed1.asSeconds() > 0.5 && elapsed1.asSeconds() < 1.0) { outlineColor = sf::Color::Black; }
							else if (elapsed1.asSeconds() > 1.0) { clock.restart(); }
						}
					}

					sf::CircleShape circle(radius);
					circle.setPosition(centerX, centerY);
					circle.setOrigin(radius / 2, radius / 2);
					circle.setFillColor(fillColor);
					circle.setOutlineThickness(outline);
					circle.setOutlineColor(outlineColor);
					// Draw the circle in the buffer
					window.draw(circle);
				}
			}
		}

		//*******************************************************************************************

		// set text string for the winner
		if		(check == 1)	{ winnerText.setString("COMPUTER WINS"); }
		else if (check == 2)	{ winnerText.setString("YOU WIN!!!"); }
		else					{ winnerText.setString(" "); }

		// set text for algorithm to window title
		if		(g_selection == 1)	{ window.setTitle("MiniMax"); }
		else if (g_selection == 2)	{ window.setTitle("NegaMax"); }
		else if (g_selection == 3)	{ window.setTitle("NegaMax with Alpha Beta Pruning"); }

		// draw the rest and display
		window.draw(winnerText);
		window.draw(quitText);
		window.draw(newgameText);

		window.draw(playerText);
		window.draw(plRect);
		window.draw(cpuText);
		window.draw(cpuRect);


		// draw menu
		if (menu == true)
		{
			window.draw(plSelectCirc1);
			window.draw(PLselection1);
			window.draw(plSelectCirc2);
			window.draw(PLselection2);

			window.draw(algSelectCirc1);
			window.draw(Algselection1);
			window.draw(algSelectCirc2);
			window.draw(Algselection2);
			window.draw(algSelectCirc3);
			window.draw(Algselection3);

			window.draw(difSelectCirc1);
			window.draw(Difselection1);
			window.draw(difSelectCirc2);
			window.draw(Difselection2);
			window.draw(difSelectCirc3);
			window.draw(Difselection3);

			window.draw(newSelectCirc);
			window.draw(newGameselection);
		}

		window.display();
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

