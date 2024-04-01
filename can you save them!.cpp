#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;


float const ground = 600;
float const right_wall = 1850;
float const left_wall = 0;

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


        if (rect.left > right_wall)
            rect.left = right_wall;
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

void resizedview(const sf::RenderWindow& window, sf::View& view);
void Game_Play(RenderWindow& window);

int main() {
    cout << "hello";
    cout << " game team" << endl;
    
    RenderWindow window(VideoMode(1634, 1080), "SFML Window");
    

    // Load the splash texture
    Texture splashTexture,level1texture;
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
   View cam(Vector2f(0.f, 0.f), Vector2f(1500.f, 1080.f));

    Texture playerTexture;
    playerTexture.loadFromFile("Walk.png");

    Texture level1texture;
    level1texture.loadFromFile("level1_background.png");


    Sprite background(level1texture);
    background.setScale(1.1, 1.35);
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


    player player1;
    player1.initial_values(playerTexture);
    player1.sprite.setTextureRect(IntRect(0, 0, 128, 128));
    player1.rect.left = 10;
    player1.rect.top = 500;
    Vector2f playerPosition(50, 60); // Initial position



    Clock clock;
    int posCNT = 1, bgINX=0;

   
    while (window.isOpen())
    {
        Event event;
       cam.setCenter(Vector2f(player1.sprite.getPosition().x+600,500));
        window.setView(cam);
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        float timer = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        timer /= 1000;
        if (timer > 100)
            timer = 100;

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

      
        player1.update(timer);

        window.clear();
        window.draw(background);
        window.draw(player1.sprite);
        window.display();


    }
}