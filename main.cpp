#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <psmoveapi/psmove.h>
#include <psmoveapi/psmove_tracker.h>

#define PI = 3.14159f;
void resetBall(sf::CircleShape *ball,sf::Vector2f *ballVec,sf::RenderWindow *window)
{
   *ballVec=sf::Vector2f(8,-6);
	ball->setPosition((window->getView().getSize().x - ball->getRadius()*2) / 2, (window->getView().getSize().y - ball->getRadius()*2) / 2);
}
int main()
{
    // create the window
	sf::RenderWindow window(sf::VideoMode(640, 480), "MoveBreaker");
    window.setVerticalSyncEnabled(true);

	PSMove *move=psmove_connect();
	PSMoveTracker *tracker=psmove_tracker_new();

	while(psmove_tracker_enable(tracker,move)!=Tracker_CALIBRATED){}
	psmove_tracker_set_mirror(tracker,PSMove_True);
	float x,y,radius;
	sf::RectangleShape paddle=sf::RectangleShape(sf::Vector2f(100,20));
	sf::CircleShape ball=sf::CircleShape(10);
	sf::Vector2f ballVec;
	resetBall(&ball,&ballVec,&window);

    // run the main loop
    bool running = true;
    while (running)
    {
		psmove_tracker_update_image(tracker);
		psmove_tracker_update(tracker,NULL);
		psmove_tracker_get_position(tracker,move,&x,&y,&radius);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                running = false;
            }
        }
		while(psmove_poll(move))
		{
			int buttons=psmove_get_buttons(move);
			if(buttons & Btn_PS){
				running=false;
			}
		}

		//collision checks
		sf::Vector2f ballPos=ball.getPosition();
		if(ballPos.y<0.f)
			ballVec.y=-ballVec.y;
		if(ballPos.x<0.f || ballPos.x+ball.getRadius()*2>window.getView().getSize().x)
			ballVec.x=-ballVec.x;
		if(ballPos.y>window.getView().getSize().y)
			resetBall(&ball,&ballVec,&window);
		//Paddle collisons
		if(ballPos.x+ballVec.x >= paddle.getPosition().x && ballPos.x+ballVec.x+ball.getRadius()*2<paddle.getPosition().x+paddle.getSize().x
				&& ballPos.y+ballVec.x+ball.getRadius()*2>=paddle.getPosition().y)
			ballVec.y=-ballVec.y;
		ball.move(ballVec);
		paddle.setPosition(x,window.getView().getSize().y-30);
		window.clear();
		window.draw(ball);
		window.draw(paddle);

        window.display();
    }
    return EXIT_SUCCESS;
}
