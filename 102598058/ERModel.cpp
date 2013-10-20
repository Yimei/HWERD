#include "ERModel.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <iomanip>
#include "AddComponentCmd.h"
#include "DeleteComponentCmd.h"
#include "ConnectComponentsCmd.h"
#define A "A"
#define E "E"
#define R "R"
#define C "C"
#define COMMA ","
#define EMPTY_STRING ""
#define BLANK " "
#define DOUBLE_BLANK "  "
#define TWO 2
#define COMMABLANK ", "
#define ZERO_STRING "0"
#define NONAME "No name"
#define ATTRIBUTE "Attribute"
#define ENTITY "Entity"
#define RELATIONSHIP "Relationship"
using namespace std;
ERModel::ERModel(){
	_entityTemp=NULL;
	_primaryKey = EMPTY_STRING;
	thisKey=0;
	id=0;
}
ERModel::~ERModel()
{
	
}
void ERModel::undo()//�_��
{
	commandManager.undo();
	cout << commandManager.getHint() <<endl;
}
void ERModel::redo()//���@
{
	commandManager.redo();
	cout << commandManager.getHint() <<endl;
}
void ERModel::addNodePresentation(string type)//addNode command
{
	commandManager.execute(new AddComponentCmd(this, type));
}
void ERModel::deleteComponentPresentation(int deleteComponent)//delete Component command
{
	commandManager.execute(new DeleteComponentCmd(this,deleteComponent));
}
void ERModel::connectComponentPresentation()//connect component command
{
	commandManager.execute(new ConnectComponentsCmd(this,C));
}
vector<Component*> ERModel::getComponentsVector()//��������component vector
{
	return _components;
}
void ERModel::addNode(string type)//�s�W
{
	string text;
	cin >> text;
	componentFactory = new ComponentFactory();
	Component* component = componentFactory->createComponent(type);
	component->setID(id);
	updateID();
	component->setText(text);
	component->setType(type);
	_components.push_back(component);
}
void ERModel::setComponentsVector(Component* component)//�]�w component vector
{
	_components.push_back(component);
}
vector<Component*> ERModel::getConnectionsVector()//���o����connection vector
{
	return _connections;
}
void ERModel::setConnectionsVector(Component* component)//�]�wconnection vector
{
	_connections.push_back(component);
}
Component* ERModel::clone(string type)//�ƻscomponents vector�̫�@�ӭ�
{
	componentFactory = new ComponentFactory();
	Component* component = componentFactory->createComponent(type);
	component->setID(_components.back()->getID());
	component->setText(_components.back()->getText());
	component->setType(type);
	return component;
}
Component* ERModel::connectionsClone(string type,int pos)//�ƻsconnections component
{
	componentFactory = new ComponentFactory();
	Component* component = componentFactory->createComponent(type);
	component->setID(_connections.at(pos)->getID());
	component->setText(_connections.at(pos)->getText());
	component->setType(type);
	return component;
}
Component* ERModel::componentsClone(string type, int pos)//�ƻscomponents component
{
	componentFactory = new ComponentFactory();
	Component* component = componentFactory->createComponent(type);
	component->setID(_components.at(pos)->getID());
	component->setText(_components.at(pos)->getText());
	component->setType(type);
	return component;
}
void ERModel::setDeleteID(int deleteId)//�]�wdeleted component id
{
	_deleteId = deleteId;
}
int ERModel::getDelelteID()//���odeleted component id
{
	return _deleteId;
}
int ERModel::checkDeleteComponentIDLoop()
{
	//checkDeleteComponentHint = BLANK;
	cin >> checkDeletIdTemp;
	if (checkDeletIdTemp == ZERO_STRING)
	{
		return 0;
	}
	else if (atoi(checkDeletIdTemp.c_str()) == 0)
	{
		cout <<"The component ID You entered does not exist. Please enter a valid one again."<<endl;

		checkDeleteComponentIDLoop();
	}
	else if (existId(atoi(checkDeletIdTemp.c_str())))
	{
		return atoi(checkDeletIdTemp.c_str());
	}
	else
	{
		cout <<  "The component ID You entered does not exist. Please enter a valid one again."<<endl;
		checkDeleteComponentIDLoop();
	}
}
bool ERModel::existId(int id)//�O�_�s�b��id
{
	for(unsigned int i = 0; i < _components.size();i++)
	{
		if(_components[i]->getID() == id)
		{
			return true;
		}
	}
	return false;	
}
void ERModel::addComponentsFromFile(vector<string> nodeVectorFromFile)//�qfile�W�[components
{
		componentFactory = new ComponentFactory();
		Component* component = componentFactory->createComponent(nodeVectorFromFile[0].substr(0,1));
		component->setID(id);
		updateID();
		component->setType(nodeVectorFromFile[0].substr(0,1));
		if (nodeVectorFromFile.size() == TWO)
		{
			component->setText(nodeVectorFromFile[1]);
		}
		else
		{
			component->setText(BLANK);
		}
		_components.push_back(component);
}
void ERModel::updateID()//��sid
{
	id = id + 1;
}
void ERModel::addConnectionFromFile(vector<string> connectionVectorFromFile)//�W�[connections �qfile
{
	componentFactory = new ComponentFactory();
	for (unsigned int i = 0; i < connectionVectorFromFile.size();i++)
	{
		Component* component = componentFactory->createComponent(convertIdtoComponent(atoi(connectionVectorFromFile[i].c_str()))->getType());
		component->setID(atoi(connectionVectorFromFile[i].c_str()));
		component->setType(convertIdtoComponent(atoi(connectionVectorFromFile[i].c_str()))->getType());
		component->setText(convertIdtoComponent(atoi(connectionVectorFromFile[i].c_str()))->getText());
		_connections.push_back(component);
	}
}
Component* ERModel::convertIdtoComponent(int id)//��Jid �ǥXcomponents�̪�index
{
	for (unsigned int i = 0; i < _components.size();i++)
	{
		if (id == _components[i]->getID())
			return _components[i];
	}
	return NULL;
}
void ERModel::loadFile()//���J�ɮ�
{
	string _filePath;
	string line;
	
	cin >> _filePath;
	ifstream myfile(_filePath);
	if (myfile.is_open())
	{
		while (getline(myfile,line))
		{	
			if (line == EMPTY_STRING)
			{
				break;
			}
			addComponentsFromFile(splitString(line,BLANK));
		}	
		while (getline(myfile,line))
		{	
			if (line == EMPTY_STRING)
			{
				break;
			}
			int pos = line.find(BLANK);
			if (pos == 1)
			{
				line.replace(line.find(DOUBLE_BLANK), TWO,BLANK);
			}
			line.replace(line.find(COMMA),1,BLANK);
			addConnectionFromFile(splitString(line,BLANK));
		}	
		while (getline(myfile,line))
		{	
			if (line == EMPTY_STRING)
			{
				break;
			}
			line.replace(line.find(BLANK),1,COMMA);
			addPrimaryKeyFromFile(splitString(line,COMMA));
		}	
		myfile.close();
		cout << "The ER diagram is displayed as follows:" << endl<<"Components: " << endl;
		displayComponentTable();
		displayConnectionTable();
	}
	else cout << "File not found!!"<<endl;
}
void ERModel::saveFile()//�s��
{
	string _fileName;
	cout << "Please input the file name: ";
	cin >> _fileName;//��X���ɦW���|
	ofstream myfile(_fileName);
	if(myfile.is_open())
	{
		for (unsigned int i = 0; i < _components.size();i++)
		{
			myfile << _components[i]->getType() << COMMABLANK << _components[i]->getText() << endl;
		}
		myfile << endl;
		
		for (unsigned int i = 0; i < _connections.size();i = i+3)
		{
			myfile <<setiosflags(ios::left) <<setw(1+1+1)<< _connections[i]->getID() << _connections[i+1]->getID()<< COMMA << _connections[i+1+1]->getID()<<endl;
		}
		int entityPresentIndex = 0;//Entity count
		for (unsigned int i = 0; i < _primaryKeys.size();i++)
		{
			if(_primaryKeys[i]->getType() == "E")
			{
				myfile << endl<<_primaryKeys[i]->getID()<<BLANK;
				entityPresentIndex = i;
			}
			else 
			{
				
				if (i == entityPresentIndex+1)
					myfile << _primaryKeys[i]->getID();
				else
					myfile <<COMMA << _primaryKeys[i]->getID();
			}
		}
		myfile.close();
	}
	else cout << "Unable to open file";
}
void ERModel::addPrimaryKeyFromFile(vector<string> primaryKeyVectorFromFile)//�qfile�W�[primary key
{
	componentFactory = new ComponentFactory();
	for (unsigned int i = 0; i< primaryKeyVectorFromFile.size();i++)
	{
		Component* component = componentFactory->createComponent(convertIdtoComponent(atoi(primaryKeyVectorFromFile[i].c_str()))->getType());
		component->setID(atoi(primaryKeyVectorFromFile[i].c_str()));
		component->setType(convertIdtoComponent(atoi(primaryKeyVectorFromFile[i].c_str()))->getType());
		component->setText(convertIdtoComponent(atoi(primaryKeyVectorFromFile[i].c_str()))->getText());
		_primaryKeys.push_back(component);
	}
}
bool ERModel::checkAddConnectionNodeTwo()//�ˬd�ĤG��connected node
{
	cin >> nodeIDTwo;
	if (nodeIDTwo == ZERO_STRING)//string �����ഫ�e�N�O0
	{
		return true;
	}
	else 
	{
		for (unsigned int i = 0; i < _components.size(); i++)
		{
			if ((atoi(nodeIDTwo.c_str()) == _components[i]->getID()) && (atoi(nodeIDTwo.c_str())!=0))
			{
				return true;
			}
		}
		cout << "The node ID you entered does not exist."<< endl;
		return false;
	}
}
void ERModel::connectTwoNode()//connect nodes
{
	if (checkAddConnectionNodeTwo()) 
	{
		cout << nodeIDTwo<<endl;
		setConnectionNodes(atoi(nodeIDTwo.c_str()));
		cout << _connectionNodesVector.size() <<endl;
		if (_connectionNodesVector[1] == _connectionNodesVector[0])//itself
		{
			cout << "&"<<endl;
			cout << "The Node '" <<  _connectionNodesVector[1] <<"' cannot be connected to itself." << endl;
			_connectionNodesVector.clear();
		}//same type
		else if (_components[_connectionNodesVector[1]]->getType() == _components[_connectionNodesVector[0]]->getType())
		{
			cout << "@~"<<endl;
			cout << "The node '" <<  _connectionNodesVector[1] <<"' cannot be connected by the node '"<<  _connectionNodesVector[0] <<"'."<<endl; 
		    _connectionNodesVector.clear();
		}//�w�s�L�P�@��connector
		else if (checkExistConnection(_connectionNodesVector))//true: �w�s�b
		{
			cout << "***"<<endl;
			cout << "The node '" << _connectionNodesVector[1]<<"' has already been connected to node '" << _connectionNodesVector[0] <<"'."<<endl;
		    _connectionNodesVector.clear();
		}
		else if (!(_components[_connectionNodesVector[1]]->canConnectTo(_components[_connectionNodesVector[0]]))&&(_components[_connectionNodesVector[0]]->canConnectTo(   _components[_connectionNodesVector[1]])))
		{
			cout << "+++"<<endl;
			cout << "The node '"<<_connectionNodesVector[1]<<"' cannot be connected by the node '"<< _connectionNodesVector[0]<<"'."<<endl;
		    _connectionNodesVector.clear();
		}
		else
		{
			cout << "The node '"<<_connectionNodesVector[1]<<"' has been connected to the node '"<<_connectionNodesVector[0]<<"'."<<endl;
			createConnector(_connectionNodesVector);
			_connections.push_back(_components[getIndexOfComponentID(_connectionNodesVector[0])]);
			_connections.push_back(_components[getIndexOfComponentID(_connectionNodesVector[1])]);
			displayConnectionTable();
			_connectionNodesVector.clear();
		}
	}
	else
	{
		cout << "The node ID you entered does not exist." <<endl;
		_connectionNodesVector.clear();
	}
}
void ERModel::createConnector(vector<int> connectionNodes)//�ͥXconnector
{
	componentFactory = new ComponentFactory();
	Component* component = componentFactory->createComponent(C);
	component->setID(id);
	updateID();
	component->setType(C);
	if ((_components[connectionNodes[0]]->getType() == "R") || (_components[connectionNodes[1]]->getType() == "R"))
	{
		string cardinality;
		string cardinalityTemp;
		cout << "Enter the type of the cardinality:" <<endl<<"[0]1 [1]N"<<endl<<"> ";
		cin >> cardinality;
		if (atoi(cardinality.c_str()) == 0)
		{
			component->setText("1");
			cardinalityTemp = "1";
		}
		else if (atoi(cardinality.c_str()) == 1)
		{
			component->setText("N");
			cardinalityTemp = "N";
		}
		else
		{
			cout << "Wrong cardinality."<<endl;
		}
		cout << "Its cardinality of the relationship is '"<< cardinalityTemp << "'."<< endl;
	}
	else
	{
		component->setText(BLANK);
	}
	_components.push_back(component);
	_connections.push_back(component);
}
bool ERModel::checkExistConnection(vector<int> connectionNodes)//�ˬd�O�_�s�b��connection
{
	for (unsigned  int i = 0; i < _connections.size(); i++)
	{
		if (i%3 != 0)
		{
			if (_connections[i]->getID() == connectionNodes[0])
			{
				if (_connections[i+1]->getID() == connectionNodes[1])
				{
					return true;
				}
			}
			else if (_connections[i]->getID() == connectionNodes[1])
			{
				if (_connections[i+1]->getID() == connectionNodes[0])
				{
					return true;
				}
			}
		}
	}
	return false;
}
void ERModel::setConnectionNodes(int nodeOne)//�]�w�nconnect �I��vector
{
	_connectionNodesVector.push_back(nodeOne);
}
string ERModel::getWholeName(string type)//���o���W
{ 
	if (type == A)
		return ATTRIBUTE;
	else if (type == E)
		return ENTITY;
	else if (type == R)
		return RELATIONSHIP;
	else
		return NONAME;
}
int ERModel::checkAddConnectionNodeOneLoop()//�ˬd�Ĥ@�ӭnconnect���I
{
	cin >> nodeIDOne;
	if (nodeIDOne == ZERO_STRING)//string �����ഫ�e�N�O0
	{
		return atoi(nodeIDOne.c_str());
	}
	else 
	{
		for (unsigned int i = 0; i < _components.size(); i++)
		{
			if ((atoi(nodeIDOne.c_str()) == _components[i]->getID()) && (atoi(nodeIDOne.c_str())!=0))
			{
				return atoi(nodeIDOne.c_str());
			}
		}
		cout << "The node ID you entered does not exist. Please enter a valid one again." <<endl<<"> ";
		checkAddConnectionNodeOneLoop();
	}
}
void ERModel::setPrimaryKey()//�]�wprimary key
{
	cout << "Enter the ID of the entity:" << endl << "> ";
	checkEntityLoop();
	displayAttributeOfEntity();//ok
	cout << "Enter the IDs of the attributes (use a comma to separate two attributes):" << endl << "> ";
	checkPrimaryKeyLoop();
	cout << "The entity '"<< getPKEntity() <<"' has the primary key ("<<endl;
	for (unsigned int i = 0; i < _primaryKeys.size()-1;i++)
	{
		cout << _primaryKeys[i]  <<COMMA;
	}
	cout << _primaryKeys[_primaryKeys.size()-1] <<")."<<endl;
}
vector<string> ERModel::splitString(string line, string reference)//���Rstring
{
	int pos=1;
	splitStringItem.clear();
	for (int i = 0; pos > 0; i++)
	{
		pos = line.find(reference);
		string stringItem = line.substr(0, pos);
		line = line.substr(pos+1,line.size());
		splitStringItem.push_back(stringItem);
	}
	return splitStringItem;
}
void ERModel::checkPrimaryKeyLoop()//�ˬd primary key ���쥿�T
{
	_primaryKeys.clear();
	string PKString;
	cin >> PKString;
	vector<string> pkTemp = splitString(PKString,COMMA);//id is string
	for (unsigned int i = 0; i < pkTemp.size(); i++)
	{
		vector<Component*> entityAttributes =  _components[getPKEntity()]->getConnections();
		 _primaryKeys.push_back(_components[getPKEntity()]);
		for (unsigned int j = 0; j < entityAttributes.size();j++)
		{
			if(entityAttributes[j]->getType()=="A")
			{
				if (atoi(pkTemp[i].c_str()) == entityAttributes[j]->getID())
				{
					_primaryKeys.push_back(_components[atoi(pkTemp[i].c_str())]);
				}
			}
		}
		if(_primaryKeys.size() != i+1)
		{
			cout << "The node '"<< pkTemp[i] <<"' does not belong to Entity '"<< getPKEntity() <<"'. Please enter a valid one again."<<endl<<"> ";
			checkPrimaryKeyLoop();
		}
	}
	return ;
}
void ERModel::checkPrimaryKey()//�ˬd primary key
{
	cin >> _primaryKey;
	splitString(_primaryKey, COMMA);
	for (unsigned int i = 0; i < _primaryKeyVector.size();i++)
	{
		thisKey = 0;
		for (unsigned int j = 0; (j < _attributesId.size() && thisKey == 0);j++)
		{	
			if (_attributesId[j] == atoi(_primaryKeyVector[i].c_str()))
			{
				thisKey = 1;
			}
		}
		if (thisKey == 0)
		{
			cout << "The node '" << _primaryKeyVector[i] <<"' does not belong to Entity '" << _entityTemp->getID() << "'. Please enter a valid one again." << endl << "> ";
			checkPrimaryKey();
		}
	}
}
void ERModel::displayAttributeOfEntity()//���Entity�Ҧ���attributes
{
	Component* entityTemp;
	entityTemp = _components[getPKEntity()];
	cout << "Attributes of the entity '"<< getPKEntity() <<"'"<<endl;
	cout << "---------------------------------" << endl;
	cout << "Type | ID | Name  " << endl;
	cout << "-----+----+----------------------" << endl;
	for (unsigned int i = 0; i < entityTemp->getConnections().size();i++)
	{
		if (entityTemp->getConnections()[i]->getType()=="A")
			cout << DOUBLE_BLANK << entityTemp->getConnections()[i]->getType() << "  |  " << entityTemp->getConnections()[i]->getID() << "  |  " << entityTemp->getConnections()[i]->getText()<< endl;
	}
	cout << "------------------------------------" << endl;
}
void ERModel::checkEntityLoop()//�ˬdEntity
{
	string entityId;
	cin >> entityId;
	if ((entityId == ZERO_STRING) && (_components[0]->getType() == E))
	{
		setPKEntity(atoi(entityId.c_str()));
		return;
	}
	else if (atoi(entityId.c_str()) == 0) //�O�r��
	{
		cout << "The node ID you entered does not exist. Please enter a valid one again." << endl << "> "<< endl;
		checkEntityLoop();
	}
	else if (_components[atoi(entityId.c_str())]->getType() != "E")
	{
		cout << "The node '" << entityId << "' is not an entity. Please enter a valid one again." << endl << "> "<< endl;
		checkEntityLoop();
	}
	else
	{
		setPKEntity(atoi(entityId.c_str()));
		return;
	}
}
void ERModel::setPKEntity(int entityid)//�]�wEntity �� Primary key
{
	entityID = entityid;
}
int ERModel::getPKEntity()//���oEntity �� Primary key
{
	return entityID;
}
void ERModel::displayEntityTable()//show Entities table
{
	cout << "Entities:" << endl;
	cout << "---------------------------------" << endl;
	cout << "Type | ID | Name  " << endl;
	cout << "-----+----+----------------------" << endl;
	for (unsigned int i = 0; i < _components.size();i++)
	{
		if (_components[i]->getType() == "E")
		{
			cout <<DOUBLE_BLANK<<_components[i]->getType()<< "  |  "<<_components[i]->getID()<<"  |  "<< _components[i]->getText()<<endl;
		}
	}
	cout << "---------------------------------" << endl;
}
void ERModel::showPrimary()//show primary key
{
	cout << "The entity '"<< _entityTemp->getID() <<"' has the primary key (";
	for (unsigned int s = 0; s < _primaryKeyVector.size();s++)
	{
		cout << _primaryKeyVector[s] <<COMMA;
	}
	cout <<"). "<< endl;
	for (unsigned int i = 0; i <_attributeWithOutPK.size();i++)
	{
		cout << "_attribute WithOut PK= " << _attributeWithOutPK[i]->getID() << endl;
	}
	_entityHasPrimaryKey.push_back(_entityTemp);
	_pkOfEntities.push_back(_primaryKeyVector);
}
void ERModel::displayTable()//show entity and it attributies table
{
	cout << "Tables:" << endl;
	cout << "-----------------------------------" << endl;
	cout << "Entity | Attribute  " << endl;
	cout << "-------+----------------------" << endl;
	for (unsigned int i = 0; i < _entityHasPrimaryKey.size();i++)
	{
			cout << DOUBLE_BLANK << _entityHasPrimaryKey[i]->getText() << "  |  " << "PK(";
			for (unsigned int j = 0; j < _pkOfEntities[i].size();j++)
				cout <<_pkOfEntities[i][j] << COMMA;
			cout << "), " ;
			cout << endl; 
	}
	cout << endl<<"------------------------------------" << endl;
}
void ERModel::displayConnectionTable()//show connection table
{
	cout << "Connections:" <<endl;
	cout << "------------------------------------" << endl;
	cout << "Connection | node | node  " << endl;
	cout << "------- ---+------+-----------------" << endl;
	for (unsigned int i = 0; i < _connections.size();i = i+3)
		cout << setw(6) << _connections[i]->getID() << "     |  "<<setw(2) << _connections[i+1]->getID() << "  |  "<<setw(2) << _connections[i+2]->getID() << endl;
	cout << "------------------------------------" << endl;
}
void ERModel::displayComponentTable()//show component table
{
	cout << "---------------------------------" << endl;
	cout << " Type |  ID  |  Name  " << endl;
	cout << "------+------+-------------------" << endl;
	for (unsigned int i = 0; i < _components.size();i++)
		cout << "   " << _components[i]->getType() << "  |  "<< setw(2) << _components[i]->getID()  << "  |  " << _components[i]->getText() << endl;
	cout << "---------------------------------" << endl;
}
void ERModel::deleteComponent(int id)//delete component
{
	setDeleteID(id);
	if (_components[id]->getType() == C)
	{
		cout << _connections.size()<<endl;
		for (unsigned int i = 0; i < _connections.size();i++)
		{
			if(_connections[i]->getID() == id)
			{
				_connections.erase(_connections.begin()+i+TWO);
				_connections.erase(_connections.begin()+i+1);
				_connections.erase(_connections.begin()+i);
				continue;
			}
		}
		for (unsigned int i = 0; i < _components.size(); i++)
		{
			if (_components[i]->getID()==id)
			{
				_components.erase(_components.begin()+i);
			}
		}
	}
	else
	{
		int updateConnectionsSize = _connections.size();
		for(int i = 0; i < updateConnectionsSize;i++)
		{
			if(_connections[i]->getID() == id)
			{
				if(_connections[i-1]->getType() == C)//��i��node1
				{
					_connections.erase(_connections.begin()+i+1);
					_connections.erase(_connections.begin()+i);
					_components.erase(_components.begin()+getIndexOfComponentID(_connections[i-1]->getID()));
					_connections.erase(_connections.begin()+i-1);
				}
				else 
				{
					int temp = i-TWO;
					_connections.erase(_connections.begin()+temp+TWO);
					_connections.erase(_connections.begin()+temp+1);
					_components.erase(_components.begin()+getIndexOfComponentID(_connections[temp]->getID()));
					_connections.erase(_connections.begin()+temp);
				}
				updateConnectionsSize = _connections.size();
				i=0;
			}
		}
		for (unsigned int i = 0; i < _components.size(); i++)
		{
			if (_components[i]->getID()==id)
			{
				_components.erase(_components.begin()+i);
			}
		}
	}
}
int ERModel::getIndexOfComponentID(int componentID)//�� id �ഫ�� components vector �� index
{
	for (unsigned int i = 0; i < _components.size();i++)
	{
		if(_components[i]->getID() == componentID)
			return i;
	}
}
int ERModel::getIndexOfConnectionsID(int connectionsID)//�� id �ഫ��connections vector �� index
{
	for (unsigned int i = 0; i < _connections.size();i++)
	{
		if (_connections[i]->getID() == connectionsID)
			return i;
	}
}

