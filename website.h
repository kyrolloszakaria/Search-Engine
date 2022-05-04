#pragma once
#include <bits/stdc++.h>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept> // std::out_of_range
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;
class website
{
private:
    string webName;
    int webNum; // numeric value for the website.
    string keyWords;
    double PR;
    int impressions;
    double CTR;
    int clicks;
    int outgoing_links;
    int score;
    // for pageRank calculations:
    const double dF = 0.85; // damping factor
    const double maxError = 0.001;
    const int maxIterations = 10;
    double MIN = 999999.0; // for normalization.
    double MAX = 0.0;
    vector<double> prevPR;
    vector<double> curPR;

public:
    website(); // default constructor;
    website(int num, string name);
    void setWebNum(int Num);
    void setImpressions(int imp);
    void addKeyWord(string s);
    void static updatePR(map<int, website>& websites, multimap<int, int>& TransposedwebGraph);
    void setPR(double PR);
    void setCTR(int CTR);
    void setName(string s);
    void setClicks(int clicks);
    void updateOutLinks();    // increment outgoing_links by 1.
    void updateClicks();      // increment clicks by 1.
    void updateImpressions(); // increment impressions by 1.
    void calculatePR(map<int, website>& websites, vector<vector<int>>& TransposedVector);

    int getPR();
    int getImpressions();
    int getClicks();
    int getWebNum();
    string getName();
    int getOutLinks();
    string getKeyWords();
    double getCTR();
    double getScore();
    bool isStable(int n);
};
