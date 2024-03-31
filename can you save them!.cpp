#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

static const float view_height = 800;
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
void displayTextLetterByLetter(RenderWindow& window, const Font& font, const string& fullText, float delay, Sound& sound, const SoundBuffer& soundBuffer, float yOffset) {
    Text text("", font, 36);
    text.setPosition(0, yOffset);
    string displayedText = "";
    unsigned int charIndex = 0;
    Clock clock;
    sound.setBuffer(soundBuffer);

    while (charIndex < fullText.size()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (fullText[charIndex] == '\n') {
            yOffset += 40.0f; // Move to the next line
            text.setPosition(0, yOffset);
            charIndex++; // Skip the newline character
            continue;
        }

        if (clock.getElapsedTime().asSeconds() >= delay) {
            displayedText += fullText[charIndex];
            text.setString(displayedText);
            charIndex++;
            clock.restart();
            sound.play();
        }
        window.draw(text);
        window.display();
    }
    sound.stop();
}
void displayTransition(RenderWindow& window, const Font& font, const SoundBuffer& soundBuffer, Sound& sound, const vector<string>& transitionTexts, float yOffset) {
    RectangleShape transitionRect(Vector2f(1200, 800));
    transitionRect.setFillColor(Color::Black);
    float transitionAlpha = 0.0f; // Initial transparency of the transition rectangle

    Clock transitionTimer;

    while (transitionAlpha < 255) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        transitionAlpha += 3.0f; // Increment alpha gradually
        transitionRect.setFillColor(Color(0, 0, 0, static_cast<Uint8>(transitionAlpha)));

        // Draw the transition effect on top of everything
        window.draw(transitionRect);
        window.display();
    }

    // Display transition theme text letter by letter
    for (const auto& transitionText : transitionTexts) {
        displayTextLetterByLetter(window, font, transitionText, 0.1f, sound, soundBuffer, yOffset);
        yOffset += 40.0f; // Move to the next line
    }

    // Reset yOffset for the reverse transition
    yOffset = 0.0f;

    // Gradually fade back out
    while (transitionAlpha < 255) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        transitionAlpha += 1.0f; // Increase alpha gradually
        transitionRect.setFillColor(Color(0, 0, 0, static_cast<Uint8>(transitionAlpha)));

        // Draw the transition effect on top of everything
        window.draw(transitionRect);
        window.display();
    }
}

void resizedview(const sf::RenderWindow& window, sf::View& view) {

    float ratio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(view_height * ratio, view_height);
}

int main() {
    cout << "hello";
    cout << " game team" << endl;
    // The intro window
    RenderWindow window(sf::VideoMode(1200, 800), "Game Intro");
    window.setFramerateLimit(60);

    View view(FloatRect(0, 0, 800, 600)); // Set up a default view

    // Create text
    Font font;
    if (!font.loadFromFile("28 Days Later.ttf")) {
        cerr << "Failed to load font!" << endl;
        return -1;
    }
    // Set up the sound
    SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("keyboard1.mp3")) {
        cerr << "Failed to load sound!" << endl;
        return -1;
    }
    Sound sound;
    sound.setBuffer(soundBuffer);

    // Load background image
    Texture backThemeTexture;
    if (!backThemeTexture.loadFromFile("test2.png")) {
        cerr << "Failed to load haunted house image!" << endl;
        return -1;
    }
    // Create sprites and set positions and scales
    Sprite backTheme_1(backThemeTexture);
    Sprite backTheme_2(backThemeTexture);
    Sprite backTheme_buffer(backThemeTexture); // New sprite acting as a buffer
    backTheme_1.setPosition(0, 0);
    backTheme_2.setPosition(backTheme_1.getGlobalBounds().width, 0);
    backTheme_buffer.setPosition(backTheme_2.getPosition().x + backTheme_2.getGlobalBounds().width, 0); // Set buffer sprite position
    float scaleX = static_cast<float>(window.getSize().x) / backThemeTexture.getSize().x;
    float scaleY = static_cast<float>(window.getSize().y) / backThemeTexture.getSize().y;
    backTheme_1.setScale(scaleX, scaleY);
    backTheme_2.setScale(scaleX, scaleY);
    backTheme_buffer.setScale(scaleX, scaleY);
    // Display intro text letter by letter with sound
    float yOffset = 0.0f;
    string fullText1 = "It was a terrifying night when Scarlet woke up   \n";
    displayTextLetterByLetter(window, font, fullText1, 0.1f, sound, soundBuffer, yOffset);
    yOffset += 40.0f;
    string fullText2 = "his memories lost to the shadows of uncertainty   \n";
    displayTextLetterByLetter(window, font, fullText2, 0.1f, sound, soundBuffer, yOffset);
    yOffset += 40.0f;
    string fullText3 = "in a house that seems to be his doom  \n";
    displayTextLetterByLetter(window, font, fullText3, 0.1f, sound, soundBuffer, yOffset);

    // Set up the second sound
    SoundBuffer soundBuffer2;
    if (!soundBuffer2.loadFromFile("editedSound.mp3")) {
        cerr << "Failed to load sound!" << endl;
        return -1;
    }
    Sound sound2;
    sound2.setBuffer(soundBuffer2);
    // Track whether the second sound has been played
    bool sound2Played = false;

    // Set up the second sound
    SoundBuffer soundBuffer3;


    // Sector to display text and cut background motion
    Clock sound2Timer;
    Clock transitionTimer;
    bool transitionComplete = false;
    bool secondTransitionComplete = false;// Flag to track if transition is complete
    Clock outroTimer;
    bool outroStarted = false;
    // Main loop
    while (window.isOpen())
    {
        Event event;
        if (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // Move backgrounds horizontally
        float speed = 1.0f; // Adjust this value to change the speed of motion
        backTheme_1.move(-speed, 0);
        backTheme_2.move(-speed, 0);
        backTheme_buffer.move(-speed, 0);

        // If backTheme1 moves out of the window, move it to the right of backTheme2
        if (backTheme_1.getPosition().x + backTheme_1.getGlobalBounds().width < 0)
            backTheme_1.setPosition(backTheme_buffer.getPosition().x + backTheme_buffer.getGlobalBounds().width - 1, 0);

        // If backTheme2 moves out of the window, move it to the right of backTheme1
        if (backTheme_2.getPosition().x + backTheme_2.getGlobalBounds().width < 0)
            backTheme_2.setPosition(backTheme_1.getPosition().x + backTheme_1.getGlobalBounds().width - 1, 0);

        // If buffer sprite moves out of the window, move it to the right of backTheme2
        if (backTheme_buffer.getPosition().x + backTheme_buffer.getGlobalBounds().width < 0)
            backTheme_buffer.setPosition(backTheme_2.getPosition().x + backTheme_2.getGlobalBounds().width - 1, 0);

        // Play the second sound when backgrounds start to appear
        if ((backTheme_1.getPosition().x >= 0 || backTheme_2.getPosition().x >= 0) && !sound2Played) {
            sound2.play();
            sound2Played = true;
            sound2Timer.restart();
        }

        // Check if 7 seconds have passed since sound2 started playing
        if (sound2Timer.getElapsedTime().asSeconds() >= 7 && !transitionComplete) {
            // Call the transition function with the transition text
            vector<string> transitionTexts = {
                "Outside the woods stretched endlessly into the night  ",
                "the whispers of the dark calling to him  ",
                "assuring him that the threats he sensed had indeed arrived  "
            };

            displayTransition(window, font, soundBuffer, sound, transitionTexts, yOffset);
            transitionComplete = true;
            transitionTimer.restart(); // Transition is complete
        }
        // Check if 12 seconds have passed since sound2 started playing and the first transition is complete
        if (transitionComplete && !secondTransitionComplete && transitionTimer.getElapsedTime().asSeconds() >= 5) {
            // Call the transition function with the transition text for the second transition
            vector<string> secondTransitionTexts = {
                "Can you defy the odds alongside Scarlet  ",
                "guiding him to find what brought him here  ",
            };

            displayTransition(window, font, soundBuffer, sound, secondTransitionTexts, yOffset);
            secondTransitionComplete = true; // Second transition is complete
            outroTimer.restart(); // Start the outro timer
            outroStarted = true; // Set the outro flag
        }
        // Check if 5 seconds have passed since the last transition
        if (outroStarted && outroTimer.getElapsedTime().asSeconds() >= 5) {
            sound2.stop();
            if (!soundBuffer3.loadFromFile("sudden.mp3")) {
                cerr << "Failed to load sound!" << endl;
                return -1;
            }
            Sound sound3;
            sound3.setBuffer(soundBuffer3);
            sound3.play();

            // Wait until sound3 finishes playing
            while (sound3.getStatus() == Sound::Playing) {
                // Keep the program running until sound3 finishes playing
            }

            // Close the window to end the intro
          //  window.close();
        }


        // Draw backgrounds and transition rectangle
        window.clear();
        window.draw(backTheme_1);
        window.draw(backTheme_2);
        window.draw(backTheme_buffer);
        window.display();
    }


    // Load the splash texture
    Texture splashTexture;
    if (!splashTexture.loadFromFile("brosp.png")) {
        cerr << "Failed to load background texture!" << endl;
        return 1; // Error
    }

    // Create a sprite for the splash screen
    Sprite splash(splashTexture);
    splash.setScale(window.getSize().x / static_cast<float>(splashTexture.getSize().x),
        window.getSize().y / static_cast<float>(splashTexture.getSize().y));

    // Load the background texture
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("level1_background.png")) {
        cerr << "Failed to load background texture!" << endl;
        return 1; // Error
    }

    // Create a sprite for the background
    Sprite background(backgroundTexture);
    background.setScale(1.1, 1.35);

    // Set the initial position of the background
    Vector2f backgroundPosition(-300, -200);

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
    float scrollSpeed = 0.5f;

    // Load the font
    Font font2;
    if (!font2.loadFromFile("font text.ttf")) {
        cerr << "Failed to load font!" << endl;
        return 1;
    }

    // Create a text object
    Text titleText;
    titleText.setFont(font2);
    titleText.setString("CAN YOU SAVE THEM?");
    titleText.setCharacterSize(80);
    titleText.setFillColor(Color::White);
    titleText.setPosition(400.f, 550.f);

    Text press;
    press.setFont(font2);
    press.setString("Press SPACE to continue....");
    press.setCharacterSize(25);
    press.setFillColor(Color::White);
    press.setPosition(410.f, 650.f);

    bool showSplashScreen = true;

    // Game loop
    while (window.isOpen()) {
        // Handle events
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
                showSplashScreen = false;
        }
        // Move the background and player
        if (Keyboard::isKeyPressed(Keyboard::W)) {
            playerPosition.y -= scrollSpeed;  // Move player up
        }
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            if (player.getPosition().x > background.getGlobalBounds().left)
                playerPosition.x -= scrollSpeed;// Move player left
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            playerPosition.y += scrollSpeed;     // Move player down
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            if (player.getPosition().x + player.getGlobalBounds().width < background.getGlobalBounds().left + background.getGlobalBounds().width)
                playerPosition.x += scrollSpeed;     // Move player right
        }

        sf::Event::Resized;
        resizedview(window, view);

        if (view.getCenter().x + view.getSize().x / 2.f >= background.getGlobalBounds().left + background.getGlobalBounds().width) {
            view.setCenter(background.getGlobalBounds().left + background.getGlobalBounds().width - view.getSize().x / 2.f, view.getCenter().y);
        }
        // Update the view only if the player reaches the edges of the window
        if (player.getPosition().x > view.getCenter().x + 400) {
            view.move(5, 0); // Move the view to the right
        }
        else if (player.getPosition().x < view.getCenter().x - 800) {
            view.move(-5, 0); // Move the view to the left
        }


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

           //view.setCenter(player.getPosition());
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
