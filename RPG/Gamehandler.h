#pragma once
#include <vector>
#include "Sprite.h"
#include "GameObject.h"
#include "Map.h"
#include "Player.h"
#include "Layers.h"
#include "Tileset.h"
#include "Text.h"

//master game object, holds the global Loop and draw functions along with lists of maps, objects and layers
class GameEvents
{
	//game events are not processed every frame, they are processed every 6 frames,
	//to reduce impact on the CPU and to keep gameplay at a reasonable speed 
public:
	//constructor filled with test code atm
	GameEvents(Player * mainPlayer, Display *Graphics):M_Text("Pokemon GB.ttf", Graphics)
	{
		M_MainPlayer = mainPlayer;
		M_MainPlayer->x = 256;
		M_MainPlayer->y = 256;
		M_graphics = Graphics;
		M_Vbuffer.push_back(new TileLayer());
		M_Vbuffer.push_back(new TileLayer());
		M_Vbuffer.push_back(new SpriteLayer(M_graphics));
		M_Vbuffer.push_back(new collisionLayer(M_graphics));
	}
	~GameEvents()
	{
		M_Vbuffer.clear();
		M_maps.clear();
		M_graphics = nullptr;
		M_MainPlayer = nullptr;
	}

	//loads new map into the vector of maps, handles the path generation and object processing 
	void loadNewMap(std::string MapName){
	std::string path = "Maps/" + MapName + ".tmx";
	std::cout << path << std::endl;
	M_maps.push_back(new Map(path,M_graphics));
	G_MapObjects.push_back(M_MainPlayer);
	M_maps[0]->handleObjects(&G_MapObjects);

	M_maps[0]->AddToLayer(&M_Vbuffer);
	if (SpriteLayer * sprites = dynamic_cast<SpriteLayer*>(M_Vbuffer.at(2))) {
		for (int i = 0; i < G_MapObjects.size(); i++)
		{
			G_MapObjects.at(i)->init(M_graphics);
			G_MapObjects.at(i)->OverWorldImage->setSpriteID(i);

			sprites->spritecount = i+1;

		}
		sprites->Sprites.resize(sprites->spritecount);
		sprites->Spriteimages.resize(sprites->spritecount);
		sprites->ActualLocations.resize(sprites->spritecount);
	}
	M_graphics->mapSizeX = M_maps[0]->mWidth;
	M_graphics->mapSizeY = M_maps[0]->mHeight;
	M_MainPlayer->getMap(M_maps[0]);
	for (int i = 0; i < M_maps[0]->Layers.size(); i++) {
		if (collisionLayer * Collision = dynamic_cast<collisionLayer*>(M_Vbuffer.at(3)) ) {
			Collision->generateTileCollisionMap(&M_maps[0]->Layers.at(i));

		}
	}


	}

	//master Draw function will hold layer drawing code in future versions 
	void LoopDraw()
	{
		std::string playerpos = std::to_string(M_MainPlayer->x);
		playerpos += " ";
		playerpos += std::to_string(M_MainPlayer->y);
			OnscreenTextObject PlayerCoords = M_Text.GenerateText(playerpos,10,10);
		if (SpriteLayer * SpriteCanvas = dynamic_cast<SpriteLayer*>(M_Vbuffer.at(2)))
		{
			for (int i = 0; i < G_MapObjects.size(); i++)
			{
				G_MapObjects[i]->AddToLayer(SpriteCanvas);
				
			}
		}
		for (int i = 0; i < M_Vbuffer.size(); i++) {
			M_Vbuffer[i]->draw();
		}
		M_Text.RenderText(PlayerCoords);
	}
	void GameLoop(std::array<bool, SDL_NUM_SCANCODES>*Input)
	{
		LoopDraw();
		if (M_counter == 6) {
			M_MainPlayer->passScanCodes(Input);
			UICatcher(Input);
			if (collisionLayer * Collision = dynamic_cast<collisionLayer*>(M_Vbuffer.at(3))) {

				if (SpriteLayer * Sprites = dynamic_cast<SpriteLayer*>(M_Vbuffer.at(2))) {
					Collision->generateSpriteCollisionMap(Sprites);
				}
				M_CollisionData = Collision->pushCollisionData();

			}
			for (int i = 0; i < G_MapObjects.size(); i++)
			{
				G_MapObjects.at(i)->CollsionData = M_CollisionData.at(i);
				
			}
			for (int i = 0; i < M_Vbuffer.size(); i++)
			{
				M_Vbuffer.at(i)->update();
			}
			for (int i = 0; i < G_MapObjects.size(); i++)
			{

				G_MapObjects[i]->update();
			}

			M_counter = 0;
		}
		
		M_counter++;
	}

	void UICatcher(std::array<bool, SDL_NUM_SCANCODES>*Input){
		
		OnscreenTextObject UI = M_Text.GenerateText("UI is open", 500, 500);
		if(Input->at(SDL_SCANCODE_ESCAPE) == true || KeepUI == true){
			std::cout << "UI open" << std::endl;
			KeepUI = true;
			while(KeepUI == true){
				M_Text.RenderText(UI);
				if(Input->at(SDL_SCANCODE_ESCAPE) == true){
					KeepUI = false;
				}
				break;
				
			}
		}
	}
	void ErrorCatcher(){
		
		if(G_hasError == true){


		}
	}
	std::vector<GameObject*>G_MapObjects;

private:
	bool KeepUI = false;
	bool G_hasError;
	std::vector<layer*>M_Vbuffer;
	TextHandler M_Text;
	Display * M_graphics;
	std::vector<Map*>M_maps;
	std::vector<CollisionBool>M_CollisionData;
	Player* M_MainPlayer;
	int M_Objects;
	int8_t M_counter = 0; 
};