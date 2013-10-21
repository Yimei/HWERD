#include "gtest/gtest.h"
#include "Attribute.h"
#include "Component.h"
class AttributeUnitTest:public::testing::Test
{
	
protected:
	virtual void SetUp()
	{
		attribute = new Attribute();
		
	}
	virtual void TearDown()
	{
		delete attribute;
		delete component;
		
	}
	Attribute* attribute;
	Component* component;
};
TEST_F(AttributeUnitTest, testCanConnectTo)
{
	component->setType("A");
	EXPECT_FALSE(attribute->canConnectTo(component));
	component->setType("R");
	EXPECT_FALSE(attribute->canConnectTo(component));
	component->setType("C");
	EXPECT_FALSE(attribute->canConnectTo(component));
	component->setType("E");//�i�sEntity �B �ثe�@��
	EXPECT_TRUE(attribute->canConnectTo(component));
	EXPECT_FALSE(attribute->canConnectTo(component));


}
