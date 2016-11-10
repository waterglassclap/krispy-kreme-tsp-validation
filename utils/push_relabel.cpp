#include "push_relabel.h"

using namespace std;

PushRelabel::PushRelabel(int s, int t, int nodeNum, float** arr) {
	int i,j;
	
	source = s;
	sink = t;
	nodeNumber = nodeNum;

	height = new int[nodeNumber];
	excess = new float[nodeNumber];

	for(i = 0;i < nodeNumber; i++) {
		height[i] = 0;
		excess[i] = 0.0f;
	}

	//the height function of the source should be changed first
	height[s] = nodeNumber;

	cMatrix = new float*[nodeNumber];
	fMatrix = new float*[nodeNumber];
	rMatrix = new float*[nodeNumber];

	for(i = 0; i < nodeNumber; i++)	{
		rMatrix[i] = new float[nodeNumber];
		cMatrix[i] = new float[nodeNumber];
		fMatrix[i] = new float[nodeNumber];
		for(j = 0; j < nodeNumber; j++)			{
			cMatrix[i][j] = arr[i][j];
			fMatrix[i][j] = 0.0f;
			rMatrix[i][j] = 0.0f;
		}
	}
	//all the outgoing links from source should be satuated first
	for(i = 0; i < nodeNumber; i++) {
		if(cMatrix[s][i] != 0.0f) {
			fMatrix[s][i] = cMatrix[s][i];
			fMatrix[i][s] =  -1.0f * cMatrix[s][i];
			excess[i] = cMatrix[s][i];
			//at first, the excess of source node should be negative. 
			excess[s] -= cMatrix[s][i];
			if (i != t) {
				//the sink cannot puch flow back!
				excessNode.push_back(i);
			}
		}
	}
  
  	//compose the residual network.
	residualBuilder();
	//printInfo();
}


//check whether the excess node i can push some flow out
int PushRelabel::checkNode(int i) {
	int j;
	//first, i should have some excess flow
	if(excess[i] <= 0.0f) {
		return -1;
	}
	for(j = 0; j < nodeNumber; j++)	{
		//fulfill the height condition and also have "extra capacity" in residual network
		if(rMatrix[i][j] > 0.0f && height[i] == height[j] + 1) {			
			return j;
		}
	}
	return -1;
}


//push excess flow from node i to node j
bool PushRelabel::push(int a, int b) {
	float delta;
	if (height[a] != height[b] + 1) {
		cout<<"b is not near to a. terminate push."<<endl;
		return false;
	}

	if(excess[a] <= rMatrix[a][b]) {
		delta = excess[a];
	} else {
		//identify the amount we can push
		delta = rMatrix[a][b];
	}

	rMatrix[b][a] += delta;
	rMatrix[a][b] -= delta;

	if(cMatrix[a][b] > 0.0f) {
		fMatrix[a][b] += delta;
		fMatrix[b][a] = -1.0f * fMatrix[a][b];
	} else	{
		fMatrix[b][a] -= delta;
		fMatrix[a][b] = -1.0f * fMatrix[b][a];
	}
	excess[a] -= delta;
	excess[b] += delta;

	//sometimes j can be the source node,so it's better we check here, and sink cannot push flow back
	if(excess[b] > 0.0f && b != sink) {
		excessNode.push_back(b);
	}
	//if i is still excess, push it back into the queue,and we wish to satuarate
	if(excess[a] > 0.0f) {
		excessNode.push_front(a);
	}

	//printInfo();
	return true;
}


bool PushRelabel::relable(int a) {
	//cout<<"now we do a relable for node "<<a<<endl;
	if(excess[a] <= 0.0f) {
		cout << "no excess for " << a << ". terminate relable" << endl;
		return false;
	}

	int i;
	int temp = height[0];
	for(i = 0; i < nodeNumber; i++) {
		if(rMatrix[a][i] > 0.0f && height[i] < temp) {
			temp = height[i];
		}
	}

	height[a] = temp + 1;

	excessNode.push_back(a);

	//printInfo();

	return true;
}


void PushRelabel::mineMaxFlow() {
	int i, j;
	float current, target;
	bool flag = true;

	while(excessNode.size() != 0 && flag) {
		current = excessNode.at(0);
		excessNode.pop_front();
		target = checkNode(current);
		if(checkNode(current) < 0) {
			flag = relable(current);
		} else {
			flag = push(current, target);
		}
	}


	cout<<"the height function of each node is like: ";
	for(i = 0; i < nodeNumber; i++) {
		cout<<height[i]<<" ";
	}
	cout<<endl;

	cout<<"the final flow is like:"<<endl;
	for(i = 0;i<nodeNumber;i++) {
		for(j = 0;j<nodeNumber;j++) {
			cout<<fMatrix[i][j]<<" ";
		}
		cout<<endl;
	}
}


MinCutInfo PushRelabel::getMinCutInfo() {

	mineMaxFlow();

	MinCutInfo minCutInfo;
	minCutInfo.size = 0;
	minCutInfo.totalFlow = 0.0f;
	minCutInfo.edgeInfos = new EdgeInfo[nodeNumber];

	// height 별로 봐서 빈 하이트가 있으면, 그거 기준으로 위 아래 그래프 자르고 그 사이에 있는 edge들을 리턴함
	// get no presence height
	int noPresenceHeight = -1;
	int maxHeight = *max_element(height, height + nodeNumber);
	for(int i = 0 ; i < maxHeight; i++) {
		// if given height not exists
		if (find(height, height + nodeNumber, i) == height + nodeNumber) {
			noPresenceHeight = i;
			break;
		}
	}

	if (noPresenceHeight < 0) {
		throw runtime_error("there is no height that no nodes included. terminate.");
	}

	// get min cut info
	for (int i = 0; i < nodeNumber; i++) {
		for (int j = 0; j < nodeNumber; j++) {
			if (height[i] > noPresenceHeight && height[j] < noPresenceHeight
			    && height[i] > height[j] && cMatrix[i][j] > 0.0f) {
				minCutInfo.edgeInfos[minCutInfo.size].source = i;
				minCutInfo.edgeInfos[minCutInfo.size].sink = j;
				minCutInfo.edgeInfos[minCutInfo.size].weight = cMatrix[i][j];
				minCutInfo.totalFlow += cMatrix[i][j];
				minCutInfo.size++;
			}
		}
	}

	return minCutInfo;
}


void PushRelabel::residualBuilder() {
	int i,j;
	for(i = 0; i < nodeNumber; i++) {
		for(j = 0;j < nodeNumber; j++) {
			if(fMatrix[i][j] >= 0.0f) {
				rMatrix[i][j] = cMatrix[i][j] - fMatrix[i][j];
			} else	{
				rMatrix[i][j] = fMatrix[j][i];
			}
		}
	}
}


void PushRelabel::printInfo() {
	int i,j;
	cout<<"the capacity situation of this graph is:"<<endl;
	for (i = 0; i < nodeNumber; i++) {
	   for (j = 0; j < nodeNumber; j++) {
	      cout<<cMatrix[i][j]<<" ";
	   }
	   cout<<endl;
	}

	cout<<"the flow situation of this graph is: "<<endl;
	for(i = 0;i<nodeNumber;i++) {
		for(j = 0;j<nodeNumber;j++) {
			cout<<fMatrix[i][j]<<" ";
		}
		cout<<endl;
	}

	cout<<"the residual network looks like: "<<endl;
	for(i = 0; i < nodeNumber; i++) {
		for(j = 0; j < nodeNumber; j++) {
			cout << rMatrix[i][j] << " ";
		}
		cout<<endl;
	}

	cout<<"the height function of each node is like: ";
	for(i = 0; i < nodeNumber; i++)
		cout<<height[i]<<" ";
	cout<<endl;
}
