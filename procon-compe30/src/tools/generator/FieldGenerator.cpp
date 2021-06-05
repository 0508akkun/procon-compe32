#include "FieldGenerator.h"
using namespace std;

void GenerateSymmetricY_axisField(bool deviationFlag, int height, int width, int minusPercentage, int average, double deviation, vector<vector<int>>& points) {
	random_device rnd;
	mt19937 mt(rnd());
	int sum = 0;
	int point = 0;

	bernoulli_distribution minusPoint((double)((double)minusPercentage + (double)MINUS_CORRECTION) / (double)100);

	for (int i = 0; i < height; i++) {
		points.emplace_back();
		for (int j = 0; j < (width + (2 - 1)) / 2; j++) {
			do {
				if (deviationFlag) {
					normal_distribution<> ndist(average, deviation);
					point = ndist(mt);
				}
				else {
					if (minusPoint(mt)) {
						uniform_int_distribution<> dist(-16, -1);
						point = dist(mt);
					}
					else {
						uniform_int_distribution<> dist(0, 16);
						point = dist(mt);
					}
				}
			} while ((sum + point) < 0 || point < -16 || point > 16);
			sum += point;
			points[i].emplace_back(point);
		}
		for (int j = (width + (2 - 1)) / 2; j < width; j++) {
			points[i].emplace_back(points[i][width - j - 1]);
		}
	}
}

void GenerateSymmetricX_axisField(bool deviationFlag,int height, int width, int minusPercentage, int average, double deviation, vector<vector<int>>& points) {
	random_device rnd;
	mt19937 mt(rnd());
	int sum = 0;
	int point = 0;

	bernoulli_distribution minusPoint((double)((double)minusPercentage + (double)MINUS_CORRECTION) / (double)100);
	
	for (int i = 0; i < (height + (2 - 1)) / 2; i++) {
		points.emplace_back();
		for (int j = 0; j < width; j++) {
			do {
				if (deviationFlag) {
					normal_distribution<> ndist(average, deviation);
					point = ndist(mt);
				}
				else {
					if (minusPoint(mt)) {
						uniform_int_distribution<> dist(-16, -1);
						point = dist(mt);
					}
					else {
						uniform_int_distribution<> dist(0, 16);
						point = dist(mt);
					}
				}
			} while ((sum + point) < 0 || point < -16 || point > 16);
			sum += point;
			points[i].emplace_back(point);
		}
	}
	for (int i = (height + (2 - 1)) / 2; i < height; i++) {
		points.emplace_back();
		for (int j = 0; j < width; j++) {
			points[i].emplace_back(points[height - i - 1][j]);
		}
	}
}

void GenerateSymmetricXY_axisField(bool deviationFlag,int height, int width, int minusPercentage, int average, double deviation, vector<vector<int>>& points) {
	random_device rnd;
	mt19937 mt(rnd());
	int sum = 0;
	int point = 0;

	bernoulli_distribution minusPoint((double)((double)minusPercentage + (double)MINUS_CORRECTION) / (double)100);

	for (int i = 0; i < (height + (2 - 1)) / 2; i++) {
		points.emplace_back();
		for (int j = 0; j < (width + (2 - 1)) / 2; j++) {
			do {
				if (deviationFlag) {
					normal_distribution<> ndist(average, deviation);
					point = ndist(mt);
				}
				else {
					if (minusPoint(mt)) {
						uniform_int_distribution<> dist(-16, -1);
						point = dist(mt);
					}
					else {
						uniform_int_distribution<> dist(0, 16);
						point = dist(mt);
					}
				}
			} while ((sum + point) < 0 || point < -16 || point > 16);
			sum += point;
			points[i].emplace_back(point);
		}
		for (int j = (width + (2 - 1)) / 2; j < width; j++) {
			points[i].emplace_back(points[i][width - j - 1]);
		}
	}
	for (int i = (height + (2 - 1)) / 2; i < height; i++) {
		points.emplace_back();
		for (int j = 0; j < width; j++) {
			points[i].emplace_back(points[height - i - 1][j]);
		}
	}
}

void GenerateSymmetricPointField(bool deviationFlag,int height, int width, int minusPercentage, int average, double deviation, vector<vector<int>>& points) {
	random_device rnd;
	mt19937 mt(rnd());
	int sum = 0;
	int point = 0;

	bernoulli_distribution minusPoint((double)((double)minusPercentage + (double)MINUS_CORRECTION) / (double)100);
	
	for (int i = 0; i < height / 2; i++) {
		points.emplace_back();
		for (int j = 0; j < width; j++) {
			do {
				if (deviationFlag) {
					normal_distribution<> ndist(average, deviation);
					point = ndist(mt);
				}
				else {
					if (minusPoint(mt)) {
						uniform_int_distribution<> dist(-16, -1);
						point = dist(mt);
					}
					else {
						uniform_int_distribution<> dist(0, 16);
						point = dist(mt);
					}
				}
			} while ((sum + point) < 0 || point < -16 || point > 16);
			sum += point;
			points[i].emplace_back(point);
		}
	}
	if (height % 2 == 1) {
		points.emplace_back();
		for (int j = 0; j < (width + (2 - 1)) / 2; j++) {
			do {
				if (deviationFlag) {
					normal_distribution<> ndist(average, deviation);
					point = ndist(mt);
				}
				else {
					if (minusPoint(mt)) {
						uniform_int_distribution<> dist(-16, -1);
						point = dist(mt);
					}
					else {
						uniform_int_distribution<> dist(0, 16);
						point = dist(mt);
					}
				}
			} while ((sum + point) < 0 || point < -16 || point > 16);
			sum += point;
			points[(height + (2 - 1)) / 2 - 1].emplace_back(point);
		}
		for (int j = (width + (2 - 1)) / 2; j < width; j++) {
			points[(height + (2 - 1)) / 2 - 1].emplace_back(points[(height + (2 - 1)) / 2 - 1][width - j - 1]);
		}
	}
	for (int i = (height + (2 - 1)) / 2; i < height; i++) {
		points.emplace_back();
		for (int j = 0; j < width; j++) {
			points[i].emplace_back(points[height - i - 1][width - j - 1]);
		}
	}
}

void AssignSymmetricY_axisAgent(int height, int width, int agentNum, vector<vector<int>>& allyTiled, vector<vector<int>>& enemyTiles, vector<Agent>& allyAgents, vector<Agent>& enemyAgents) {
	random_device rnd;
	mt19937 mt(rnd());
	int agentFaction = 2;
	for (int i = 0; i < height; i++) {
		allyTiled.emplace_back();
		enemyTiles.emplace_back();
		for (int j = 0; j < (width + (2 - 1)) / 2; j++) {
			if (allyAgents.size() + enemyAgents.size() >= agentNum) {
				allyTiled[i].emplace_back(0);
				enemyTiles[i].emplace_back(0);
				continue;
			}
			bernoulli_distribution generateAgent((double)agentNum / ((double)width * (double)height));
			if (generateAgent(mt)) {
				agentFaction = 3 - agentFaction;
				if (agentFaction == 1) {
					allyTiled[i].emplace_back(1);
					enemyTiles[i].emplace_back(0);
					allyAgents.emplace_back(Agent(allyAgents.size(), i, j));
				}
				if (agentFaction == 2) {
					enemyTiles[i].emplace_back(1);
					allyTiled[i].emplace_back(0);
					enemyAgents.emplace_back(Agent(enemyAgents.size() + 10, i, j));
				}
			}
			else {
				allyTiled[i].emplace_back(0);
				enemyTiles[i].emplace_back(0);
			}
		}
		for (int j = (width + (2 - 1)) / 2; j < width; j++) {
			if (allyTiled[i][width - j - 1] == 1) {
				enemyTiles[i].emplace_back(1);
				allyTiled[i].emplace_back(0);
				enemyAgents.emplace_back(Agent(enemyAgents.size() + 10, i, j));
			}
			else if (enemyTiles[i][width - j - 1] == 1) {
				allyTiled[i].emplace_back(1);
				enemyTiles[i].emplace_back(0);
				allyAgents.emplace_back(Agent(allyAgents.size(), i, j));
			}
			else {
				allyTiled[i].emplace_back(0);
				enemyTiles[i].emplace_back(0);
			}
		}
	}
}

void AssignSymmetricX_axisAgent(int height, int width, int agentNum, vector<vector<int>>& allyTiled, vector<vector<int>>& enemyTiles, vector<Agent>& allyAgents, vector<Agent>& enemyAgents) {
	random_device rnd;
	mt19937 mt(rnd());
	int agentFaction = 2;
	for (int i = 0; i < (height + (2 - 1)) / 2; i++) {
		allyTiled.emplace_back();
		enemyTiles.emplace_back();
		for (int j = 0; j < width; j++) {
			if (allyAgents.size() + enemyAgents.size() >= agentNum) {
				allyTiled[i].emplace_back(0);
				enemyTiles[i].emplace_back(0);
				continue;
			}
			bernoulli_distribution generateAgent((double)agentNum / ((double)width * (double)height));
			if (generateAgent(mt)) {
				agentFaction = 3 - agentFaction;
				if (agentFaction == 1) {
					allyTiled[i].emplace_back(1);
					enemyTiles[i].emplace_back(0);
					allyAgents.emplace_back(allyAgents.size(), i, j);
				}
				if (agentFaction == 2) {
					enemyTiles[i].emplace_back(1);
					allyTiled[i].emplace_back(0);
					enemyAgents.emplace_back(enemyAgents.size() + 10, i, j);
				}
			}
			else {
				allyTiled[i].emplace_back(0);
				enemyTiles[i].emplace_back(0);
			}
		}
	}
	for (int i = (height + (2 - 1)) / 2; i < height; i++) {
		allyTiled.emplace_back();
		enemyTiles.emplace_back();
		for (int j = 0; j < width; j++) {
			if (allyTiled[height - i - 1][j] == 1) {
				enemyTiles[i].emplace_back(1);
				allyTiled[i].emplace_back(0);
				enemyAgents.emplace_back(Agent(enemyAgents.size() + 10, i, j));
			}
			else if (enemyTiles[height - i - 1][j] == 1) {
				allyTiled[i].emplace_back(1);
				enemyTiles[i].emplace_back(0);
				allyAgents.emplace_back(Agent(allyAgents.size(), i, j));
			}
			else {
				allyTiled[i].emplace_back(0);
				enemyTiles[i].emplace_back(0);
			}
		}
	}
}

void AssignSymmetricPointAgent(int height, int width, int agentNum, vector<vector<int>>& allyTiled, vector<vector<int>>& enemyTiles, vector<Agent>& allyAgents, vector<Agent>& enemyAgents) {
	random_device rnd;
	mt19937 mt(rnd());
	int agentFaction = 2;

	for (int i = 0; i < height / 2; i++) {
		allyTiled.emplace_back();
		enemyTiles.emplace_back();
		for (int j = 0; j < width; j++) {
			if (allyAgents.size() + enemyAgents.size() >= agentNum) {
				allyTiled[i].emplace_back(0);
				enemyTiles[i].emplace_back(0);
				continue;
			}
			bernoulli_distribution generateAgent((double)agentNum / ((double)width * (double)height / (double)2));
			if (generateAgent(mt)) {
				agentFaction = 3 - agentFaction;
				if (agentFaction == 1) {
					allyTiled[i].emplace_back(1);
					enemyTiles[i].emplace_back(0);
					allyAgents.emplace_back(Agent(allyAgents.size(), i, j));
				}
				if (agentFaction == 2) {
					enemyTiles[i].emplace_back(1);
					allyTiled[i].emplace_back(0);
					enemyAgents.emplace_back(Agent(enemyAgents.size() + 10, i, j));
				}
			}
			else {
				allyTiled[i].emplace_back(0);
				enemyTiles[i].emplace_back(0);
			}
		}
	}
	if (height % 2 == 1) {
		allyTiled.emplace_back();
		enemyTiles.emplace_back();
		for (int j = 0; j < (width + (2 - 1)) / 2; j++) {
			if (allyAgents.size() + enemyAgents.size() >= agentNum) {
				allyTiled[(height + (2 - 1)) / 2 - 1].emplace_back(0);
				enemyTiles[(height + (2 - 1)) / 2 - 1].emplace_back(0);
				continue;
			}
			bernoulli_distribution generateAgent((double)agentNum / ((double)width * (double)height));
			if (generateAgent(mt)) {
				agentFaction = 3 - agentFaction;
				if (agentFaction == 1) {
					allyTiled[(height + (2 - 1)) / 2 - 1].emplace_back(1);
					enemyTiles[(height + (2 - 1)) / 2 - 1].emplace_back(0);
					allyAgents.emplace_back(Agent(allyAgents.size(), (height + (2 - 1)) / 2 - 1, j));
				}
				if (agentFaction == 2) {
					enemyTiles[(height + (2 - 1)) / 2 - 1].emplace_back(1);
					allyTiled[(height + (2 - 1)) / 2 - 1].emplace_back(0);
					enemyAgents.emplace_back(Agent(enemyAgents.size() + 10, (height + (2 - 1)) / 2 - 1, j));
				}
			}
			else {
				allyTiled[(height + (2 - 1)) / 2 - 1].emplace_back(0);
				enemyTiles[(height + (2 - 1)) / 2 - 1].emplace_back(0);
			}
		}
		for (int j = (width + (2 - 1)) / 2; j < width; j++) {
			if (allyTiled[(height + (2 - 1)) / 2 - 1][width - j - 1] == 1) {
				enemyTiles[(height + (2 - 1)) / 2 - 1].emplace_back(1);
				allyTiled[(height + (2 - 1)) / 2 - 1].emplace_back(0);
				enemyAgents.emplace_back(Agent(enemyAgents.size() + 10, (height + (2 - 1)) / 2 - 1, j));
			}
			else if (enemyTiles[(height + (2 - 1)) / 2 - 1][width - j - 1] == 1) {
				allyTiled[(height + (2 - 1)) / 2 - 1].emplace_back(1);
				enemyTiles[(height + (2 - 1)) / 2 - 1].emplace_back(0);
				allyAgents.emplace_back(Agent(allyAgents.size(), (height + (2 - 1)) / 2 - 1, j));
			}
			else {
				allyTiled[(height + (2 - 1)) / 2 - 1].emplace_back(0);
				enemyTiles[(height + (2 - 1)) / 2 - 1].emplace_back(0);
			}
		}
	}
	for (int i = (height + (2 - 1)) / 2; i < height; i++) {
		allyTiled.emplace_back();
		enemyTiles.emplace_back();
		for (int j = 0; j < width; j++) {
			if (allyTiled[height - i - 1][width - j - 1] == 1) {
				enemyTiles[i].emplace_back(1);
				allyTiled[i].emplace_back(0);
				enemyAgents.emplace_back(Agent(enemyAgents.size() + 10, i, j));
			}
			else if (enemyTiles[height - i - 1][width - j - 1] == 1) {
				allyTiled[i].emplace_back(1);
				enemyTiles[i].emplace_back(0);
				allyAgents.emplace_back(Agent(allyAgents.size(), i, j));
			}
			else {
				allyTiled[i].emplace_back(0);
				enemyTiles[i].emplace_back(0);
			}
		}
	}
}

void AssignTiled(int allyTeamID,int enemyTeamID,vector<vector<int>>& allyTiled, vector<vector<int>>& enemyTiles, vector<vector<int>>& tiled)
{
	for (int i = 0; i < allyTiled.size(); i++) {
		tiled.emplace_back();
		for (int j = 0; j < allyTiled[i].size(); j++) {
			if (allyTiled[i][j] == 1) {
				tiled[i].emplace_back(allyTeamID);
			}
			else if (enemyTiles[i][j] == 1) {
				tiled[i].emplace_back(enemyTeamID);
			}
			else {
				tiled[i].emplace_back(0);
			}
		}
	}
}
