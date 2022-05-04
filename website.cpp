#include "website.h"
website::website() {
	impressions = 1;
	clicks = 0;
	outgoing_links = 0;
	keyWords = "";
}
website::website(int num, string name) {
	webNum = num;
	webName = name;
	keyWords = "";
	impressions = 1;
	clicks = 0;
	outgoing_links = 0;
}
void website::setWebNum(int Num) {
	webNum = Num;
}
void website::setImpressions(int imp) {
	impressions = imp;
}
void website::addKeyWord(string s) {
	keyWords += s;
}
void website::calculatePR(map<int, website>& websites, vector<vector<int>>& TransposedVector) {
	int iterations = 1;
	double n = websites.size(); // number of websites.
	prevPR.resize(n + 1, 1/(n)); // to make it 1-indexed;
	curPR.resize(n + 1);
	while (iterations < maxIterations)
	{
		for (int i = 1; i< n+1l;i++ )
		{
				curPR[i] = 0;
				for(auto j : TransposedVector[i])
				{
					curPR[i] += prevPR[j] / double(websites.at(j).getOutLinks());
				}
			curPR[i] *= dF;
			curPR[i] += (1 - dF) / n;
		}
		if (isStable(n)) break;
		for (int i = 0; i < n + 1; i++) prevPR[i] = curPR[i];
		iterations++;
	}
	// Normalization:
	double sum = 0;
	for (int i = 1; i < n + 1; i++) {
		sum += curPR[i];
	}
	for (int i = 1; i < n + 1; i++) {
		curPR[i] /= sum;
	}
	// set PR to each website:
	for (int i =1; i<n+1; i++)
	{
		websites[i].setPR(curPR[i]);
	}
}

void website::setPR(double PR) 
{
	this->PR = PR;
}

void website::setCTR(int CTR) {
	this->CTR = CTR;
}
void website::setClicks(int clicks) {
	this->clicks = clicks;
}
void website::updateOutLinks() {// increment outgoing_links by 1.
	outgoing_links++;
}
void website::updateClicks() {
	clicks++;
}
void website::updateImpressions() {
	impressions++;
}
void website::setName(string s) {
	webName = s;
}

int  website::getPR() {
	return PR;
}
int website::getImpressions()
{
	return impressions;
}
int website::getClicks()
{
	return clicks;
}
int  website::getOutLinks() {
	return outgoing_links;
}
int  website::getWebNum() {
	return webNum;
}
string website::getKeyWords() {
	return keyWords;
}
string website::getName() {
	return webName;
}
double website::getCTR() {
	return double(clicks) / double(impressions);
}
double website::getScore() {
	double frac = (0.1 * impressions) / (1.0 + 0.1 * impressions);

	return 0.4 * PR + ((1 - frac) * PR + frac * getCTR()) * 0.6;
}
bool website::isStable(int n) {
	for (int i = 0; i < n + 1; i++) {
		if (abs(curPR[i] - prevPR[i]) > 0.001)
			return false;
	}
	return true;
}


