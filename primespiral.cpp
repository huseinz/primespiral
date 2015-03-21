/*	primespiral
*	Zubir Husein
*	Mar 12 2015
*	
*	Generates a prime spiral of a given size.
*/

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
//#include <fstream>

sf::RenderWindow window;
sf::Image render_img;
sf::Texture render_tx;
sf::Sprite render_spr;

int size = 540;

bool isPrime(int n) {
	if (n == 2) return true;
	if (n%2==0 || n == 1) return false;

	for(int i=3; i*i<=n; i+=2) {
		if(n%i==0)
			return false;
	}
	return true;
}

//return the next pixel (step) in the path we're walking
//0 is right, 1 is up, 2 is left, 3 is down
sf::Vector2u step(sf::Vector2u v, int direction) {
	switch(direction) {
	case 0:
		return sf::Vector2u(v.x+1, v.y);
	case 1:
		return sf::Vector2u(v.x, v.y-1);
	case 2:
		return sf::Vector2u(v.x-1, v.y);
	case 3:
		return sf::Vector2u(v.x, v.y+1);
	default:
		return v;
	}
}

//We start at the center pixel.
//We then walk the spiral, coloring each n which is prime
void draw_prime_spiral() {

	//current pixel, initially center
	sf::Vector2u current(size/2 , size/2 );

	//direction counter, 0 is right, 1 is up, 2 is left, 3 is down
	int direction = 0;
	//distance of our current walk
	int walk_length = 1;
	//the number of the current pixel
	int n = 1;
	
	//loop through each walk
	for (int k = 0; current.x >= 0 && current.x < size && current.y >= 0 && current.y < size; k++)
	{
		//increase walk length every other time we change direction
		if(k % 2 == 0 && k != 0)
			walk_length++;
			
		//loop through each step
		for(int i = 0; i < walk_length && current.x >= 0 && current.x <= size && current.y >= 0 && current.y <= size; i++)
		{
			//color pixel if it's prime
			if(isPrime(n))
				render_img.setPixel(current.x, current.y, sf::Color(0,255,255));
			
			//take a step
			current = step(current, direction);
			n++;
		}
		
		//change direction
		if(direction == 3)
			direction = 0;
		else
			direction++;
	}
	
	//mark center point so we can see it
	render_img.setPixel(size/2, size/2, sf::Color::Red);
}

//parse program args (size)
long parse_args(int argc, char* argv[]){
	
	//no args
	if(argc == 1){
		printf("using default size %d\n", size);
		return size;
	}
	
	errno = 0;
	char* end;
	int new_size = strtol(argv[1], &end, 10);
	//check errno and if new size > 0
	if((errno == ERANGE && (new_size == LONG_MAX || new_size <= 0))
		|| (errno != 0 && new_size == 0)){
		fprintf(stderr, "Invalid size, using default size %d\n", size); 
		return size;
	}
	//malformed arg
	if(end == argv[1]){
		fprintf(stderr, "Invalid size, using default size %d\n", size); 
		return size;
	}

	return new_size;
}

int main(int argc, char* argv[]) {

	//get size
	size = parse_args(argc, argv);
	
	//initialize our canvas
	render_img.create(size,size,sf::Color::Black);
	
	//draw spiral
	draw_prime_spiral();
	
	//load canvas into texture 
	render_tx.loadFromImage(render_img);
	
	//load canvas into sprite object so it can be drawn
	render_spr.setTexture(render_tx);


	//declare renderwindow
	window.create(sf::VideoMode(size, size, 32), "wireframe", sf::Style::Default);
	window.setFramerateLimit(60);

	//main loop
	while (window.isOpen())
	{	
		//check for close events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.key.code == sf::Keyboard::Q)
				window.close();
		}
		
		//display spiral
		window.clear(sf::Color::Black);
		window.draw(render_spr);
		window.display();
	}
	
	//write spiral to PNG file
	render_tx.copyToImage().saveToFile("primespiral.png");

	return 0;
}
