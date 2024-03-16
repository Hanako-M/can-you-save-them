#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Button    {
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

    // Load the splash texture
    Texture splashTexture;
    if (!splashTexture.loadFromFile("main menu.png")) {
        cerr << "Failed to load background texture!" << endl;
        return 1; // Error
    }
    /*Texture backgroundtext;
    if (!splashTexture.loadFromFile("main menu.png")) {
        cerr << "Failed to load background texture!" << endl;
        return 1; // Error
    }*/

    // Create a sprite for the splash screen
    Sprite splash(splashTexture);
   splash.setScale(window.getSize().x / static_cast<float>(splashTexture.getSize().x),
        window.getSize().y / static_cast<float>(splashTexture.getSize().y));

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

    bool showSplashScreen = true;

    // Game loop
    while (window.isOpen()) {
        // Handle events
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

        }


        // Clear the window
            window.clear();

            // Draw the background
            window.draw(splash);

            // Draw the title text
            window.draw(titleText);
            window.draw(press);

            // Display the contents of the window
            window.display();
        
    }

    return 0;
}
