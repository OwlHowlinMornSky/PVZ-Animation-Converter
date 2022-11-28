#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <list>
#include <map>

namespace {
	/// <summary>
	/// XML 对象
	/// </summary>
	struct XmlEntity {
		/// <summary>
		/// 是 XML 指令则为 true
		/// 默认为 false
		/// </summary>
		bool isCommand = false;
		/// <summary>
		/// 是 XML 结束指令则为 true
		/// 默认为 false
		/// </summary>
		bool commandEnd = false;
		/// <summary>
		/// XML 对象的值, 用字符串保存
		/// 默认为 空字符串
		/// </summary>
		std::string value = "";
	};
	/// <summary>
	/// 从文件获取下一个 XML 对象
	/// </summary>
	/// <param name="fpp">文件指针的指针</param>
	/// <returns>XML 对象</returns>
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
	/// 动画的一帧的数据
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
		/// 从文件读取 帧数据
		/// </summary>
		/// <param name="fpp">文件指针的指针</param>
		/// <param name="lastOne">上一帧数据</param>
		void readFrame(FILE** fpp, Frame lastOne);
	private:
		/// <summary>
		/// 0 或 -1
		/// 是否为空白帧
		/// -1 时为空白帧, 默认为 0
		/// </summary>
		bool f;
		/// <summary>
		/// 数字
		/// 图像 X 坐标
		/// 与 FLASH 里对应 (一位小数)
		/// </summary>
		float x;
		/// <summary>
		/// 数字
		/// 图像 Y 坐标
		/// 与 FLASH 里对应 (一位小数)
		/// </summary>
		float y;
		/// <summary>
		/// 数字
		/// X 轴方向倾斜度数
		/// 与 FLASH 里对应 (一位小数)
		/// </summary>
		float kx;
		/// <summary>
		/// 数字
		/// Y轴方向倾斜度数
		/// 与 FLASH 里对应 (一位小数)
		/// </summary>
		float ky;
		/// <summary>
		/// 数字
		/// X 轴方向缩放百分比
		/// 与 FLASH 里对应 (三位小数)
		/// </summary>
		float sx;
		/// <summary>
		/// 数字
		/// Y 轴方向缩放百分比
		/// 与 FLASH 里对应 (三位小数)
		/// </summary>
		float sy;
		/// <summary>
		/// IMAGE_REANIM_图像名称
		/// 所用图像名称, 全大写
		/// 必须要在 reanim 文件夹中存在
		/// 在第一帧说明图像名称后无需再次说明, 除非图像被改变
		/// </summary>
		std::string i;
		/// <summary>
		/// 数字
		/// Alpha 百分比
		/// 与 FLASH 里对应
		/// 取值范围在 0 到 1 的两位小数, 包括 0 和 1
		/// </summary>
		float a;
		/// <summary>
		/// 表示读取是否成功
		/// </summary>
		bool nice;
		//bool isReallyEmpty;
	};
	/// <summary>
	/// 动画一个轨道的数据
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
		/// 从文件读取一个轨道的数据
		/// </summary>
		/// <param name="fpp">文件指针的指针</param>
		void readTrack(FILE** fpp);
	private:
		/// <summary>
		/// 轨道的所有帧
		/// </summary>
		std::vector<ohms::Frame> frames;
		/// <summary>
		/// 轨道的名称
		/// </summary>
		std::string name;
		/// <summary>
		/// 轨道读取是否成功
		/// </summary>
		bool nice;
		bool isCtrlTrack;
	};
	/// <summary>
	/// 整个动画的数据
	/// </summary>
	class AnimationData {
		friend class Animation;
		friend class AnimationExportor;
		friend class Spine3875Exportor;
	public:
		~AnimationData();
		AnimationData();
		/// <summary>
		/// 从文件加载 PVZ 动画
		/// </summary>
		/// <param name="path">文件路径</param>
		void load(const std::string& path);
	private:
		/// <summary>
		/// 清空
		/// </summary>
		void clear();
	private:
		/// <summary>
		/// 动画 每秒帧数
		/// </summary>
		size_t fps;
		/// <summary>
		/// 动画 总帧数
		/// </summary>
		size_t frameCount;
		/// <summary>
		/// 动画 总轨道数
		/// </summary>
		size_t trackCount;
		/// <summary>
		/// 轨道指针 序列
		/// </summary>
		std::vector<ohms::Track*> trackArray;
		/// <summary>
		/// 轨道名称 到 轨道指针 的 映射
		/// </summary>
		std::map<std::string, ohms::Track*> tracks;
		/// <summary>
		/// 加载是否成功
		/// </summary>
		bool nice;
	};

	class Animation : public sf::Drawable {
	private:
		struct Track {
			/// <summary>
			/// 纹理
			/// updateFrame 时更新
			/// </summary>
			sf::Texture texture;
			/// <summary>
			/// 纹理大小
			/// updateFrame 时更新
			/// </summary>
			sf::Vector2u texSize;
			/// <summary>
			/// 四个顶点
			/// </summary>
			sf::Vertex vertex[4];
			/// <summary>
			/// 是否为空
			/// </summary>
			bool empty = false;
		};
	public:
		/// <summary>
		/// 加载一个动画实例
		/// </summary>
		/// <param name="data">加载好的动画数据 的地址</param>
		/// <param name="linearFix">是否线性插值</param>
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
