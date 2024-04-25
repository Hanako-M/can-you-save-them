#include <iostream>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>


using namespace std;
using namespace sf;


float const ground = 600;
float const right_wall = 1850;
float const left_wall = 0;
int health = 100;

static const float view_height = 800;

// Function to handle level transition




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

    void update(float timer)
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
        if (currentframe > 8)
        {
            currentframe -= 8;
        }

        if (move_x > 0)
            sprite.setTextureRect(IntRect(128 * int(currentframe), 0, 128, 128));
        if (move_x < 0)
            sprite.setTextureRect(IntRect(128 * int(currentframe) + 128, 0, -128, 128));

        sprite.setPosition(rect.left, rect.top);
        move_x = 0;

    }
};
bool levelTransitionCompleted = false;


void handleLevelTransition(Sprite& background, player& player1, const Vector2f& originalPlayerPosition, const Texture& levelTexture, Sprite ob1[9], Sprite ob2[9], Sprite ob3[9], Sprite ob4[9], const Texture& ob3tex, const Texture& ob4tex, const Texture& ob1tex, const Texture& ob2tex) {
    // Change the background to level 2
    background.setTexture(levelTexture);
    background.setScale(1.2f, 1.2f);
    background.setPosition(0.f, -40.f);

    // Reset player's position
    player1.rect.left = originalPlayerPosition.x;
    player1.rect.top = originalPlayerPosition.y;

    // Update ob3 and ob4
    for (int i = 0; i < 5; i++) {
        ob3[i].setTexture(ob3tex);
        ob3[i].setScale(0.08, 0.1);
        int randomX = static_cast<float>(rand() % (5800 - static_cast<int>(ob2[i].getGlobalBounds().width))) + ((i + 1) * 650);
        ob3[i].setPosition(randomX, 765);

        ob4[i].setTexture(ob4tex);
        ob4[i].setScale(0.1, 0.1);
        randomX = static_cast<float>(rand() % (5000 - static_cast<int>(ob1[i].getGlobalBounds().width))) + ((i + 1) * 600);
        ob4[i].setPosition(randomX, 750);
    }
    levelTransitionCompleted = true;
}

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
    std::chrono::steady_clock::time_point time;
};

void resizedview(const sf::RenderWindow& window, sf::View& view);
void Game_Play(RenderWindow& window);

int main() {
    cout << "hello";
    cout << " game team" << endl;

    RenderWindow window(VideoMode(1634, 1080), "SFML Window");
    window.setFramerateLimit(60);

    // Load the splash texture
    Texture splashTexture, level1texture;
    splashTexture.loadFromFile("brosp.png");


    // Create a sprite for the splash screen
    Sprite splash(splashTexture);
    splash.setScale(window.getSize().x / static_cast<float>(splashTexture.getSize().x),
        window.getSize().y / static_cast<float>(splashTexture.getSize().y));

    Sprite background(level1texture);
    background.setScale(1.1, 1.35);
    /*for (int i = 0; i < 2; i++) {
        level1[i].setTexture(level1texture);
        level1[i].setPosition(Vector2f(i * 1634, 0));
    }*/

    // Load the font
    Font font;
    font.loadFromFile("font text.ttf");
    // Create a text object
    

    Text titleText;
    titleText.setFont(font);
    titleText.setString("CAN YOU SAVE THEM?");
    titleText.setCharacterSize(80);
    titleText.setFillColor(Color::White);
    titleText.setPosition(340.f, 760.f);

    Text press;
    press.setFont(font);
    press.setString("Press SPACE to continue....");
    press.setCharacterSize(25);
    press.setFillColor(Color::White);
    press.setPosition(350.f, 880.f);

    bool showSplashScreen = true;
    int pagenum = 1;

    // Game loop
    while (window.isOpen()) {
        // Handle events
        Event event;


        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space)
            {
                showSplashScreen = false;
                // to start main game 
                pagenum = 2;

            }
        }



        // Clear the window
        window.clear();

        if (pagenum == 2)
        {
            Game_Play(window);
        }

        if (showSplashScreen)
        {
            // Draw splash screen elements
            window.draw(splash);
            window.draw(titleText);
            window.draw(press);

        }


        // Display the contents of the window
        window.display();



    }




    return 0;
}


void resizedview(const sf::RenderWindow& window, sf::View& view) {

    float ratio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(view_height * ratio, view_height);
}



void Game_Play(RenderWindow& window)
{
     //obstacles
    Texture ob1tex, ob2tex, ob3tex, ob4tex, ob5tex;
    ob1tex.loadFromFile("ob1.png");
    ob2tex.loadFromFile("spikes_1.png");
    ob3tex.loadFromFile("spike2.png");
    ob4tex.loadFromFile("spike1.png");
    ob5tex.loadFromFile("blade_2.png");

    // Initialize obstacle sprites
    Sprite ob1[9], ob2[9], ob3[9], ob4[9], ob5[9];
    for (int i = 0; i < 6; i++) {
        ob2[i].setTexture(ob2tex);
        ob2[i].setScale(0.145, 0.2);
        int randomX = static_cast<float>(rand() % (5800-static_cast<int>(ob2[i].getGlobalBounds().width))) + ((i + 1) * 650);

        ob2[i].setPosition(randomX, 765);
    }
    for (int i = 0; i < 9; i++) {
        ob1[i].setTexture(ob1tex);
        ob1[i].setScale(0.186, 0.15);

        // Generate random positions within the window boundaries
        int randomX = static_cast<float>(rand() % (5000 - static_cast<int>(ob1[i].getGlobalBounds().width))) + ((i + 1) * 600);
        ob1[i].setPosition(randomX, 750);
    }
    
    RectangleShape rec(Vector2f( 28.f, 210.f));
    rec.setFillColor(Color::White);


    //pause menu
    Font font;
    font.loadFromFile("28 Days Later.ttf");

    Text pause;
    pause.setFont(font);
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
    Button resumeButton("Resume", font, 80, sf::Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 1.f, pausePicture.getPosition().y + 500),
        sf::Color::White, sf::Color::Green, sf::Color::Red);

    // Create quit button
    Button quitButton("Quit", font, 80, sf::Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 2.f, pausePicture.getPosition().y + 900),
        sf::Color::White, sf::Color::Green, sf::Color::Red);

    Clock clock;
    Clock clock2;
    View cam(FloatRect(0, 0, 1200, 1100));

    Texture playerTexture;
    playerTexture.loadFromFile("Walk-player.png");
   

    Texture level1texture;
    level1texture.loadFromFile("level1_background.png");


    Sprite background(level1texture);
    background.setScale(1.1, 1.35);
    

    Texture hyenatext;
    hyenatext.loadFromFile("Hyena_walk.png");

    RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    rectangle.setPosition(100.f, 400.f); // Position (x=300, y=200)
    rectangle.setFillColor(sf::Color::White); // Fill color


    player player1;
    player1.initial_values(playerTexture);
    player1.sprite.setTextureRect(IntRect(0, 0, 128, 128));
    player1.rect.left = 10;
    player1.rect.top = 500;
    Vector2f playerPosition(10, 500); // Initial position

    Texture level2texture;
    level2texture.loadFromFile("level2_background.png");

    Texture wolfwalktexture;
    wolfwalktexture.loadFromFile("walk.png");
    IntRect rectsourcewolfwalk(1408, 0, 128, 128);
    Sprite wolfwalk(wolfwalktexture, rectsourcewolfwalk);

    wolfwalk.setPosition(600.f, 530.f);
    wolfwalk.setScale(2.2f, 2.2f);


    Texture wolfattack1texture;
    wolfattack1texture.loadFromFile("Attack_1.png");
    IntRect rectsourcewolfattack1(640, 0, 128, 128);
    Sprite wolfattack1(wolfattack1texture, rectsourcewolfattack1);

    wolfattack1.setPosition(300.f, 300.f);
    wolfattack1.setScale(2.2f, 2.2f);


    Texture wolfattack2texture;
    wolfattack2texture.loadFromFile("Run+Attack.png");
    IntRect rectsourcewolfattack2(768, 0, 128, 128);
    Sprite wolfattack2(wolfattack2texture, rectsourcewolfattack2);

    wolfattack2.setPosition(300.f, 300.f);
    wolfattack2.setScale(2.2f, 2.2f);

    Texture wolfhurttexture;
    wolfhurttexture.loadFromFile("Hurt.png");
    IntRect rectsourcewolfhurt(256, 0, 128, 128);
    Sprite wolfhurt(wolfhurttexture, rectsourcewolfhurt);
    wolfhurt.setPosition(350.f, 300.f);
    wolfhurt.setScale(2.2f, 2.2f);

    Texture wolfdeadtexture;
    wolfdeadtexture.loadFromFile("Dead.png");
    IntRect rectsourcewolfdead(256, 0, 128, 128);
    Sprite wolfdead(wolfdeadtexture, rectsourcewolfdead);
    wolfdead.setPosition(350.f, 300.f);
    wolfdead.setScale(2.2f, 2.2f);

    Texture Centipedewalktexture;
    Centipedewalktexture.loadFromFile("Centipede_walk.png");
    IntRect rectsourcecentwalk(288, 0, 72, 72);
    Sprite centipedewalk(Centipedewalktexture, rectsourcecentwalk);
    centipedewalk.setPosition(400.f, 500.f);
    centipedewalk.setScale(2.2f, 2.2f);

    Texture Centipedeattacktexture;
    Centipedeattacktexture.loadFromFile("Centipede_attack2.png");
    IntRect rectsourcecentattack(432, 0, 72, 72);
    Sprite centipedeattack(Centipedeattacktexture, rectsourcecentattack);
    centipedeattack.setPosition(450.f, 500.f);
    centipedeattack.setScale(2.2f, 2.2f);
    Texture Centipedehurttexture;
    Centipedehurttexture.loadFromFile("Centipede_hurt.png");
    IntRect rectsourcecenthurt(144, 0, 72, 72);
    Sprite centipedehurt(Centipedehurttexture, rectsourcecenthurt);
    centipedehurt.setPosition(700.f, 600.f);
    centipedehurt.setScale(2.2f, 2.2f);

    Texture Centipededeadtexture;
    Centipededeadtexture.loadFromFile("Centipede_death.png");
    IntRect rectsourcecentdead(288, 0, 72, 72);
    Sprite centipededead(Centipededeadtexture, rectsourcecentdead);
    centipededead.setPosition(750.f, 600.f);
    centipededead.setScale(2.2f, 2.2f);

   
    bool isPaused = false;
    bool keyPressed = false; 

    int posCNT = 1, bgINX = 0;
    window.clear(sf::Color(0, 0, 0, 128));

    LastCollisionTime lastCollisionOb1[9]; // Store last collision time for ob1
    LastCollisionTime lastCollisionOb2[9]; // Store last collision time for ob2

    // Define the delay duration (e.g., 2 seconds)
    const std::chrono::seconds delayDuration(1);


    while (window.isOpen())
    {
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



        if (player1.sprite.getGlobalBounds().intersects(rectangle.getGlobalBounds())) {
            handleLevelTransition(background, player1, playerPosition, level2texture, ob1, ob2, ob3, ob4, ob3tex, ob4tex, ob2tex, ob1tex);

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
                        lastCollisionOb2[i].time = currentTime; // Update last collision time
                    }
                }
            }
        }
        else {
            for (int i = 0; i < 9; ++i) {
                if (isColliding(rec, ob3[i])) {
                    auto currentTime = std::chrono::steady_clock::now();
                    auto timeSinceLastCollision = currentTime - lastCollisionOb1[i].time;

                    // Check if enough time has passed since the last collision
                    if (timeSinceLastCollision >= delayDuration) {
                        health--;
                        cout << health << endl;
                        lastCollisionOb1[i].time = currentTime; // Update last collision time
                    }
                }


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
                        lastCollisionOb2[i].time = currentTime; // Update last collision time
                    }
                }
            }
        }

        if (!isPaused) {
            sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

            float timer = clock.getElapsedTime().asMicroseconds();
            clock.restart();
            timer /= 1000;
            if (timer > 100)
                timer = 100;

            /*    updateAnimation(wolfwalk, rectsourcewolfwalk, 1408, clock2);
                updateAnimation(wolfattack1, rectsourcewolfattack1, 640, clock2);
                updateAnimation(wolfattack2, rectsourcewolfattack2, 768, clock2);
                updateAnimation(wolfhurt, rectsourcewolfhurt, 256, clock2);
                updateAnimation(wolfdead, rectsourcewolfdead, 256, clock2);*/

            if (clock2.getElapsedTime().asSeconds() > 0.1f) {

                rectsourcewolfwalk.left += 128;
                if (rectsourcewolfwalk.left >= 1408)
                    rectsourcewolfwalk.left = 0;

                rectsourcewolfattack1.left += 128;
                if (rectsourcewolfattack1.left >= 640)
                    rectsourcewolfattack1.left = 0;

                rectsourcewolfattack2.left += 128;
                if (rectsourcewolfattack2.left >= 768)
                    rectsourcewolfattack2.left = 0;

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
                centipededead.setTextureRect(rectsourcecentdead);


                // Set texture rectangles for both animations
                wolfwalk.setTextureRect(rectsourcewolfwalk);
                wolfattack1.setTextureRect(rectsourcewolfattack1);
                wolfattack2.setTextureRect(rectsourcewolfattack2);

                wolfhurt.setTextureRect(rectsourcewolfhurt);


                centipedehurt.setTextureRect(rectsourcecenthurt);
                wolfdead.setTextureRect(rectsourcewolfdead);

                centipedewalk.setTextureRect(rectsourcecentwalk);
                centipedewalk.setPosition(1500,550);
                if (player1.rect.top >= 1200) {
                    // Calculate direction vector from centipede to player
                    sf::Vector2f direction = player1.sprite.getPosition() - centipedewalk.getPosition();


                    // Set speed
                    float speed = 1.0f; // Adjust as needed

                    // Move centipede towards the player
                    centipedewalk.move(direction * speed);
                }
                // Move centipede towards the player
              
                centipedeattack.setTextureRect(rectsourcecentattack);


                clock2.restart();
            }

            // Handle level transition
           

            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                player1.move_x = 0.25;

            }
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                
                player1.move_x = -0.25;
            }

            if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                if (player1.onground)
                {
                    player1.move_y = -1.4;
                    player1.onground = false;
                    continue;
                }

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
            sf::Event::Resized;
            resizedview(window, cam);


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



            player1.update(timer);

           
            float dt = clock.restart().asSeconds();
            rec.setPosition(player1.sprite.getPosition().x+94, player1.sprite.getPosition().y);
            //hyena.update(dt);
            
            window.clear();
            window.draw(background);
            //window.draw(rec);
           
            window.draw(player1.sprite);
            //window.draw(hyena.sprite);
            window.draw(rectangle);
            if (!levelTransitionCompleted) {
                for (int i = 0; i < 9; i++) {
                    window.draw(ob1[i]);
                    window.draw(ob2[i]);
                }
                
            }
            else {
                for (int i = 0; i < 9; i++) {
                    window.draw(ob3[i]);
                    window.draw(ob4[i]);
                }
            }
           
        }
        else {
            sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

            resumeButton.update(mousePosition, isMousePressed);
            quitButton.update(mousePosition, isMousePressed);

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
        /*for (int i = 0; i < 3; i++) {
            window.draw(cacodemon[i].sprite);
        }*/
        window.display();


    }
   
}

