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

//0 is right, 1 is up, 2 is left, 3 is down
sf::Vector2u walk(sf::Vector2u v, int direction) {
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

void draw_prime_spiral() {

	//current pixel, initially center
	sf::Vector2u current(size/2 , size/2 );

	//direction counter, 0 is right, 1 is up, 2 is left, 3 is down
	int direction = 0;
	//how far to walk in a direction
	int walk_length = 1;
	//the actual current number counter
	int number = 1;

	//output file stream
	//  std::ofstream o("out");

	for (int k = 0; current.x >= 0 && current.x <= size && current.y >= 0 && current.y <= size; k++)
	{
		//increment walk length every other change in direction
		if(k % 2 == 0 && k != 0)
			walk_length++;

		for(int i = 0; i < walk_length && current.x >= 0 && current.x <= size && current.y >= 0 && current.y <= size; i++)
		{
			//        o << number << " Walk Length: " << walk_length << " Direction: " << direction << " K: " << k;

			//check if number is prime
			bool p = isPrime(number);

			if(p) {
				//fancy colors
				//            if(k % 2 == 0)
				//                render_img.setPixel(current.x, current.y, sf::Color(0,i,i));
				//            else
				//                render_img.setPixel(current.x, current.y, sf::Color(i,0,0));
				render_img.setPixel(current.x, current.y, sf::Color(0,255,255));
			}
			current = walk(current, direction);
			number++;

			//      if(p)
			//        o << " P\n";
			//  else
			//    o << "\n";
		}

		if(direction == 3)
			direction = 0;
		else
			direction++;
	}

	//close output file stream
//    o.close();
	//mark center point
	//  render_img.setPixel(width/2 + 1, height/2 + 1, sf::Color::Red);
}

long parse_args(int argc, char* argv[]){
	
	if(argc == 1){
		printf("using default size %d\n", size);
		return size;
	}
		int new_size;
		errno = 0;
		char* end;
		new_size = strtol(argv[1], &end, 10);

		if((errno == ERANGE && (new_size == LONG_MAX || new_size <= 0))
			|| (errno != 0 && new_size == 0)){
			fprintf(stderr, "Invalid size, using default size %d\n", size); 
			return size;
		}
		if(end == argv[1]){
			fprintf(stderr, "Invalid size, using default size %d\n", size); 
			return size;
		}

		return new_size;
	return size;
}

int main(int argc, char* argv[]) {

	size = parse_args(argc, argv);

	render_img.create(size,size,sf::Color::Black);

	draw_prime_spiral();

	render_tx.loadFromImage(render_img);

	render_spr.setTexture(render_tx);


	//declare renderwindow
	window.create(sf::VideoMode(size, size, 32), "wireframe", sf::Style::Default);
	window.setFramerateLimit(60);

	//main loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.key.code == sf::Keyboard::Q)
				window.close();
		}

		window.clear(sf::Color::Black);
		window.draw(render_spr);
		window.display();
	}

	render_tx.copyToImage().saveToFile("primespiral.png");

	return 0;
}
