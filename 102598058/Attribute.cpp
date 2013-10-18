#include <string>
#include <iostream>
#include "Attribute.h"
#define E "E"
using namespace std;
Attribute::Attribute():Node()
{	
	for (int i = 0; i < connectedEntity.size();i++)
		connectedEntity.push_back(NULL);
}
void Attribute::connectTo(Component* component)
{
}
bool Attribute::canConnectTo(Component* component){
	if ((component->getType() != E) )//���|������connector���D
	{
		return false;
	}
	else if(connectedEntity.size() > 0)
	{
		return false;
	}
	else
	{
		connectedEntity.push_back(component);
	}
}
vector<Component*> Attribute::getConnections()
{
	return connectedEntity;
}
Attribute::~Attribute()
{
}