#include <iostream>
#include <queue>
#include <cstring>
using namespace std;

const int N = 5;
int K, M;
int hMap[N][N] = { 0 };
int dir[4][2] = { {-1,0},{1,0},{0,1},{0,-1} };
int visited[N][N] = { 0 };
queue<int> uWater;

struct possible {
	int value;
	int degree;
	int r, c;

	bool operator < (const possible& a) const {
		if (value == a.value) {
			if (degree == a.degree) {
				if (c == a.c) {
					return r > a.r;
				}
				return c > a.c;
			}
			return degree > a.degree;
		}
		return value < a.value;
	}
};
struct position {
	int r, c;

	bool operator < (const position& a) const {
		if (c == a.c) return r < a.r;
		return c > a.c;
	}

};

priority_queue<possible> cases;
priority_queue<position> uWaterIget;

void rotateMap(int r, int c) {
	r--;
	c--;
	int tmpMap[N][N];
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
		{
			tmpMap[i][j] = hMap[i][j];
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			hMap[r + i][c + j] = tmpMap[r + 2 - j][c + i];
		}
	}
}

int findUWater(int r, int c) {
	queue<pair<int, int>> que;
	queue<pair<int, int>> visitPos;
	pair<int, int> location;
	que.push({ r, c });
	int count = 0;
	visited[r][c] = 1;

	while (!que.empty()) {
		location = que.front();
		visitPos.push({ location.first,location.second });
		que.pop();
		count++;
		for (int i = 0; i < 4; i++)
		{
			int nr = location.first + dir[i][0];
			int nc = location.second + dir[i][1];

			if (nr < 0 || nc < 0 || nr >= N || nc >= N) continue;
			if (visited[nr][nc]) continue;
			if (hMap[nr][nc] == hMap[location.first][location.second]) {
				que.push({ nr, nc });
				visited[nr][nc] = 1;
			}
		}
	}
	if (count >= 3) {
		while (!visitPos.empty()) {
			uWaterIget.push({ visitPos.front().first, visitPos.front().second });
			visitPos.pop();
		}
		return count;
	}
	return 0;
}

int getUwater() {
	int total = 0;
	memset(visited, 0, sizeof(visited));
	for (int r = 0; r < N; r++)
	{
		for (int c = 0; c < N; c++)
		{
			if (visited[r][c]) continue;
			total += findUWater(r, c);
		}
	}

	position tmp;
	while (!uWaterIget.empty() && !uWater.empty()) {
		tmp = uWaterIget.top();
		uWaterIget.pop();

		hMap[tmp.r][tmp.c] = uWater.front();
		uWater.pop();
	}
	return total;
}

int explore() {
	for (int i = 1; i < N - 1; i++)
	{
		for (int j = 1; j < N - 1; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				rotateMap(i, j);
				if (k == 3) continue;

				possible tmpCase;
				tmpCase.value = 0;
				memset(visited, 0, sizeof(visited));
				for (int r = 0; r < N; r++)
				{
					for (int c = 0; c < N; c++)
					{
						if (visited[r][c]) continue;
						tmpCase.value += findUWater(r, c);
					}
				}
				tmpCase.degree = k + 1;
				tmpCase.r = i;
				tmpCase.c = j;

				cases.push(tmpCase);
				while (!uWaterIget.empty()) uWaterIget.pop();
			}
		}
	}

	possible curCase = cases.top();
	if (curCase.value == 0) return 0;
	while (!cases.empty()) cases.pop();

	for (int i = 0; i < curCase.degree; i++)
	{
		rotateMap(curCase.r, curCase.c);
	}

	int total = 0;
	int ans;
	while (1) {
		ans = getUwater();
		if (ans == 0) break;
		total += ans;
	}

	return total;
}

int main() {
	cin >> K >> M;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cin >> hMap[i][j];
		}
	}
	int tmp;
	for (int i = 0; i < M; i++)
	{
		cin >> tmp;
		uWater.push(tmp);
	}

	for (int i = 0; i < K; i++)
	{
		int ans = explore();
		if (ans != 0) {
			cout << ans << " ";
		}
	}

	return 0;
}