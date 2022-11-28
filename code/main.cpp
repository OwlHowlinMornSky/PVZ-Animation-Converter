#include "animation.h"
#include "exportor.h"
#include <sfml/Audio.hpp>
#include "json.h"

void test();

int main() {


	ohms::AnimationData testdata;

	//testdata.load("assets/test/Zombie.reanim");
	//testdata.load("assets/test/Zombie_polevaulter.reanim");
	//testdata.load("assets/test/Zombie_pogo.reanim");
	//testdata.load("assets/test/Cabbagepult.reanim");
	//testdata.load("assets/test/Blover.reanim");
	//testdata.load("assets/test/CrazyDave.reanim");
	testdata.load("assets/test/Sunflower.reanim");

	//ohms::Spine3875Exportor exportor(&testdata);
	//exportor.spineExport("");
	//return 0;

	ohms::Animation test(&testdata, true);

	//if (!test.setAnimation("anim_eat")) {
	//if (!test.setAnimation("anim_swim")) {
	//if (!test.setAnimation("anim_run")) {
	//if (!test.setAnimation("anim_pogo")) {
	//if (!test.setAnimation("anim_shooting")) {
	//if (!test.setAnimation("anim_loop")) {
	if (!test.setAnimation("anim_idle")) {

		return 0;
	}
	float timeScale = 1.0f;


	/*sf::SoundBuffer sb;
	sb.loadFromFile("assets/jackinthebox.ogg");
	sf::Sound sound;
	sound.setBuffer(sb);
	sound.setLoop(true);
	sound.play();*/


	sf::RectangleShape line;
	line.setSize({ 1000.0f, 1.0f });
	line.setOrigin(500.0f, 0.5f);
	line.setOutlineThickness(0.0f);

	sf::Transform transform;
	transform.translate(250.0f, 200.0f);
	transform.translate(50.0f, 0.0f);
	transform.scale(2.0f, 2.0f);

	float frameTime = 1.0f / 12.0f;

	sf::RenderWindow window;
	window.create(sf::VideoMode(800, 800), "PVZ Animation");
	window.setFramerateLimit(60);
	sf::Event event;
	sf::Clock clock;
	float dt;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			/*
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Space) {
					test.update(frameTime);
				}
			}*/
		}

		dt = timeScale * clock.restart().asSeconds();
		test.update(dt);

		window.clear();
		line.setRotation(0.0f);
		window.draw(line, { transform });
		line.setRotation(90.0f);
		window.draw(line, { transform });
		window.draw(test, { transform });
		window.display();
	}

	return 0;
}

void test() {

	Json::Value root;

	Json::Value& p = root["bones"];

	Json::Value tmp;

	p["a"] = 123;

	tmp["name"] = "123";

	p["test"].append(tmp);

	tmp["name"] = "456";
	p["test"].append(tmp);

	Json::StreamWriterBuilder wbuilder;
	wbuilder["indentation"] = " ";
	std::string document = Json::writeString(wbuilder, root);

	printf_s("%s", document.c_str());
	return;
}
