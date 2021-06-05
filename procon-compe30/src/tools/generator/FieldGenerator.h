#pragma once
#include <vector>
#include <random>
# include "../../search/FieldReader.h"

#define MINUS_CORRECTION 5

using namespace std;

void GenerateSymmetricY_axisField(bool deviationFlag,int height, int width, int minusPercentage, int average, double deviation, vector<vector<int>>& points);
void GenerateSymmetricX_axisField(bool deviationFlag,int height, int width, int minusPercentage, int average, double deviation, vector<vector<int>>& points);
void GenerateSymmetricXY_axisField(bool deviationFlag,int height, int width, int minusPercentage, int average, double deviation, vector<vector<int>>& points);
void GenerateSymmetricPointField(bool deviationFlag,int height, int width, int minusPercentage, int average, double deviation, vector<vector<int>>& points);
void AssignSymmetricY_axisAgent(int height, int width, int agentNum, vector<vector<int>>& allyTiles, vector<vector<int>>& enemyTiles, vector<Agent>& allyAgents, vector<Agent>& enemyAgents);
void AssignSymmetricX_axisAgent(int height, int width, int agentNum, vector<vector<int>>& allyTiles, vector<vector<int>>& enemyTiles, vector<Agent>& allyAgents, vector<Agent>& enemyAgents);
void AssignSymmetricPointAgent(int height, int width, int agentNum, vector<vector<int>>& allyTiles, vector<vector<int>>& enemyTiles, vector<Agent>& allyAgents, vector<Agent>& enemyAgents);
void AssignTiled(int allyTeamID, int enemyTeamID, vector<vector<int>>& allyTiles, vector<vector<int>>& enemyTiles, vector<vector<int>>& tiled);