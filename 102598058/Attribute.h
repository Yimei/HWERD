#ifndef _Attribute_H_ 
#define _Attribute_H_
#include <gtest/gtest_prod.h>
#include <string>
#include "Node.h"
#include "Connector.h"
using namespace std;
class Attribute:public Node{
	friend class AttributeUnitTest;
	
	FRIEND_TEST(AttributeUnitTest, testCanConnectTo);
public:
	Attribute();
	virtual ~Attribute();
	vector<Component*> getConnections();
	void connectTo(Component* component);
	bool canConnectTo(Component* component);
	/*void pushConnectedEntity(Component* component);*/
private:
	Connector* connector;
	vector<Component*> connectedEntity;
	
};
#endif