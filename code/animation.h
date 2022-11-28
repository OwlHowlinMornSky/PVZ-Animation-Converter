#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <list>
#include <map>

namespace {
	/// <summary>
	/// XML ����
	/// </summary>
	struct XmlEntity {
		/// <summary>
		/// �� XML ָ����Ϊ true
		/// Ĭ��Ϊ false
		/// </summary>
		bool isCommand = false;
		/// <summary>
		/// �� XML ����ָ����Ϊ true
		/// Ĭ��Ϊ false
		/// </summary>
		bool commandEnd = false;
		/// <summary>
		/// XML �����ֵ, ���ַ�������
		/// Ĭ��Ϊ ���ַ���
		/// </summary>
		std::string value = "";
	};
	/// <summary>
	/// ���ļ���ȡ��һ�� XML ����
	/// </summary>
	/// <param name="fpp">�ļ�ָ���ָ��</param>
	/// <returns>XML ����</returns>
	XmlEntity getNextEntity(FILE** fpp);
}

namespace ohms {

	class AnimationExportor {
	public:
		AnimationExportor();
		virtual bool spineExport(const std::string& folder, bool takeTextures = true, bool packTextures = false) = 0;
	private:
		virtual void scanTracks() = 0;
		virtual void outputJson() = 0;
	};

	/// <summary>
	/// ������һ֡������
	/// </summary>
	class Frame {
		friend class Track;
		friend class AnimationData;
		friend class Animation;
		friend class AnimationExportor;
		friend class Spine3875Exportor;
	public:
		Frame();
		~Frame();
	protected:
		/// <summary>
		/// ���ļ���ȡ ֡����
		/// </summary>
		/// <param name="fpp">�ļ�ָ���ָ��</param>
		/// <param name="lastOne">��һ֡����</param>
		void readFrame(FILE** fpp, Frame lastOne);
	private:
		/// <summary>
		/// 0 �� -1
		/// �Ƿ�Ϊ�հ�֡
		/// -1 ʱΪ�հ�֡, Ĭ��Ϊ 0
		/// </summary>
		bool f;
		/// <summary>
		/// ����
		/// ͼ�� X ����
		/// �� FLASH ���Ӧ (һλС��)
		/// </summary>
		float x;
		/// <summary>
		/// ����
		/// ͼ�� Y ����
		/// �� FLASH ���Ӧ (һλС��)
		/// </summary>
		float y;
		/// <summary>
		/// ����
		/// X �᷽����б����
		/// �� FLASH ���Ӧ (һλС��)
		/// </summary>
		float kx;
		/// <summary>
		/// ����
		/// Y�᷽����б����
		/// �� FLASH ���Ӧ (һλС��)
		/// </summary>
		float ky;
		/// <summary>
		/// ����
		/// X �᷽�����Űٷֱ�
		/// �� FLASH ���Ӧ (��λС��)
		/// </summary>
		float sx;
		/// <summary>
		/// ����
		/// Y �᷽�����Űٷֱ�
		/// �� FLASH ���Ӧ (��λС��)
		/// </summary>
		float sy;
		/// <summary>
		/// IMAGE_REANIM_ͼ������
		/// ����ͼ������, ȫ��д
		/// ����Ҫ�� reanim �ļ����д���
		/// �ڵ�һ֡˵��ͼ�����ƺ������ٴ�˵��, ����ͼ�񱻸ı�
		/// </summary>
		std::string i;
		/// <summary>
		/// ����
		/// Alpha �ٷֱ�
		/// �� FLASH ���Ӧ
		/// ȡֵ��Χ�� 0 �� 1 ����λС��, ���� 0 �� 1
		/// </summary>
		float a;
		/// <summary>
		/// ��ʾ��ȡ�Ƿ�ɹ�
		/// </summary>
		bool nice;
		//bool isReallyEmpty;
	};
	/// <summary>
	/// ����һ�����������
	/// </summary>
	class Track {
		friend class AnimationData;
		friend class Animation;
		friend class AnimationExportor;
		friend class Spine3875Exportor;
	public:
		~Track();
	protected:
		Track();
		/// <summary>
		/// ���ļ���ȡһ�����������
		/// </summary>
		/// <param name="fpp">�ļ�ָ���ָ��</param>
		void readTrack(FILE** fpp);
	private:
		/// <summary>
		/// ���������֡
		/// </summary>
		std::vector<ohms::Frame> frames;
		/// <summary>
		/// ���������
		/// </summary>
		std::string name;
		/// <summary>
		/// �����ȡ�Ƿ�ɹ�
		/// </summary>
		bool nice;
		bool isCtrlTrack;
	};
	/// <summary>
	/// ��������������
	/// </summary>
	class AnimationData {
		friend class Animation;
		friend class AnimationExportor;
		friend class Spine3875Exportor;
	public:
		~AnimationData();
		AnimationData();
		/// <summary>
		/// ���ļ����� PVZ ����
		/// </summary>
		/// <param name="path">�ļ�·��</param>
		void load(const std::string& path);
	private:
		/// <summary>
		/// ���
		/// </summary>
		void clear();
	private:
		/// <summary>
		/// ���� ÿ��֡��
		/// </summary>
		size_t fps;
		/// <summary>
		/// ���� ��֡��
		/// </summary>
		size_t frameCount;
		/// <summary>
		/// ���� �ܹ����
		/// </summary>
		size_t trackCount;
		/// <summary>
		/// ���ָ�� ����
		/// </summary>
		std::vector<ohms::Track*> trackArray;
		/// <summary>
		/// ������� �� ���ָ�� �� ӳ��
		/// </summary>
		std::map<std::string, ohms::Track*> tracks;
		/// <summary>
		/// �����Ƿ�ɹ�
		/// </summary>
		bool nice;
	};

	class Animation : public sf::Drawable {
	private:
		struct Track {
			/// <summary>
			/// ����
			/// updateFrame ʱ����
			/// </summary>
			sf::Texture texture;
			/// <summary>
			/// �����С
			/// updateFrame ʱ����
			/// </summary>
			sf::Vector2u texSize;
			/// <summary>
			/// �ĸ�����
			/// </summary>
			sf::Vertex vertex[4];
			/// <summary>
			/// �Ƿ�Ϊ��
			/// </summary>
			bool empty = false;
		};
	public:
		/// <summary>
		/// ����һ������ʵ��
		/// </summary>
		/// <param name="data">���غõĶ������� �ĵ�ַ</param>
		/// <param name="linearFix">�Ƿ����Բ�ֵ</param>
		Animation(const AnimationData* data, bool linearFix = true);
		~Animation();

		bool setAnimation(const std::string& name);
		void update(float dt);
	protected:
		void updateFrame(size_t frame);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		const AnimationData* dataRef;
		ohms::Animation::Track* tracks;
		ohms::Track* animationNow;
		size_t trackCount, lastFramePoint, animationOffset, animationLength;
		float timePoint;
		sf::Text text;
		sf::Font font;
		bool linearFix;
		bool nice;
	};

	void test();

} // end namespace ohms
