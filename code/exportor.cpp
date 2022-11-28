#include "exportor.h"

namespace ohms {
	void TexturePacker::setOutputPath(const std::wstring& path) {
		this->outputPath = path;
		return;
	}
	void TexturePacker::addInputImage(const std::wstring& path) {
		this->images.push_back(path);
		return;
	}
	void TexturePacker::pack() {
	}
	Spine3875Exportor::Spine3875Exportor(ohms::AnimationData* data) {
		this->dataRef = data;
		this->groundTrack = nullptr;
		return;
	}
	bool Spine3875Exportor::spineExport(const std::string& folder, bool takeTextures, bool packTextures) {
		this->scanTracks();
		this->outputJson();
		return false;
	}
	void Spine3875Exportor::scanTracks() {
		// 扫描 所有 Track
		for (ohms::Track* it : this->dataRef->trackArray) {

			if (it->name.find("_ground") != 0xffffffffffffffff) {
				this->groundTrack = it;
				continue;
			}

			if(it->isCtrlTrack) {
				CTRLTracksData* track;
				this->ctrlTracks.emplace_back();
				track = &(this->ctrlTracks.back());

				track->name = it->name;
				track->trackRef = it;

				track->offset = 0;
				track->length = 0;
				while ((track->offset < this->dataRef->frameCount) && it->frames[track->offset].f)
					++track->offset;
				track->length = track->offset;
				while ((track->length < this->dataRef->frameCount) && !(it->frames[track->length].f))
					++track->length;
				track->length = track->length - track->offset;
			}
			AnimationTrackData* track;
			this->animationTracks.emplace_back();
			track = &(this->animationTracks.back());

			track->name = it->name;
			track->trackRef = it;

			for (ohms::Frame& ii : it->frames) {
				if (ii.i != "") {
					this->textures.emplace(ii.i);
					track->attachments.emplace_back(ii.i);
				}
			}
		}
		return;
	}
	void Spine3875Exportor::outputJson() {
		Json::Value root, tmp;

		Json::Value& skeleton = root["skeleton"];
		Json::Value& bones = root["bones"];
		Json::Value& slots = root["slots"];
		Json::Value& skins = root["skins"];
		Json::Value& animations = root["animations"];

		// skeleton
		//skeleton["hash"] = "dFlyoVZu35f6oAiN3IhjHZ+10xE";
		skeleton["spine"] = "3.8.75";
		//skeleton["x"] = -89.24;
		//skeleton["y"] = -21.89;
		//skeleton["width"] = 147.87;
		//skeleton["height"] = 146.57;
		skeleton["fps"] = this->dataRef->fps;

		CTRLTracksData* idle_animation = nullptr;
		for (CTRLTracksData& it : this->ctrlTracks) {
			if (it.name == "anim_idle") {
				idle_animation = &it;
			}
		}

		float stdx = 0.0f, stdy = 0.0f, stdsx = 1.0f, stdsy = 1.0f, stdkx = 0.0f, stdky = 0.0f;

		// bones
		// slots
		tmp.clear();
		tmp["name"] = "root";
		bones.append(tmp);
		tmp.clear();
		tmp["name"] = "offset_bone";
		tmp["parent"] = "root";
		bones.append(tmp);
		for (AnimationTrackData& it : this->animationTracks) {
			// 添加 bones 信息
			tmp.clear();
			tmp["name"] = it.name;
			tmp["parent"] = "offset_bone";
			tmp["length"] = 8.0f;

			/*
			if (idle_animation) {
				tmp["x"] = stdx = it.trackRef->frames[idle_animation->offset].x;
				tmp["y"] = stdy = -it.trackRef->frames[idle_animation->offset].y;
				tmp["scaleX"] = stdsx = it.trackRef->frames[idle_animation->offset].sx;
				tmp["scaleY"] = stdsy = it.trackRef->frames[idle_animation->offset].sy;
				tmp["shearX"] = stdkx = -it.trackRef->frames[idle_animation->offset].kx;
				tmp["shearY"] = stdky = -it.trackRef->frames[idle_animation->offset].ky;
			}*/

			bones.append(tmp);
			// 添加 slots 信息
			tmp.clear();
			tmp["name"] = it.name;
			tmp["bone"] = it.name;
			if (it.attachments.size()) {
				if (it.attachments.size() == 1) {
					tmp["attachment"] = it.attachments.front();
				}
			}
			slots.append(tmp);
		}

		// skins
		tmp.clear();
		tmp["name"] = "default";
		for (AnimationTrackData& it : this->animationTracks) {
			for (std::string& ii : it.attachments) {
				sf::Image im;
				im.loadFromFile(std::string("assets/test/").append(ii).append(".png"));
				tmp["attachments"][it.name][ii]["width"] = im.getSize().x;
				tmp["attachments"][it.name][ii]["height"] = im.getSize().y;
				tmp["attachments"][it.name][ii]["x"] = im.getSize().x / 2.0f;
				tmp["attachments"][it.name][ii]["y"] = -(im.getSize().y / 2.0f);
			}
		}
		skins.append(tmp);

		// animations
		float frameTime = 1.0f / this->dataRef->fps;
		for (CTRLTracksData& it : this->ctrlTracks) {
			Json::Value& animation = animations[it.name];
			for (AnimationTrackData& ii : this->animationTracks) {
				Track* track = ii.trackRef;
				for (size_t i = 0, n = it.length; i < n; ++i) {
					size_t absF = it.offset + i;
					float time = frameTime * i;

					if (!track->frames[absF].f) {
						// bones
						if ((i == 0) || (i > 0 &&
							(track->frames[absF].x != track->frames[absF - 1].x ||
								track->frames[absF].y != track->frames[absF - 1].y))) {
							tmp.clear();
							if (time != 0.0f)
								tmp["time"] = time;
							tmp["x"] = track->frames[absF].x - stdx;
							tmp["y"] = -track->frames[absF].y - stdy;
							animation["bones"][ii.name]["translate"].append(tmp);
						}
						if ((i == 0) || (i > 0 &&
							(track->frames[absF].sx != track->frames[absF - 1].sx ||
								track->frames[absF].sy != track->frames[absF - 1].sy))) {
							tmp.clear();
							if (time != 0.0f)
								tmp["time"] = time;
							tmp["x"] = track->frames[absF].sx / stdsx;
							tmp["y"] = track->frames[absF].sy / stdsy;
							animation["bones"][ii.name]["scale"].append(tmp);
						}
						if ((i == 0) || (i > 0 &&
							(track->frames[absF].kx != track->frames[absF - 1].kx ||
								track->frames[absF].ky != track->frames[absF - 1].ky))) {
							tmp.clear();
							if (time != 0.0f)
								tmp["time"] = time;
							tmp["x"] = -track->frames[absF].kx - stdkx;
							tmp["y"] = -track->frames[absF].ky - stdky;
							animation["bones"][ii.name]["shear"].append(tmp);
						}
					}

					// slots
					if (!track->frames[absF].f) {
						if (track->frames[absF].i != "") {
							tmp.clear();
							if (time != 0.0f)
								tmp["time"] = time;
							tmp["name"] = track->frames[absF].i;
							animation["slots"][ii.name]["attachment"].append(tmp);
						}
					}
					else if (i > 0 && !track->frames[absF - 1].f) {
						tmp.clear();
						if (time != 0.0f)
							tmp["time"] = time;
						tmp["name"];
						animation["slots"][ii.name]["attachment"].append(tmp);
					}

					if (i > 0 &&
						((track->frames[absF].a != track->frames[absF - 1].a) || (track->frames[absF].a != 1.0f))) {
						tmp.clear();
						if (time != 0.0f)
							tmp["time"] = time;
						unsigned short clr = static_cast<unsigned short>(track->frames[absF].a * 255.1f);
						unsigned short ch;
						std::string clrstr = "ffffff";
						ch = clr / 16;
						clrstr.push_back(ch > 9? 'a' + ch - 10 : '0' + ch);
						ch = clr % 16;
						clrstr.push_back(ch > 9? 'a' + ch - 10 : '0' + ch);
						tmp["color"] = clrstr;
						animation["slots"][ii.name]["attachment"].append(tmp);
					}
				}
			}
		}


		Json::StreamWriterBuilder wbuilder;
		wbuilder["indentation"] = " ";
		wbuilder["precision"] = 6;
		wbuilder["decimal"] = "decimal";
		std::string document = Json::writeString(wbuilder, root);

		FILE* fp;
		fopen_s(&fp, "assets/output.json", "w");
		if (fp == 0) {
			printf_s("%s", document.c_str());
			return;
		}
		fprintf_s(fp, "%s", document.c_str());
		fclose(fp);
		return; 
	}
} // end namspace ohms
