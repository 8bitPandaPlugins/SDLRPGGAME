#pragma once
#include "Sprite.h"

class GameObject {
	// master gameobject, all others inheret from this 
public:
	GameObject(){}
	~GameObject(){
		graphics = nullptr;
		OverWorldImage = nullptr;
	}
	virtual void init(Display* graph) = 0;
	virtual void update() = 0;
	virtual void onCollide() = 0;
	virtual void AddToLayer(SpriteLayer * layer) = 0;
	void imageload() {
		OverWorldImage = new Sprite(ImgPath ,*graphics);
	}
	int x; 
	int y;
	int w;
	int h;
	CollisionBool CollsionData;
	Display * graphics;
	Sprite * OverWorldImage;
	std::string ImgPath;
};

class MoveableGameObject : public GameObject {
	//base class for any moveable objects, all NPCs inherit from this
public:
	virtual void move() = 0;
	 


};

class NPCObject: public MoveableGameObject
{
public:
	NPCObject() {}
	//all NPC characters are built from this object, contains all interaction text and other code for interaction

	void init(Display* graph) override
	{

	}

	void update() override
	{
	}
	void move() override
	{

	}
	void AddToLayer(SpriteLayer * layer) override
	{

	}

	void onCollide() override
	{

	}

	bool hasItem;
	
};



class Door : public GameObject {
public:
	Door() 
	{

	}
	void init(Display* graph) override 	{
		OverWorldImage = new Sprite(*graph);
	}
	//handles all enterences and exits, transistions between maps 
	void update() override {

	}

	void onCollide()override {

	}

	void AddToLayer(SpriteLayer * layer) override {
	
	}
	int destX;
	int destY;
	std::string mapname;

};