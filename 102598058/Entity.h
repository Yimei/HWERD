#ifndef _Entity_H_ 
#define _Entity_H_
#include <vector>
#include <string>
#include "Node.h"
using namespace std;
class Entity:public Node{
public:
	Entity();
	virtual ~Entity();
	vector<Component*> getConnections();
	void connectTo(Component*);
	bool canConnectTo(Component*);
	void setConnections(Component*);
private:
	vector<Component*> connectedNodesVector;
};
#endif