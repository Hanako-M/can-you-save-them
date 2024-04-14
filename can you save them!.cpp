#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;


float const ground = 600;
float const right_wall = 1850;
float const left_wall = 0;

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

void handleLevelTransition(Sprite& background, player& player1, const Vector2f& originalPlayerPosition, const Texture& levelTexture) {
    // Change the background to level 2
    background.setTexture(levelTexture);
    background.setScale(1.2f, 1.2f);
    background.setPosition(0.f, -40.f);

    // Reset player's position
    player1.rect.left = originalPlayerPosition.x;
    player1.rect.top = originalPlayerPosition.y;
}
/*void updateAnimation(Sprite& sprite, IntRect& rectSource, int boundary, Clock& clock2) {
    if (clock2.getElapsedTime().asSeconds() > 0.1f) {
        rectSource.left += 128;
        if (rectSource.left >= boundary)
            rectSource.left = 0;

        sprite.setTextureRect(rectSource);
        clock2.restart();
    }
}*/
struct enemyhana {
    Sprite sprite;
    int speed;
    float animation = 0;
    FloatRect rect;
    float currentframe;
    float move_x;
    bool movingRight; // Flag to indicate the direction of movement

    void initial_values(Texture& playerTexture)
    {
        sprite.setTexture(playerTexture);
        sprite.setScale(2.0, 2.0);
        move_x = 0;
        currentframe = 0;
        movingRight = true; // Start moving to the right initially

        // Set initial position
        rect.left = 900; // Start position x = 500
        rect.top = 700;  // Adjust as needed
    }

    void update(float timer)
    {

        // Update position
        rect.left += move_x * timer;

        // Switch direction if reaching the boundaries
        if (rect.left >= 800)
            movingRight = false;
        else if (rect.left <= 500)
            movingRight = true;

        // Update animation
        currentframe += 0.005 * timer;
        if (currentframe > 8)
        {
            currentframe -= 8;
        }

        // Set sprite texture based on direction
        if (move_x > 0)
            sprite.setTextureRect(IntRect(48 * int(currentframe), 0, 48, 48));
        else if (move_x < 0)
            sprite.setTextureRect(IntRect(48 * int(currentframe) + 48, 0, -48, 48));

        sprite.setPosition(rect.left, rect.top);
        move_x = 0;
    }
};

enemyhana hyena;



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
    titleText.setPosition(400.f, 550.f);

    Text press;
    press.setFont(font);
    press.setString("Press SPACE to continue....");
    press.setCharacterSize(25);
    press.setFillColor(Color::White);
    press.setPosition(410.f, 650.f);

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
    //pause menu
    Font font;
    font.loadFromFile("font text.ttf");

  
    Texture pausetex;
    pausetex.loadFromFile("pausemenu.png");
    sf::Sprite pausePicture(pausetex);
   // pausePicture.setOrigin(pausetex.getSize().x / 2.0f, pausetex.getSize().y / 2.0f);
    pausePicture.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
    pausePicture.setScale(1.12f, 0.768f);

    // Create resume button
    Button resumeButton("Resume", font, 70, sf::Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 1.f, pausePicture.getPosition().y + 220),
        sf::Color::White, sf::Color::Green, sf::Color::Red);

    // Create quit button
    Button quitButton("Quit", font, 70, sf::Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 1.f, pausePicture.getPosition().y + 330),
        sf::Color::White, sf::Color::Green, sf::Color::Red);

    Clock clock;
    Clock clock2;
    View cam(FloatRect(0, 0, 1200, 1100));

    Texture playerTexture, ob1tex;
    playerTexture.loadFromFile("Walk-player.png");
    ob1tex.loadFromFile("ob1.png");

    Texture level1texture;
    level1texture.loadFromFile("level1_background.png");


    Sprite background(level1texture);
    background.setScale(1.1, 1.35);
    Sprite ob1(ob1tex);
    ob1.setScale(0.2, 0.2);
    ob1.setPosition(300, 750);


    Texture hyenatext;
    hyenatext.loadFromFile("Hyena_walk.png");

    RectangleShape rectangle(sf::Vector2f(100.f, 100.f));
    rectangle.setPosition(6400.f, 800.f); // Position (x=300, y=200)
    rectangle.setFillColor(sf::Color::White); // Fill color

    /*Texture attack_Texture;
    attack_Texture.loadFromFile("Attack_1.png");


    Texture attack2_Texture;
    attack2_Texture.loadFromFile("Attack_2.png");


    Texture jump_Texture;
    jump_Texture.loadFromFile("Jump.png");

    Texture run_Texture;
    run_Texture.loadFromFile("Run.png");

    Texture dead_Texture;
    dead_Texture.loadFromFile("Dead.png");

    */
    enemyhana hyena;
    hyena.initial_values(hyenatext);
    hyena.sprite.setTextureRect(IntRect(0, 0, 48, 48));
    hyena.rect.left = 10;

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

    /*for (int i = 0; i < 3; i++) {
        cacodemon[i].sprite.setTexture(cacotext);
        cacodemon[i].speed = rand() % (6 - 1 + 1) + 1;
        cacodemon->sprite.setScale(2.5, 2.5);
        int y = rand() % (1500 - 90 + 1) + 90;

        cacodemon[i].sprite.setPosition(0, y);
        cacodemon[i].sprite.setTextureRect(IntRect(0,0,85,64));
    }*/
    hyena.speed = 1; // Set the speed of the enemy

    bool isPaused = false;
    bool keyPressed = false; // Track if 'P' key was pressed

    int posCNT = 1, bgINX = 0;
    window.clear(sf::Color(0, 0, 0, 128));



    while (window.isOpen())
    {
        Event event;
        //cam.setCenter(Vector2f(player1.sprite.getPosition().x + 600, 500));
        window.setView(cam);
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::P) {
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
       resumeButton.text.setPosition(Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 2.4f, pausePicture.getPosition().y + 200));
       quitButton.text.setPosition(sf::Vector2f(pausePicture.getPosition().x + pausePicture.getGlobalBounds().width / 2.4f, pausePicture.getPosition().y + 330));

       sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        bool isMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

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
                centipedeattack.setTextureRect(rectsourcecentattack);


                clock2.restart();
            }

            // Handle level transition
            if (player1.sprite.getGlobalBounds().intersects(rectangle.getGlobalBounds())) {
                handleLevelTransition(background, player1, playerPosition, level2texture);
            }

            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                player1.move_x = 0.25;

            }
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                player1.move_x = -0.25;
            }

            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                if (player1.onground)
                {
                    player1.move_y = -1.4;
                    player1.onground = false;
                    continue;
                }

            }
            sf::Event::Resized;
            resizedview(window, cam);


            if (cam.getCenter().x + cam.getSize().x / 2.f >= background.getGlobalBounds().left + background.getGlobalBounds().width) {
                cam.setCenter(background.getGlobalBounds().left + background.getGlobalBounds().width - cam.getSize().x / 2.f, cam.getCenter().y);
            }
            // Update the view only if the player reaches the edges of the window
            if (player1.sprite.getPosition().x > cam.getCenter().x + 350) {
                cam.move(5, 0); // Move the view to the right
            }
            else if (player1.sprite.getPosition().x < cam.getCenter().x - 600) {
                cam.move(-5, 0); // Move the view to the left
            }



            player1.update(timer);

            /* for (int i = 0; i < 3; i++) {
                 cacodemon[i].animation += 0.03;
                 cacodemon[i].sprite.move(cacodemon[i].speed, 0);

                 if (cacodemon[i].sprite.getPosition().x < 0) {
                     int y = rand() % (9) + 8;
                     cacodemon[i].sprite.setPosition(0, y);
                     cacodemon[i].speed = rand() % (6 - 1 + 1) + 1;
                 }

                 // Calculate the correct animation frame index, wrapping around if necessary
                 int frameWidth = 85;
                 int maxFrames = 512 / frameWidth; // Total frames in the animation
                 cacodemon[i].animation = fmod(cacodemon[i].animation, maxFrames);

                 cacodemon[i].sprite.setTextureRect(IntRect(frameWidth * int(cacodemon[i].animation), 0, frameWidth, 64));
             }*/
            float dt = clock.restart().asSeconds();

            hyena.update(dt);

            window.clear();
            window.draw(background);
            window.draw(ob1);
            window.draw(player1.sprite);
            //window.draw(hyena.sprite);
            window.draw(rectangle);
            window.draw(wolfwalk);
            window.draw(wolfattack1);
            window.draw(wolfattack2);
            window.draw(wolfhurt);
            window.draw(wolfdead);
            window.draw(centipedewalk);
            window.draw(centipedeattack);
            window.draw(centipedehurt);
            window.draw(centipededead);
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
