#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "cpoint.h"
#include "screen.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <unistd.h>

enum state { PLAY, PAUSE, INFO };

class CSnake:public CFramedWindow
{
private:
	vector<CPoint> snakeParts;
	CPoint snakeHead;
	CPoint fruit;
	enum state gameState = INFO;
	int theMove = KEY_RIGHT;
	int currentMove = KEY_RIGHT;
	int lastMove = KEY_RIGHT;
	int level = 1;
	int tick = 0;
	int isDead = 0;
	void restart()
	{
		isDead = 0;
		level = 1;
		tick = 0;
		snakeParts.clear();
		initSnake();
		spawnFruit();
		theMove = KEY_RIGHT;
		currentMove = KEY_RIGHT;
		lastMove = KEY_RIGHT;
		gameState = PLAY;
	}
	void gameOver()
	{
		gameState = INFO;
		isDead = 1;
	}
	void checkCollision()
	{
		for (int i = 0; i < (int)snakeParts.size(); i++)
		{
			if (snakeHead.x == snakeParts[i].x && snakeHead.y == snakeParts[i].y)
			{
				gameOver();
				return;
			}
		}
	}
	void printInfo()
	{
		if(!isDead)
		{
			mvprintw(geom.topleft.y + 4, geom.topleft.x + 2, "Press 'p' to play");
			mvprintw(geom.topleft.y + 5, geom.topleft.x + 2, "Press 'h' to show/hide info");
			mvprintw(geom.topleft.y + 6, geom.topleft.x + 2, "Press 'r' to restart");
		}
		else
		{
			mvprintw(geom.topleft.y + 4, geom.topleft.x + 2, "GAME OVER. SCORE: %d", level);
			mvprintw(geom.topleft.y + 5, geom.topleft.x + 2, "Press 'r' to restart");
		}
	}
	void printLevel()
	{
		mvprintw(geom.topleft.y - 2, geom.topleft.x, "|LEVEL: %2d|", level);
		mvprintw(geom.topleft.y - 1, geom.topleft.x, "|_________|");
	}
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
	void addPart()
	{
		snakeParts.push_back(CPoint(-1, -1));
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
		printLevel();
	}
	void eatFruit()
	{
		spawnFruit();
		addPart();
		level += 1;
	}
	int validateMove()
	{
		switch(currentMove)
		{
			case KEY_UP:
				if(lastMove == KEY_DOWN) 
				{
					return 1;
				}
				break;
			case KEY_DOWN:
				if(lastMove == KEY_UP) 
				{
					return 1;
				}
				break;
			case KEY_RIGHT:
				if(lastMove == KEY_LEFT) 
				{
					return 1;
				}
				break;
			case KEY_LEFT:
				if(lastMove == KEY_RIGHT) 
				{
					return 1;
				}
				break;
			default:
				return 0;
		}
		return 0;
	}
	void move()
	{
		int thisMove = validateMove() ? lastMove : currentMove;
		switch(thisMove)
		{
			case KEY_UP:
				snakeParts.insert(snakeParts.begin(), snakeHead);
				snakeParts.pop_back();
				snakeHead.y = (snakeHead.y - 1);
				if(snakeHead.y < 1) snakeHead.y = geom.size.y - 2;
				break;
			case KEY_DOWN:
				snakeParts.insert(snakeParts.begin(), snakeHead);
				snakeParts.pop_back();
				snakeHead.y = (snakeHead.y) % (geom.size.y - 2) + 1;
				break;
			case KEY_RIGHT:
				snakeParts.insert(snakeParts.begin(), snakeHead);
				snakeParts.pop_back();
				snakeHead.x = (snakeHead.x) % (geom.size.x - 2) + 1;
				break;
			case KEY_LEFT:
				snakeParts.insert(snakeParts.begin(), snakeHead);
				snakeParts.pop_back();
				snakeHead.x = (snakeHead.x - 1);
				if(snakeHead.x < 1) snakeHead.x = geom.size.x - 2;
				break;
			default:
				break;
		}
		if(validateMove())
		{
			currentMove = lastMove;
		}
		if(snakeHead.y == fruit.y && snakeHead.x == fruit.x)
		{
			eatFruit();
		}
	}
	void makeLogic()
	{
		move();
		checkCollision();
	}
	
	
public:
	CSnake(CRect r, char _c = ' ') :
        CFramedWindow(r, _c) {
		srand(time(NULL));
		initSnake();
		spawnFruit();
        paint();
    }
	void paint()
	{
        CFramedWindow::paint();
        if (gameState == INFO) printInfo();
		else printBoard();
	}
	bool handleEvent(int key){
		if (key == 'r')
		{
			restart();
			return true;
		}
		if(!isDead)
		{
			if ((key == 'h' || key == 'p') && gameState != PLAY) gameState = PLAY;
			else if (key == 'h') 
			{
				gameState = INFO;
				printInfo();
			}
			else if (key == 'p') gameState = PAUSE;
			else if (key == KEY_UP || key == KEY_DOWN || key == KEY_LEFT || key == KEY_RIGHT) 
			{
				theMove = key;
			}
		}
		if (gameState == PLAY && tick < (10 - (level / 3) )) tick++;
		else 
		{
			if(gameState == PLAY)
			{
				lastMove = currentMove;
				currentMove = theMove;
			}
			tick = 0;
			switch(gameState)
			{
				case INFO:
					return true;
				case PAUSE:
					return CFramedWindow::handleEvent(key);
				case PLAY:
					makeLogic();
					return true;
				default:
					return CFramedWindow::handleEvent(key);
			}
		}
		return true;
		
	}
	
  
};

#endif
