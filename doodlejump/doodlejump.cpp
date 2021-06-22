
#include <SFML\Graphics.hpp>
#include <random>
#include <ctime>

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 700), "Doodle Jump", sf::Style::Close); //création de la fenêtre
	window.setFramerateLimit(80); //taux d'images 
	
	sf::Texture fondTexture; //l'image du background
	fondTexture.loadFromFile("images/background.png");
	sf::Sprite background(fondTexture);

	sf::Texture doodleTexture; // l'image du joueur
	doodleTexture.loadFromFile("images/doodle.png");
	sf::Sprite player(doodleTexture);

	sf::Texture platformTexture; //l'image de la plateforme
	platformTexture.loadFromFile("images/platform.png");
	sf::Sprite platform(platformTexture);

	sf::RectangleShape fondGameOver(sf::Vector2f(500, 700)); //fenetre de game over
	fondGameOver.setFillColor(sf::Color::White);

	sf::Font font; //initialisation de l'écriture
	font.loadFromFile("font/arial.ttf");
	sf::Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(sf::Color::Red);
	sf::Text textGameOver;
	textGameOver.setFont(font);
	textGameOver.setString("Game Over!");
	textGameOver.setCharacterSize(80);
	textGameOver.setFillColor(sf::Color::Red);

	// initialisation de toutes les plateformes
	sf::Vector2u platformPosition[10];
	std::uniform_int_distribution<unsigned> x(0, 500 - platformTexture.getSize().x);
	std::uniform_int_distribution<unsigned> y(100, 700);
	std::default_random_engine e(time(0));
	for (size_t i = 0; i < 10; ++i)
	{
		platformPosition[i].x = x(e);
		platformPosition[i].y = y(e);
	}

	// initialisation de la position du joueur au début de la partie et de la vélocité du saut
	int playerX = 250;
	int playerY = 151;
	float dy = 0;
	int height = 150;
	int score = 0;

	//bond du joueur
	const int PLAYER_LEFT_BOUNDING_BOX = 20;
	const int PLAYER_RIGHT_BOUNDING_BOX = 60;
	const int PLAYER_BOTTOM_BOUNDING_BOX = 70;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//déplacement du joueur 
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			playerX -= 4;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			playerX += 4;

		// joueur en dehors de la fenetre retourne de l'autre côté
		if (playerX > 500)
			playerX = 0;
		if (playerX < -40)
			playerX = window.getSize().x - doodleTexture.getSize().x;

		// Affichage du score de la partie
		if (playerY == height && dy < (-1.62))
		{
			score += 1;
			scoreText.setString("Score: " + std::to_string(score));
		}

		// tous les méchanismes du saut du joueur
		dy += 0.2;
		playerY += dy;

		if (playerY < height)
			for (size_t i = 0; i < 10; ++i)
			{
				playerY = height;
				platformPosition[i].y -= dy;  
				if (platformPosition[i].y > 700)
				{
					platformPosition[i].y = 0;
					platformPosition[i].x = x(e);
				}

			}
		//Pour la détection du saut sur une plateforme
		for (size_t i = 0; i < 10; ++i)
		{
			if ((playerX + PLAYER_RIGHT_BOUNDING_BOX > platformPosition[i].x) && (playerX + PLAYER_LEFT_BOUNDING_BOX < platformPosition[i].x + platformTexture.getSize().x) && (playerY + PLAYER_BOTTOM_BOUNDING_BOX > platformPosition[i].y) && (playerY + PLAYER_BOTTOM_BOUNDING_BOX < platformPosition[i].y + platformTexture.getSize().y) && (dy > 0)) 
			{
				dy = -10;
			}

		}
		player.setPosition(playerX, playerY); //rafraichir la position du joueur

		window.draw(background);
		window.draw(player);

		// initialiser de nouvelles plateformes et les dessiner lors du défilement
		for (size_t i = 0; i < 10; ++i)
		{
			platform.setPosition(platformPosition[i].x, platformPosition[i].y);
			window.draw(platform);
		}

		// initialiser le game over
		if (playerY > 700)
		{
			textGameOver.setPosition(30, 200);
			scoreText.setPosition(150, 400);
			goto gameover;
		}
		window.draw(scoreText);
		window.display();
	}

	// Game Over
gameover:
	while (window.isOpen()) //fenetre de game over
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.draw(fondGameOver); //dessine le fond et le text de gameover
		window.draw(textGameOver);
		window.draw(scoreText);
		window.display();
	}
	return 0;
}