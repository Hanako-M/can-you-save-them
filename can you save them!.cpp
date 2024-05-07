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
int health = 40;
int healthwolf = 30;
int healthwolf2 = 25;
int healthcent = 20;
int healthcent2 = 20;
int healthdemon = 25;
int healthmud = 15;
bool mudhurt = false;
bool mudd;


bool specialswitch = false;
bool win = 0;

float wolf2speed = -2.5f;
float wolfspeed = -2.5f;
float centspeed = -2.8f;
float centspeed2 = -2.8f;
float demonspeed = 2.1f;
bool continueanimation = true;
bool wolfwalking = false;
bool wolfattacking1 = false;
bool wolfattacking2 = false;
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

bool wolfdamage = false;


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




    }

    void initial_values_attack(Texture& playerTexture)
    {
        sprite.setTexture(playerTexture);
        sprite.setScale(1.7, 1.7);
        move_x = 0;
        move_y = 0;
        currentframe = 0;


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

        if ((move_x > 0) || (no_frames == 2))
            sprite.setTextureRect(IntRect(128 * int(currentframe), 0, 128, 128));
        if (move_x < 0)
            sprite.setTextureRect(IntRect(128 * int(currentframe) + 128, 0, -128, 128));

        sprite.setPosition(rect.left, rect.top);
        move_x = 0;

    }
};
struct enemy {
    Sprite spritee;
    Texture textures[5];
    IntRect rectSource[5];
};
struct Enemy {
    Sprite sprite;
    int damage;
    float currentframe;
    float move_x;
    FloatRect rectenemy;
    int ip_x;
    int ip_y;
    int chase_range;
    bool attack = false;
    bool dead = false;
    bool stunned = false;
    int direction = 1;

    void init(Texture& enemy_texture) {
        sprite.setTexture(enemy_texture);
        move_x = 0;
        currentframe = 0;
    }

    void update_enemy(float timer, bool attack, Texture& attack_texture, bool dead, Texture& death_texture) {
        if (attack) {
            sprite.setTexture(attack_texture);
        }
        if (dead) {
            sprite.setTexture(death_texture);
        }
        rectenemy.left += move_x * timer;
        currentframe += 0.004 * timer;
        if (!dead) {
            if (currentframe > 4)
                currentframe -= 4;

        }
        if (move_x > 0) {
            sprite.setTextureRect(IntRect(48 * int(currentframe) + 48, 0, -48, 48));
        }
        if (move_x < 0) {
            sprite.setTextureRect(IntRect(48 * int(currentframe), 0, 48, 48));
        }
        sprite.setPosition(rectenemy.left + ip_x, ip_y);
        move_x = 0;
    }

    void update_enemy3(float timer, bool attack, Texture& enemy_texture, Texture attack_textures[], bool dead, Texture& death_texture, bool stunned, Texture stunned_texture) {
        if (attack) {
            sprite.setTexture(attack_textures[2]);
            if (currentframe > 4)
                currentframe -= 4;
        }
        if (dead) {
            sprite.setTexture(death_texture);
            move_x = 0.001;
            currentframe += 0.0001;
            if (currentframe > 5) {
                move_x = 0;
                rectenemy.left = 90000;
            }
        }
        if (stunned) {
            sprite.setTexture(stunned_texture);
            move_x = 0.001;
            //currentframe += 0.0000001;
            if (currentframe > 3) {
                stunned = false;
                sprite.setTexture(enemy_texture);
            }
        }
        rectenemy.left += move_x * timer;
        currentframe += 0.008 * timer;
        if (!dead) {
            if (currentframe > 6)
                currentframe -= 6;
        }
        if (move_x > 0) {
            sprite.setTextureRect(IntRect(128 * int(currentframe) + 128, 0, 128, 128));
        }
        if (move_x < 0) {
            sprite.setTextureRect(IntRect(128 * int(currentframe), 0, -128, 128));
        }
        sprite.setPosition(rectenemy.left + ip_x, ip_y);
        move_x = 0;
    }
};


void loadTextureAndRect(Texture& texture, IntRect& rect, const string& filename, int rectWidth, int rectHeight) {
    texture.loadFromFile(filename);
    rect = IntRect(0, 0, rectWidth, rectHeight);
}
void setupSprite(enemy& enemy, float posX, float posY, float scaleX, float scaleY, float originX, float originY) {
    enemy.spritee.setPosition(posX, posY);
    enemy.spritee.setScale(scaleX, scaleY);
    enemy.spritee.setOrigin(originX, originY);
}
bool levelTransitionCompleted = false;
bool level3Completed = false;

void Level3Transition(Sprite& background, player& player1, const Vector2f& originalPlayerPosition, const Texture& levelTexture) {
    // Change the background to level 2
    background.setTexture(levelTexture);
    background.setScale(1.1, 1.35);
    background.setPosition(0.f, 0);

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


void resizedview(const RenderWindow& window, View& view) {

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
void switchCharacter(player& player, const Texture& newTexture) {
    player.sprite.setTexture(newTexture);
    player.sprite.setScale(3.0f, 3.0f);
    //  player.sprite.setOrigin(10 / 2 , 0);


      // Additional setup or logic for switching characters if needed
}

void Game_Play(RenderWindow& window);
struct Button {
    Text text;
    Color idleColor;
    Color hoverColor;
    Color pressedColor;

    Button(const std::string& buttonText, const Font& font, unsigned int characterSize, Vector2f position,
        Color idleCol, Color hoverCol, Color pressedCol)
        : text(buttonText, font, characterSize), idleColor(idleCol), hoverColor(hoverCol), pressedColor(pressedCol) {
        text.setFillColor(idleColor);
        text.setPosition(position);
    }
    bool isClicked(Vector2f point) const {
        return text.getGlobalBounds().contains(point);
    }

    

    void setTextColor(Color color) {
        text.setFillColor(color);
    }

    void draw(RenderWindow& window) const {
        window.draw(text);
    }

    void update(Vector2f mousePosition, bool isPressed) {
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
    std::chrono::steady_clock::time_point time;
};

void displayLetterTransition(RenderWindow& window, Sprite& transitionSprite, bool& transitionTriggered, View cam, Sound sound) {
    float transitionAlpha = 0.0f; // Initial transparency of the transition sprite
    bool spacePressed = false; // Flag to indicate if space key is pressed
    Clock transitionTimer;
    sound.play();
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
void resizedview(const RenderWindow& window, View& view);

LastCollisionTime demontime;
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
    if (!backThemeTexture.loadFromFile("mainCave.png")) {
        cerr << "Failed to load haunted house image!" << endl;
        return -1;
    }
 
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
    Clock hclock;
    //obstacles
    Texture ob1tex, ob2tex, ob3tex, ob4tex, ob5tex;
    ob1tex.loadFromFile("ob1.png");
    ob2tex.loadFromFile("blade_3.png");
    ob3tex.loadFromFile("spike2.png");
    ob4tex.loadFromFile("spike1.png");
    ob5tex.loadFromFile("blade_2.png");

    // Initialize obstacle sprites
    Sprite ob1[9], ob2[9], ob3[9], ob4[9], ob5[9];
    for (int i = 0; i < 4; i++) {
        ob2[i].setTexture(ob2tex);
        ob2[i].setScale(0.145, 0.2);
        int randomX = static_cast<float>(rand() % (5100 - static_cast<int>(ob2[i].getGlobalBounds().width))) + ((i + 1) * 600);

        ob2[i].setPosition(randomX, 765);
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
        int randomX = (i * 900);

        ob5[i].setPosition(randomX, 765);
    }

    RectangleShape rec(Vector2f(28.f, 210.f));
    rec.setFillColor(Color::White);

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
    //pause menu

     // Load the font
    Font font1;
    font1.loadFromFile("28 Days Later.ttf");

    Font font2;
    font2.loadFromFile("28 Days Later.ttf");

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

    // Create pause event  
    Button quitButtonpuase("Quit", font1, 80, sf::Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 2.f, pausePicture.getPosition().y + 900),
        sf::Color::White, sf::Color::Green, sf::Color::Red);



    Text gameover;
    gameover.setFont(font2);
    gameover.setString("Gameover");
    gameover.setCharacterSize(150);
    gameover.setFillColor(Color::White);
    gameover.setPosition(360.f, 140.f);

    Texture gameovtex;
    gameovtex.loadFromFile("pausemenu.png");
    sf::Sprite gameovPicture(gameovtex);
    gameovPicture.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    gameovPicture.setScale(1.12f, 0.768f);


    // Create restart button
    Button restartButton("restart", font2, 80, sf::Vector2f(gameovPicture.getPosition().x + gameovPicture.getGlobalBounds().width / 1.f, gameovPicture.getPosition().y + 500),
        sf::Color::White, sf::Color::Green, sf::Color::Red);



    // for gameover event 
    Button quitButtongameover("Quit", font2, 80, sf::Vector2f(gameovPicture.getPosition().x + gameovPicture.getGlobalBounds().width / 2.f, gameovPicture.getPosition().y + 900),
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

    Texture Enemyphoto;
    Enemyphoto.loadFromFile("Snake_walk.png");
    Texture Enemyattack;
    Enemyattack.loadFromFile("Snake_attack.png");
    Texture Enemydeath;
    Enemydeath.loadFromFile("Snake_death.png");
    Enemy enemy1;
    enemy1.init(Enemyphoto);
    enemy1.sprite.setScale(3.5, 3);
    enemy1.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    enemy1.ip_x = 1300;
    enemy1.ip_y = 690;
    enemy1.sprite.setPosition(enemy1.ip_x, enemy1.ip_y);
    enemy1.chase_range = 200;

    Enemy enemy2;
    enemy2.init(Enemyphoto);
    enemy2.sprite.setScale(3.5, 3);
    enemy2.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    enemy2.ip_x = 2100;
    enemy2.ip_y = 690;
    enemy2.sprite.setPosition(enemy2.ip_x, enemy2.ip_y);
    enemy2.chase_range = 200;


    Enemy enemy4;
    enemy4.init(Enemyphoto);
    enemy4.sprite.setScale(3.5, 3);
    enemy4.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    enemy4.ip_x = 3200;
    enemy4.ip_y = 690;
    enemy4.sprite.setPosition(enemy4.ip_x, enemy4.ip_y);
    enemy4.chase_range = 200;

    Enemy enemy5;
    enemy5.init(Enemyphoto);
    enemy5.sprite.setScale(3.5, 3);
    enemy5.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    enemy5.ip_x = 4000;
    enemy5.ip_y = 690;
    enemy5.sprite.setPosition(enemy5.ip_x, enemy5.ip_y);
    enemy5.chase_range = 200;

    Enemy enemy6;
    enemy6.init(Enemyphoto);
    enemy6.sprite.setScale(3.5, 3);
    enemy6.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    enemy6.ip_x = 1300;
    enemy6.ip_y = 690;
    enemy6.sprite.setPosition(enemy6.ip_x, enemy6.ip_y);
    enemy6.chase_range = 200;

    Enemy enemy7;
    enemy7.init(Enemyphoto);
    enemy7.sprite.setScale(3.5, 3);
    enemy7.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    enemy7.ip_x = 2100;
    enemy7.ip_y = 690;
    enemy7.sprite.setPosition(enemy7.ip_x, enemy7.ip_y);
    enemy7.chase_range = 200;

    Enemy enemy8;
    enemy8.init(Enemyphoto);
    enemy8.sprite.setScale(3.5, 3);
    enemy8.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    enemy8.ip_x = 3200;
    enemy8.ip_y = 690;
    enemy8.sprite.setPosition(enemy8.ip_x, enemy8.ip_y);
    enemy8.chase_range = 200;

    Enemy enemy9;
    enemy9.init(Enemyphoto);
    enemy9.sprite.setScale(3.5, 3);
    enemy9.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    enemy9.ip_x = 4000;
    enemy9.ip_y = 690;
    enemy9.sprite.setPosition(enemy9.ip_x, enemy9.ip_y);
    enemy9.chase_range = 200;

    Enemy enemy10;
    enemy10.init(Enemyphoto);
    enemy10.sprite.setScale(3.5, 3);
    enemy10.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    enemy10.ip_x = 1300;
    enemy10.ip_y = 690;
    enemy10.sprite.setPosition(enemy10.ip_x, enemy10.ip_y);
    enemy10.chase_range = 200;

    Enemy enemy11;
    enemy11.init(Enemyphoto);
    enemy11.sprite.setScale(3.5, 3);
    enemy11.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    enemy11.ip_x = 2100;
    enemy11.ip_y = 690;
    enemy11.sprite.setPosition(enemy11.ip_x, enemy11.ip_y);
    enemy11.chase_range = 200;


    Texture Enemy3photo;
    Enemy3photo.loadFromFile("gWalk (1).png");
    Texture Enemy3attack1;
    Enemy3attack1.loadFromFile("gAttack_1.png");
    Texture Enemy3attack2;
    Enemy3attack2.loadFromFile("gAttack_2.png");
    Texture Enemy3attack3;
    Enemy3attack3.loadFromFile("gAttack_3.png");
    Texture attack_textures[3] = { Enemy3attack1 ,Enemy3attack2, Enemy3attack3 };
    Texture Enemy3death;
    Enemy3death.loadFromFile("gDead.png");
    Texture enemy3stunned;
    enemy3stunned.loadFromFile("gStunned.png");
    Enemy enemy3;
    enemy3.init(Enemy3photo);
    enemy3.sprite.setScale(1.8, 1.8);
    enemy3.sprite.setTextureRect(IntRect(0, 0, 128, 128));
    enemy3.ip_x = 850;
    enemy3.ip_y = 600;
    enemy3.sprite.setPosition(enemy3.ip_x, enemy3.ip_y);
    enemy3.chase_range = 200;

    RectangleShape pl2(Vector2f(50.f, 50.f));
    pl2.setFillColor(sf::Color::Red);
    pl2.setPosition(player1.sprite.getPosition().x + 80.f, player1.sprite.getPosition().y + 80.f);
    float width = 50.f;
    float height = 180.f;
    pl2.setSize(Vector2f(width, height));

    RectangleShape snake(Vector2f(50.f, 50.f));
    snake.setFillColor(sf::Color::Blue);
    snake.setPosition(enemy1.sprite.getPosition().x + 20.f, enemy1.sprite.getPosition().y + 70.f);
    snake.setSize(Vector2f(78, 68));

    RectangleShape snake2(Vector2f(50.f, 50.f));
    snake2.setFillColor(sf::Color::Green);
    snake2.setPosition(enemy2.sprite.getPosition().x + 20.f, enemy2.sprite.getPosition().y + 70.f);
    snake2.setSize(Vector2f(78, 68));

    RectangleShape snake4(Vector2f(50.f, 50.f));
    snake4.setFillColor(sf::Color::Green);
    snake4.setPosition(enemy4.sprite.getPosition().x + 20.f, enemy4.sprite.getPosition().y + 70.f);
    snake4.setSize(Vector2f(78, 68));

    RectangleShape snake5(Vector2f(50.f, 50.f));
    snake5.setFillColor(sf::Color::Green);
    snake5.setPosition(enemy5.sprite.getPosition().x + 20.f, enemy5.sprite.getPosition().y + 70.f);
    snake5.setSize(Vector2f(78, 68));

    RectangleShape snake6(Vector2f(50.f, 50.f));
    snake6.setFillColor(sf::Color::Green);
    snake6.setPosition(enemy6.sprite.getPosition().x + 20.f, enemy6.sprite.getPosition().y + 70.f);
    snake6.setSize(Vector2f(78, 68));

    RectangleShape snake7(Vector2f(50.f, 50.f));
    snake7.setFillColor(sf::Color::Green);
    snake7.setPosition(enemy7.sprite.getPosition().x + 20.f, enemy7.sprite.getPosition().y + 70.f);
    snake7.setSize(Vector2f(78, 68));

    RectangleShape snake8(Vector2f(50.f, 50.f));
    snake8.setFillColor(sf::Color::Green);
    snake8.setPosition(enemy8.sprite.getPosition().x + 20.f, enemy8.sprite.getPosition().y + 70.f);
    snake8.setSize(Vector2f(78, 68));

    RectangleShape snake9(Vector2f(50.f, 50.f));
    snake9.setFillColor(sf::Color::Green);
    snake9.setPosition(enemy9.sprite.getPosition().x + 20.f, enemy9.sprite.getPosition().y + 70.f);
    snake9.setSize(Vector2f(78, 68));

    RectangleShape snake10(Vector2f(50.f, 50.f));
    snake10.setFillColor(sf::Color::Green);
    snake10.setPosition(enemy10.sprite.getPosition().x + 20.f, enemy10.sprite.getPosition().y + 70.f);
    snake10.setSize(Vector2f(78, 68));

    RectangleShape snake11(Vector2f(50.f, 50.f));
    snake11.setFillColor(sf::Color::Green);
    snake11.setPosition(enemy11.sprite.getPosition().x + 20.f, enemy11.sprite.getPosition().y + 70.f);
    snake11.setSize(Vector2f(78, 68));


    RectangleShape b(Vector2f(50.f, 50.f));
    b.setFillColor(sf::Color::Magenta);
    b.setPosition(enemy3.sprite.getPosition().x + 90.f, enemy3.sprite.getPosition().y + 100.f);
    b.setSize(Vector2f(50, 120));

    RectangleShape t(Vector2f(50.f, 50.f));
    t.setFillColor(sf::Color::Yellow);
    t.setPosition(enemy3.ip_x, enemy3.ip_y + 150);
    t.setSize(Vector2f(40, 40));


    Texture level1texture;
    level1texture.loadFromFile("back1new.png");

    Sprite background(level1texture);
    background.setPosition(-10, 1);
    background.setScale(0.85, 1.1);
    Texture level3texture;
    level3texture.loadFromFile("level1_backgroundnew.png");

    RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    rectangle.setPosition(6600.f, 750.f);
    rectangle.setFillColor(sf::Color::White); // Fill color

    RectangleShape reclevel3(sf::Vector2f(100.f, 100.f));
    reclevel3.setPosition(6900.f, 750.f);
    reclevel3.setFillColor(sf::Color::Red); // Fill color

    // Sprite ob1(ob1tex);
    Texture level2texture;
    level2texture.loadFromFile("level2_background.png");
    Texture splashTexture;
    splashTexture.loadFromFile("brosp.png");
    Sprite splashScreen(splashTexture);
    SoundBuffer soundBuffer3;
    if (!soundBuffer3.loadFromFile("out.wav")) {
        cerr << "Failed to load sound!" << endl;
        return;
    }
    Sound sound3;
    sound3.setBuffer(soundBuffer3);



    // Load the picture texture
    Texture letter1Texture;
    if (!letter1Texture.loadFromFile("FirstLetter.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite letter1Sprite(letter1Texture);
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

    Texture overTexture;
    if (!overTexture.loadFromFile("gameisOver.png")) {
        cerr << "Failed to load over\n";
        return;
    }
    Sprite gameOver(overTexture);

    Texture winTexture;
    if (!winTexture.loadFromFile("winLetter.png")) {
        cerr << "Failed to load over\n";
        return;
    }
    Sprite winLetter(winTexture);
    /*Texture tryTexture;
    if (tryTexture.loadFromFile("try.png")) {
        cerr << "Failed to load try nela\n";
        return;
    }
    Sprite tryLetter(tryTexture);*/
    Texture necklaceTexture;
    if (!necklaceTexture.loadFromFile("necklace.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite necklaceSprite(necklaceTexture);
    necklaceSprite.setPosition(400, 400);
    necklaceSprite.setScale(2, 2);

    Texture ballonTexture;
    if (!ballonTexture.loadFromFile("ballon.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite ballonSprite(ballonTexture);
    ballonSprite.setPosition(700, 400);
    ballonSprite.setScale(2, 2);

    Texture keyTexture;
    if (!keyTexture.loadFromFile("key.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite keySprite(keyTexture);
    keySprite.setPosition(1000, 400);
    keySprite.setScale(2, 2);

    Texture arrowTexture;
    if (!arrowTexture.loadFromFile("arrow.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite arrowSprite(arrowTexture);
    arrowSprite.setPosition(1300, 400);
    arrowSprite.setScale(2, 2);

    Texture torchTexture;
    if (!torchTexture.loadFromFile("torch.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite torchSprite(torchTexture);
    torchSprite.setPosition(1500, 400);
    torchSprite.setScale(2, 2);

    Texture tool2Texture;
    if (!tool2Texture.loadFromFile("tool2.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite tool2Sprite(tool2Texture);
    tool2Sprite.setPosition(1800, 400);
    tool2Sprite.setScale(2, 2);

    Texture diamondTexture;
    if (!diamondTexture.loadFromFile("diamond.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite diamondSprite(diamondTexture);
    diamondSprite.setPosition(2100, 400);
    diamondSprite.setScale(2, 2);

    Texture fireTexture;
    if (!fireTexture.loadFromFile("fire.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite fireSprite(fireTexture);
    fireSprite.setPosition(2400, 400);
    fireSprite.setScale(2, 2);

    Texture tool1Texture;
    if (!tool1Texture.loadFromFile("tool1.png")) {
        cerr << "Failed to load picture texture\n";
        return;
    }
    Sprite tool1Sprite(tool1Texture);
    tool1Sprite.setPosition(2700, 400);
    tool1Sprite.setScale(2, 2);

    SoundBuffer attacksoundBuffer;
    if (!attacksoundBuffer.loadFromFile("attacks.wav")) {
        cerr << "Failed to load sound!" << endl;

    }
    Sound attackSound;
    attackSound.setBuffer(attacksoundBuffer);

    SoundBuffer papersoundBuffer;
    if (!papersoundBuffer.loadFromFile("paper.wav")) {
        cerr << "Failed to load sound!" << endl;

    }
    Sound paperSound;
    paperSound.setBuffer(papersoundBuffer);

    SoundBuffer enemysoundBuffer;
    if (!enemysoundBuffer.loadFromFile("enemyDamage.wav")) {
        cerr << "Failed to load sound!" << endl;

    }
    Sound enemySound;
    enemySound.setBuffer(enemysoundBuffer);


    SoundBuffer PausesoundBuffer;
    if (!PausesoundBuffer.loadFromFile("pauseSound1.wav")) {
        cerr << "Failed to load sound!" << endl;

    }
    Sound pauseSound;
    pauseSound.setBuffer(PausesoundBuffer);

    SoundBuffer againBuffer;
    if (!againBuffer.loadFromFile("gametry.wav")) {
        cerr << "Failed to load sound!" << endl;

    }
    Sound againSound;
    againSound.setBuffer(againBuffer);

    SoundBuffer deathBuffer;
    if (!deathBuffer.loadFromFile("attackSound1.wav")) {
        cerr << "Failed to load sound!" << endl;

    }
    Sound deathSound;
    deathSound.setBuffer(deathBuffer);

    SoundBuffer bossBattleBuffer;
    if (!bossBattleBuffer.loadFromFile("bossBa.wav")) {
        cerr << "Failed to load sound!" << endl;

    }
    Sound bossBattleSound;
    bossBattleSound.setBuffer(bossBattleBuffer);



    SoundBuffer soundBuffer;
    if (!soundBuffer.loadFromFile("gameLoop.wav")) {
        cerr << "Failed to load sound!" << endl;

    }
    Sound sound;
    sound.setBuffer(soundBuffer);

    sound.play();
    sound.setLoop(true);

    Clock death;
    enemy wolf;
    enemy wolf2;
    enemy centipede;
    enemy centipede2;
    enemy demon;
    enemy mud;
    enemy waechter;


    RectangleShape pl(Vector2f(50.f, 50.f));
    pl.setFillColor(Color::Red); // Set its color
    pl.setPosition(50.f, 300.f); //

    RectangleShape wol(Vector2f(50.f, 50.f)); 
    wol.setFillColor(Color::Blue); // Set its color
    wol.setPosition(50.f, 300.f); //

    RectangleShape dem(Vector2f(50.f, 50.f)); 
    dem.setFillColor(Color::Blue); // Set its color
    dem.setPosition(50.f, 300.f); //
    RectangleShape cen(Vector2f(50.f, 50.f));
    cen.setFillColor(Color::Blue); 
    cen.setPosition(50.f, 300.f); //

    setupSprite(waechter, 500.f, 750.f, 2.f, 2.f, 96 / 2, 96 / 2);
    Texture waechterwalktext;
    IntRect rectwaechterwalk;
    loadTextureAndRect(waechterwalktext, rectwaechterwalk, "waechter_walk.png", 96, 96);
    Texture specialwalktext;
    IntRect rectspecialwalk;
    loadTextureAndRect(specialwalktext, rectspecialwalk, "special_walk.png", 65, 100);
    Sprite sister;
    Texture sistertext;
    sistertext.loadFromFile("Idle_KG_1.png");
    sister.setPosition(7600, 740);
    sister.setScale(-2.f, 2.f);

    const int frameWidth = 55;
    const int frameHeight = 67; 
    const int numFrames = 4;
    const int sheetWidth = frameWidth * numFrames;
    IntRect textureRect(0, 0, frameWidth, frameHeight);

    Texture specialattacktext;
    IntRect rectspecialattack;
    loadTextureAndRect(specialattacktext, rectspecialattack, "special_attack.png", 128, 100);


    /* Sprite wolfff;
     Sprite wolfff2;*/
    setupSprite(wolf, 7500.f, 640.f, 3.f, 3.f, 128 / 2, 128 / 2);
    setupSprite(wolf2, 4500.f, 670.f, 2.5f, 2.5f, 128 / 2, 128 / 2);

    Texture wolfwalktexture;
    IntRect rectsourcewolfwalk;
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

    /*  Sprite centipede;*/
    setupSprite(centipede, 2500.f, 750.f, 2.0f, 2.0f, 72 / 2, 72 / 2);
    setupSprite(centipede2, 1500.f, 750.f, 2.0f, 2.0f, 72 / 2, 72 / 2);

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

    /*Sprite demon;*/
    setupSprite(demon, 1000.f, 700.f, 2.5f, 2.5f, 65 / 2, 65 / 2);

    Texture demonwalktext;
    IntRect rectdemonwalk;
    loadTextureAndRect(demonwalktext, rectdemonwalk, "cacodemon_walk.png", 65, 65);

    Texture demonattacktext;
    IntRect rectdemonattack;
    loadTextureAndRect(demonattacktext, rectdemonattack, "cacodemon_attack.png", 65, 65);

    Texture demonhurttext;
    IntRect rectdemonhurt;
    loadTextureAndRect(demonhurttext, rectdemonhurt, "cacodemon-hurt.png", 65, 65);
    Texture demondeadtext;
    IntRect rectdemondead;
    loadTextureAndRect(demondeadtext, rectdemondead, "cacodemon_dead.png", 65, 65);


    /*Sprite mud;*/
    setupSprite(mud, 800.f, 760.f, 2.0f, 2.0f, 63 / 2, 63 / 2);
    Texture mudtext;
    IntRect rectmud;
    loadTextureAndRect(mudtext, rectmud, "spr_enemy_mud_strip8.png", 63, 63);

    Texture mudtexthurt;
    IntRect rectmudhurt;
    loadTextureAndRect(mudtexthurt, rectmudhurt, "spr_enemy_mud_hurt_strip4.png", 63, 63);

    bool isPaused = false;
    bool keyPressed = false;
    bool gameisov = false;

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

    int currentFrame = 0;
    Clock clock;
    Clock clockenemy;
    int posCNT = 1, bgINX = 0;
    window.clear(Color(0, 0, 0, 128));

    LastCollisionTime lastCollisionOb1[9]; // Store last collision time for ob1
    LastCollisionTime lastCollisionOb2[9]; // Store last collision time for ob2
    LastCollisionTime enemy1t;
    LastCollisionTime enemy2t;
    LastCollisionTime enemy4t;
    LastCollisionTime enemy5t;
    LastCollisionTime enemy6t;
    LastCollisionTime enemy7t;
    LastCollisionTime enemy8t;
    LastCollisionTime enemy9t;
    LastCollisionTime enemy10t;
    LastCollisionTime enemy11t;


    // Define the delay duration (e.g., 2 seconds)
    const std::chrono::seconds delayDuration(2);


    bool transitionTriggered = false;
    bool bossS = 0;
    map < string, bool>enemyHealth;

    while (window.isOpen())
    {
        float htimer = hclock.getElapsedTime().asMicroseconds();
        hclock.restart();
        htimer /= 1000;
        if (htimer > 100)
            htimer = 100;
        // wolfattacking = false;
         // sound.play();
        Event event;
        //cam.setCenter(Vector2f(player1.sprite.getPosition().x + 600, 500));
        window.setView(cam);
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    isPaused = !isPaused;
                    keyPressed = true;
                    sound.stop();
                    pauseSound.play();
                    pauseSound.setLoop(true);
                }
            }
            if (event.type == Event::KeyReleased && event.key.code == Keyboard::P) {
                keyPressed = false;
            }

        }

        window.clear(Color(0, 0, 0, 128));
      Vector2f cameraCenter = cam.getCenter();

        pausePicture.setPosition(cameraCenter - Vector2f(pausePicture.getGlobalBounds().width / 2.f, pausePicture.getGlobalBounds().height / 2.f));
        pause.setPosition(Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 3.7f, pausePicture.getPosition().y + 110));
        resumeButton.text.setPosition(Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 2.45f, pausePicture.getPosition().y + 350));
        quitButtonpuase.text.setPosition(Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 2.30f, pausePicture.getPosition().y + 480));


        gameovPicture.setPosition(cameraCenter - Vector2f(gameovPicture.getGlobalBounds().width / 2.f, gameovPicture.getGlobalBounds().height / 2.f));
        gameover.setPosition(Vector2f(gameovPicture.getPosition().x + gameovPicture.getGlobalBounds().width / 3.7f, gameovPicture.getPosition().y + 150));
        restartButton.text.setPosition(Vector2f(gameovPicture.getPosition().x + gameovPicture.getGlobalBounds().width / 2.45f, gameovPicture.getPosition().y + 350));
        quitButtongameover.text.setPosition(Vector2f(gameovPicture.getPosition().x + gameovPicture.getGlobalBounds().width / 2.30f, gameovPicture.getPosition().y + 480));


        Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));

        bool isMousePressed = Mouse::isButtonPressed(Mouse::Left);


        if (player1.sprite.getGlobalBounds().intersects(reclevel3.getGlobalBounds())) {
            displayLetterTransition(window, letter3Sprite, transitionTriggered, cam, paperSound);
            Level3Transition(background, player1, playerPosition, level3texture);
            bossBattleSound.play();
            bossBattleSound.setLoop(true);

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
            pl2.setPosition(player1.sprite.getPosition().x + 80.f, player1.sprite.getPosition().y + 80.f);
            snake.setPosition(enemy1.sprite.getPosition().x + 20.f, enemy1.sprite.getPosition().y + 70.f);
            snake2.setPosition(enemy2.sprite.getPosition().x + 20.f, enemy2.sprite.getPosition().y + 70.f);
            snake4.setPosition(enemy4.sprite.getPosition().x + 20.f, enemy4.sprite.getPosition().y + 70.f);
            snake5.setPosition(enemy5.sprite.getPosition().x + 20.f, enemy5.sprite.getPosition().y + 70.f);
            b.setPosition(enemy3.sprite.getPosition().x + 90.f, enemy3.sprite.getPosition().y + 100.f);
            t.setPosition(enemy3.ip_x, enemy3.ip_y + 150);

            //enemy1
            if (player1.sprite.getPosition().x < (enemy1.sprite.getPosition().x - 300) || player1.sprite.getPosition().x >(enemy1.sprite.getPosition().x + 300)) {
                enemy1.move_x = -0.25;

                if ((enemy1.sprite.getPosition().x) < (enemy1.ip_x - enemy1.chase_range) || (enemy1.sprite.getPosition().x > (enemy1.ip_x + enemy1.chase_range)))
                    enemy1.direction *= -1;

                enemy1.move_x *= enemy1.direction;
                enemy1.sprite.move(enemy1.move_x, 0);
            }
            else {
                enemy1.attack = true;

                if (player1.sprite.getPosition().x < (enemy1.sprite.getPosition().x))
                    enemy1.direction = -1;

                else if (player1.sprite.getPosition().x >= (enemy1.sprite.getPosition().x)) {
                    enemy1.direction = 1;
                }

                enemy1.move_x = enemy1.direction * 0.2;
            }

            if (enemy1.sprite.getPosition().x < player1.sprite.getPosition().x + 0.2 && enemy1.sprite.getPosition().x > player1.sprite.getPosition().x)
                enemy1.move_x = 0;

            if (enemy1.sprite.getPosition().x > player1.sprite.getPosition().x - 0.2 && enemy1.sprite.getPosition().x < player1.sprite.getPosition().x)
                enemy1.move_x = 0;

            if (pl2.getGlobalBounds().intersects(snake.getGlobalBounds())) {

                if (player1.sprite.getPosition().x <= snake.getPosition().x) {
                    player1.rect.left = snake.getPosition().x - 170;

                    auto currentTime = std::chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - enemy1t.time;

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
                        enemy1t.time = currentTime; // Update last collision time
                    }

                }


                float player_bottom = pl2.getPosition().y + height;
                float snake_top = snake.getPosition().y + 20;
                if (player_bottom < snake_top) {
                    enemy1.dead = true;
                    enemySound.play();

                }

            }

            if (enemy1.dead == true) {
                enemy1.move_x = 0.001;
                if (enemy1.currentframe > 4) {
                    enemy1.rectenemy.left = 90000;
                    snake.setPosition(90000, 80000);
                }
            }


            //enemy2
            if (player1.sprite.getPosition().x < (enemy2.sprite.getPosition().x - 300) || player1.sprite.getPosition().x >(enemy2.sprite.getPosition().x + 300)) {
                enemy2.move_x = -0.25;

                if ((enemy2.sprite.getPosition().x) < (enemy2.ip_x - enemy2.chase_range) || (enemy2.sprite.getPosition().x > (enemy2.ip_x + enemy2.chase_range)))
                    enemy2.direction *= -1;

                enemy2.move_x *= enemy2.direction;
                enemy2.sprite.move(enemy2.move_x, 0);
            }
            else {
                enemy2.attack = true;

                if (player1.sprite.getPosition().x < (enemy2.sprite.getPosition().x))
                    enemy2.direction = -1;

                else if (player1.sprite.getPosition().x >= (enemy2.sprite.getPosition().x)) {
                    enemy2.direction = 1;
                }

                enemy2.move_x = enemy2.direction * 0.2;
            }

            if (enemy2.sprite.getPosition().x < player1.sprite.getPosition().x + 0.2 && enemy2.sprite.getPosition().x > player1.sprite.getPosition().x)
                enemy2.move_x = 0;

            if (enemy2.sprite.getPosition().x > player1.sprite.getPosition().x - 0.2 && enemy2.sprite.getPosition().x < player1.sprite.getPosition().x)
                enemy2.move_x = 0;

            if (pl2.getGlobalBounds().intersects(snake2.getGlobalBounds())) {

                if (player1.sprite.getPosition().x <= snake2.getPosition().x) {
                    player1.rect.left = snake2.getPosition().x - 170;
                    auto currentTime = std::chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - enemy2t.time;

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
                        enemy2t.time = currentTime; // Update last collision time
                    }
                }

                float player_bottom = pl2.getPosition().y + height;
                float snake_top = snake2.getPosition().y + 20;
                if (player_bottom < snake_top) {
                    enemy2.dead = true;
                    enemySound.play();

                }

            }

            if (enemy2.dead == true) {
                enemy2.move_x = 0.001;
                if (enemy2.currentframe > 4) {
                    enemy2.rectenemy.left = 90000;
                    snake2.setPosition(90000, 80000);
                }
            }



            //enemy4
            if (player1.sprite.getPosition().x < (enemy4.sprite.getPosition().x - 300) || player1.sprite.getPosition().x >(enemy4.sprite.getPosition().x + 300)) {
                enemy4.move_x = -0.25;

                if ((enemy4.sprite.getPosition().x) < (enemy4.ip_x - enemy4.chase_range) || (enemy4.sprite.getPosition().x > (enemy4.ip_x + enemy4.chase_range)))
                    enemy4.direction *= -1;

                enemy4.move_x *= enemy4.direction;
                enemy4.sprite.move(enemy4.move_x, 0);
            }
            else {
                enemy4.attack = true;

                if (player1.sprite.getPosition().x < (enemy4.sprite.getPosition().x))
                    enemy4.direction = -1;

                else if (player1.sprite.getPosition().x >= (enemy4.sprite.getPosition().x)) {
                    enemy4.direction = 1;
                }

                enemy4.move_x = enemy4.direction * 0.2;
            }

            if (enemy4.sprite.getPosition().x < player1.sprite.getPosition().x + 0.2 && enemy4.sprite.getPosition().x > player1.sprite.getPosition().x)
                enemy4.move_x = 0;

            if (enemy4.sprite.getPosition().x > player1.sprite.getPosition().x - 0.2 && enemy4.sprite.getPosition().x < player1.sprite.getPosition().x)
                enemy4.move_x = 0;

            if (pl2.getGlobalBounds().intersects(snake4.getGlobalBounds())) {

                if (player1.sprite.getPosition().x <= snake4.getPosition().x) {
                    player1.rect.left = snake4.getPosition().x - 170;
                    auto currentTime = std::chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - enemy4t.time;

                    // Check if enough time has passed since the last collision
                    if (timeSinceLastCollision >= delayDuration) {

                        health--;
                        cout << health << endl;
                        cout << "flag4" << endl;
                        if (health <= 0)
                        {
                            player1.sprite.setTexture(playerTexture[2]);
                            player1.move_x = 0.25;
                            dead = 1;
                        }
                        enemy4t.time = currentTime; // Update last collision time
                    }
                }

                float player_bottom = pl2.getPosition().y + height;
                float snake_top = snake4.getPosition().y + 20;
                if (player_bottom < snake_top) {
                    enemy4.dead = true;
                    enemySound.play();

                }

            }

            if (enemy4.dead == true) {
                enemy4.move_x = 0.001;
                if (enemy4.currentframe > 4) {
                    enemy4.rectenemy.left = 90000;
                    snake4.setPosition(90000, 80000);
                }
            }

            //enemy5
            if (player1.sprite.getPosition().x < (enemy5.sprite.getPosition().x - 300) || player1.sprite.getPosition().x >(enemy5.sprite.getPosition().x + 300)) {
                enemy5.move_x = -0.25;

                if ((enemy5.sprite.getPosition().x) < (enemy5.ip_x - enemy5.chase_range) || (enemy5.sprite.getPosition().x > (enemy5.ip_x + enemy5.chase_range)))
                    enemy5.direction *= -1;

                enemy5.move_x *= enemy5.direction;
                enemy5.sprite.move(enemy5.move_x, 0);
            }
            else {
                enemy5.attack = true;

                if (player1.sprite.getPosition().x < (enemy5.sprite.getPosition().x))
                    enemy5.direction = -1;

                else if (player1.sprite.getPosition().x >= (enemy5.sprite.getPosition().x)) {
                    enemy5.direction = 1;
                }

                enemy5.move_x = enemy5.direction * 0.2;
            }

            if (enemy5.sprite.getPosition().x < player1.sprite.getPosition().x + 0.2 && enemy5.sprite.getPosition().x > player1.sprite.getPosition().x)
                enemy5.move_x = 0;

            if (enemy5.sprite.getPosition().x > player1.sprite.getPosition().x - 0.2 && enemy5.sprite.getPosition().x < player1.sprite.getPosition().x)
                enemy5.move_x = 0;

            if (pl2.getGlobalBounds().intersects(snake5.getGlobalBounds())) {

                if (player1.sprite.getPosition().x <= snake5.getPosition().x) {
                    player1.rect.left = snake5.getPosition().x - 170;
                    auto currentTime = std::chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - enemy5t.time;

                    // Check if enough time has passed since the last collision
                    if (timeSinceLastCollision >= delayDuration) {

                        health--;
                        cout << health << endl;
                        cout << "flag5" << endl;
                        if (health <= 0)
                        {
                            player1.sprite.setTexture(playerTexture[2]);
                            player1.move_x = 0.25;
                            dead = 1;
                        }
                        enemy5t.time = currentTime; // Update last collision time
                    }
                }

                float player_bottom = pl2.getPosition().y + height;
                float snake_top = snake5.getPosition().y + 20;
                if (player_bottom < snake_top) {
                    enemy5.dead = true;
                    enemySound.play();

                }

            }

            if (enemy5.dead == true) {
                enemy5.move_x = 0.001;
                if (enemy5.currentframe > 4) {
                    enemy5.rectenemy.left = 90000;
                    snake5.setPosition(90000, 80000);
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
                snake6.setPosition(enemy6.sprite.getPosition().x + 20.f, enemy6.sprite.getPosition().y + 70.f);
                snake7.setPosition(enemy7.sprite.getPosition().x + 20.f, enemy7.sprite.getPosition().y + 70.f);
                snake8.setPosition(enemy8.sprite.getPosition().x + 20.f, enemy8.sprite.getPosition().y + 70.f);
                snake9.setPosition(enemy9.sprite.getPosition().x + 20.f, enemy9.sprite.getPosition().y + 70.f);
                pl2.setPosition(player1.sprite.getPosition().x + 80.f, player1.sprite.getPosition().y + 80.f);


                //enemy6
                if (player1.sprite.getPosition().x < (enemy6.sprite.getPosition().x - 300) || player1.sprite.getPosition().x >(enemy6.sprite.getPosition().x + 300)) {
                    enemy6.move_x = -0.25;

                    if ((enemy6.sprite.getPosition().x) < (enemy6.ip_x - enemy6.chase_range) || (enemy6.sprite.getPosition().x > (enemy6.ip_x + enemy6.chase_range)))
                        enemy6.direction *= -1;

                    enemy6.move_x *= enemy6.direction;
                    enemy6.sprite.move(enemy6.move_x, 0);
                }
                else {
                    enemy6.attack = true;

                    if (player1.sprite.getPosition().x < (enemy6.sprite.getPosition().x))
                        enemy6.direction = -1;

                    else if (player1.sprite.getPosition().x >= (enemy6.sprite.getPosition().x)) {
                        enemy6.direction = 1;
                    }

                    enemy6.move_x = enemy6.direction * 0.2;
                }

                if (enemy6.sprite.getPosition().x < player1.sprite.getPosition().x + 0.2 && enemy6.sprite.getPosition().x > player1.sprite.getPosition().x)
                    enemy6.move_x = 0;

                if (enemy6.sprite.getPosition().x > player1.sprite.getPosition().x - 0.2 && enemy6.sprite.getPosition().x < player1.sprite.getPosition().x)
                    enemy6.move_x = 0;

                if (pl2.getGlobalBounds().intersects(snake6.getGlobalBounds())) {

                    if (player1.sprite.getPosition().x <= snake6.getPosition().x) {
                        player1.rect.left = snake6.getPosition().x - 170;

                        auto currentTime = std::chrono::steady_clock::now();
                        auto timeSinceLastCollision = currentTime - enemy6t.time;

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
                            enemy6t.time = currentTime; // Update last collision time
                        }

                    }


                    float player_bottom = pl2.getPosition().y + height;
                    float snake6_top = snake6.getPosition().y + 20;
                    if (player_bottom < snake6_top) {
                        enemy6.dead = true;
                        enemySound.play();

                    }

                }

                if (enemy6.dead == true) {
                    enemy6.move_x = 0.001;
                    if (enemy6.currentframe > 4) {
                        enemy6.rectenemy.left = 90000;
                        snake6.setPosition(90000, 80000);
                    }
                }


                //enemy7
                if (player1.sprite.getPosition().x < (enemy7.sprite.getPosition().x - 300) || player1.sprite.getPosition().x >(enemy7.sprite.getPosition().x + 300)) {
                    enemy7.move_x = -0.25;

                    if ((enemy7.sprite.getPosition().x) < (enemy7.ip_x - enemy7.chase_range) || (enemy7.sprite.getPosition().x > (enemy7.ip_x + enemy7.chase_range)))
                        enemy7.direction *= -1;

                    enemy7.move_x *= enemy7.direction;
                    enemy7.sprite.move(enemy7.move_x, 0);
                }
                else {
                    enemy7.attack = true;

                    if (player1.sprite.getPosition().x < (enemy7.sprite.getPosition().x))
                        enemy7.direction = -1;

                    else if (player1.sprite.getPosition().x >= (enemy7.sprite.getPosition().x)) {
                        enemy7.direction = 1;
                    }

                    enemy7.move_x = enemy7.direction * 0.2;
                }

                if (enemy7.sprite.getPosition().x < player1.sprite.getPosition().x + 0.2 && enemy7.sprite.getPosition().x > player1.sprite.getPosition().x)
                    enemy7.move_x = 0;

                if (enemy7.sprite.getPosition().x > player1.sprite.getPosition().x - 0.2 && enemy7.sprite.getPosition().x < player1.sprite.getPosition().x)
                    enemy7.move_x = 0;

                if (pl2.getGlobalBounds().intersects(snake7.getGlobalBounds())) {

                    if (player1.sprite.getPosition().x <= snake7.getPosition().x) {
                        player1.rect.left = snake7.getPosition().x - 170;

                        auto currentTime = std::chrono::steady_clock::now();
                        auto timeSinceLastCollision = currentTime - enemy7t.time;

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
                            enemy7t.time = currentTime; // Update last collision time
                        }

                    }


                    float player_bottom = pl2.getPosition().y + height;
                    float snake7_top = snake7.getPosition().y + 20;
                    if (player_bottom < snake7_top) {
                        enemy7.dead = true;
                        enemySound.play();

                    }

                }

                if (enemy7.dead == true) {
                    enemy7.move_x = 0.001;
                    if (enemy7.currentframe > 4) {
                        enemy7.rectenemy.left = 90000;
                        snake7.setPosition(90000, 80000);
                    }
                }

                //enemy8
                if (player1.sprite.getPosition().x < (enemy8.sprite.getPosition().x - 300) || player1.sprite.getPosition().x >(enemy8.sprite.getPosition().x + 300)) {
                    enemy8.move_x = -0.25;

                    if ((enemy8.sprite.getPosition().x) < (enemy8.ip_x - enemy8.chase_range) || (enemy8.sprite.getPosition().x > (enemy8.ip_x + enemy8.chase_range)))
                        enemy8.direction *= -1;

                    enemy8.move_x *= enemy8.direction;
                    enemy8.sprite.move(enemy8.move_x, 0);
                }
                else {
                    enemy8.attack = true;

                    if (player1.sprite.getPosition().x < (enemy8.sprite.getPosition().x))
                        enemy8.direction = -1;

                    else if (player1.sprite.getPosition().x >= (enemy8.sprite.getPosition().x)) {
                        enemy8.direction = 1;
                    }

                    enemy8.move_x = enemy8.direction * 0.2;
                }

                if (enemy8.sprite.getPosition().x < player1.sprite.getPosition().x + 0.2 && enemy8.sprite.getPosition().x > player1.sprite.getPosition().x)
                    enemy8.move_x = 0;

                if (enemy8.sprite.getPosition().x > player1.sprite.getPosition().x - 0.2 && enemy8.sprite.getPosition().x < player1.sprite.getPosition().x)
                    enemy8.move_x = 0;

                if (pl2.getGlobalBounds().intersects(snake8.getGlobalBounds())) {

                    if (player1.sprite.getPosition().x <= snake8.getPosition().x) {
                        player1.rect.left = snake8.getPosition().x - 170;

                        auto currentTime = std::chrono::steady_clock::now();
                        auto timeSinceLastCollision = currentTime - enemy8t.time;

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
                            enemy8t.time = currentTime; // Update last collision time
                        }

                    }


                    float player_bottom = pl2.getPosition().y + height;
                    float snake8_top = snake8.getPosition().y + 20;
                    if (player_bottom < snake8_top) {
                        enemy8.dead = true;
                        enemySound.play();
                    }

                }

                if (enemy8.dead == true) {
                    enemy8.move_x = 0.001;
                    if (enemy8.currentframe > 4) {
                        enemy8.rectenemy.left = 90000;
                        snake8.setPosition(90000, 80000);
                    }
                }


                //snake9
                if (player1.sprite.getPosition().x < (enemy9.sprite.getPosition().x - 300) || player1.sprite.getPosition().x >(enemy9.sprite.getPosition().x + 300)) {
                    enemy9.move_x = -0.25;

                    if ((enemy9.sprite.getPosition().x) < (enemy9.ip_x - enemy9.chase_range) || (enemy9.sprite.getPosition().x > (enemy9.ip_x + enemy9.chase_range)))
                        enemy9.direction *= -1;

                    enemy9.move_x *= enemy9.direction;
                    enemy9.sprite.move(enemy9.move_x, 0);
                }
                else {
                    enemy9.attack = true;

                    if (player1.sprite.getPosition().x < (enemy9.sprite.getPosition().x))
                        enemy9.direction = -1;

                    else if (player1.sprite.getPosition().x >= (enemy9.sprite.getPosition().x)) {
                        enemy9.direction = 1;
                    }

                    enemy9.move_x = enemy9.direction * 0.2;
                }

                if (enemy9.sprite.getPosition().x < player1.sprite.getPosition().x + 0.2 && enemy9.sprite.getPosition().x > player1.sprite.getPosition().x)
                    enemy9.move_x = 0;

                if (enemy9.sprite.getPosition().x > player1.sprite.getPosition().x - 0.2 && enemy9.sprite.getPosition().x < player1.sprite.getPosition().x)
                    enemy9.move_x = 0;

                if (pl2.getGlobalBounds().intersects(snake9.getGlobalBounds())) {

                    if (player1.sprite.getPosition().x <= snake9.getPosition().x) {
                        player1.rect.left = snake9.getPosition().x - 170;

                        auto currentTime = std::chrono::steady_clock::now();
                        auto timeSinceLastCollision = currentTime - enemy9t.time;

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
                            enemy9t.time = currentTime; // Update last collision time
                        }

                    }


                    float player_bottom = pl2.getPosition().y + height;
                    float snake9_top = snake9.getPosition().y + 20;
                    if (player_bottom < snake9_top) {
                        enemy9.dead = true;
                        enemySound.play();


                    }

                }

                if (enemy9.dead == true) {
                    enemy9.move_x = 0.001;
                    if (enemy9.currentframe > 4) {
                        enemy9.rectenemy.left = 90000;
                        snake9.setPosition(90000, 80000);
                    }
                }


            }
            else {
                reclevel3.setPosition(9000.f, 750.f);
                //handle enemies and letters in level3
                for (int i = 0; i < 6; ++i) {
                    if (isColliding(rec, ob5[i])) {
                        auto currentTime = std::chrono::steady_clock::now();
                        auto timeSinceLastCollision = currentTime - lastCollisionOb2[i].time;

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

                pl2.setPosition(player1.sprite.getPosition().x + 80.f, player1.sprite.getPosition().y + 80.f);
                snake10.setPosition(enemy10.sprite.getPosition().x + 20.f, enemy10.sprite.getPosition().y + 70.f);
                snake11.setPosition(enemy11.sprite.getPosition().x + 20.f, enemy11.sprite.getPosition().y + 70.f);



                //enemy10
                if (player1.sprite.getPosition().x < (enemy10.sprite.getPosition().x - 300) || player1.sprite.getPosition().x >(enemy10.sprite.getPosition().x + 300)) {
                    enemy10.move_x = -0.25;

                    if ((enemy10.sprite.getPosition().x) < (enemy10.ip_x - enemy10.chase_range) || (enemy10.sprite.getPosition().x > (enemy10.ip_x + enemy10.chase_range)))
                        enemy10.direction *= -1;

                    enemy10.move_x *= enemy10.direction;
                    enemy10.sprite.move(enemy10.move_x, 0);
                }
                else {
                    enemy10.attack = true;

                    if (player1.sprite.getPosition().x < (enemy10.sprite.getPosition().x))
                        enemy10.direction = -1;

                    else if (player1.sprite.getPosition().x >= (enemy10.sprite.getPosition().x)) {
                        enemy10.direction = 1;
                    }

                    enemy10.move_x = enemy10.direction * 0.2;
                }

                if (enemy10.sprite.getPosition().x < player1.sprite.getPosition().x + 0.2 && enemy10.sprite.getPosition().x > player1.sprite.getPosition().x)
                    enemy10.move_x = 0;

                if (enemy10.sprite.getPosition().x > player1.sprite.getPosition().x - 0.2 && enemy10.sprite.getPosition().x < player1.sprite.getPosition().x)
                    enemy10.move_x = 0;

                if (pl2.getGlobalBounds().intersects(snake10.getGlobalBounds())) {

                    if (player1.sprite.getPosition().x <= snake10.getPosition().x) {
                        player1.rect.left = snake10.getPosition().x - 170;

                        auto currentTime = std::chrono::steady_clock::now();
                        auto timeSinceLastCollision = currentTime - enemy10t.time;

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
                            enemy10t.time = currentTime; // Update last collision time
                        }

                    }


                    float player_bottom = pl2.getPosition().y + height;
                    float snake106_top = snake10.getPosition().y + 20;
                    if (player_bottom < snake106_top) {
                        enemy10.dead = true;
                        enemySound.play();

                    }

                }

                if (enemy10.dead == true) {
                    enemy10.move_x = 0.001;
                    if (enemy10.currentframe > 4) {
                        enemy10.rectenemy.left = 90000;
                        snake10.setPosition(90000, 80000);
                    }
                }


                //enemy11
                if (player1.sprite.getPosition().x < (enemy11.sprite.getPosition().x - 300) || player1.sprite.getPosition().x >(enemy11.sprite.getPosition().x + 300)) {
                    enemy11.move_x = -0.25;

                    if ((enemy11.sprite.getPosition().x) < (enemy11.ip_x - enemy11.chase_range) || (enemy11.sprite.getPosition().x > (enemy11.ip_x + enemy11.chase_range)))
                        enemy11.direction *= -1;

                    enemy11.move_x *= enemy11.direction;
                    enemy11.sprite.move(enemy11.move_x, 0);
                }
                else {
                    enemy11.attack = true;

                    if (player1.sprite.getPosition().x < (enemy11.sprite.getPosition().x))
                        enemy11.direction = -1;

                    else if (player1.sprite.getPosition().x >= (enemy11.sprite.getPosition().x)) {
                        enemy11.direction = 1;
                    }

                    enemy11.move_x = enemy11.direction * 0.2;
                }

                if (enemy11.sprite.getPosition().x < player1.sprite.getPosition().x + 0.2 && enemy11.sprite.getPosition().x > player1.sprite.getPosition().x)
                    enemy11.move_x = 0;

                if (enemy11.sprite.getPosition().x > player1.sprite.getPosition().x - 0.2 && enemy11.sprite.getPosition().x < player1.sprite.getPosition().x)
                    enemy11.move_x = 0;

                if (pl2.getGlobalBounds().intersects(snake11.getGlobalBounds())) {

                    if (player1.sprite.getPosition().x <= snake11.getPosition().x) {
                        player1.rect.left = snake11.getPosition().x - 170;

                        auto currentTime = std::chrono::steady_clock::now();
                        auto timeSinceLastCollision = currentTime - enemy11t.time;

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
                            enemy11t.time = currentTime; // Update last collision time
                        }

                    }


                    float player_bottom = pl2.getPosition().y + height;
                    float snake116_top = snake11.getPosition().y + 20;
                    if (player_bottom < snake116_top) {
                        enemy11.dead = true;
                        enemySound.play();

                    }

                }

                if (enemy11.dead == true) {
                    enemy11.move_x = 0.001;
                    if (enemy11.currentframe > 4) {
                        enemy11.rectenemy.left = 90000;
                        snake11.setPosition(90000, 80000);
                    }
                }


              }
        }
        if (health >= 0) {
            hpp.setScale(Vector2f((1.8 * (static_cast<float>(health) / 40)), 1.f));
        }
        if (!gameisov)
        {
            if (!isPaused) {
                Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
                bool isMousePressed =Mouse::isButtonPressed(Mouse::Left);

                // sound.play();
                float timer = clock.getElapsedTime().asMicroseconds();
                clock.restart();
                timer /= 1000;
                if (timer > 100)
                    timer = 100;



                if (pl.getGlobalBounds().intersects(wolf2.spritee.getGlobalBounds())) {
                  
                    wolfwalking = false;
                    wolfattacking1 = true;
                    wolf2.spritee.setScale(-2.5f, 2.5f);
                }

                if ((wolf2.spritee.getPosition().x < pl.getPosition().x) && (wolfwalking = true) || (wolf2.spritee.getPosition().x <= 6800.f)) {
                    wolfwalking = true;
                    wolfattacking1 = false;
                    wolf2.spritee.setScale(2.5f, 2.5f);
                    wolf2.spritee.move(-wolf2speed, 0.0f);
                }

                if ((wolf2.spritee.getPosition().x > pl.getPosition().x) && (wolfwalking = true)) {
                    wolfattacking1 = false;
                    wolfwalking = true;

                    wolf2.spritee.setScale(-2.5f, 2.5f);
                    wolf2.spritee.move(wolf2speed, 0.0f);
                }


                if (abs(wolf2.spritee.getPosition().x - pl.getPosition().x) <= 2) {
                    wolfwalking = false;
                    wolfattacking1 = true;
                    wolf2.spritee.setScale(2.5f, 2.5f);
                }



                if ((centipede2.spritee.getPosition().x < pl.getPosition().x) && (centwalking = true) || (centipede2.spritee.getPosition().x <= 2200.f)) {
                    centwalking = true;
                    centattacking = false;
                    centipede2.spritee.setScale(-2.0f, 2.0f);
                    centipede2.spritee.move(-centspeed2, 0.0f);
                }

                if ((centipede2.spritee.getPosition().x > pl.getPosition().x) && (centwalking = true)) {
                    centattacking = false;
                    centwalking = true;
                    centipede2.spritee.setScale(2.0f, 2.0f);
                    centipede2.spritee.move(centspeed2, 0.0f);
                }
                if (abs(centipede2.spritee.getPosition().x - pl.getPosition().x) <= 2) {
                    centwalking = false;
                    centattacking = true;

                }

                if ((demon.spritee.getPosition().x < pl.getPosition().x)) {
                    demon.spritee.move(demonspeed, 0.0f);
                    demonwalking = true;
                    demonattacking = false;
                    demon.spritee.setScale(-2.5f, 2.5f);

                }
                else if ((demon.spritee.getPosition().x > pl.getPosition().x)) {
                    demon.spritee.move(-demonspeed, 0.0f);
                    demonattacking = false;
                    demonwalking = true;
                    demon.spritee.setScale(2.5f, 2.5f);

                }


                if (abs(demon.spritee.getPosition().x - pl.getPosition().x) <= 2) {
                    demonwalking = false;
                    demonattacking = true;
                    demon.spritee.setScale(2.5f, 2.5f);
                }
                timer2 += letterClock.restart().asSeconds();
                if (continueanimation == true) {


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
                        {
                          
                            rectsourcewolfdead.left = 128;
                        }
                        //currentFrame = (currentFrame + 1) % numFrames; // Loop through frames
                        //// Update the texture rectangle for the next frame
                        //textureRect.left = currentFrame * frameWidth;
                        rectspecialwalk.left += 140;
                        if (rectspecialwalk.left >= 180)
                            rectspecialwalk.left = 0;


                        rectspecialattack.left += 67;
                        if (rectspecialattack.left >= 1120)
                            rectspecialattack.left = 0;


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
                        if (rectsourcecentdead.left >= 144) {
                     
                            rectsourcecentdead.left = 144;
                        }

                        rectdemonwalk.left += 65;
                        if (rectdemonwalk.left >= 390)
                            rectdemonwalk.left = 0;

                        rectdemonattack.left += 65;
                        if (rectdemonattack.left >= 390)
                            rectdemonattack.left = 0;

                        rectdemondead.left += 65;
                        if (rectdemondead.left >= 512) {
                          
                            rectdemondead.left = 448;
                        }



                        rectmud.left += 62;
                        if (rectmud.left >= 495)

                            rectmud.left = 0;


                        rectmudhurt.left += 62;
                        if (rectmudhurt.left >= 248)
                            rectmudhurt.left = 0;
               

                        if (wolfwalking == true) {
                            wolf.spritee.setTextureRect(rectsourcewolfwalk);
                            wolf.spritee.setTexture(wolfwalktexture);
                        }

                        if (wolfattacking2 == true) {
                            wolf.spritee.setTextureRect(rectsourcewolfattack1);
                            wolf.spritee.setTexture(wolfattack1texture);

                        }
                    
                        if (wolfhurting == true) {
                            wolf.spritee.setTexture(wolfhurttexture);
                            wolf.spritee.setTextureRect(rectsourcewolfhurt);
                        }

                        if (healthwolf == 0) {
                            wolf.spritee.setTexture(wolfdeadtexture);
                            wolf.spritee.setTextureRect(rectsourcewolfdead);
                        }
                        if (demonwalking == true) {
                            demon.spritee.setTexture(demonwalktext);
                            demon.spritee.setTextureRect(rectdemonwalk);
                        }
                        if (demonattacking == true) {
                            demon.spritee.setTexture(demonattacktext);
                            demon.spritee.setTextureRect(rectdemonattack);
                        }
                        if (demonhurt == true) {
                            demon.spritee.setTexture(demonhurttext);
                            demon.spritee.setTextureRect(rectdemonhurt);
                        }
                        if (healthdemon == 0) {
                            demon.spritee.setTexture(demondeadtext);
                            demon.spritee.setTextureRect(rectdemondead);

                        }
                        if (mudd == true) {
                            mud.spritee.setTexture(mudtext);
                            mud.spritee.setTextureRect(rectmud);
                        }
                        if (wolfdamage == true && mudhurt == true) {
                            mud.spritee.setTexture(mudtexthurt);
                            mud.spritee.setTextureRect(rectmudhurt);
                        }
                        if (centwalking == true) {
                            centipede2.spritee.setTexture(Centipedewalktexture);
                            centipede2.spritee.setTextureRect(rectsourcecentwalk);
                        }

                        if (centattacking == true) {
                            centipede2.spritee.setTexture(Centipedeattacktexture);
                            centipede2.spritee.setTextureRect(rectsourcecentattack);
                        }
                        if (wolfdamage == true) {
                            centipede2.spritee.setTexture(Centipedehurttexture);
                            centipede2.spritee.setTextureRect(rectsourcecenthurt);
                        }
                        if (healthcent2 == 0) {
                            centipede2.spritee.setTexture(Centipededeadtexture);
                            centipede2.spritee.setTextureRect(rectsourcecentdead);
                        }
                        //   
                        if (specialswitch) {
                            player1.sprite.setTexture(specialwalktext);
                            player1.sprite.setTextureRect(rectspecialwalk);
                        }
                        if (specialswitch && attack) {
                            player1.sprite.setTexture(specialattacktext);
                            player1.sprite.setTextureRect(rectspecialattack);
                        }
                        sister.setTexture(sistertext);



                        clockenemy.restart();
                    }
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
                    player1.move_x = 0.1;
                    attack = 1;
                    attackSound.play();
                }

                if (Keyboard::isKeyPressed(Keyboard::Z))
                {
                    player1.sprite.setTexture(playerTexture[1]);
                    player1.move_x = -0.1;
                    attack = 1;
                    attackSound.play();
                }
                if (Keyboard::isKeyPressed(Keyboard::P))
                {
                    specialswitch = true;
                    player1.sprite.setPosition(200, 800);
                }
                if (specialswitch) {
                    switchCharacter(player1, specialwalktext);
                    if (Keyboard::isKeyPressed(Keyboard::Right))
                    {
                        switchCharacter(player1, specialwalktext);
                        player1.move_x = 0.25;
                    }
                    if (Keyboard::isKeyPressed(Keyboard::Left))
                    {
                        switchCharacter(player1, specialwalktext);

                        player1.move_x = -0.25;
                    }
                    if (Keyboard::isKeyPressed(Keyboard::X))
                    {
                        switchCharacter(player1, specialattacktext);
                        player1.move_x = 0.25;
                        attack = 1;
                    }


                }
               
                if (wolfdamage == false && Keyboard::isKeyPressed(Keyboard::X) && (pl.getGlobalBounds().intersects(wol.getGlobalBounds()))) {
                    wolfdamage = true;
                    keyPressed = false;
                    attack = 0;
                    if (healthwolf > 0)
                        healthwolf--;
                }
                if (wolfdamage == false && Keyboard::isKeyPressed(Keyboard::X) && (pl.getGlobalBounds().intersects(wolf2.spritee.getGlobalBounds()))) {
                    wolfdamage = true;
                    keyPressed = false;
                    attack = 0;
                    if (healthwolf2 > 0)
                        healthwolf2--;
                }
                if (healthwolf2 == 0) {
                    wolf2.spritee.move(0, 0);
                    wolf2speed = 0;
                    wolf2.spritee.setPosition(8000.f, 8000.f);
                }
                if (healthwolf2 == 0 && enemyHealth["wolf2h"] == 0) {
                    enemySound.play();
                    enemyHealth["wolf2h"] = 1;
                }
                if (wolfdamage == false && Keyboard::isKeyPressed(Keyboard::X) && (pl.getGlobalBounds().intersects(cen.getGlobalBounds()))) {
                    wolfdamage = true;
                    keyPressed = false;
                    attack = 0;
                    if (healthcent > 0)
                        healthcent--;
                }
                if (wolfdamage == false && Keyboard::isKeyPressed(Keyboard::X) && (pl.getGlobalBounds().intersects(centipede2.spritee.getGlobalBounds()))) {
                    wolfdamage = true;
                    keyPressed = false;
                    attack = 0;
                    if (healthcent2 > 0)
                        healthcent2--;
                }
                if (wolfdamage == false && Keyboard::isKeyPressed(Keyboard::X) && (pl.getGlobalBounds().intersects(dem.getGlobalBounds()))) {
                    wolfdamage = true;
                    keyPressed = false;
                    attack = 0;
                    if (healthdemon > 0)
                        healthdemon--;
                }
                if (wolfdamage == false && Keyboard::isKeyPressed(Keyboard::X) && (pl.getGlobalBounds().intersects(mud.spritee.getGlobalBounds()))) {
                    wolfdamage = true;
                    mudhurt = true;
                    keyPressed = false;
                    attack = 0;
                    if (healthmud > 0)
                        healthmud--;

                }
                if (wolfdamage == true && !Keyboard::isKeyPressed(Keyboard::X)) {
                    wolfdamage = false;
                }

            
                if (demonattacking && healthdemon != 0) {

                    auto currentTime = chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - demontime.time;

                    // Check if enough time has passed since the last collision
                    if (timeSinceLastCollision >= delayDuration) {
                        health--;

                        if (health <= 0)
                        {
                            player1.sprite.setTexture(playerTexture[2]);
                            player1.move_x = 0.25;
                            dead = 1;
                        }
                        demontime.time = currentTime; // Update last collision time
                    }


                }


              

                if (wolfattacking1 && healthwolf2 != 0) {
                    auto currentTime = chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - demontime.time;

                    // Check if enough time has passed since the last collision
                    if (timeSinceLastCollision >= delayDuration) {
                        health--;

                        if (health <= 0)
                        {
                            player1.sprite.setTexture(playerTexture[2]);
                            player1.move_x = 0.25;
                            dead = 1;
                        }
                        demontime.time = currentTime; // Update last collision time
                    }

                }
               

                // handle enemies collision
                if (healthwolf == 0) {
                    wolf.spritee.move(0, 0);
                    wolfspeed = 0;
                    wolf.spritee.setPosition(8000.f, 8000.f);

                }
                if (healthwolf == 0 && enemyHealth["wolfh"] == 0) {
                    enemySound.play();
                    enemyHealth["wolfh"] = 1;
                }
                cout << health << endl;

                if (wolfattacking2 && healthwolf != 0) {
                    auto currentTime = chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - demontime.time;

                    
                    if (timeSinceLastCollision >= delayDuration) {
                        health -= 2;

                        if (health <= 0)
                        {
                            player1.sprite.setTexture(playerTexture[2]);
                            player1.move_x = 0.25;
                            dead = 1;
                        }
                        demontime.time = currentTime; // Update last collision time
                    }
                }
                if (wolfdamage == false && Keyboard::isKeyPressed(Keyboard::X) && (pl.getGlobalBounds().intersects(cen.getGlobalBounds()))) {
                    wolfdamage = true;
                    keyPressed = false;
                    player1.sprite.setTexture(playerTexture[1]);
                    player1.move_x = 0.25;
                    if (healthcent > 0)
                        healthcent--;
                }
                if (healthdemon == 0) {
                   
                    demonspeed = 0;
                    demon.spritee.setPosition(8000.f, 8000.f);
                }
                if (healthdemon == 0 && enemyHealth["demonh"] == 0) {
                    enemySound.play();
                    enemyHealth["demonh"] = 1;
                }
                if (healthmud == 0) {
                   


                    mud.spritee.setPosition(8000.f, 8000.f);
                }
                if (wolfwalking == true) {
                    wolf2.spritee.setTextureRect(rectsourcewolfwalk);
                    wolf2.spritee.setTexture(wolfwalktexture);
                }

                if (wolfattacking1 == true) {
                    wolf2.spritee.setTextureRect(rectsourcewolfattack1);
                    wolf2.spritee.setTexture(wolfattack1texture);

                }
               
                if (wolfhurting == true) {
                    wolf2.spritee.setTexture(wolfhurttexture);
                    wolf2.spritee.setTextureRect(rectsourcewolfhurt);
                }

                if (healthwolf2 == 0) {
                    wolf2.spritee.setTexture(wolfdeadtexture);
                    wolf2.spritee.setTextureRect(rectsourcewolfdead);
                }
                if (healthcent == 0) {
                   
                    centspeed = 0;
                    centipede.spritee.setPosition(8000.f, 8000.f);
                   
                }
                if (healthcent == 0 && enemyHealth["cent1h"] == 0) {
                    enemySound.play();
                    enemyHealth["cent1h"] == 1;
                }
                if (pl.getGlobalBounds().intersects(centipede.spritee.getGlobalBounds())) {

                    centwalking = false;
                    centattacking = true;

                }
                if ((centipede.spritee.getPosition().x < pl.getPosition().x) && (centwalking = true) || (centipede.spritee.getPosition().x <= 2200.f)) {
                    centwalking = true;
                    centattacking = false;
                    centipede.spritee.setScale(-2.0f, 2.0f);
                    centipede.spritee.move(-centspeed, 0.0f);
                }

                if ((centipede.spritee.getPosition().x > pl.getPosition().x) && (centwalking = true)) {
                    centattacking = false;
                    centwalking = true;
                    centipede.spritee.setScale(2.0f, 2.0f);
                    centipede.spritee.move(centspeed, 0.0f);
                }
                if (abs(centipede.spritee.getPosition().x - pl.getPosition().x) <= 2) {
                    centwalking = false;
                    centattacking = true;

                }
                if (centattacking && healthcent != 0) {
                    auto currentTime = chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - demontime.time;

                    // Check if enough time has passed since the last collision
                    if (timeSinceLastCollision >= delayDuration) {
                        health--;

                        if (health <= 0)
                        {
                            player1.sprite.setTexture(playerTexture[2]);
                            player1.move_x = 0.25;
                            dead = 1;
                        }
                        demontime.time = currentTime; // Update last collision time
                    }

                }



                if (player1.sprite.getGlobalBounds().intersects(reclevel3.getGlobalBounds())) {
                    displayLetterTransition(window, letter3Sprite, transitionTriggered, cam, paperSound);
                    Level3Transition(background, player1, playerPosition, level3texture);
                }


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
                            if (continueanimation == true)

                            {


                                if (clockenemy.getElapsedTime().asSeconds() > 0.1f) {

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
                                    if (rectsourcecentdead.left >= 144) {

                                        rectsourcecentdead.left = 144;
                                    }
                                    rectmud.left += 62;
                                    if (rectmud.left >= 495)

                                        rectmud.left = 0;


                                    rectmudhurt.left += 62;
                                    if (rectmudhurt.left >= 248)
                                        rectmudhurt.left = 0;
                                    if (mudd == true) {
                                        mud.spritee.setTexture(mudtext);
                                        mud.spritee.setTextureRect(rectmud);
                                    }
                                    if (wolfdamage == true && mudhurt == true) {
                                        mud.spritee.setTexture(mudtexthurt);
                                        mud.spritee.setTextureRect(rectmudhurt);
                                    }



                                    clockenemy.restart();
                                }  

                            } if (centwalking == true) {
                                centipede.spritee.setTexture(Centipedewalktexture);
                                centipede.spritee.setTextureRect(rectsourcecentwalk);
                            }

                            if (centattacking == true) {
                                centipede.spritee.setTexture(Centipedeattacktexture);
                                centipede.spritee.setTextureRect(rectsourcecentattack);
                            }
                            if (wolfdamage == true) {
                                centipede.spritee.setTexture(Centipedehurttexture);
                                centipede.spritee.setTextureRect(rectsourcecenthurt);
                            }
                            if (healthcent == 0) {
                                centipede.spritee.setTexture(Centipededeadtexture);
                                centipede.spritee.setTextureRect(rectsourcecentdead);
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

                        for (int i = 0; i < 9; i++) {
                            if (isColliding(rec, ob5[i])) {

                                player1.move_x = 0;
                            }
                        }

                        if (continueanimation == true) {


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
                                {
                                    //  wolfdead = true;
                                    rectsourcewolfdead.left = 128;
                                }
                                if (centwalking == true) {
                                    centipede2.spritee.setTexture(Centipedewalktexture);
                                    centipede2.spritee.setTextureRect(rectsourcecentwalk);
                                }

                                if (centattacking == true) {
                                    centipede2.spritee.setTexture(Centipedeattacktexture);
                                    centipede2.spritee.setTextureRect(rectsourcecentattack);
                                }
                                if (wolfdamage == true) {
                                    centipede2.spritee.setTexture(Centipedehurttexture);
                                    centipede2.spritee.setTextureRect(rectsourcecenthurt);
                                }
                                if (healthcent2 == 0) {
                                    centipede2.spritee.setTexture(Centipededeadtexture);
                                    centipede2.spritee.setTextureRect(rectsourcecentdead);
                                }
                                if (wolfwalking == true) {
                                    wolf.spritee.setTextureRect(rectsourcewolfwalk);
                                    wolf.spritee.setTexture(wolfwalktexture);
                                }

                                if (wolfattacking2 == true) {
                                    wolf.spritee.setTextureRect(rectsourcewolfattack1);
                                    wolf.spritee.setTexture(wolfattack1texture);

                                }
                                //   wolfattack2.setTextureRect(rectsourcewolfattack2);
                                if (wolfhurting == true) {
                                    wolf.spritee.setTexture(wolfhurttexture);
                                    wolf.spritee.setTextureRect(rectsourcewolfhurt);
                                }

                                if (healthwolf == 0) {
                                    wolf.spritee.setTexture(wolfdeadtexture);
                                    wolf.spritee.setTextureRect(rectsourcewolfdead);
                                }
                                clockenemy.restart();
                            }
                        }
                        if (centattacking && healthcent2 != 0) {
                            auto currentTime = chrono::steady_clock::now();
                            auto timeSinceLastCollision = currentTime - demontime.time;

                            // Check if enough time has passed since the last collision
                            if (timeSinceLastCollision >= delayDuration) {
                                health--;

                                if (health <= 0)
                                {
                                    player1.sprite.setTexture(playerTexture[2]);
                                    player1.move_x = 0.25;
                                    dead = 1;
                                }
                                demontime.time = currentTime; // Update last collision time
                            }

                        }
                       
                        if (healthcent2 == 0) {
                           
                            centspeed2 = 0;
                            centipede2.spritee.setPosition(8000.f, 8000.f);

                            //     centipede.setScale(0, 0);
                        }
                        if (healthcent2 == 0 && enemyHealth["cent2h"] == 0) {
                            enemySound.play();
                            enemyHealth["cent2h"] = 1;
                        }  
                        
                      
                        
                        if ((pl.getGlobalBounds().intersects(mud.spritee.getGlobalBounds()) && healthmud != 0)) {

                            auto currentTime = chrono::steady_clock::now();
                            auto timeSinceLastCollision = currentTime - demontime.time;

                            // Check if enough time has passed since the last collision
                            if (timeSinceLastCollision >= delayDuration) {
                                health--;

                                if (health <= 0)
                                {
                                    player1.sprite.setTexture(playerTexture[2]);
                                    player1.move_x = 0.25;
                                    dead = 1;
                                }
                                demontime.time = currentTime; // Update last collision time
                            }


                        }
                        if ((wol.getPosition().x < pl.getPosition().x) && (wolfwalking = true) || (wol.getPosition().x <= 6000.f)) {
                            wolfwalking = true;
                            wolfattacking2 = false;
                            wolf.spritee.setScale(3.f, 3.f);
                            wolf.spritee.move(-wolfspeed, 0.0f);

                        }

                        if (pl.getGlobalBounds().intersects(wol.getGlobalBounds())) {

                            // If collision occurs with the player, make the wolf attacking
                            wolfwalking = false;
                            wolfattacking2 = true;
                            wolf.spritee.setScale(3.f, 3.f);
                        }


                        if ((wol.getPosition().x > pl.getPosition().x) && (wolfwalking = true)) {
                            wolfattacking2 = false;
                            wolfwalking = true;
                            wolf.spritee.setScale(-3.f, 3.f);
                            wolf.spritee.move(wolfspeed, 0.0f);

                        }

                        if (abs(wol.getPosition().x - pl.getPosition().x) <= 2) {
                            wolfwalking = false;
                            wolfattacking2 = true;
                            wolf.spritee.setScale(3.f, 3.f);
                        }
                        // handle enemies collision
                        if (healthwolf == 0) {
                            wolf.spritee.move(0, 0);
                            wolfspeed = 0;
                            wolf.spritee.setPosition(8000.f, 8000.f);
                        }

                        if (player1.sprite.getGlobalBounds().intersects(sister.getGlobalBounds())) {
                            displayLetterTransition(window, winLetter, transitionTriggered, cam, paperSound);
                            bossBattleSound.stop();
                            win = 1;
                            gameisov = 1;
                       
                        }
                       
                    }
                }

            


                Event::Resized;
                resizedview(window, cam);
               
                if (timer2 > 2.0) {
                        showLetter = true; // Set showLetter to true to display the letter
                    
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
           

                if (attack) {
                    player1.update(timer, 3);
                    attack = false;
                }
                else if (attack == 0)
                    player1.update(timer, 3);


                if (health <= 0)
                {
                    player1.sprite.setTexture(playerTexture[2]);
                    player1.update(timer, 2);
                    displayLetterTransition(window, gameOver, transitionTriggered, cam, deathSound);
                    gameisov = true;
                    sound.stop();
                    againSound.play();
                    againSound.setLoop(true);


                }


                float dt = clock.restart().asSeconds();
                rec.setPosition(player1.sprite.getPosition().x + 94, player1.sprite.getPosition().y);

                hpp.setPosition(cam.getCenter().x - 595, cam.getCenter().y - 360.5);
                hpbar.setPosition(cam.getCenter().x - 590, cam.getCenter().y - 356.5);
                phealth.setPosition(cam.getCenter().x - 590, cam.getCenter().y - 410);

                window.clear();
                window.draw(background);
                window.draw(hpbar);
                window.draw(phealth);
                window.draw(hpp);
                window.draw(demon.spritee);
                window.draw(waechter.spritee);
                window.draw(necklaceSprite);
                necklaceSprite.rotate(3.f);
                window.draw(arrowSprite);
                arrowSprite.rotate(3.f);
                window.draw(ballonSprite);
                ballonSprite.rotate(3.f);
                window.draw(fireSprite);
                fireSprite.rotate(3.f);
                window.draw(tool2Sprite);
                tool2Sprite.rotate(3.f);
                window.draw(diamondSprite);
                diamondSprite.rotate(3.f);
                window.draw(keySprite);
                keySprite.rotate(3.f);
                window.draw(torchSprite);
                torchSprite.rotate(3.f);




              
                pl.setPosition(player1.sprite.getPosition().x + 80.f, player1.sprite.getPosition().y + 80.f);
                float width = 50.f;
                float height = 100.f;
                pl.setSize(Vector2f(width, height));
                wol.setPosition(wolf.spritee.getPosition().x - 70.f, wolf.spritee.getPosition().y - 20.f);

                dem.setPosition(demon.spritee.getPosition().x - 70.f, demon.spritee.getPosition().y - 20.f);
                cen.setPosition(centipede.spritee.getPosition().x - 70.f, centipede.spritee.getPosition().y - 20.f);
                dem.setSize(Vector2f(100, 140));
                cen.setSize(Vector2f(100, 80));
                wol.setSize(Vector2f(120, 140));
            
                window.draw(player1.sprite);
                window.draw(letterSprite);
       

                if (player1.sprite.getGlobalBounds().intersects(rectangle.getGlobalBounds())) {

                    // Display the letter transition
                    displayLetterTransition(window, letter2Sprite, transitionTriggered, cam, paperSound);
                    // window.draw(letter1Sprite);
                    handleLevelTransition(background, player1, playerPosition, level2texture, ob1, ob2, ob3, ob4, ob3tex, ob4tex, ob2tex, ob1tex);

                }
                // Display the letter only once the falling animation is finished
                if (showLetter && !letterDisplayed) {
                    // Assuming you have already loaded your transition sprite
                    cout << "DONE" << endl;
                    // Replace "transitionSprite" with your actual sprite variable
                    displayLetterTransition(window, letter1Sprite, transitionTriggered, cam, paperSound);
                    showLetter = false;
                    letterDisplayed = true; // Update the flag to indicate that the letter has been displayed
                }

                if (!levelTransitionCompleted) {
                    for (int i = 0; i < 9; i++) {
                        window.draw(ob1[i]);
                        window.draw(ob2[i]);
                    }
                    window.draw(enemy1.sprite);
                    window.draw(enemy2.sprite);
                    window.draw(enemy4.sprite);
                    window.draw(enemy5.sprite);
              

                    enemy1.update_enemy(htimer, enemy1.attack, Enemyattack, enemy1.dead, Enemydeath);
                    enemy2.update_enemy(htimer, enemy2.attack, Enemyattack, enemy2.dead, Enemydeath);
                    enemy4.update_enemy(htimer, enemy4.attack, Enemyattack, enemy4.dead, Enemydeath);
                    enemy5.update_enemy(htimer, enemy5.attack, Enemyattack, enemy5.dead, Enemydeath);
                   
                }

                else {
                    if (!level3Completed) {
                        for (int i = 0; i < 9; i++) {
                            window.draw(ob3[i]);
                            window.draw(ob4[i]);
                        }


                        window.draw(enemy6.sprite);
                        enemy6.update_enemy(htimer, enemy6.attack, Enemyattack, enemy6.dead, Enemydeath);
                        window.draw(enemy7.sprite);
                        enemy7.update_enemy(htimer, enemy7.attack, Enemyattack, enemy7.dead, Enemydeath);
                        window.draw(enemy8.sprite);
                        enemy8.update_enemy(htimer, enemy8.attack, Enemyattack, enemy8.dead, Enemydeath);
                        window.draw(enemy9.sprite);
                        enemy9.update_enemy(htimer, enemy9.attack, Enemyattack, enemy9.dead, Enemydeath);



                        window.draw(wolf2.spritee);
                        window.draw(centipede.spritee);

                     
                    }
                    else {
                        for (int i = 0; i < 9; i++) {
                            window.draw(ob5[i]);
                        }
                        window.draw(enemy10.sprite);
                        enemy10.update_enemy(htimer, enemy10.attack, Enemyattack, enemy10.dead, Enemydeath);
                        window.draw(enemy11.sprite);
                        enemy11.update_enemy(htimer, enemy11.attack, Enemyattack, enemy11.dead, Enemydeath);

                        window.draw(mud.spritee);
                        window.draw(wolf.spritee);
                        window.draw(centipede2.spritee);
                        window.draw(sister);
                    }
                }
                window.display();


            }
            else
            {
                Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
                bool isMousePressed = Mouse::isButtonPressed(Mouse::Left);

                resumeButton.update(mousePosition, isMousePressed);
                quitButtonpuase.update(mousePosition, isMousePressed);
                cout << "Paused" << endl;

                window.clear();
                // Draw the buttons if paused
                window.clear(Color(0, 0, 0, 128));
                window.draw(pausePicture);
                window.draw(pause);
                resumeButton.draw(window);
                quitButtonpuase.draw(window);


                if (isMousePressed && resumeButton.isClicked(mousePosition)) {
                    isPaused = false; // Resume the game
                    pauseSound.stop();
                    sound.play();
                }

                if (isMousePressed && quitButtonpuase.isClicked(mousePosition)) {
                    window.close(); // Close the window
                    pauseSound.stop();
                    cout << "Quit button clicked!" << endl;
                }

                window.display();
            }


        }
        else
        {
            
            if (win) {
                healthwolf = 35;
                healthwolf2 = 25;
                healthcent = 20;
                healthcent2 = 20;

                healthmud = 15;
                healthdemon = 25;
                gameisov = false; // Resume the game
                health = 40;
                level3Completed = false;
                levelTransitionCompleted = false;
                rectangle.setPosition(6600.f, 750.f);
                reclevel3.setPosition(7900.f, 750.f);

                Text titleText;
                titleText.setFont(font2);
                titleText.setString("CAN YOU SAVE THEM?");
                titleText.setCharacterSize(80);
                titleText.setFillColor(Color::White);
                // titleText.setPosition(280.6f, 760.f);

                Text press;
                press.setFont(font2);
                press.setString("Press SPACE to continue....");
                press.setCharacterSize(25);
                press.setFillColor(Color::White);
                //press.setPosition(290.f, 880.f);
                vector<string> guidtext = {

                " controls: \n press[x][z] to attack \n press[->] to move right \n press[<-] to move left \n press[space] to jump" };

                Text guide;
                guide.setFont(font2);
                guide.setString("controls: \n\n press [x] [z] to attack \n press [->] to move right \n press [<-] to move left \n press [space] to jump \n press [esc] to pause");
                guide.setCharacterSize(24);
                guide.setFillColor(Color::White);
                //  guide.setPosition(1360.f, 760.f);

                //  splashScreen.setPosition(cam.getCenter().x - 570, cam.getCenter().y - 430.5);
                //  splashScreen.setOrigin(0, 0);

                splashScreen.setPosition(cameraCenter - Vector2f(splashScreen.getGlobalBounds().width / 2.f, splashScreen.getGlobalBounds().height / 2.f));
                titleText.setPosition(Vector2f(splashScreen.getPosition().x + splashScreen.getGlobalBounds().width / 3.7f, splashScreen.getPosition().y + 150));
                press.setPosition(Vector2f(splashScreen.getPosition().x + splashScreen.getGlobalBounds().width / 3.7f, splashScreen.getPosition().y + 350));
                press.setPosition(Vector2f(splashScreen.getPosition().x + splashScreen.getGlobalBounds().width / 3.7f, splashScreen.getPosition().y + 480));
           

                // Draw the splash screen elements
                sound3.play();
                window.draw(splashScreen);
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
            else {
               
                Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
                bool isMousePressed = Mouse::isButtonPressed(Mouse::Left);

                quitButtongameover.update(mousePosition, isMousePressed);

                restartButton.update(mousePosition, isMousePressed);

                window.clear();
                // Draw the buttons if paused
                window.clear(Color(0, 0, 0, 128));
                window.draw(gameovPicture);
                window.draw(gameover);
                quitButtongameover.draw(window);
                restartButton.draw(window);


                if (isMousePressed && restartButton.isClicked(mousePosition))
                {
                    healthwolf = 35;
                    healthwolf2 = 25;
                    healthcent = 20;
                    healthcent2 = 20;

                    healthmud = 15;
                    healthdemon = 25;
                    gameisov = false; // Resume the game
                    health = 40;
                    level3Completed = false;
                    levelTransitionCompleted = false;
                    rectangle.setPosition(6600.f, 750.f);
                    reclevel3.setPosition(7900.f, 750.f);
                    againSound.stop();
                    Game_Play(window);


                }

                if (isMousePressed && quitButtongameover.isClicked(mousePosition))
                {
                    window.close(); // Close the window
                    cout << "Quit button clicked!" << endl;
                }
            }

            window.display();

        }

    }
}
