#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cmath>
#include <time.h>
#include <vector>

using namespace std;

#pragma
sf::RenderWindow window;
int vitesse = 6;
int vitesseZ= 1;
float attenu = 1/sqrt(2);
int taille= 32;
int gros= 2;
int posini= 250;
int screenx = 800;
int screeny = 600;
float repouss = 0.12    ;
sf::Texture perso;
sf::Sprite sprite_perso;
sf::Texture zombierd;
sf::Sprite sprite_zombierd;
sf::Texture zombieai;
sf::Sprite sprite_zombieai;
enum Dir{Down,Left,Right,Up};
sf::Vector2i anim(1, Down);
sf::Vector2i animrdZ(1, Down);
sf::Vector2i animaiZ(1, Down);
bool updateFPS=false;
sf::Clock timer;
sf::Clock timerZ;
sf::Clock bulletTime;
sf::Color color;
unsigned int hp= 5;
#pragma

class Bullet
{
    public:
    sf::CircleShape shape;
    sf::Vector2f currvelocity;
    float maxspeed;

    Bullet (float radius=5.f)
        :currvelocity(0.f,0.f),maxspeed(10.f)
    {

        this->shape.setRadius(radius);
        this->shape.setFillColor(sf::Color::Red);
    }
};

// SOURIS tir? VI 4
// probleme ordre image animation
//vitesse diago Zombies
//musique
//anim.x en parametre? pareil collision

void collisionscreen()
{
    if((sprite_perso.getPosition().x+gros*taille)>=screenx)
        sprite_perso.setPosition(screenx-gros*taille,sprite_perso.getPosition().y);
    if((sprite_perso.getPosition().x<=0))
        sprite_perso.setPosition(0,sprite_perso.getPosition().y);
    if((sprite_perso.getPosition().y+gros*taille)>=screeny)
        sprite_perso.setPosition(sprite_perso.getPosition().x,screeny-gros*taille);
    if((sprite_perso.getPosition().y<=0))
        sprite_perso.setPosition(sprite_perso.getPosition().x,0);
}

void collisionZ()
{
    if(abs((sprite_perso.getPosition().x +taille*0.5) - (sprite_zombieai.getPosition().x+taille*0.5))<taille*1.5 and abs((sprite_perso.getPosition().y+taille*0.5) - (sprite_zombieai.getPosition().y+taille*0.5))<taille*gros)
    {
        cout <<"choc"<< endl;
        hp-=1;
        if(sprite_perso.getPosition().x<sprite_zombieai.getPosition().x)
        {
        sprite_zombieai.setPosition(sprite_zombieai.getPosition().x+taille*repouss,sprite_zombieai.getPosition().y);
        sprite_perso.setPosition(sprite_perso.getPosition().x-taille*repouss,sprite_perso.getPosition().y);
        }
        if(sprite_perso.getPosition().x>sprite_zombieai.getPosition().x)
        {
        sprite_zombieai.setPosition(sprite_zombieai.getPosition().x-taille*repouss,sprite_zombieai.getPosition().y);
        sprite_perso.setPosition(sprite_perso.getPosition().x+taille*repouss,sprite_perso.getPosition().y);
        }
        if(sprite_perso.getPosition().y<sprite_zombieai.getPosition().y)
        {
        sprite_zombieai.setPosition(sprite_zombieai.getPosition().x,sprite_zombieai.getPosition().y+taille*repouss);
        sprite_perso.setPosition(sprite_perso.getPosition().x,sprite_perso.getPosition().y-taille*repouss);
        }
        if(sprite_perso.getPosition().y>sprite_zombieai.getPosition().x)
        {
        sprite_zombieai.setPosition(sprite_zombieai.getPosition().x,sprite_zombieai.getPosition().y-taille*repouss);
        sprite_perso.setPosition(sprite_perso.getPosition().x,sprite_perso.getPosition().y+taille*repouss);
        }
    }
}

void collisionbullet()
{

}

void projectiles()
{
    Bullet b1;
    vector<Bullet> bullets;
    bullets.push_back(Bullet(b1));

    sf::Vector2f playerCenter;
    sf::Vector2f mousePos;
    sf::Vector2f aimDir;
    sf::Vector2f aimDirNorm;

    playerCenter = sf::Vector2f(sprite_perso.getPosition().x+taille,sprite_perso.getPosition().y+taille);
    mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    aimDir = mousePos - playerCenter;
    aimDirNorm = sf::Vector2f(aimDir) / (sqrt((aimDir.x*aimDir.x)+ (aimDir.y*aimDir.y)));
    cout <<aimDirNorm.x<<aimDirNorm.y<<  endl;


    //tire
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        b1.shape.setPosition(playerCenter);
        b1.currvelocity = aimDirNorm*b1.maxspeed;
        bullets.push_back(Bullet(b1));
    }
    for(size_t i=0; i<bullets.size();i++)
        bullets[i].shape.move(bullets[i].currvelocity);

       for(size_t i=0; i<bullets.size();i++)
        window.draw(bullets[i].shape);

}

void zic()
{
    sf::Music music;
    music.openFromFile("terraria.ogg");
    music.play();
    music.setVolume(50);
}

void rdZ()
{
    int a=-1,b=2;
    int nombre_aleatoire1 = 0,nombre_aleatoire2 = 0;

    srand(time(NULL)); // initialisation de rand
    if(!zombierd.loadFromFile("multZ.png", sf::IntRect(96,0,96,128)))
    {
            cout <<"fail zombie" <<endl;
    }
    zombierd.setSmooth(false); //gourmant mais + beau
    sprite_zombierd.setTexture(zombierd);

    nombre_aleatoire1 = rand()%(b-a) +a;
    //cout << nombre_aleatoire1 << endl;
    nombre_aleatoire2 = rand()%(b-a) +a;
    //cout << nombre_aleatoire2 << endl;
    sprite_zombierd.move(nombre_aleatoire1*vitesseZ,nombre_aleatoire2*vitesseZ);

    if (nombre_aleatoire2*vitesseZ==-1)  //IA PROBLEME TEMPS ENTRE DECISION car dépend de fps
        {animrdZ.y = Up;}
    else if(nombre_aleatoire2*vitesseZ==1)
        {animrdZ.y = Down;}
    else if(nombre_aleatoire1*vitesseZ==1)
        {animrdZ.y = Right;}
    else if(nombre_aleatoire1*vitesseZ==-1)
        {animrdZ.y = Left;}

    if(timerZ.getElapsedTime().asMilliseconds()>100) //si le temps écoulé en ms est plus grand que 50ms
    {
        animrdZ.x++;
        timerZ.restart();
        if(animrdZ.x*taille>=perso.getSize().x)
        {
            animrdZ.x=0;
        }
    }

    if((sprite_perso.getPosition().x+gros*taille)>=screenx)
        sprite_perso.setPosition(screenx-gros*taille,sprite_perso.getPosition().y);
    if((sprite_perso.getPosition().x<=0))
        sprite_perso.setPosition(0,sprite_perso.getPosition().y);
    if((sprite_perso.getPosition().y+gros*taille)>=screeny)
        sprite_perso.setPosition(sprite_perso.getPosition().x,screeny-gros*taille);
    if((sprite_perso.getPosition().y<=0))
        sprite_perso.setPosition(sprite_perso.getPosition().x,0);

}

void aiZ()
{
    int xp=sprite_perso.getPosition().x;
    int yp=sprite_perso.getPosition().y;
    int xzai=sprite_zombieai.getPosition().x;
    int yzai=sprite_zombieai.getPosition().y;

    int trackx= 0, tracky=0;
    if(!zombieai.loadFromFile("multZ.png", sf::IntRect(192,128,96,128)))
    {
            cout <<"fail zombie" <<endl;
    }
    zombieai.setSmooth(false); //gourmant mais + beau
    sprite_zombieai.setTexture(zombieai);

    if(xp>xzai)
        {trackx=1;animaiZ.y = Right;}
    if(xp<xzai)
        {trackx=-1;animaiZ.y = Left;}

    if(yp>yzai)
        {tracky=1;animaiZ.y = Down;}
    if(yp<yzai)
        {tracky=-1;animaiZ.y = Up;}

    if(timerZ.getElapsedTime().asMilliseconds()>100) //si le temps écoulé en ms est plus grand que 50ms
    {
        animaiZ.x++;
        timerZ.restart();
        if(animaiZ.x*taille>=perso.getSize().x)
        {
            animaiZ.x=0;
        }
    }
    sprite_zombieai.move(trackx*vitesseZ,tracky*vitesseZ);
}

void controle()
{
    float att=1;
    if((sf::Keyboard::isKeyPressed(sf::Keyboard::Z) and sf::Keyboard::isKeyPressed(sf::Keyboard::D))
       or (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) and sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
       or (sf::Keyboard::isKeyPressed(sf::Keyboard::S) and sf::Keyboard::isKeyPressed(sf::Keyboard::D))
       or (sf::Keyboard::isKeyPressed(sf::Keyboard::S) and sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
       )
        att=1/sqrt(2);
    else att=1;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {sprite_perso.move(0,-vitesse*att);
        anim.y = Up;}
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {sprite_perso.move(0,vitesse*att);
        anim.y = Down;}
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {sprite_perso.move(vitesse*att,0);
        anim.y = Right;}
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {sprite_perso.move(-vitesse*att,0);
        anim.y = Left;}
}

void animation ()
{
    if(timer.getElapsedTime().asMilliseconds()>100) //si le temps écoulé en ms est plus grand que 50ms
    {
        anim.x++;  //joue l'annimation
        timer.restart();                       //horloge retourne à 0
        if(anim.x*taille>=perso.getSize().x)
        {
            anim.x=0;
            //cout << sprite_perso.getPosition().x <<" et "<<  sprite_perso.getPosition().y<< endl;
        }
    }
}

void personnage()
{
    if(!perso.loadFromFile("capuche.png",sf::IntRect(288,128,96,128)))
    {
        cout <<"fail perso" <<endl;
    }
    perso.setSmooth(false); //gourmant mais + beau
    sprite_perso.setTexture(perso);

    sf::RectangleShape hpbar;
    hpbar.setFillColor(sf::Color::Green);
    hpbar.setSize(sf::Vector2f(hp*100,10));
    hpbar.setPosition(150,20);
    window.draw(hpbar);
    if (hp==0)
        cout <<"game over ";


}

void fenetre()
{
    Bullet b1;
    vector<Bullet> bullets;
    bullets.push_back(Bullet(b1));

    sf::Vector2f playerCenter;
    sf::Vector2f mousePos;
    sf::Vector2f aimDir;
    sf::Vector2f aimDirNorm;

    window.create(sf::VideoMode(screenx,screeny),"Quentin t gay"); // cree fenetre
    window.setFramerateLimit(60); //limite images par sec  WTFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
    sprite_perso.scale(gros,gros);
    sprite_perso.setPosition(posini+100,posini+100);
    sprite_zombierd.scale(gros,gros);
    sprite_zombierd.setPosition(posini,posini);
    sprite_zombieai.scale(gros,gros);
    sprite_zombieai.setPosition(posini,posini);

    sf::Music music;
    music.openFromFile("terraria.ogg"); // Music fonction?
    music.play();
    music.setVolume(10);

    while(window.isOpen())
    {
        sf::Event event;
        if(window.pollEvent(event)) //Teste evenements
        {
            if (event.type == sf::Event::Closed) //si clique sur croix, ferme fenetre
                {window.close ();}
            if(event.type==sf::Event::KeyPressed)
                    {updateFPS = true;}
            else {updateFPS = false;}
        }

        rdZ();
        personnage();
        aiZ();
        controle();
        collisionscreen();
        collisionZ();
        if(updateFPS)
            {animation();}

        playerCenter = sf::Vector2f(sprite_perso.getPosition().x+taille,sprite_perso.getPosition().y+taille);
        mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        aimDir = mousePos - playerCenter;
        aimDirNorm = sf::Vector2f(aimDir) / (sqrt((aimDir.x*aimDir.x)+ (aimDir.y*aimDir.y)));
        //cout <<aimDirNorm.x<<aimDirNorm.y<<  endl;


        //tire
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            b1.shape.setPosition(playerCenter);
            b1.currvelocity = aimDirNorm*b1.maxspeed;
            bullets.push_back(Bullet(b1));
        }
        for(size_t i=0; i<bullets.size();i++)
        {
            bullets[i].shape.move(bullets[i].currvelocity);
            window.draw(bullets[i].shape);


        //if(bullets[i].shape.getPosition().x < (sprite_zombieai.getPosition().x ) and b1.shape.getPosition()>sprite_zombieai.getPosition().x)
            //cout <<"oui"<<  endl;
        }

        sprite_perso.setTextureRect(sf::IntRect(anim.x*taille,anim.y*taille,taille,taille));
        sprite_zombierd.setTextureRect(sf::IntRect(animrdZ.x*taille,animrdZ.y*taille,taille,taille));
        sprite_zombieai.setTextureRect(sf::IntRect(animaiZ.x*taille,animaiZ.y*taille,taille,taille));

        window.draw(sprite_zombierd);
        window.draw(sprite_zombieai);
        window.draw(sprite_perso);
        window.display();
        window.clear();
    }
}

int main ()
{
    fenetre();
    return 0;
}


