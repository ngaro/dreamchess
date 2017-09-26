/*  DreamChess
**
**  DreamChess is the legal property of its developers, whose names are too
**  numerous to list here. Please refer to the COPYRIGHT file distributed
**  with this source distribution.
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DREAMCHESS_DREAMCHESS_H
#define DREAMCHESS_DREAMCHESS_H

class GameConfig;
class History; 
class TitleScene;
class Scene;

#define PLAYER_UI 0
#define PLAYER_ENGINE 1

class LaunchArguments {
public:
	LaunchArguments(int c, char **v) {
		argc = c;
		argv = v;
	}
	
    int argc;
    char **argv;
};

class DreamChess {
public:
	DreamChess() {_currentScene = nullptr; }
	~DreamChess() { }

	void go();
	void gameLoop();
	int init(LaunchArguments *arg);
private:
	TitleScene *_titleScene; 
	Scene *_currentScene;
};

extern DreamChess *g_DreamChess;

#endif
