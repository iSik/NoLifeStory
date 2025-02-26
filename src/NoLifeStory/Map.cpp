////////////////////////////////////////////////////
// This file is part of NoLifeStory.              //
// Please see SuperGlobal.h for more information. //
////////////////////////////////////////////////////
#include "Global.h"

NLS::Node NLS::Map::node;
ustring NLS::Map::nextmap;
ustring NLS::Map::curmap;
ustring NLS::Map::nextportal;
vector<NLS::Back*> NLS::Map::Backgrounds;
NLS::Map::Layer NLS::Map::Layers[8];
vector<NLS::Back*> NLS::Map::Foregrounds;
NLS::Sound NLS::Map::bgmusic;

void NLS::Map::Load(const ustring& id, const ustring& portal) {
	nextmap = id;
	nextportal = portal;
}

void NLS::Map::Load() {
	auto teleport = [&](ustring portal, bool change) {
		if (portal.empty()) {
			if (change) {
				portal = "sp";
			} else {
				return;
			}
		}
		vector <Portal*> possible;
		for (auto it = Portal::Portals.begin(); it != Portal::Portals.end(); it++) {
			Portal* p = *it;
			if (portal == p->pn) {
				possible.push_back(p);
			}
		}
		if (possible.empty()) {
			return;
		}
		int r = rand()%possible.size();
		ThisPlayer.Reset(possible[r]->x, possible[r]->y-16);
		if (change) {
			View.vx = ThisPlayer.x-View.width/2;
			View.vy = ThisPlayer.y-View.height/2;
		}
	};
	if (curmap == nextmap) {
		ucerr << U("The specified map is already loaded") << endl;
		teleport(nextportal, false);
		nextmap = "";
		nextportal = "";
		return;
	}
	if (nextmap == "MapLogin") {
		node = WZ["UI"]["MapLogin"];
		throw(273);//We don't deal with this shit yet
	} else {
		nextmap.pad('0', 9);
		uchar zone = nextmap[0];
		node = WZ["Map"]["Map"][ustring("Map")+zone][nextmap];
	}
	if (!node) {
		ucerr << U("Unable to locate map ") << nextmap << endl;
		teleport(nextportal, false);
		nextmap = "";
		nextportal = "";
		return;
	}
	Time.Reset();
	curmap = nextmap;
	ucout << U("Loading map ") << nextmap << endl;
	ustring bgm = node["info"]["bgm"];
	vector<ustring> p = bgm.split('/');
	bgmusic = WZ["Sound"][p[0]][p[1]];
	bgmusic.Play(true);
	for (uint8_t i = 0; i < 8; i++) {
		Layers[i].Tiles.clear();
		Layers[i].Objs.clear();
	}
	Backgrounds.clear();
	Foregrounds.clear();
	Sprite::Unload();
	Foothold::Load(node);
	Tile::Load(node);
	Obj::Load(node);
	Back::Load(node);
	Portal::Load(node);
	Reactor::Load(node);
	View.tx = 0;
	View.ty = 0;
	if (node["info"]["VRLeft"]) {
		View.xmin = node["info"]["VRLeft"];
		View.xmax = node["info"]["VRRight"];
		View.ymin = node["info"]["VRTop"];
		View.ymax = node["info"]["VRBottom"];
	} else {
		View.xmin = 1000000;
		View.xmax = -1000000;
		View.ymin = 1000000;
		View.ymax = -1000000;
		for (auto it = footholds.begin(); it != footholds.end(); it++) {
			View.xmin = min(min((double)View.xmin, (*it)->x1), (*it)->x2);
			View.ymin = min(min((double)View.ymin, (*it)->y1), (*it)->y2);
			View.xmax = max(max((double)View.xmax, (*it)->x1), (*it)->x2);
			View.ymax = max(max((double)View.ymax, (*it)->y1), (*it)->y2);
		}
		View.ymax += 128;
		View.ymin -= View.height;
	}
	teleport(nextportal, true);
	nextmap = "";
	nextportal = "";
}

void NLS::Map::Draw() {
	for (uint32_t i = 0; i < Backgrounds.size(); i++) {
		Backgrounds[i]->Draw();
	}
	for (uint32_t i = 0; i < Reactor::Reactors.size(); i++) {
		Reactor::Reactors[i]->Draw();
	}
	for (uint8_t i = 0; i < 8; i++) {
		Layers[i].Draw();
	}
	ThisPlayer.Draw();
	for (uint32_t i = 0; i < Portal::Portals.size(); i++) {
		Portal::Portals[i]->Draw();
	}
	for (uint32_t i = 0; i < Foregrounds.size(); i++) {
		Foregrounds[i]->Draw();
	}
}

void NLS::Map::Layer::Draw() {
	for (auto it = Objs.begin(); it != Objs.end(); it++) {
		(*it)->Draw();
	}
	for (auto it = Tiles.begin(); it != Tiles.end(); it++) {
		(*it)->Draw();
	}
}
