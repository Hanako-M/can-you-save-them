#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;
using namespace sf;

float const ground = 600;
float const right_wall = 1850;
float const left_wall = 0;
int health = 20;

static const float view_height = 800;
struct player
{
    Sprite sprite;
    float currentframe;
    float move_x, move_y;
    FloatRect rect;
    bool onground;

    void initial_values(Texture& playerTexture)
    {
        sprite.setTexture(playerTexture);
        sprite.setScale(1.7, 1.7);
        move_x = 0;
        move_y = 0;
        currentframe = 0;
        //  sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);



    }

    void initial_values_attack(Texture& playerTexture)
    {
        sprite.setTexture(playerTexture);
        sprite.setScale(1.7, 1.7);
        move_x = 0;
        move_y = 0;
        currentframe = 0;
        // sprite.setOrigin(sprite.getTextureRect().width / 2, sprite.getTextureRect().height / 2);

    }

    void update(float timer, int no_frames)
    {
        rect.left += move_x * timer;
        rect.top += move_y * timer;

        //to achieve gravity after jumping 
        if (!onground)
        {
            move_y += (0.005 * timer);
        }
        onground = false;

        /* if (rect.left > right_wall)
                    rect.left = right_wall;*/
        if (rect.left < left_wall)
            rect.left = left_wall;

        if (rect.top > ground)
        {
            rect.top = ground;
            move_y = 0;
            onground = true;
        }

        currentframe += 0.005 * timer;
        if (currentframe > no_frames)
        {
            currentframe -= no_frames;
        }

        if (move_x > 0)
            sprite.setTextureRect(IntRect(128 * int(currentframe), 0, 128, 128));
        if (move_x < 0)
            sprite.setTextureRect(IntRect(128 * int(currentframe) + 128, 0, -128, 128));

        sprite.setPosition(rect.left, rect.top);
        move_x = 0;

    }
};
void loadTextureAndRect(Texture& texture, IntRect& rect, const string& filename, int rectWidth, int rectHeight) {
    texture.loadFromFile(filename);
    rect = IntRect(0, 0, rectWidth, rectHeight);
}
void setupSprite(sf::Sprite& sprite, float posX, float posY, float scaleX, float scaleY, float originX, float originY) {
    sprite.setPosition(posX, posY);
    sprite.setScale(scaleX, scaleY);
    sprite.setOrigin(originX, originY);
}
bool levelTransitionCompleted = false;
bool level3Completed = false;

void Level3Transition(Sprite& background, player& player1, const Vector2f& originalPlayerPosition, const Texture& levelTexture) {
    // Change the background to level 2
    background.setTexture(levelTexture);
    background.setScale(0.9f, 1.0f);
    background.setPosition(0.f, 115.f);

    // Reset player's position
    player1.rect.left = originalPlayerPosition.x;
    player1.rect.top = originalPlayerPosition.y;

    level3Completed = true;
}



void displayTextLetterByLetter(RenderWindow& window, const Font& font, const string& fullText, float delay, Sound& sound, const SoundBuffer& soundBuffer, float yOffset) {
    Text text("", font, 45);
    text.setPosition(75, yOffset);
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
            yOffset += 60.0f; // Move to the next line
            text.setPosition(20, yOffset);
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
void resizedview(const sf::RenderWindow& window, sf::View& view) {

    float ratio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(view_height * ratio, view_height);
}
void handleLevelTransition(Sprite& background, player& player1, const Vector2f& originalPlayerPosition, const Texture& levelTexture, Sprite ob1[9], Sprite ob2[9], Sprite ob3[9], Sprite ob4[9], const Texture& ob3tex, const Texture& ob4tex, const Texture& ob1tex, const Texture& ob2tex) {
    // Change the background to level 2
    background.setTexture(levelTexture);
    background.setScale(1.2f, 1.2f);
    background.setPosition(0.f, -40.f);

    // Reset player's position
    player1.rect.left = originalPlayerPosition.x;
    player1.rect.top = originalPlayerPosition.y;

    // Update ob3 and ob4

    levelTransitionCompleted = true;
}
void displayTransition(RenderWindow& window, const Font& font, const SoundBuffer& soundBuffer, Sound& sound, const vector<string>& transitionTexts, float yOffset) {
    RectangleShape transitionRect(Vector2f(1634, 1080));
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
    //    window.clear();
        window.draw(transitionRect);
        window.display();
    }

    // Display transition theme text letter by letter
    for (const auto& transitionText : transitionTexts) {
        displayTextLetterByLetter(window, font, transitionText, 0.1f, sound, soundBuffer, yOffset);
        yOffset += 60.0f; // Move to the next line
    }

    // Reset yOffset for the reverse transition
    yOffset = 70.0f;

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
        window.clear();
        window.draw(transitionRect);
        window.display();
    }
}
void Game_Play(RenderWindow& window);

struct Button {
    sf::Text text;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color pressedColor;

    Button(const std::string& buttonText, const sf::Font& font, unsigned int characterSize, sf::Vector2f position,
        sf::Color idleCol, sf::Color hoverCol, sf::Color pressedCol)
        : text(buttonText, font, characterSize), idleColor(idleCol), hoverColor(hoverCol), pressedColor(pressedCol) {
        text.setFillColor(idleColor);
        text.setPosition(position);
    }
    bool isClicked(Vector2f point) const {
        return text.getGlobalBounds().contains(point);
    }

    /*bool contains(sf::Vector2f point) const {
        return text.getGlobalBounds().contains(point);
    }*/

    void setTextColor(sf::Color color) {
        text.setFillColor(color);
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(text);
    }

    void update(sf::Vector2f mousePosition, bool isPressed) {
        if (isClicked(mousePosition)) {
            if (isPressed) {
                text.setFillColor(pressedColor);
            }
            else {
                text.setFillColor(hoverColor);
            }
        }
        else {
            text.setFillColor(idleColor);
        }
    }
};


bool isColliding(const RectangleShape& sprite1, const Sprite& sprite2) {
    return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
}


struct LastCollisionTime {
    chrono::steady_clock::time_point time;
};

void displayLetterTransition(RenderWindow& window, Sprite& transitionSprite, bool& transitionTriggered, View cam) {
    float transitionAlpha = 0.0f; // Initial transparency of the transition sprite
    bool spacePressed = false; // Flag to indicate if space key is pressed
    Clock transitionTimer;
    transitionTriggered = 0;
    // Only execute if the transition has not been triggered yet
    if (!transitionTriggered) {
        while (transitionAlpha <= 1 && !spacePressed) {
            Event event2;
            while (window.pollEvent(event2)) {
                if (event2.type == Event::Closed)
                    window.close();
                else if (event2.type == Event::KeyPressed && event2.key.code == Keyboard::Space)
                    spacePressed = true; // Set flag to true when space is pressed
            }

            transitionAlpha++; // Increment alpha gradually

            //transitionSprite.setColor(Color(255, 255, 255, static_cast <Uint8>(transitionAlpha)));
            transitionSprite.setPosition(cam.getCenter().x - 570, cam.getCenter().y - 430.5);
            transitionSprite.setOrigin(0, 0);
            //  window.clear(); // Clear the window before drawing
            window.draw(transitionSprite); // Draw the transition sprite
            window.display(); // Display the window
            cout << "letter is ready to display";
        }
        // Set the transition triggered flag
        transitionTriggered = true;
    }
    // Wait for space key press to continue
    while (!spacePressed) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
                spacePressed = true; // Set flag to true when space is pressed
        }
    }
}
void displaySplashScreen(RenderWindow& window, const Texture& splashTexture, const Font& font, Sound sound) {
    // Create a sprite for the splash screen
    Sprite splash(splashTexture);
    splash.setScale(window.getSize().x / static_cast<float>(splashTexture.getSize().x),
        window.getSize().y / static_cast<float>(splashTexture.getSize().y));

    // Create text objects for the splash screen
    Text titleText;
    titleText.setFont(font);
    titleText.setString("CAN YOU SAVE THEM?");
    titleText.setCharacterSize(80);
    titleText.setFillColor(Color::White);
    titleText.setPosition(280.f, 760.f);

    Text press;
    press.setFont(font);
    press.setString("Press SPACE to continue....");
    press.setCharacterSize(25);
    press.setFillColor(Color::White);
    press.setPosition(290.f, 880.f);
    vector<string> guidtext = {

    " controls: \n press[x][z] to attack \n press[->] to move right \n press[<-] to move left \n press[space] to jump" };

    Text guide;
    guide.setFont(font);
    guide.setString("controls: \n\n press [x] [z] to attack \n press [->] to move right \n press [<-] to move left \n press [space] to jump \n press [esc] to pause");
    guide.setCharacterSize(24);
    guide.setFillColor(Color::White);
    guide.setPosition(1360.f, 760.f);


    // Draw the splash screen elements
    sound.play();
    window.draw(splash);
    window.draw(titleText);
    window.draw(press);
    window.draw(guide);
    window.display();
    bool spacePressed = false;
    while (!spacePressed) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
                spacePressed = true; // Set flag to true when space is pressed
        }
    }
    if (spacePressed) Game_Play(window);
}
void resizedview(const sf::RenderWindow& window, sf::View& view);

int main()
{
    // The intro window
    RenderWindow window(sf::VideoMode(1634, 1080), "Game Intro");
    window.setFramerateLimit(60);

    // Create text
    Font font;
    if (!font.loadFromFile("28 Days Later.ttf")) {
        cerr << "Failed to load font!" << endl;
        return -1;
    }
    Font font1;
    font1.loadFromFile("font text.ttf");

    // Set up the sound
    SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("keys.wav")) {
        cerr << "Failed to load sound!" << endl;
        return -1;
    }
    Sound sound;
    sound.setBuffer(soundBuffer);

    // Load background image
    Texture backThemeTexture;
    if (!backThemeTexture.loadFromFile("intro town.png")) {
        cerr << "Failed to load haunted house image!" << endl;
        return -1;
    }
    /* SoundBuffer mainSoundBuffer;
     if (!mainSoundBuffer.loadFromFile("main_Sound.wav")) {
         cerr << "Failed to load sound!" << endl;

     }
     Sound mainSound;
     mainSound.setBuffer(mainSoundBuffer);*/
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
    float yOffset = 100.0f;
    string fullText1 = "It was a terrifying night when Ori woke up   \n";
    displayTextLetterByLetter(window, font, fullText1, 0.1f, sound, soundBuffer, yOffset);
    yOffset += 60.0f;
    string fullText2 = "his memories lost to the shadows of uncertainty   \n";
    displayTextLetterByLetter(window, font, fullText2, 0.1f, sound, soundBuffer, yOffset);
    yOffset += 60.0f;
    string fullText3 = "in a house that seems to be his doom  \n";
    displayTextLetterByLetter(window, font, fullText3, 0.1f, sound, soundBuffer, yOffset);

    // Set up the second sound
    SoundBuffer soundBuffer2;
    if (!soundBuffer2.loadFromFile("newsound.wav")) {
        cerr << "Failed to load sound!" << endl;
        return -1;
    }
    Sound sound2;
    sound2.setBuffer(soundBuffer2);

    SoundBuffer soundBuffer3;
    if (!soundBuffer3.loadFromFile("out.wav")) {
        cerr << "Failed to load sound!" << endl;
        return -1;
    }
    Sound sound3;
    sound3.setBuffer(soundBuffer3);
    // Track whether the second sound has been played
    bool sound2Played = false;
    bool sound3Played = false;

    // Set up the second sound
   // SoundBuffer soundBuffer3;

    // Boolean flags to track game state
    bool introComplete = false;
    bool splashScreenShown = false;
    bool spacePressed = false;
    bool gameStarted = false;

    // Sector to display text and cut background motion
    Clock sound2Timer;
    Clock transitionTimer;
    bool transitionComplete = false;
    bool secondTransitionComplete = false;// Flag to track if transition is complete
    Clock outroTimer;
    bool outroStarted = false;



    // Load the  texture
    Texture splashTexture, level1texture;
    splashTexture.loadFromFile("brosp.png");



    //// Create a sprite for the splash screen


    Sprite background(level1texture);
    background.setScale(1.1, 1.35);




    //bool showSplashScreen = true;
    int pagenum = 1;
    bool introSkipped = false;

    // Main loop
    while (window.isOpen())
    {
        Event event;
        if (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                introSkipped = true; // Set flag to true to skip intro
                sound2.stop();
            }

        }

        if (introSkipped) {
            displaySplashScreen(window, splashTexture, font1, sound3);
            // skipIntro(window, splashTexture, font1);
        }
        else {
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
            yOffset = 100.0f;
            // Check if 7 seconds have passed since sound2 started playing
            if (sound2Timer.getElapsedTime().asSeconds() >= 7 && !transitionComplete) {
                // Call the transition function with the transition text
                vector<string> transitionTexts = {
                    "Outside the woods stretched endlessly into the night  ",
                    "the whispers of the dark calling to him  ",
                    "assuring him that the threats he sensed ",
                    "had indeed arrived   "
                };

                displayTransition(window, font, soundBuffer, sound, transitionTexts, yOffset);
                transitionComplete = true;
                transitionTimer.restart(); // Transition is complete
            }
            // Check if 12 seconds have passed since sound2 started playing and the first transition is complete
            if (transitionComplete && !secondTransitionComplete && transitionTimer.getElapsedTime().asSeconds() >= 5) {
                // Call the transition function with the transition text for the second transition
                vector<string> secondTransitionTexts = {
                    "Can you defy the odds alongside Ori  ",
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
                sound3Played = true;
                if (sound3Played) {

                    // Display the splash screen while sound3 is playing
                    displaySplashScreen(window, splashTexture, font1, sound3);
                }
            }

            // Draw backgrounds and transition rectangle
            window.clear();
            window.draw(backTheme_1);
            window.draw(backTheme_2);
            window.draw(backTheme_buffer);
        }

        window.display();
    }

    return 0;
}
void Game_Play(RenderWindow& window)
{
    //obstacles
    Texture ob1tex, ob2tex, ob3tex, ob4tex, ob5tex;
    ob1tex.loadFromFile("ob1.png");
    ob2tex.loadFromFile("blade_2.png");
    ob3tex.loadFromFile("spike2.png");
    ob4tex.loadFromFile("spike1.png");
    ob5tex.loadFromFile("blade_3.png");


    // Initialize obstacle sprites
    Sprite ob1[9], ob2[9], ob3[9], ob4[9], ob5[9];
    for (int i = 0; i < 4; i++) {
        ob2[i].setTexture(ob2tex);
        ob2[i].setScale(0.145, 0.2);
        int randomX = static_cast<float>(rand() % (5100 - static_cast<int>(ob2[i].getGlobalBounds().width))) + ((i + 1) * 600);

        ob2[i].setPosition(randomX, 750);
    }
    for (int i = 0; i < 4; i++) {
        ob1[i].setTexture(ob1tex);
        ob1[i].setScale(0.186, 0.15);

        // Generate random positions within the window boundaries
        int randomX = static_cast<float>(rand() % (5000 - static_cast<int>(ob1[i].getGlobalBounds().width))) + ((i + 1) * 600);
        ob1[i].setPosition(randomX, 750);
    }
    for (int i = 0; i < 5; i++) {
        ob3[i].setTexture(ob3tex);
        ob3[i].setScale(0.08, 0.1);
        int randomX = static_cast<float>(rand() % (5600 - static_cast<int>(ob2[i].getGlobalBounds().width))) + ((i + 1) * 650);
        ob3[i].setPosition(randomX, 765);

        ob4[i].setTexture(ob4tex);
        ob4[i].setScale(0.1, 0.1);
        randomX = static_cast<float>(rand() % (5000 - static_cast<int>(ob1[i].getGlobalBounds().width))) + ((i + 1) * 600);
        ob4[i].setPosition(randomX, 750);
    }

    for (int i = 0; i < 9; i++) {
        ob5[i].setTexture(ob5tex);
        ob5[i].setScale(0.145, 0.2);
        int randomX = static_cast<float>(rand() % (3400 - static_cast<int>(ob5[i].getGlobalBounds().width))) + ((i + 1) * 600);

        ob5[i].setPosition(randomX, 765);
    }

    RectangleShape rec(Vector2f(28.f, 210.f));
    rec.setFillColor(Color::White);

    //healthbar
    Texture healthtex, redtex;
    healthtex.loadFromFile("healthbar.png");
    redtex.loadFromFile("blood_red_bar.png");
    Sprite hpp;
    hpp.setTexture(redtex);
    hpp.setScale(Vector2f(1.8f, 1.f));
    hpp.setPosition(Vector2f(200.f, 200.f));

    Font fonthp;
    fonthp.loadFromFile("font text.ttf");
    RectangleShape hpbar(Vector2f(28.f, 210.f));
    hpbar.setSize(Vector2f(525.f, 30.f));

    hpbar.setFillColor(Color(25, 25, 25, 200));

    Text phealth;
    phealth.setFont(fonthp);
    phealth.setString("Health");
    phealth.setCharacterSize(50);
    phealth.setFillColor(Color::Red);
    phealth.setPosition(20.f, 110.f);


    // Load the font
    Font font;
    font.loadFromFile("font text.ttf");
    Font font1;
    font1.loadFromFile("28 Days Later.ttf");

    Text pause;
    pause.setFont(font1);
    pause.setString("Pause");
    pause.setCharacterSize(200);
    pause.setFillColor(Color::White);
    pause.setPosition(360.f, 140.f);

    Texture pausetex;
    pausetex.loadFromFile("pausemenu.png");
    sf::Sprite pausePicture(pausetex);
    pausePicture.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    pausePicture.setScale(1.12f, 0.768f);

    // Create resume button
    Button resumeButton("Resume", font1, 80, sf::Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 1.f, pausePicture.getPosition().y + 500),
        sf::Color::White, sf::Color::Green, sf::Color::Red);

    // Create quit button
    Button quitButton("Quit", font1, 80, sf::Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 2.f, pausePicture.getPosition().y + 900),
        sf::Color::White, sf::Color::Green, sf::Color::Red);





    View cam(FloatRect(0, 0, 1200, 1100));

    Texture playerTexture[3];
    playerTexture[0].loadFromFile("Walk-player.png");
    playerTexture[1].loadFromFile("Attack_2.png");
    playerTexture[2].loadFromFile("Dead_player.png");
    player player1;
    player1.initial_values_attack(playerTexture[2]);
    player1.initial_values_attack(playerTexture[1]);
    player1.initial_values(playerTexture[0]);
    player1.sprite.setTextureRect(IntRect(0, 0, 128, 128));
    player1.rect.left = 10;
    player1.rect.top = 500;
    Vector2f playerPosition(10, 500); // Initial position
    bool attack = 0;
    bool dead = 0;

    Texture level1texture;
    level1texture.loadFromFile("level1_background.png");



    Sprite background(level1texture);
    background.setScale(1.1, 1.35);
    Texture level3texture;
    level3texture.loadFromFile("level3_background.png");


    RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    rectangle.setPosition(6500.f, 750.f);
    rectangle.setFillColor(sf::Color::White); // Fill color

    RectangleShape reclevel3(sf::Vector2f(100.f, 100.f));
    reclevel3.setPosition(6600.f, 750.f);
    reclevel3.setFillColor(sf::Color::Red); // Fill color

    // Sprite ob1(ob1tex);
    Texture level2texture;
    level2texture.loadFromFile("level2_background.png");


    // Load the picture texture
    Texture letter1Texture;
    if (!letter1Texture.loadFromFile("FirstLetter.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite letter1Sprite(letter1Texture);
    // Load the picture texture
    Texture letter2Texture;
    if (!letter2Texture.loadFromFile("secondLetter.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite letter2Sprite(letter2Texture);
    // Load the picture texture
    Texture letter3Texture;
    if (!letter3Texture.loadFromFile("thirdLetterNew.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite letter3Sprite(letter3Texture);
    /*SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("main_Sound.wav")){
        cerr << "Failed to load sound!" << endl;

    }
    Sound sound;
    sound.setBuffer(soundBuffer);*/



    //RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    //rectangle.setPosition(6400.f, 800.f); // Position (x=300, y=200)
    //rectangle.setFillColor(sf::Color::White); // Fill color


    RectangleShape pl(Vector2f(50.f, 50.f)); // Create a rectangle shape
    pl.setFillColor(sf::Color::Red); // Set its color
    pl.setPosition(50.f, 300.f); //

    RectangleShape wol(Vector2f(50.f, 50.f)); // Create a rectangle shape
    wol.setFillColor(sf::Color::Blue); // Set its color
    wol.setPosition(50.f, 300.f); //

    Sprite wolf;
    setupSprite(wolf, 3500.f, 670.f, 2.5f, 2.5f, 128 / 2, 128 / 2);

    Texture wolfwalktexture;
    IntRect rectsourcewolfwalk(0, 0, 128, 128);
    loadTextureAndRect(wolfwalktexture, rectsourcewolfwalk, "Run+Attack.png", 128, 128);


    Texture wolfattack1texture;
    IntRect rectsourcewolfattack1;
    loadTextureAndRect(wolfattack1texture, rectsourcewolfattack1, "Attack_1.png", 128, 128);

    Texture wolfhurttexture;
    IntRect rectsourcewolfhurt;
    loadTextureAndRect(wolfhurttexture, rectsourcewolfhurt, "Hurt.png", 128, 128);


    Texture wolfdeadtexture;
    IntRect rectsourcewolfdead;
    loadTextureAndRect(wolfdeadtexture, rectsourcewolfdead, "Dead.png", 128, 128);

    Sprite centipede;
    setupSprite(centipede, 1000.f, 750.f, 2.0f, 2.0f, 72 / 2, 72 / 2);

    Texture Centipedewalktexture;
    IntRect rectsourcecentwalk;
    loadTextureAndRect(Centipedewalktexture, rectsourcecentwalk, "Centipede_walk.png", 72, 72);

    Texture Centipedeattacktexture;
    IntRect rectsourcecentattack;
    loadTextureAndRect(Centipedeattacktexture, rectsourcecentattack, "Centipede_attack2.png", 72, 72);

    Texture Centipedehurttexture;
    IntRect rectsourcecenthurt;
    loadTextureAndRect(Centipedehurttexture, rectsourcecenthurt, "Centipede_hurt.png", 72, 72);

    Texture Centipededeadtexture;
    IntRect rectsourcecentdead;
    loadTextureAndRect(Centipededeadtexture, rectsourcecentdead, "Centipede_death.png", 72, 72);

    Sprite demon;
    setupSprite(demon, 1000.f, 700.f, 2.5f, 2.5f, 65 / 2, 65 / 2);

    Texture demonwalktext;
    IntRect rectdemonwalk;
    loadTextureAndRect(demonwalktext, rectdemonwalk, "cacodemon_walk.png", 65, 65);

    Texture demonattacktext;
    IntRect rectdemonattack;
    loadTextureAndRect(demonattacktext, rectdemonattack, "cacodemon_attack.png", 65, 65);

    Sprite mud;
    setupSprite(mud, 670.f, 760.f, 2.0f, 2.0f, 63 / 2, 63 / 2);
    Texture mudtext;
    IntRect rectmud;
    loadTextureAndRect(mudtext, rectmud, "spr_enemy_mud_strip8.png", 63, 63);

    Texture mudtexthurt;
    IntRect rectmudhurt;
    loadTextureAndRect(mudtexthurt, rectmudhurt, "spr_enemy_mud_hurt_strip4.png", 63, 63);

    bool isPaused = false;
    bool keyPressed = false;

    // Load letter texture and create sprite
    Texture letterTexture;
    if (!letterTexture.loadFromFile("scrollPap2.png")) {
        cerr << "Failed to load letter texture\n";
        return;
    }
    Sprite letterSprite(letterTexture);
    letterSprite.setScale(0.05f, 0.05f);


    Clock letterClock;
    bool showLetter = false;
    bool letter1 = 0;
    bool letterDisplayed = false;
    bool fallingAnimationFinished = false; // Add a variable to track if the falling animation has finished
    float letterFallSpeed = 2.0f;
    float timer2 = 0.0f;


    float wolfspeed = -2.3f;
    float centspeed = -2.8f;
    float demonspeed = 2.1f;

    bool wolfwalking = false;
    bool wolfattacking = false;
    bool wolfhurting = false;
    bool wolfdead = false;

    bool centwalking = false;
    bool centattacking = false;
    bool centhurting = false;
    bool centdead = false;

    bool demonwalking = false;
    bool demonattacking = false;
    bool demonhurt = false;
    bool demondead = false;
    bool mudattack = false;

    Clock clock;
    Clock clockenemy;
    int posCNT = 1, bgINX = 0;
    window.clear(sf::Color(0, 0, 0, 128));

    LastCollisionTime lastCollisionOb1[9]; // Store last collision time for ob1
    LastCollisionTime lastCollisionOb2[9]; // Store last collision time for ob2

    // Define the delay duration (e.g., 2 seconds)
    const std::chrono::seconds delayDuration(1);


    bool transitionTriggered = false;

    while (window.isOpen())
    {
        // sound.play();
        // sound.setLoop(true);
        Event event;
        //cam.setCenter(Vector2f(player1.sprite.getPosition().x + 600, 500));
        window.setView(cam);
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    isPaused = !isPaused;
                    keyPressed = true;
                }
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::P) {
                keyPressed = false;
            }

        }

        window.clear(sf::Color(0, 0, 0, 128));
        sf::Vector2f cameraCenter = cam.getCenter();
        pausePicture.setPosition(cameraCenter - sf::Vector2f(pausePicture.getGlobalBounds().width / 2.f, pausePicture.getGlobalBounds().height / 2.f));
        resumeButton.text.setPosition(Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 2.45f, pausePicture.getPosition().y + 300));
        quitButton.text.setPosition(sf::Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 2.30f, pausePicture.getPosition().y + 480));

        sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

        if (player1.sprite.getGlobalBounds().intersects(reclevel3.getGlobalBounds())) {
            displayLetterTransition(window, letter3Sprite, transitionTriggered, cam);
            Level3Transition(background, player1, playerPosition, level3texture);

        }

        if (!levelTransitionCompleted) {
            for (int i = 0; i < 9; ++i) {
                if (isColliding(rec, ob1[i])) {
                    auto currentTime = std::chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - lastCollisionOb1[i].time;

                    // Check if enough time has passed since the last collision
                    if (timeSinceLastCollision >= delayDuration) {
                        health--;
                        cout << health << endl;
                        if (health <= 0)
                        {
                            player1.sprite.setTexture(playerTexture[2]);
                            player1.move_x = 0.25;
                            dead = 1;
                        }
                        lastCollisionOb1[i].time = currentTime; // Update last collision time
                    }

                }


                if (isColliding(rec, ob2[i])) {
                    auto currentTime = std::chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - lastCollisionOb2[i].time;
                    if (i == 5) {
                        continue;
                    }
                    // Check if enough time has passed since the last collision
                    if (timeSinceLastCollision >= delayDuration) {
                        health--;
                        cout << health << endl;
                        if (health <= 0)
                        {
                            player1.sprite.setTexture(playerTexture[2]);
                            player1.move_x = 0.25;
                            dead = 1;
                        }
                        lastCollisionOb2[i].time = currentTime; // Update last collision time
                    }

                }
            }
        }
        else {
            if (!level3Completed) {
                for (int i = 0; i < 9; ++i) {
                    if (isColliding(rec, ob3[i])) {
                        auto currentTime = std::chrono::steady_clock::now();
                        auto timeSinceLastCollision = currentTime - lastCollisionOb1[i].time;

                        // Check if enough time has passed since the last collision
                        if (timeSinceLastCollision >= delayDuration) {
                            health--;
                            cout << health << endl;
                            if (health <= 0)
                            {
                                player1.sprite.setTexture(playerTexture[2]);
                                player1.move_x = 0.25;
                                dead = 1;
                            }
                            lastCollisionOb1[i].time = currentTime; // Update last collision time
                        }

                    }

                    rectangle.setPosition(9000.f, 750.f);

                    if (isColliding(rec, ob4[i])) {
                        auto currentTime = std::chrono::steady_clock::now();
                        auto timeSinceLastCollision = currentTime - lastCollisionOb2[i].time;
                        if (i == 5) {
                            continue;
                        }
                        // Check if enough time has passed since the last collision
                        if (timeSinceLastCollision >= delayDuration) {

                            health--;
                            cout << health << endl;
                            if (health <= 0)
                            {
                                player1.sprite.setTexture(playerTexture[2]);
                                player1.move_x = 0.25;
                                dead = 1;
                            }
                            lastCollisionOb2[i].time = currentTime; // Update last collision time
                        }

                    }
                }
            }
            else {
                reclevel3.setPosition(9000.f, 750.f);
                //handle enemies and letters in level3
                for (int i = 0; i < 9; ++i) {
                    if (isColliding(rec, ob5[i])) {
                        auto currentTime = std::chrono::steady_clock::now();
                        auto timeSinceLastCollision = currentTime - lastCollisionOb2[i].time;
                        if (i == 5) {
                            continue;
                        }
                        // Check if enough time has passed since the last collision
                        if (timeSinceLastCollision >= delayDuration) {

                            health--;
                            cout << health << endl;
                            if (health <= 0)
                            {
                                player1.sprite.setTexture(playerTexture[2]);
                                player1.move_x = 0.25;
                                dead = 1;
                            }
                            lastCollisionOb2[i].time = currentTime; // Update last collision time
                        }

                    }
                }
            }
        }
        if (health >= 0) {
            hpp.setScale(Vector2f((1.8 * (static_cast<float>(health) / 20)), 1.f));
        }

        if (!isPaused) {
            sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

            // sound.play();
            float timer = clock.getElapsedTime().asMicroseconds();
            clock.restart();
            timer /= 1000;
            if (timer > 100)
                timer = 100;


            if (pl.getGlobalBounds().intersects(wol.getGlobalBounds())) {
                // If collision occurs with the player, make the wolf attacking
                wolfwalking = false;
                wolfattacking = true;

            }

            if ((wol.getPosition().x < pl.getPosition().x) && (wolfwalking = true)) {
                wolfwalking = true;
                wolfattacking = false;
                wolf.setScale(-2.2f, 2.2f);
                wolf.move(-wolfspeed, 0.0f);
            }
            if ((wol.getPosition().x > pl.getPosition().x) && (wolfwalking = true)) {
                wolfattacking = false;
                wolfwalking = true;
                wolf.setScale(2.2f, 2.2f);
                wolf.move(wolfspeed, 0.0f);
            }

            if (abs(wol.getPosition().x - pl.getPosition().x) <= 2) {
                wolfwalking = false;
                wolfattacking = true;
                wolf.setScale(2.2f, 2.2f);
            }

            if (pl.getGlobalBounds().intersects(centipede.getGlobalBounds())) {

                centwalking = false;
                centattacking = true;

            }
            if ((centipede.getPosition().x < pl.getPosition().x) && (centwalking = true)) {
                centwalking = true;
                centattacking = false;
                centipede.setScale(-2.0f, 2.0f);
                centipede.move(-centspeed, 0.0f);
            }
            if ((centipede.getPosition().x > pl.getPosition().x) && (centwalking = true)) {
                centattacking = false;
                centwalking = true;
                centipede.setScale(2.0f, 2.0f);
                centipede.move(centspeed, 0.0f);
            }
            if (abs(centipede.getPosition().x - pl.getPosition().x) <= 2) {
                centwalking = false;
                centattacking = true;

            }
            if ((demon.getPosition().x < pl.getPosition().x)) {
                demon.move(demonspeed, 0.0f);
                demonwalking = true;
                demonattacking = false;
                demon.setScale(-2.5f, 2.5f);

            }
            else if ((demon.getPosition().x > pl.getPosition().x)) {
                demon.move(-demonspeed, 0.0f);
                demonattacking = false;
                demonwalking = true;
                demon.setScale(2.5f, 2.5f);

            }


            if (abs(demon.getPosition().x - pl.getPosition().x) <= 2) {
                demonwalking = false;
                demonattacking = true;
                demon.setScale(2.5f, 2.5f);
            }
            timer2 += letterClock.restart().asSeconds();


            if (clockenemy.getElapsedTime().asSeconds() > 0.1f) {

                rectsourcewolfwalk.left += 128;
                if (rectsourcewolfwalk.left >= 896)
                    rectsourcewolfwalk.left = 0;

                rectsourcewolfattack1.left += 128;
                if (rectsourcewolfattack1.left >= 640)
                    rectsourcewolfattack1.left = 0;



                rectsourcewolfhurt.left += 128;
                if (rectsourcewolfhurt.left >= 256)
                    rectsourcewolfhurt.left = 0;

                rectsourcewolfdead.left += 128;
                if (rectsourcewolfdead.left >= 256)
                    rectsourcewolfdead.left = 0;



                rectsourcecentwalk.left += 72;
                if (rectsourcecentwalk.left >= 288)
                    rectsourcecentwalk.left = 0;

                rectsourcecentattack.left += 72;
                if (rectsourcecentattack.left >= 432)
                    rectsourcecentattack.left = 0;

                rectsourcecenthurt.left += 72;
                if (rectsourcecenthurt.left >= 144)
                    rectsourcecenthurt.left = 0;

                rectsourcecentdead.left += 72;
                if (rectsourcecentdead.left >= 144)
                    rectsourcecentdead.left = 0;

                rectdemonwalk.left += 65;
                if (rectdemonwalk.left >= 390)
                    rectdemonwalk.left = 0;

                rectdemonattack.left += 65;
                if (rectdemonattack.left >= 390)
                    rectdemonattack.left = 0;

                rectmud.left += 62;
                if (rectmud.left >= 495)
                    rectmud.left = 0;


                rectmudhurt.left += 62;
                if (rectmudhurt.left >= 248)
                    rectmudhurt.left = 0;
                // Set texture rectangles for both animations

                if (wolfwalking == true) {
                    wolf.setTextureRect(rectsourcewolfwalk);
                    wolf.setTexture(wolfwalktexture);
                }

                if (wolfattacking == true) {
                    wolf.setTextureRect(rectsourcewolfattack1);
                    wolf.setTexture(wolfattack1texture);

                }
                //   wolfattack2.setTextureRect(rectsourcewolfattack2);
                if (wolfhurting == true) {
                    wolf.setTexture(wolfhurttexture);
                    wolf.setTextureRect(rectsourcewolfhurt);
                }

                if (wolfdead == true) {
                    wolf.setTexture(wolfdeadtexture);
                    wolf.setTextureRect(rectsourcewolfdead);
                }
                if (centwalking == true) {
                    centipede.setTexture(Centipedewalktexture);
                    centipede.setTextureRect(rectsourcecentwalk);
                }

                if (centattacking == true) {
                    centipede.setTexture(Centipedeattacktexture);
                    centipede.setTextureRect(rectsourcecentattack);
                }
                if (centhurting == true) {
                    centipede.setTexture(Centipedehurttexture);
                    centipede.setTextureRect(rectsourcecenthurt);
                }
                if (centdead == true) {
                    centipede.setTexture(Centipededeadtexture);
                    centipede.setTextureRect(rectsourcecentdead);
                }
                if (demonwalking == true) {
                    demon.setTexture(demonwalktext);
                    demon.setTextureRect(rectdemonwalk);
                }
                if (demonattacking == true) {
                    demon.setTexture(demonattacktext);
                    demon.setTextureRect(rectdemonattack);
                }


                mud.setTexture(mudtext);
                mud.setTextureRect(rectmud);
                //   mud.setTexture(mudtexthurt);
                  // mud.setTextureRect(rectmudhurt);


                clockenemy.restart();
            }


            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                player1.sprite.setTexture(playerTexture[0]);
                player1.move_x = 0.25;

            }
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                player1.sprite.setTexture(playerTexture[0]);
                player1.move_x = -0.25;
            }

            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                if (player1.onground)
                {
                    if (attack == 0)
                    {
                        player1.sprite.setTexture(playerTexture[0]);
                        player1.move_y = -1.4;
                        player1.onground = false;
                        continue;
                    }
                    else if (attack)
                    {
                        player1.sprite.setTexture(playerTexture[1]);
                        player1.move_y = -1.4;
                        player1.onground = false;
                        continue;
                    }
                }

            }



            if (Keyboard::isKeyPressed(Keyboard::X))
            {
                player1.sprite.setTexture(playerTexture[1]);
                player1.move_x = 0.25;
                attack = 1;
            }

            if (Keyboard::isKeyPressed(Keyboard::Z))
            {
                player1.sprite.setTexture(playerTexture[1]);
                player1.move_x = -0.25;
                attack = 1;
            }
            //to make player stop when collides with an obsatcle 
            if (!levelTransitionCompleted) {
                for (int i = 0; i < 9; i++) {
                    if (isColliding(rec, ob1[i])) {

                        player1.move_x = 0;
                    }
                }
                for (int i = 0; i < 9; i++) {
                    if (isColliding(rec, ob2[i])) {
                        if (i == 5) {
                            continue;
                        }
                        player1.move_x = 0;
                    }
                }
            }
            else {
                if (!level3Completed) {
                    for (int i = 0; i < 9; i++) {
                        if (isColliding(rec, ob3[i])) {

                            player1.move_x = 0;
                        }
                    }
                    for (int i = 0; i < 9; i++) {
                        if (isColliding(rec, ob4[i])) {
                            if (i == 5) {
                                continue;
                            }
                            player1.move_x = 0;
                        }
                    }
                }
                else {

                    // handle enemies collision
                }
            }


            Event::Resized;
            resizedview(window, cam);
            // Start falling animation if timer exceeds a certain value and the animation hasn't finished yet
            if (timer2 > 4.0 && !fallingAnimationFinished) {
                // Animate falling of letterSprite
                if (letterSprite.getPosition().y < 400.0f) { // Adjust the final position as needed
                    // Move letterSprite downwards
                    letterSprite.move(0.0f, letterFallSpeed * timer);
                }
                else {
                    // Falling animation finished
                    fallingAnimationFinished = true;
                    showLetter = true; // Set showLetter to true to display the letter
                }
            }

            if (cam.getCenter().x + cam.getSize().x / 2.f >= background.getGlobalBounds().left + background.getGlobalBounds().width) {
                cam.setCenter(background.getGlobalBounds().left + background.getGlobalBounds().width - cam.getSize().x / 2.f, cam.getCenter().y);
            }
            // Update the view only if the player reaches the edges of the window
            if (player1.sprite.getPosition().x > cam.getCenter().x + 50) {
                cam.move(5, 0); // Move the view to the right
            }
            else if (player1.sprite.getPosition().x < cam.getCenter().x - 600) {
                cam.move(-5, 0); // Move the view to the left
            }
            hpp.setPosition(cam.getCenter().x - 595, cam.getCenter().y - 360.5);
            hpbar.setPosition(cam.getCenter().x - 590, cam.getCenter().y - 356.5);
            phealth.setPosition(cam.getCenter().x - 590, cam.getCenter().y - 410);

            if (attack) {
                player1.update(timer, 3);
                attack = false;
            }
            else if (attack == 0)
                player1.update(timer, 3);
            else if (dead == 1)
            {

                player1.update(timer, 2);
            }
            float dt = clock.restart().asSeconds();
            rec.setPosition(player1.sprite.getPosition().x + 94, player1.sprite.getPosition().y);

            window.clear();
            window.draw(background);
            window.draw(hpbar);
            window.draw(phealth);
            window.draw(hpp);

            //window.draw(rec);

          //  window.draw(player1.sprite);
            pl.setPosition(player1.sprite.getPosition().x + 80.f, player1.sprite.getPosition().y + 80.f);
            float width = 50.f;
            float height = 100.f;
            pl.setSize(Vector2f(width, height));
            wol.setPosition(wolf.getPosition().x - 70.f, wolf.getPosition().y - 20.f);
            wol.setSize(Vector2f(120, 140));

            //   float dt = clock.restart().asSeconds();
               //   window.clear();
                //  window.draw(background);
            window.draw(player1.sprite);
            window.draw(letterSprite);
            window.draw(rectangle);
            window.draw(demon);

            window.draw(wolf);

            window.draw(mud);


            window.draw(centipede);
            /*for (int i = 0; i < 3; i++) {
                window.draw(cacodemon[i].sprite);
            }*/
            //window.draw(hyena.sprite);

            if (player1.sprite.getGlobalBounds().intersects(rectangle.getGlobalBounds())) {

                // Display the letter transition
                displayLetterTransition(window, letter2Sprite, transitionTriggered, cam);
                // window.draw(letter1Sprite);
                handleLevelTransition(background, player1, playerPosition, level2texture, ob1, ob2, ob3, ob4, ob3tex, ob4tex, ob2tex, ob1tex);

            }
            // Display the letter only once the falling animation is finished
            if (showLetter && !letterDisplayed) {
                // Assuming you have already loaded your transition sprite
                cout << "DONE" << endl;
                // Replace "transitionSprite" with your actual sprite variable
                displayLetterTransition(window, letter1Sprite, transitionTriggered, cam);
                // window.draw(letter1Sprite);
                showLetter = false;
                letterDisplayed = true; // Update the flag to indicate that the letter has been displayed
            }

            if (!levelTransitionCompleted) {
                for (int i = 0; i < 9; i++) {
                    window.draw(ob1[i]);
                    window.draw(ob2[i]);
                }
                window.draw(rectangle);
            }

            else {
                if (!level3Completed) {
                    for (int i = 0; i < 9; i++) {
                        window.draw(ob3[i]);
                        window.draw(ob4[i]);
                    }
                    window.draw(reclevel3);
                }
                else {
                    // handle enemies draw
                    for (int i = 0; i < 9; i++) {
                        window.draw(ob5[i]);
                    }
                }
            }

        }
        else {
            sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

            resumeButton.update(mousePosition, isMousePressed);
            quitButton.update(mousePosition, isMousePressed);
            cout << "Paused" << endl;

            window.clear();
            // Draw the buttons if paused
            window.clear(sf::Color(0, 0, 0, 128));
            window.draw(pausePicture);
            window.draw(pause);
            resumeButton.draw(window);
            quitButton.draw(window);

        }
        if (isMousePressed && resumeButton.isClicked(mousePosition)) {
            isPaused = false; // Resume the game

        }

        if (isMousePressed && quitButton.isClicked(mousePosition)) {
            window.close(); // Close the window
            cout << "Quit button clicked!" << endl;
        }

        window.display();


    }
}








