#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "cpoint.h"
#include "screen.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

enum state { PLAY, PAUSE, INFO
};

class CSnake:public CFramedWindow
{
private:
	vector<CPoint> snakeParts;
	CPoint snakeHead;
	CPoint fruit;
	enum state gameState = PLAY;
	int currentMove = KEY_RIGHT;
	
	void spawnFruit()
	{
		fruit.y = rand() % (geom.size.y - 6) + 3;
		fruit.x = rand() % (geom.size.x - 6) + 3;
	}
	void initSnake()
	{
		snakeHead.y = rand() % (geom.size.y - 8) + 4;
		snakeHead.x = rand() % (geom.size.x - 8) + 4;
		snakeParts.push_back(CPoint(snakeHead.x - 1, snakeHead.y));
		snakeParts.push_back(CPoint(snakeHead.x - 2, snakeHead.y));
	}
	void printSnake()
	{
		mvprintw(geom.topleft.y + snakeHead.y, geom.topleft.x + snakeHead.x, "o");
		for(int i = 0; i < (int)snakeParts.size(); i++)
		{
			mvprintw(geom.topleft.y + snakeParts[i].y, geom.topleft.x + snakeParts[i].x, "*");
		}
	}
	void printFruit()
	{
		gotoyx(geom.topleft.y + fruit.y, geom.topleft.x + fruit.x);
		printc('O');
	}
	void printBoard()
	{
		printSnake();
		printFruit();
	}
	void move()
	{
		switch(currentMove)
		{
			case KEY_UP:
				snakeParts.insert(snakeParts.begin(), snakeHead);
				snakeParts.pop_back();
				snakeHead.y = (snakeHead.y - 1) % geom.size.y;
				if(snakeHead.y < 1) snakeHead.y = geom.size.y - 2;
				break;
			case KEY_DOWN:
				snakeParts.insert(snakeParts.begin(), snakeHead);
				snakeParts.pop_back();
				snakeHead.y = (snakeHead.y + 1) % geom.size.y;
				if(snakeHead.y < 1) snakeHead.y = geom.size.y - 2;
				break;
			case KEY_RIGHT:
				snakeParts.insert(snakeParts.begin(), snakeHead);
				snakeParts.pop_back();
				snakeHead.x = (snakeHead.x + 1) % geom.size.x;
				if(snakeHead.x < 1) snakeHead.x = geom.size.x - 2;
				break;
			case KEY_LEFT:
				snakeParts.insert(snakeParts.begin(), snakeHead);
				snakeParts.pop_back();
				snakeHead.x = (snakeHead.x - 1) % geom.size.x;
				if(snakeHead.x < 1) snakeHead.x = geom.size.x - 2;
				break;
			default:
				break;
		}
	}
	void makeLogic()
	{
		move();
	}
	
	
public:
	CSnake(CRect r, char _c = ' ') :
        CFramedWindow(r, _c) {
		srand(time(NULL));
		initSnake();
		spawnFruit();
        paint();
    }
	void paint(){
        CFramedWindow::paint();
		printBoard();
	}
	bool handleEvent(int key){
		switch(gameState)
		{
			case INFO:
				return CFramedWindow::handleEvent(key);
			case PAUSE:
				return CFramedWindow::handleEvent(key);
			case PLAY:
				currentMove = key;
				move();
				return true;
			default:
				return CFramedWindow::handleEvent(key);
		}
	}
	
  
};

#endif
