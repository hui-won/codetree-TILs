#include <iostream>
#include <queue>
#include <cstring>
using namespace std;

#define ELF_MAX 1001
#define MAP_MAX 71

enum {
	North, East, South, West, End
};

int R, C, K;
int golemStart[ELF_MAX][2] = { 0 };
int forest[MAP_MAX][MAP_MAX] = { 0 };
int dir[4][2] = { {-1,0},{0,1},{1,0},{0,-1} };
int rightClock[4] = { East,South, West,North };
int reverseClock[4] = { West, North, East, South };
int visited[MAP_MAX][MAP_MAX] = { 0 };

int checkDir(int cR, int cC) {
	//1) 남쪽으로 갈 수 있는가?
	if (cR + 2 >= R) return End;

	if (!forest[cR + 2][cC] && !forest[cR + 1][cC - 1] && !forest[cR + 1][cC + 1]) {
		return South;
	}
	//2) 서쪽으로 갈 수 있는가?
	if (cC - 2 >= 0) {
		if (!forest[cR - 1][cC - 1] && !forest[cR][cC - 2] && !forest[cR + 1][cC - 1] && !forest[cR + 1][cC - 2] && !forest[cR + 2][cC - 1])
			return West;
	}
	//3) 동쪽으로 갈 수 있는가?
	if (cC + 2 < C) {
		if (!forest[cR - 1][cC + 1] && !forest[cR][cC + 2] && !forest[cR + 1][cC + 1] && !forest[cR + 1][cC + 2] && !forest[cR + 2][cC + 1])
			return East;
	}
	return End;
}
int nextExit(int curExit, int nextDir) {
	if (nextDir == South)
		return curExit;
	if (nextDir == West)
		return reverseClock[curExit];
	if (nextDir == East)
		return rightClock[curExit];
}
void fillForest(int cR, int cC, int exit, int idx) {
	forest[cR][cC] = idx;
	for (int i = 0; i < 4; i++)
	{
		if (i == exit) {
			forest[cR + dir[i][0]][cC + dir[i][1]] = -1*idx;
		}
		else {
			forest[cR + dir[i][0]][cC + dir[i][1]] = idx;
		}
	}
}

int moveElf(int cR, int cC) {
	int curIdx;
	int rMax = 0;
	memset(visited, 0, sizeof(visited));
	queue<pair<int, int>> que;

	que.push({ cR, cC });
	while (!que.empty()) {
		pair<int, int> location = que.front();
		curIdx = forest[location.first][location.second];
		que.pop();

		if (location.first > rMax) {
			rMax = location.first;
		}
		visited[location.first][location.second] = 1;
		for (int i = 0; i < 4; i++)
		{
			int nR = location.first + dir[i][0];
			int nC = location.second + dir[i][1];

			if (nR < 0 || nC < 0 || nR >= R || nC >= C) continue;
			if (visited[nR][nC]) continue;
			if (curIdx < 0) {
				if (forest[nR][nC] != 0) que.push({ nR,nC });
			}
			else {
				if (forest[nR][nC] == curIdx || forest[nR][nC] == -1*curIdx) que.push({ nR,nC });
			}
		}
	}

	return rMax;
}

int  moveGolem(int cC, int exit, int idx) {
	int centerR = -2;
	int centerC = cC;
	int curExit = exit;
	int nextDir = checkDir(centerR, centerC);

	while (nextDir != End) {	
		curExit = nextExit(curExit, nextDir);

		centerR = centerR + dir[nextDir][0];
		centerC = centerC + dir[nextDir][1];
		if (nextDir != South) {
			centerR = centerR + dir[South][0];
			centerC = centerC + dir[South][1];
		}
		nextDir = checkDir(centerR, centerC);
	}

	if (centerR < 1) {
		return 0;//moveGolem 호출 시 false면 forest 초기화 진행, moveElf 호출X
	}

	fillForest(centerR, centerC, curExit, idx);
	return moveElf(centerR, centerC);
}

int main() {
	cin >> R >> C >> K;

	for (int i = 1; i <= K; i++)
	{
		cin >> golemStart[i][0] >> golemStart[i][1];
		golemStart[i][0]--;//(0,0)에서 시작하기 위해 
	}

	int answer = 0;


	for (int i = 1; i <= K; i++)
	{
		int tmpAns = moveGolem(golemStart[i][0], golemStart[i][1], i);

		if (tmpAns == 0) {
			memset(forest, 0, sizeof(forest));
		}
		else {
			answer += (tmpAns+1);
		}
	}
	
	cout << answer;

	return 0;
}