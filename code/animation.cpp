#include "animation.h"

namespace {
	XmlEntity getNextEntity(FILE ** fpp) {
		char c = fgetc(*fpp);
		while (c == ' ' || c == '\n' || c == '\r' || c == '\t') c = fgetc(*fpp);
		XmlEntity res;
		if (c == -1) {
			res.isCommand = true;
			res.commandEnd = true;
			res.value = "EOF";
			return res;
		}
		switch (c) {
		case '<':
			res.isCommand = true;
			c = fgetc(*fpp);
			if (c != '/') {
				while (c != '>') {
					res.value.push_back(c);
					c = fgetc(*fpp);
				}
			}
			else {
				res.commandEnd = true;
				c = fgetc(*fpp);
				while (c != '>') {
					res.value.push_back(c);
					c = fgetc(*fpp);
				}
			}
			break;
		case '/':
			res.isCommand = true;
			res.commandEnd = true;
			c = fgetc(*fpp);
			while (c != '>') {
				res.value.push_back(c);
				c = fgetc(*fpp);
			}
			break;
		default:
			while (c != '<') {
				res.value.push_back(c);
				c = fgetc(*fpp);
			}
		}
		return res;
	}
}

namespace ohms {
	Frame::Frame() {
		this->f = false;
		this->x = 0.0f;
		this->y = 0.0f;
		this->kx = 0.0f;
		this->ky = 0.0f;
		this->sx = 1.0f;
		this->sy = 1.0f;
		this->i = "";
		this->a = 1.0f;
		this->nice = false;
		//this->isReallyEmpty = true;
		return;
	}
	Frame::~Frame() {
		return;
	}
	void Frame::readFrame(FILE** fpp, Frame lastOne) {
		XmlEntity xml;
		unsigned char state = 0;
		bool loadContinue = true;
#ifdef _DEBUG_OUT
		printf_s("\t\tStart read Frame\n");
#endif
		this->f = lastOne.f;
		this->x = lastOne.x;
		this->y = lastOne.y;
		this->kx = lastOne.kx;
		this->ky = lastOne.ky;
		this->sx = lastOne.sx;
		this->sy = lastOne.sy;
		this->a = lastOne.a;
		//this->isReallyEmpty = lastOne.isReallyEmpty;
		while (loadContinue) {
			switch (state) {
			case 0:
				xml = getNextEntity(fpp);
				if (!(xml.isCommand && !xml.commandEnd)) {
					if (xml.isCommand && xml.commandEnd && xml.value == "t") {
						state = 1;
						break;
					}
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("\t\tRead frame: command start error\n");
#endif
					break;
				}
				if (xml.value == "f") {
					xml = getNextEntity(fpp);
					if (xml.isCommand) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <f>: value error\n");
#endif
						break;
					}
					this->f = (std::atoi(xml.value.c_str()) == -1);
#ifdef _DEBUG_OUT
					printf_s("\t\tRead <f>: get %d\n", this->f);
#endif
					xml = getNextEntity(fpp);
					if (!(xml.isCommand && xml.commandEnd && xml.value == "f")) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <f>: command end error\n");
#endif
						break;
					}
				}
				else if (xml.value == "x") {
					xml = getNextEntity(fpp);
					if (xml.isCommand) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <x>: value error\n");
#endif
						break;
					}
					this->x = static_cast<float>(std::atof(xml.value.c_str()));
					//this->isReallyEmpty = false;
#ifdef _DEBUG_OUT
					printf_s("\t\tRead <x>: get %f\n", this->x);
#endif
					xml = getNextEntity(fpp);
					if (!(xml.isCommand && xml.commandEnd && xml.value == "x")) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <x>: command end error\n");
#endif
						break;
					}
				}
				else if (xml.value == "y") {
					xml = getNextEntity(fpp);
					if (xml.isCommand) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <y>: value error\n");
#endif
						break;
					}
					this->y = static_cast<float>(std::atof(xml.value.c_str()));
					//this->isReallyEmpty = false;
#ifdef _DEBUG_OUT
					printf_s("\t\tRead <y>: get %f\n", this->y);
#endif
					xml = getNextEntity(fpp);
					if (!(xml.isCommand && xml.commandEnd && xml.value == "y")) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <y>: command end error\n");
#endif
						break;
					}
				}
				else if (xml.value == "kx") {
					xml = getNextEntity(fpp);
					if (xml.isCommand) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <kx>: value error\n");
#endif
						break;
					}
					this->kx = static_cast<float>(std::atof(xml.value.c_str()));
					//this->isReallyEmpty = false;
#ifdef _DEBUG_OUT
					printf_s("\t\tRead <kx>: get %f\n", this->kx);
#endif
					xml = getNextEntity(fpp);
					if (!(xml.isCommand && xml.commandEnd && xml.value == "kx")) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <kx>: command end error\n");
#endif
						break;
					}
				}
				else if (xml.value == "ky") {
					xml = getNextEntity(fpp);
					if (xml.isCommand) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <ky>: value error\n");
#endif
						break;
					}
					this->ky = static_cast<float>(std::atof(xml.value.c_str()));
					//this->isReallyEmpty = false;
#ifdef _DEBUG_OUT
					printf_s("\t\tRead <ky>: get %f\n", this->ky);
#endif
					xml = getNextEntity(fpp);
					if (!(xml.isCommand && xml.commandEnd && xml.value == "ky")) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <ky>: command end error\n");
#endif
						break;
					}
				}
				else if (xml.value == "sx") {
					xml = getNextEntity(fpp);
					if (xml.isCommand) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <sx>: value error\n");
#endif
						break;
					}
					this->sx = static_cast<float>(std::atof(xml.value.c_str()));
					//this->isReallyEmpty = false;
#ifdef _DEBUG_OUT
					printf_s("\t\tRead <sx>: get %f\n", this->sx);
#endif
					xml = getNextEntity(fpp);
					if (!(xml.isCommand && xml.commandEnd && xml.value == "sx")) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <sx>: command end error\n");
#endif
						break;
					}
				}
				else if (xml.value == "sy") {
					xml = getNextEntity(fpp);
					if (xml.isCommand) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <sy>: value error\n");
#endif
						break;
					}
					this->sy = static_cast<float>(std::atof(xml.value.c_str()));
					//this->isReallyEmpty = false;
#ifdef _DEBUG_OUT
					printf_s("\t\tRead <sy>: get %f\n", this->sy);
#endif
					xml = getNextEntity(fpp);
					if (!(xml.isCommand && xml.commandEnd && xml.value == "sy")) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <sy>: command end error\n");
#endif
						break;
					}
				}
				else if (xml.value == "i") {
					xml = getNextEntity(fpp);
					if (xml.isCommand) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <i>: value error\n");
#endif
						break;
					}
					this->i = xml.value.c_str();
					//this->isReallyEmpty = false;
					this->i.erase(0, 13);
					for (std::string::iterator it = this->i.begin() + 1, n = this->i.end(); it < n; ++it) {
						if ('A' <= *it && *it <= 'Z') {
							*it = *it - 'A' + 'a';
						}
					}
					//this->i = std::string("assets/test/").append(this->i).append(".png");
#ifdef _DEBUG_OUT
					printf_s("\t\tRead <i>: get %s\n", this->i.c_str());
#endif
					xml = getNextEntity(fpp);
					if (!(xml.isCommand && xml.commandEnd && xml.value == "i")) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <i>: command end error\n");
#endif
						break;
					}
				}
				else if (xml.value == "a") {
					xml = getNextEntity(fpp);
					if (xml.isCommand) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <a>: value error\n");
#endif
						break;
					}
					this->a = static_cast<float>(std::atof(xml.value.c_str()));
					//this->isReallyEmpty = false;
#ifdef _DEBUG_OUT
					printf_s("\t\tRead <a>: get %f\n", this->a);
#endif
					xml = getNextEntity(fpp);
					if (!(xml.isCommand && xml.commandEnd && xml.value == "a")) {
						state = 255;
#ifdef _DEBUG_OUT
						printf_s("\t\tRead <a>: command end error\n");
#endif
						break;
					}
				}
				else {
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("\t\tRead frame: unknown command error\n");
#endif
					break;
				}
				break;
			case 1:
				loadContinue = false;
				break;
			default:
#ifdef _DEBUG_OUT
				printf_s("\t\tRead frame Error!\n");
#endif
				return;
			}
		}
		this->nice = true;
#ifdef _DEBUG_OUT
		printf_s("\t\tRead frame success!\n");
#endif
		return;
	}
	Track::Track() {
		this->nice = false;
		this->isCtrlTrack = true;
		return;
	}
	Track::~Track() {
		return;
	}
	void Track::readTrack(FILE** fpp) {
		XmlEntity xml;
		unsigned char state = 0;
		bool loadContinue = true;
		Frame lastOne;
		Frame* frame = nullptr;
#ifdef _DEBUG_OUT
		printf_s("\tStart read Track\n");
#endif
		while (loadContinue) {
			switch (state) {
			case 0:
				xml = getNextEntity(fpp);
				if (!(xml.isCommand && !xml.commandEnd && xml.value == "name")) {
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("\tRead name: command start error!\n");
#endif
					break;
				}
				xml = getNextEntity(fpp);
				if (xml.isCommand) {
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("\tRead name: value error!\n");
#endif
					break;
				}
				this->name = xml.value.c_str();
#ifdef _DEBUG_OUT
				printf_s("\tRead name: %s\n", this->name.c_str());
#endif
				xml = getNextEntity(fpp);
				if (!(xml.isCommand && xml.commandEnd && xml.value == "name")) {
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("\tRead name: command end error!\n");
#endif
					break;
				}
				state = 1;
				break;
			case 1:
				xml = getNextEntity(fpp);
				if (!(xml.isCommand && !xml.commandEnd && xml.value == "t")) {
					if (xml.isCommand && xml.commandEnd && xml.value == "track") {
						state = 2;
#ifdef _DEBUG_OUT
						printf_s("\tRead frames: end\n");
#endif
						break;
					}
					state = 255;
					break;
				}
#ifdef _DEBUG_OUT
				printf_s("\tFind frame\n");
#endif
				this->frames.emplace_back();
				frame = &(this->frames.back());
				frame->readFrame(fpp, lastOne);
				//if (this->isCtrlTrack) {
				//	if (!frame->isReallyEmpty) {
				//		this->isCtrlTrack = false;
				//	}
				//}
				lastOne = *frame;
				if (!frame->nice) {
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("\tRead frame: value error!\n");
#endif
					break;
				}
				break;
			case 2:
				loadContinue = false;
				break;
			default:
#ifdef _DEBUG_OUT
				printf_s("\tRead track Error!\n");
#endif
				return;
			}
		}
		this->nice = true;
#ifdef _DEBUG_OUT
		printf_s("\tRead track success!\n");
#endif
		if (this->name.find("anim_") != 0xffffffffffffffff) {
			this->isCtrlTrack = true;
		}
		else {
			this->isCtrlTrack = false;
		}
		return;
	}
	AnimationData::AnimationData() {
		this->fps = 1;
		this->frameCount = 1;
		this->trackCount = 1;
		this->nice = false;
		return;
	}
	AnimationData::~AnimationData() {
		this->clear();
		return;
	}
	void AnimationData::load(const std::string& path) {
#ifdef _DEBUG_OUT
		printf_s("Open reanim file: %s\n", path.c_str());
#endif
		FILE* fp;
		fopen_s(&fp, path.c_str(), "r");
		if (fp == 0) {
#ifdef _DEBUG_OUT
			printf_s("Open reanim file failed!\n");
#endif
			return;
		}
#ifdef _DEBUG_OUT
		printf_s("Open reanim file success\n");
#endif
		XmlEntity xml;
		Track* track = nullptr;
		unsigned char state = 0;
		bool loadContinue = true;
		while (loadContinue) {
			switch (state) {
			case 0:
				/*
				xml = getNextEntity(&fp);
				if (!(xml.isCommand && !xml.commandEnd &&xml.value == "skeleton")) {
					state = 255;
					printf_s("Start read error!\n");
					break;
				}*/
				state = 1;
				break;
			case 1:
				xml = getNextEntity(&fp);
				if (!(xml.isCommand && !xml.commandEnd &&xml.value == "fps")) {
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("Read fps: command start error!\n");
#endif
					break;
				}
				xml = getNextEntity(&fp);
				if (xml.isCommand) {
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("Read fps: value error!\n");
#endif
					break;
				}
				this->fps = std::atoi(xml.value.c_str());
				xml = getNextEntity(&fp);
				if (!(xml.isCommand && xml.commandEnd && xml.value == "fps")) {
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("Read fps: command end error!\n");
#endif
					break;
				}
				state = 2;
#ifdef _DEBUG_OUT
				printf_s("Get fps: %zd\nStart read Tracks\n", this->fps);
#endif
				break;
			case 2:
				xml = getNextEntity(&fp);
				if (!(xml.isCommand && !xml.commandEnd && xml.value == "track")) {
					if (xml.isCommand && xml.commandEnd && xml.value == "EOF") {
						state = 3;
#ifdef _DEBUG_OUT
						printf_s("Read tracks: end\n");
#endif
						break;
					}
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("Read tracks: command start error!\n");
#endif
					break;
				}
#ifdef _DEBUG_OUT
				printf_s("Find track\n");
#endif
				track = new Track;
				track->readTrack(&fp);
				if (!track->nice) {
					delete track;
					state = 255;
#ifdef _DEBUG_OUT
					printf_s("Read tracks: value error!\n");
#endif
					break;
				}
				this->trackArray.push_back(track);
				this->tracks.emplace(track->name, track);
#ifdef _DEBUG_OUT
				printf_s("Add track\n");
#endif
				break;
			case 3:
				loadContinue = false;
				break;
			default:
#ifdef _DEBUG_OUT
				printf_s("Load AnimationData Error!\n");
#endif
				fclose(fp);
				fp = 0;
				this->clear();
				return;
			}
		}
		fclose(fp);
		fp = 0;
		this->frameCount = static_cast<unsigned int>(this->tracks.begin()->second->frames.size());
		this->trackCount = static_cast<unsigned int>(this->trackArray.size());
		this->nice = true;
		printf_s("Load animation success!\n");
		return;
	}
	void AnimationData::clear() {
		for (auto it = this->tracks.begin(), n = this->tracks.end(); it != n; ++it) {
			delete it->second;
		}
		this->tracks.clear();
		return;
	}
	Animation::Animation(const AnimationData* data, bool linearFix) {
		if (!data->nice) {
			this->nice = false;
			return;
		}

		this->linearFix = linearFix;

		this->font.loadFromFile("assets/test.otf");
		this->text.setFillColor(sf::Color::Red);
		this->text.setCharacterSize(40);
		this->text.setPosition(0.0f, 0.0f);
		this->text.setFont(this->font);

		this->dataRef = data;
		this->trackCount = data->tracks.size();
		this->tracks = new ohms::Animation::Track[this->trackCount];
		this->animationNow = nullptr;
		this->timePoint = 0.0f;
		this->lastFramePoint = 0;
		this->animationOffset = 0;
		this->animationLength = 0;

		this->nice = true;
		return;
	}
	Animation::~Animation() {
		delete[] this->tracks;
		return;
	}
	bool Animation::setAnimation(const std::string& name) {
		if (!this->nice) {
			return false;
		}
		if (this->animationNow && name == this->animationNow->name) {
			this->timePoint = 0.0f;
			this->update(0.0f);
			return true;
		}
		std::map<std::string, ohms::Track*>::const_iterator it = this->dataRef->tracks.find(name);
		if (it == this->dataRef->tracks.end()) {
			printf_s("Animaiton not find: %s\n", name.c_str());
			return false;
		}
		if (!it->second->isCtrlTrack) {
			printf_s("Animaiton is not control track: %s\n", name.c_str());
			return false;
		}

		this->animationNow = it->second;

		printf_s("Set animation: %s\n", name.c_str());
		//for (size_t i = 0, n = this->dataRef->frameCount; i < n; ++i){
		//	printf_s("%d ", this->animationNow->frames[i].f);
		//}

		this->animationOffset = 0;
		while ((this->animationOffset < this->dataRef->frameCount) && this->animationNow->frames[this->animationOffset].f)
			++this->animationOffset;
		this->animationLength = this->animationOffset;
		while ((this->animationLength < this->dataRef->frameCount) && !(this->animationNow->frames[this->animationLength].f))
			++this->animationLength;
		this->animationLength = this->animationLength - this->animationOffset;

		//printf_s("\nGet duration: %lld %lld\n\n", this->animationOffset, this->animationLength);

		this->timePoint = 0.0f;
		this->lastFramePoint = this->animationLength - 1;
		for (size_t i = 0; i <= this->animationOffset; ++i) {
			this->updateFrame(i - this->animationOffset);
		}
		this->update(0.0f);
		return true;
	}
	void Animation::update(float dt) {
		if (!this->nice) {
			return;
		}
		this->timePoint += dt;

		if (this->timePoint >= (float)(this->animationLength - 1) / this->dataRef->fps) {
			this->timePoint -= (float)(this->animationLength - 1) / this->dataRef->fps;
		}

		size_t framePoint = (size_t)(this->timePoint * this->dataRef->fps);
		float mixtime = this->timePoint * this->dataRef->fps - framePoint;

		if (framePoint != this->lastFramePoint) {
			for (size_t i = this->lastFramePoint + 1, n = framePoint + (i > framePoint ? this->animationLength : 0); i <= n; ++i) {
				this->updateFrame(i % this->animationLength);
			}
			this->lastFramePoint = framePoint % this->animationLength;
		}

		for (size_t i = 0, n = this->dataRef->trackCount; i < n; ++i) {
			ohms::Animation::Track& ii = this->tracks[i];
			ohms::Frame& nowAt = this->dataRef->trackArray[i]->frames[framePoint % this->animationLength + this->animationOffset];
			ohms::Frame& nextAt = this->dataRef->trackArray[i]->frames[(framePoint + 1) % this->animationLength + this->animationOffset];

			float lx, ly, lkx, lky, lsx, lsy, la, coskx, sinkx, cosky, sinky;
			if (this->linearFix && !nextAt.f) {
				lx = nowAt.x * (1.0f - mixtime) + nextAt.x * mixtime;
				ly = nowAt.y * (1.0f - mixtime) + nextAt.y * mixtime;
				if (nowAt.kx - nextAt.kx > 180.0f) {
					lkx = (nowAt.kx * (1.0f - mixtime) + (nextAt.kx + 360.0f) * mixtime) * 3.1415926535897932f / 180.0f;
				}
				else {
					lkx = (nowAt.kx * (1.0f - mixtime) + nextAt.kx * mixtime) * 3.1415926535897932f / 180.0f;
				}
				if (nowAt.ky - nextAt.ky > 180.0f) {
					lky = (nowAt.ky * (1.0f - mixtime) + (nextAt.ky + 360.0f) * mixtime) * 3.1415926535897932f / 180.0f;
				}
				else {
					lky = (nowAt.ky * (1.0f - mixtime) + nextAt.ky * mixtime) * 3.1415926535897932f / 180.0f;
				}
				lsx = nowAt.sx * (1.0f - mixtime) + nextAt.sx * mixtime;
				lsy = nowAt.sy * (1.0f - mixtime) + nextAt.sy * mixtime;
				la = (nowAt.a * (1.0f - mixtime) + nextAt.a * mixtime) * 255.0f;
			}
			else {
				lx = nowAt.x;
				ly = nowAt.y;
				lkx = nowAt.kx * 3.1415926535897932f / 180.0f;
				lky = nowAt.ky * 3.1415926535897932f / 180.0f;
				lsx = nowAt.sx * (1.0f - mixtime) + nextAt.sx * mixtime;
				lsy = nowAt.sy * (1.0f - mixtime) + nextAt.sy * mixtime;
				la = nowAt.a * 255.0f;
			}
			coskx = cos(lkx);
			sinkx = sin(lkx);
			cosky = cos(lky);
			sinky = sin(lky);
			
			ii.vertex[0].position = ii.vertex[0].texCoords = { 0.0f, 0.0f };
			ii.vertex[1].position = ii.vertex[1].texCoords = { (float)ii.texSize.x, 0.0f };
			ii.vertex[2].position = ii.vertex[2].texCoords = { 0.0f, (float)ii.texSize.y };
			ii.vertex[3].position = ii.vertex[3].texCoords = { (float)ii.texSize.x, (float)ii.texSize.y };


			for (unsigned char i = 0; i < 4; ++i) {
				ii.vertex[i].position.x *= lsx;
				ii.vertex[i].position.y *= lsy;
			}

			for (unsigned char i = 0; i < 4; ++i) {
				ii.vertex[i].position = { ii.vertex[i].position.x * coskx - ii.vertex[i].position.y * sinky,
					ii.vertex[i].position.x * sinkx + ii.vertex[i].position.y * cosky };
			}

			for (unsigned char i = 0; i < 4; ++i) {
				ii.vertex[i].position += { lx, ly };
				ii.vertex[i].color = { 255, 255, 255, (sf::Uint8)la };
			}

		}
		if (this->linearFix) {
			this->text.setString(std::to_string(this->timePoint * this->dataRef->fps));
		}
		else {
			this->text.setString(std::to_string(framePoint));
		}
		return;
	}
	void Animation::updateFrame(size_t frame) {
		//printf_s("update: %llu\n", frame);
		for (size_t i = 0, n = this->dataRef->trackCount; i < n; ++i) {
			ohms::Animation::Track& ii = this->tracks[i];
			ohms::Frame& fr = this->dataRef->trackArray[i]->frames[frame + this->animationOffset];
			if (fr.f) {
				ii.empty = true;
			}
			else {
				ii.empty = false;
				if (fr.i != "") {
					ii.texture.loadFromFile(std::string("assets/test/").append(fr.i).append(".png"));
					ii.texture.setSmooth(true);
					ii.texSize = ii.texture.getSize();
				}
			}
		}
		return;
	}
	void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const {
		if (!this->nice) {
			return;
		}

		target.draw(this->text);

		sf::VertexArray va;
		va.setPrimitiveType(sf::Quads);

		for (size_t i = 0, n = this->dataRef->trackCount; i < n; ++i) {
			if (this->tracks[i].empty) continue;
			va.clear();
			va.append(this->tracks[i].vertex[0]);
			va.append(this->tracks[i].vertex[1]);
			va.append(this->tracks[i].vertex[3]);
			va.append(this->tracks[i].vertex[2]);
			states.texture = &this->tracks[i].texture;
			target.draw(va, states);
		}
		return;
	}
	void test() {
		FILE* fp;
		fopen_s(&fp, "assets/test/cattail.reanim", "r");
		if (fp == 0) {
			printf_s("Open reanim file failed!\n");
			return;
		}
		XmlEntity xml;
		for (int i = 0; i < 100; ++i) {
			xml = getNextEntity(&fp);
			if (xml.isCommand) {
				if (xml.commandEnd) {
					printf_s("Command end: %s\n", xml.value.c_str());
				}
				else {
					printf_s("Command: %s\n", xml.value.c_str());
				}
			}
			else {
				printf_s("Value: %s\n", xml.value.c_str());
			}
		}
		fclose(fp);
		fp = 0;
		return;
	}
	AnimationExportor::AnimationExportor() {
	}
} // end namespace ohms
