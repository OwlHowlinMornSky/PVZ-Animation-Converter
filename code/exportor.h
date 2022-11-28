#pragma once
#include "animation.h"
#include "jsonio.h"
#include <set>

namespace ohms {
	class TexturePacker {
	public:
		void setOutputPath(const std::wstring& path);
		void addInputImage(const std::wstring& path);
		void pack();
	private:
		std::wstring outputPath;
		std::list<std::wstring> images;
	};
	class Spine3875Exportor : public ohms::AnimationExportor {
	private:
		struct CTRLTracksData {
			std::string name;
			ohms::Track* trackRef;
			size_t offset, length;
		};
		struct AnimationTrackData {
			std::string name;
			ohms::Track* trackRef;
			std::list<std::string> attachments;
		};
	public:
		Spine3875Exportor(ohms::AnimationData* data);
		virtual bool spineExport(const std::string& folder, bool takeTextures = true, bool packTextures = false) override;
	private:
		virtual void scanTracks() override;
		virtual void outputJson() override;
	private:
		ohms::AnimationData* dataRef;
		std::set<std::string> textures;
		std::list<CTRLTracksData> ctrlTracks;
		std::list<AnimationTrackData> animationTracks;
		ohms::Track* groundTrack;
	};
} // end namespace ohms
