#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Button {
    sf::Text text;

    Button(const std::string& buttonText, const sf::Font& font, unsigned int characterSize, sf::Vector2f position)
        : text(buttonText, font, characterSize) {
        text.setFillColor(sf::Color::White);
        text.setPosition(position);
    }

    bool contains(sf::Vector2f point) const {
        return text.getGlobalBounds().contains(point);
    }

    void setTextColor(sf::Color color) {
        text.setFillColor(color);
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(text);
    }
};

int main() {
    cout << "hello";
    // Create the game window
    RenderWindow window(VideoMode(1500, 800), "SFML Window");

    // Load the background texture
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("main menu.png")) {
        cerr << "Failed to load background texture!" << endl;
        return 1; // Error
    }

    // Create a sprite for the background
    Sprite background(backgroundTexture);
    background.setScale(window.getSize().x / static_cast<float>(backgroundTexture.getSize().x),
        window.getSize().y / static_cast<float>(backgroundTexture.getSize().y));

    // Load the font
    Font font;
    if (!font.loadFromFile("font text.ttf")) {
        cerr << "Failed to load font!" << endl;
        return 1;
    }

    // Create a text object
    Text titleText;
    titleText.setFont(font);
    titleText.setString("CAN YOU SAVE THEM?");
    titleText.setCharacterSize(80);
    titleText.setFillColor(Color::White);
    titleText.setPosition(50.f, 480.f);

    Text press;
    press.setFont(font);
    press.setString("Press SPACE to continue....");
    press.setCharacterSize(25);
    press.setFillColor(Color::White);
    press.setPosition(70.f, 600.f);

    // Create buttons
    //Button playButton("Play", font, 90, Vector2f(700, 250));
   // Button exitButton("Exit", font, 90, Vector2f(700, 350));

    // Game loop
    while (window.isOpen()) {
        // Handle events
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            /*if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2f mousePosition = Vector2f(Mouse::getPosition(window));
                    if (playButton.contains(mousePosition)) {
                        cout << "play button clicked!" << endl;
                        // Add code here to handle play button click
                    }
                    else if (exitButton.contains(mousePosition)) {
                        cout << "Exit button clicked!" << endl;
                        // Add code here to handle exit button click
                        window.close();
                    }
                }
            }*/

            /*if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
                if (playButton.contains(mousePosition)) {
                    playButton.setTextColor(sf::Color::Red);
                }
                else {
                    playButton.setTextColor(sf::Color::White);
                }
                if (exitButton.contains(mousePosition)) {
                    exitButton.setTextColor(sf::Color::Red);
                }
                else {
                    exitButton.setTextColor(sf::Color::White);
                }
            }
        */
        }


        // Clear the window
            window.clear();

            // Draw the background
            window.draw(background);

            // Draw the title text
            window.draw(titleText);
            window.draw(press);

            // Draw buttons
            //playButton.draw(window);
            //exitButton.draw(window);

            // Display the contents of the window
            window.display();
        
    }

    return 0;
}
