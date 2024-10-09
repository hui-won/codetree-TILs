#include <iostream>
#include <queue>
#include <cstring>
using namespace std;

#define MAP_MAX 21
#define MOVE_MAX 1001;

enum ViewDir{
	UP, RIGHT, DOWN, LEFT, STOP
};
enum DiceDir {
	GO, TRIGHT, TLEFT, BACK
};

int n, m;
int board[MAP_MAX][MAP_MAX] = { 0 };
int visited[MAP_MAX][MAP_MAX] = { 0 };
int dir[4][2] = { {-1,0},{0,1},{1,0},{0,-1} };
int dirClock[4][2] = { {ViewDir::RIGHT,ViewDir::LEFT}, {ViewDir::DOWN,ViewDir::UP}, {ViewDir::LEFT,ViewDir::RIGHT}, {ViewDir::UP,ViewDir::DOWN} };

struct Dice {
	int r, c;
	int diceDir;
	int viewDir;
	int bottom;
	int front;
	int right;
};

int answer = 0;
Dice diceInfo;

bool isValidBoundary(int r,int c){
	if (r < 0 || c < 0 || r >= n || c >= n) return false;
	return true;
}
void rollTheDice() {
	int bottom = diceInfo.bottom;
	int front = diceInfo.front;
	int right = diceInfo.right;

	if (diceInfo.diceDir == DiceDir::GO) {
		bottom = diceInfo.front;
	}
	else if (diceInfo.diceDir == DiceDir::TRIGHT) {
		bottom = diceInfo.right;
		right = 7 - diceInfo.front;
	}
	else if (diceInfo.diceDir == DiceDir::TLEFT) {
		bottom = 7 - diceInfo.right;
		right = diceInfo.front;
	}
	else if (diceInfo.diceDir == DiceDir::BACK) {
		bottom = 7 - diceInfo.front;
		right = 7 - diceInfo.right;
	}
	front = 7 - diceInfo.bottom;

	diceInfo.bottom = bottom;
	diceInfo.front = front;
	diceInfo.right = right;
}

void getScore(int r, int c) {
	queue<pair<int, int>> que;
	memset(visited, 0, sizeof(visited));

	que.push({ r,c });
	visited[r][c] = 1;

	while (!que.empty()) {
		answer += board[r][c];
		pair<int, int> location = que.front();
		que.pop();
		for (int i = 0; i < 4; i++)
		{
			int nr = location.first + dir[i][0];
			int nc = location.second + dir[i][1];

			if (!isValidBoundary(nr, nc)) continue;
			if (visited[nr][nc]) continue;
			if (board[nr][nc] == board[r][c]) {
				visited[nr][nc] = 1;
				que.push({ nr,nc });
			}
		}
	}
}

void decideDirection(int boardNum) {
	int tmpViewDir = ViewDir::STOP;
	if (diceInfo.bottom > boardNum) {
		tmpViewDir = dirClock[diceInfo.viewDir][0];
		int nr = diceInfo.r + dir[tmpViewDir][0];
		int nc = diceInfo.c + dir[tmpViewDir][1];

		if (isValidBoundary(nr, nc)) {
			diceInfo.diceDir = DiceDir::TRIGHT;
			diceInfo.viewDir = tmpViewDir;
		}
		else {
			diceInfo.diceDir = DiceDir::TLEFT;
			diceInfo.viewDir = (tmpViewDir + 2) % 4;
		}
	}
	if (diceInfo.bottom < boardNum) {
		tmpViewDir = dirClock[diceInfo.viewDir][1];
		int nr = diceInfo.r + dir[tmpViewDir][0];
		int nc = diceInfo.c + dir[tmpViewDir][1];

		if (isValidBoundary(nr, nc)) {
			diceInfo.diceDir = DiceDir::TLEFT;
			diceInfo.viewDir = tmpViewDir;
		}
		else {
			diceInfo.diceDir = DiceDir::TRIGHT;
			diceInfo.viewDir = (tmpViewDir + 2) % 4;
		}
	}
	if (diceInfo.bottom == boardNum) {
		tmpViewDir = diceInfo.viewDir;
		int nr = diceInfo.r + dir[tmpViewDir][0];
		int nc = diceInfo.c + dir[tmpViewDir][1];
		if (isValidBoundary(nr, nc)) {
			diceInfo.diceDir = DiceDir::GO;
			diceInfo.viewDir = tmpViewDir;
		}
		else {
			diceInfo.diceDir = DiceDir::BACK;
			diceInfo.viewDir = (tmpViewDir + 2) % 4;
		}

	}
}

void moveDice() {
	int nextR = diceInfo.r + dir[diceInfo.viewDir][0];
	int nextC = diceInfo.c + dir[diceInfo.viewDir][1];

	if (!isValidBoundary(nextR,nextC)) {
		cout << "here!!";
	}

	diceInfo.r = nextR;
	diceInfo.c = nextC;

	getScore(nextR, nextC);

	rollTheDice();
	decideDirection(board[nextR][nextC]);
}

int main() {
	cin >> n >> m;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> board[i][j];
		}
	}

	diceInfo.r = 0;
	diceInfo.c = 0;
	diceInfo.viewDir = ViewDir::RIGHT;
	diceInfo.diceDir = DiceDir::GO;
	diceInfo.bottom = 6;
	diceInfo.front = 3;
	diceInfo.right = 2;

	for (int i = 0; i < m; i++)
	{
		moveDice();
	}

	cout << answer;
	return 0;
}