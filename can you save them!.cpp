#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

static const float view_height = 800;
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

void resizedview(const sf::RenderWindow& window, sf::View& view) {

    float ratio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(view_height * ratio, view_height);
}

int main() {
    cout << "hello";
    cout << " game team"<<endl;
    // Create the game window
    RenderWindow window(VideoMode(1500, 800), "SFML Window");
    View view = window.getDefaultView();

    // Load the splash texture
    Texture splashTexture;
    if (!splashTexture.loadFromFile("main menu.png")) {
        cerr << "Failed to load background texture!" << endl;
        return 1; // Error
    }
    
    // Create a sprite for the splash screen
    Sprite splash(splashTexture);
   splash.setScale(window.getSize().x / static_cast<float>(splashTexture.getSize().x),
        window.getSize().y / static_cast<float>(splashTexture.getSize().y));

   // Load the background texture
   Texture backgroundTexture;
   if (!backgroundTexture.loadFromFile("large background.png")) {
       cerr << "Failed to load background texture!" << endl;
       return 1; // Error
   }

   // Create a sprite for the background
   Sprite background(backgroundTexture);
   background.setScale(1.1, 1.35);

   // Set the initial position of the background
   Vector2f backgroundPosition(0, -125);

   // Load the player texture
   Texture playerTexture;
   if (!playerTexture.loadFromFile("player.png")) {
       cerr << "Failed to load player texture!" << endl;
       return 1; // Error
   }

   // Create a sprite for the player
   Sprite player(playerTexture);
   player.setScale(1.5, 1.5);

   // Set the initial position of the player
   Vector2f playerPosition(500, 480); // Initial position

   // Set the speed at which the background and player move
   float scrollSpeed = 10.0f;

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
            // Move the background and player
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                playerPosition.y -= scrollSpeed;  // Move player up
            }
            if (Keyboard::isKeyPressed(Keyboard::A)) {
                playerPosition.x -= scrollSpeed;// Move player left
            }
            if (Keyboard::isKeyPressed(Keyboard::S)) {
                playerPosition.y += scrollSpeed;     // Move player down
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                playerPosition.x += scrollSpeed;     // Move player right
            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
                showSplashScreen = false;
             sf::Event::Resized;
             resizedview(window, view);
        }
        // Get the center of the player sprite
        //sf::Vector2f playerCenter(player.getPosition().x + player.getLocalBounds().width *7,
           // player.getPosition().y + player.getLocalBounds().height /5);


        // Clear the window
            window.clear();

            if (showSplashScreen) {
                // Draw splash screen elements
                window.draw(splash);
                window.draw(titleText);
                window.draw(press);
            }
            else {
                // Draw game elements
                
                view.setCenter(player.getPosition());
                window.setView(view);
                window.draw(background);
                window.draw(player);

                background.setPosition(backgroundPosition);
                player.setPosition(playerPosition);
            }

            // Display the contents of the window
            window.display();
        
    }

    return 0;
}
