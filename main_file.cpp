
/*
Projekt Zaliczeniowy Grafika Komputerowa i Wizualizacja 2021
Mateusz Olewnik i Mariusz Nowak
Prowadzący: dr inż. Witold Andrzejewski 
*/

/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "Object3D.h"
#include <Visitor.h>
#include <Exhibit.h>
#include <MainDrawingMethod.h>
#include <SecondMethodDrawing.h>
#include <RoomDrawingMethod.h>
#include <SkyDrawingMethod.h>
#include <LionDrawingMethod.h>
#include <EnvmapDrawingMethod.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <time.h>


//Speed+window parameters
float speed_x = 0;
float speed_y = 0;
float visitor_speed = 1;
float aspectRatio = 1;
float walk_speed = 0;
int * bussyObj;
int* bussyObj2;
int* bussyObj3;

//FPS free fly
//WASD - obroty kamerą
//Strzalka gora dol = latanie do przodu/tylu

//Polozenie poczatkowe: pozycja gracza i źródeł światła
glm::vec3 pos = glm::vec3(-4, 2.8,-11);
glm::vec4 zrSwiatla = glm::vec4(-4, 3.0, -10, 1);
glm::vec4 zrSwiatla2 = glm::vec4(-4, 3.0, 50, 1);
//glm::vec4 zrSwiatla3 = glm::vec4(-1.0f, 3.0f, 12.9f, 1); 
//glm::vec4 zrSwiatla = glm::vec4(pos, 1);
glm::vec4 sources[2] = {zrSwiatla,zrSwiatla2};

//All Shaders
ShaderProgram* sp;
ShaderProgram* sp_l;
ShaderProgram* sp_main;
ShaderProgram* sp_envmap;

//All models
MainDrawingMethod bear("assets/statues/BlackBear.obj");
MainDrawingMethod cer("assets/statues/cer.obj");
RoomDrawingMethod room("assets/gallery/Museum.obj");
SkyDrawingMethod sky("assets/scene/Egg.obj");
MainDrawingMethod monke("assets/statues/monke.obj");
MainDrawingMethod dino("assets/statues/dino.obj");
MainDrawingMethod cube("assets/paintings/canvas.obj");
MainDrawingMethod dog("assets/statues/doge.obj");
MainDrawingMethod coliseum("assets/statues/minikoloseum.obj");
RoomDrawingMethod corridor("assets/gallery/corridor.obj");
RoomDrawingMethod  transition("assets/gallery/transition.obj");
MainDrawingMethod parquetry("assets/paintings/canvas.obj");
MainDrawingMethod postument("assets/gallery/postument.obj");
MainDrawingMethod door("assets/gallery/door.obj");
MainDrawingMethod easter("assets/paintings/canvas.obj"),egg("assets/paintings/canvas.obj");
MainDrawingMethod visitorTom("assets/scene/character.obj"), visitorAlice("assets/scene/character.obj"), visitorJack("assets/scene/character.obj");
LionDrawingMethod lion("assets/statues/lion.obj"), brain("assets/statues/brain.obj"),
frameB("assets/paintings/fancyframe.obj"),
statue1("assets/statues/classicStatue.obj"), thinker("assets/statues/thinker.obj");

//Room1
MainDrawingMethod  painting("assets/paintings/canvas.obj"), frame("assets/paintings/frame.obj"), frameAlt("assets/paintings/frame.obj") ;
MainDrawingMethod  painting2("assets/paintings/canvas.obj"), exitSign("assets/paintings/canvas.obj");
MainDrawingMethod  painting3("assets/paintings/canvas.obj");
MainDrawingMethod  painting4("assets/paintings/canvas.obj");
MainDrawingMethod  painting5("assets/paintings/canvas.obj");

//Room2
MainDrawingMethod  painting2_1("assets/paintings/canvas.obj"), painting2_2("assets/paintings/canvas.obj");

//Room3
MainDrawingMethod  painting3_1("assets/paintings/canvas.obj");
MainDrawingMethod  painting3_2("assets/paintings/canvas.obj");
MainDrawingMethod  painting3_3("assets/paintings/canvas.obj");
MainDrawingMethod  painting3_4("assets/paintings/canvas.obj");
MainDrawingMethod  painting3_5("assets/paintings/canvas.obj");

//Room4
MainDrawingMethod  genie("assets/ceramics/genie.obj");
MainDrawingMethod  jug("assets/ceramics/jug.obj");
MainDrawingMethod  vase1("assets/ceramics/vase1.obj"), vase2("assets/ceramics/vase2.obj"),
vase3("assets/ceramics/vase3.obj"), vase4("assets/ceramics/vase4.obj");
MainDrawingMethod  painting4_1("assets/paintings/canvas.obj"), painting4_2("assets/paintings/canvas.obj"), painting4_3("assets/paintings/canvas.obj"),
painting4_4("assets/paintings/canvas.obj"), painting4_5("assets/paintings/canvas.obj");



Visitor visitor(&visitorTom, -7.68f, 1.0f, -10.0f, 0.016);
Exhibit exhibitions[] = {
	Exhibit(-3.0f, 1.0f, -5.0f, canStay::LEFT, 1),
	Exhibit(1.8f, 1.0f, 3.0f, canStay::LEFT, 1),
	Exhibit(0.3f, 1.0f, -4.8f, canStay::LEFT, 1)


};

Visitor visitor2(&visitorAlice,0.68f, 1.0f, 16.0f, 0.016);
Exhibit exhibitions2[] = {
	Exhibit(-3.87f, 1.0f, 18.0f, canStay::LEFT, 1),
	Exhibit(-3.0f, 1.0f, 16.0f, canStay::LEFT, 1),
	Exhibit(-5.3f, 1.0f, 14.0f, canStay::RIGHT, 1)


};

Visitor visitor3(&visitorJack, -3.0f, 1.0f, 38.0f, 0.016);
Exhibit exhibitions3[] = {
	Exhibit(0.70f, 1.0f, 48.9f, canStay::RIGHT, 1),
	Exhibit(-1.70f, 1.0f, 43.9f, canStay::LEFT, 1),
	Exhibit(-4.70f, 1.0f, 41.9f, canStay::RIGHT, 1)


};


glm::vec3 calcDir(float kat_x, float kat_y) {
	glm::vec4 dir = glm::vec4(0, 0, 1, 0);
	glm::mat4 M = glm::rotate(glm::mat4(1.0f), kat_y, glm::vec3(0, 1, 0));
	M = glm::rotate(M, kat_x, glm::vec3(1, 0, 0));
	dir = M * dir;
	return glm::vec3(dir); //wlasnosc wektora 3 liczbowego ucina wspolrzedne w z czteroliczbowych
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

void keyCallback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_A) speed_y = 1;
		if (key == GLFW_KEY_D) speed_y = -1;
		if (key == GLFW_KEY_S) speed_x = 1;
		if (key == GLFW_KEY_W) speed_x = -1;
		if (key == GLFW_KEY_UP) walk_speed = 7;
		if (key == GLFW_KEY_DOWN) walk_speed = -7;

	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_A) speed_y = 0;
		if (key == GLFW_KEY_D) speed_y = 0;
		if (key == GLFW_KEY_S) speed_x = 0;
		if (key == GLFW_KEY_W) speed_x = 0;
		if (key == GLFW_KEY_UP) walk_speed = 0;
		if (key == GLFW_KEY_DOWN) walk_speed = 0;
	}
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}


void allDrawInOnePlace(glm::mat4 P, glm::mat4 V, glm::mat4 M) {
	//Museum parts+scene
	room.drawModel(sp_l, P, V, M, sources, 3.0f, 1.0f, -15.0f, 180.0f, 0.002f, 0.0045f, 0.004f);
	room.drawModel(sp_l, P, V, M, sources, -9.0f, 1.0f, 58.0f, 360.0f, 0.002f, 0.0045f, 0.003f);
	sky.drawModel(sp_l, P, V, M, sources, -3.0f, -40.0f, 20.0f, 360.0f, 1.0f, 1.6, 1.6);
	corridor.drawModel(sp_main, P, V, M, sources, -3.0f, 1.0f, 26.5f, 0.0f, 0.51f, 0.47f, 0.5f);
	transition.drawModel(sp_l, P, V, M, sources,-8.4f, 3.25f, 4.9f, -90.0f, 0.6f, 0.34f,0.36f);
	transition.drawModel(sp_l, P, V, M, sources, 2.4f, 3.25f, 43.0f, 90.0f, 0.6f, 0.34f, 0.36f);
	parquetry.drawModel(sp_main, P, V, M, sources, -3.0f, 1.0f,25.0f, 0.0f, 12.0f, 0.1f, 80.7f);
	door.drawModel(sp_l, P, V, M, sources, -3.6f, 1.05f, -14.90f, 0.0f, 0.0071f, 0.0071f, 0.0071f);
	door.drawModel(sp_l, P, V, M, sources, -2.47f, 1.05f, -14.965f, -180.0f, 0.0071f, 0.0071f, 0.0071f);
	door.drawModel(sp_l, P, V, M, sources, 2.95f, 1.05f, 55.0f, 270.0f, 0.0071f, 0.0071f, 0.0071f);
	exitSign.drawModel(sp_l, P, V, M, sources, 2.98f, 2.7f, 53.6f, 270.0f, 0.7f,0.7f, 0.003f);
	
	//Museum statues and postuments
	//Statue1Room1
	cer.drawModel(sp_main, P, V, M, sources, -3.0f, 2.0f, -5.0f, 180.0f, 0.2f, 0.2f, 0.2f);
	postument.drawModel(sp_main, P, V, M, sources, -3.0f, 1.1f, -5.0f, 0.0f, 0.4f, 0.3f, 0.4f);
	//Statue2Room1
	brain.drawModel(sp_main, P, V, M, sources, -3.0f, 2.0f, 0.0f,90.0f, 0.035f, 0.035f, 0.035f);
	postument.drawModel(sp_main, P, V, M, sources, -3.0f, 1.1f, 0.0f, 0.0f, 0.4f, 0.3f, 0.4f);
	
	//Statue1%2Room2
	lion.drawModel(sp_envmap, P, V, M, sources, 1.8f, 1.0f, 3.7f, 180.0f, 0.1f, 0.1f, 0.1f);
	lion.drawModel(sp_envmap, P, V, M, sources, -7.8f, 1.0f, 3.7f, 180.0f, 0.1f, 0.1f, 0.1f);
	//Statue3Room2
	monke.drawModel(sp_l, P, V, M, sources, -7.1f, 2.7f, 21.8f, 170.0f, 0.015f,0.016f, 0.013f);
	cube.drawModel(sp_l, P, V, M, sources, -7.1f, 1.1f, 22.0f, 170.0f, 1.8f, 0.8f, 1.8f);
	//Statue4Room2
	dino.drawModel(sp_l, P, V, M, sources, 1.2f, 2.9f, 21.9f, 190.0f, 0.012f, 0.012f, 0.012f);
	cube.drawModel(sp_l, P, V, M, sources, 1.2f, 1.1f, 21.6f, 190.0f, 1.8f, 0.8f, 1.8f);
	//Statue5Room2
	bear.drawModel(sp_main, P, V, M, sources, -3.0f, 2.0f, 16.0f, -80.0f, 0.07f, 0.07f, 0.07f);
	postument.drawModel(sp_main, P, V, M, sources, -3.0f, 1.1f, 16.0f, 0.0f, 0.4f, 0.3f, 0.4f);
	//Statue6Room2
	dog.drawModel(sp_main, P, V, M, sources, -3.0f, 2.0f, 13.0f, 240.0f, 0.004f, 0.004f, 0.004f);
	postument.drawModel(sp_main, P, V, M, sources, -3.0f, 1.1f, 13.0f, 0.0f, 0.4f, 0.3f, 0.4f);

	//Statue1Room3
	coliseum.drawModel(sp_l, P, V, M, sources, -3.0f, 2.0f, 37.0f, 0.0f, 0.0009f, 0.0009f, 0.0009f);
	cube.drawModel(sp_l, P, V, M, sources, -3.0f, 1.1f, 37.0f, 0.0f, 2.3f, 1.5f, 2.3f);
	//Statue2Room3
	statue1.drawModel(sp_l, P, V, M, sources, 1.7f, 1.1f, 41.0f, 190.0f, 0.015f, 0.015f, 0.015f);
	//Statue3Room3
	thinker.drawModel(sp_l, P, V, M, sources, -8.1f, 1.0f, 41.0f, 140.0f, 0.007f, 0.007f, 0.007f);

	//Ceramics1Room4
	genie.drawModel(sp_main, P, V, M, sources, 1.5f, 1.95f, 46.0f, 0.0f, 0.4f, 0.4f, 0.4f);
	postument.drawModel(sp_main, P, V, M, sources, 1.5f, 1.1f, 46.0f, 0.0f, 0.4f, 0.3f, 0.4f);
	//Ceramics2Room4
	jug.drawModel(sp_main, P, V, M, sources, 1.25f, 1.97f, 47.7f, 40.0f, 0.03f, 0.03f, 0.03f);
	postument.drawModel(sp_main, P, V, M, sources, 1.5f, 1.1f, 48.0f, 0.0f, 0.4f, 0.3f, 0.4f);
	//Ceramics3Room4
	vase1.drawModel(sp_l, P, V, M, sources, 1.5f, 2.0f, 50.0f, 0.0f, 0.014f, 0.014f, 0.014f);
	postument.drawModel(sp_main, P, V, M, sources, 1.5f, 1.1f, 50.0f, 0.0f, 0.4f, 0.3f, 0.4f);
	//Ceramics4Room4
	vase2.drawModel(sp_l, P, V, M, sources, -7.47f, 2.0f, 46.1f, 0.0f, 0.010f, 0.010f, 0.010f);
	postument.drawModel(sp_main, P, V, M, sources, -7.5f, 1.1f, 46.0f, 0.0f, 0.4f, 0.3f, 0.4f);
	//Ceramics5Room4
    vase3.drawModel(sp_main, P, V, M, sources, -7.5f, 2.0f, 48.0f, 0.0f, 0.005f, 0.005f, 0.005f);
	postument.drawModel(sp_main, P, V, M, sources, -7.5f, 1.1f, 48.0f, 0.0f, 0.4f, 0.3f, 0.4f);
	//Ceramics6Room4
	vase4.drawModel(sp_l, P, V, M, sources, -7.5f, 2.45f, 50.0f, 0.0f, 2.6f, 2.6f, 2.6f);
	postument.drawModel(sp_main, P, V, M, sources, -7.5f, 1.1f, 50.0f, 0.0f, 0.4f, 0.3f, 0.4f);

	//Painting1Room1
	painting.drawModel(sp_l, P, V, M, sources, 2.68f, 3.0f, -10.0f, 90.0f, 1.0f, 1.0f, 0.003f);
	frame.drawModel(sp_main, P, V, M, sources, 2.8f, 3.0f, -10.0f, 90.0f, 0.5f, 0.5f, 0.5f);
	//Painting2Room1
	painting2.drawModel(sp_l, P, V, M, sources, 2.68f, 3.0f, -6.0f, 90.0f, 1.4f, 1.4f, 0.003f);
	frame.drawModel(sp_main, P, V, M, sources, 2.8f, 3.0f, -6.0f, 90.0f, 0.7f, 0.7f, 0.7f);
	//Painting3Room1
	painting3.drawModel(sp_l, P, V, M, sources, 2.68f, 3.0f, -2.0f, 90.0f, 1.0f, 1.0f, 0.003f);
	frame.drawModel(sp_main, P, V, M, sources, 2.8f, 3.0f, -2.0f, 90.0f, 0.5f, 0.5f, 0.5f);
	//Painting4Room1
	painting4.drawModel(sp_l, P, V, M, sources, -8.78f, 3.1f, -8.0f, 270.0f, 2.6f, 1.5f, 0.003f);
	frame.drawModel(sp_main, P, V, M, sources, -8.9f, 3.1f, -8.0f, 270.0f, 1.2f, 0.7f, 0.7f);
	//Painting5Room1
	painting5.drawModel(sp_l, P, V, M, sources, -8.78f, 3.1f, -2.0f, 270.0f, 2.6f, 1.5f, 0.003f);
	frame.drawModel(sp_main, P, V, M, sources, -8.9f, 3.1f, -2.0f, 270.0f, 1.2f, 0.7f, 0.7f);

	//Painting1Room2
	painting2_1.drawModel(sp_l, P, V, M, sources, 2.77f, 3.2f, 11.0f, 270.0f, 2.3f, 1.8f, 0.003f);
	frameB.drawModel(sp_main, P, V, M, sources, 2.8f, 1.4f, 11.0f, -90.0f, 0.19f, 0.17f, 0.1f);
	//Painting2Room2
	painting2_2.drawModel(sp_l, P, V, M, sources, -8.87f, 3.2f, 11.0f, 270.0f, 2.3f, 1.8f, 0.003f);
	frameB.drawModel(sp_main, P, V, M, sources, -8.9f, 1.4f, 11.0f, 90.0f, 0.19f, 0.17f, 0.1f);
  
	//Painting1Room3
	painting3_1.drawModel(sp_l, P, V, M, sources, 2.68f, 3.0f, 31.0f, 90.0f, 1.0f, 1.0f, 0.003f);
	frame.drawModel(sp_main, P, V, M, sources, 2.8f, 3.0f, 31.0f, 90.0f, 0.5f, 0.5f, 0.5f);
	//Painting2Room3
	painting3_2.drawModel(sp_l, P, V, M, sources, 2.68f, 3.0f, 35.0f, 90.0f, 1.4f, 1.4f, 0.003f);
	frame.drawModel(sp_main, P, V, M, sources, 2.8f, 3.0f, 35.0f, 90.0f, 0.7f, 0.7f, 0.7f);
	//Painting3Room3
	painting3_3.drawModel(sp_l, P, V, M, sources, 2.68f, 3.0f, 39.0f, 90.0f, 1.0f, 1.0f, 0.003f);
	frame.drawModel(sp_main, P, V, M, sources, 2.8f, 3.0f, 39.0f, 90.0f, 0.5f, 0.5f, 0.5f);
	//Painting4Room3
	painting3_4.drawModel(sp_l, P, V, M, sources, -8.78f, 3.1f, 33.0f, 270.0f, 2.6f, 1.5f, 0.003f);
	frame.drawModel(sp_main, P, V, M, sources, -8.9f, 3.1f, 33.0f, 270.0f, 1.2f, 0.7f, 0.7f);
	//Painting5Room3
	painting3_5.drawModel(sp_l, P, V, M, sources, -8.78f, 3.1f, 37.0f, 270.0f, 2.6f, 1.5f, 0.003f);
	frame.drawModel(sp_main, P, V, M, sources, -8.9f, 3.1f, 37.0f, 270.0f, 1.2f, 0.7f, 0.7f);
	//Painting1Room4
	painting4_1.drawModel(sp_l, P, V, M, sources, -6.70f, 3.3f, 57.9f, 0.0f,1.0f, 3.0f, 0.015f);
	frameAlt.drawModel(sp_l, P, V, M, sources, -6.7f, 3.3f, 58.0f, 0.0f, 0.5f, 1.4f, 0.7f);
	//Painting2Room4
	painting4_2.drawModel(sp_l, P, V, M, sources, -8.78f, 3.1f, 54.0f, 270.0f, 2.6f, 1.5f, 0.003f);
	frameAlt.drawModel(sp_main, P, V, M, sources, -8.9f, 3.1f, 54.0f, 270.0f, 1.2f, 0.7f, 0.7f);
	//Painting3Room4
	painting4_3.drawModel(sp_l, P, V, M, sources, 1.70f, 4.3f, 57.9f, 0.0f, 1.0f, 1.0f, 0.03f);
	frameAlt.drawModel(sp_l, P, V, M, sources, 1.7f, 4.3f, 58.0f, 0.0f, 0.5f, 0.5f, 0.5f);
	//Painting4Room4
	painting4_4.drawModel(sp_l, P, V, M, sources, -4.00f, 3.6f, 57.9f, 0.0f, 2.6f, 1.5f, 0.035f);
	frameAlt.drawModel(sp_l, P, V, M, sources, -4.0f, 3.6f, 58.0f, 0.0f, 1.2f, 0.7f, 0.7f);
	//Painting5Room4
	painting4_5.drawModel(sp_l, P, V, M, sources, -1.0f, 2.9f, 57.9f, 0.0f, 1.0f, 1.0f, 0.03f);
	frameAlt.drawModel(sp_l, P, V, M, sources, -1.0f, 2.9f, 58.0f, 0.0f, 0.5f, 0.5f, 0.5f);
	easter.drawModel(sp_l, P, V, M, sources, 9.0f, 2.9f, 53.7f,110.0f, 3.5f, 2.5f, 0.05f);
	egg.drawModel(sp_l, P, V, M, sources, 9.0f, 2.9f, 57.3f, 70.0f, 4.0f, 2.5f, 0.05f);

    //Visitors with simple "AI"

	  bussyObj = visitor.moveTo(
			visitor_speed,
			sp_l, P, V, M, sources,
		  0.13, 0.13, 0.13, exhibitions, 3);
		exhibitions[*bussyObj].placeToWatch[*(bussyObj +1)][0] = *(bussyObj + 2);

	   bussyObj2 = visitor2.moveTo(
			visitor_speed,
			sp_l, P, V, M, sources,
			0.13, 0.13, 0.13, exhibitions2, 3);
		exhibitions[*bussyObj2].placeToWatch[*(bussyObj2 + 1)][0] = *(bussyObj2 + 2);

		bussyObj3 = visitor3.moveTo(
			visitor_speed,
			sp_l, P, V, M, sources,
			0.13, 0.13, 0.13, exhibitions3, 3);
		exhibitions[*bussyObj3].placeToWatch[*(bussyObj3 + 1)][0] = *(bussyObj3 + 2);

	}



//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0.533, 0.921, 0.964, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);

	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
	sp_l = new ShaderProgram("v_lambert.glsl", NULL, "f_lambert.glsl");
	sp_main = new ShaderProgram("v_manysources.glsl", NULL, "f_manysources.glsl");
	sp_envmap = new ShaderProgram("v_envmap.glsl", NULL, "f_envmap.glsl");

	GLuint steelTex = readTexture("assets/materials/steel.png");
	GLuint wallTex = readTexture("assets/materials/wallwhite.png");
	GLuint skyTex = readTexture("assets/materials/clearsky.png");
	GLuint paintingTex1 = readTexture("assets/paintings/patterns/abstract1.png");
	GLuint paintingTex2 = readTexture("assets/paintings/patterns/painting1.png");
	GLuint paintingTex3 = readTexture("assets/paintings/patterns/abstract2.png");
	GLuint paintingTex4 = readTexture("assets/paintings/patterns/abstract3.png");
	GLuint paintingTex5 = readTexture("assets/paintings/patterns/abstract4.png");
	GLuint paintingTex2_1 = readTexture("assets/paintings/patterns/animal2.png");
	GLuint paintingTex2_2 = readTexture("assets/paintings/patterns/animal1.png");
	GLuint paintingTex3_1 = readTexture("assets/paintings/patterns/640px-St_Georges_Cathedral_Cape_Town_-_Cape_Colony_1800s_-_watercolour_by_Bowler.png");
	GLuint paintingTex3_2 = readTexture("assets/paintings/patterns/1024px-Pieter_Brueghel_the_Elder_-_The_Dutch_Proverbs_-_Google_Art_Project.png");
	GLuint paintingTex3_3 = readTexture("assets/paintings/patterns/Clayton_painting.png");
	GLuint paintingTex3_4 = readTexture("assets/paintings/patterns/1024px-(ukjent_monogrammist)_V.S._-_Painting_-_NG.M.00141_-_National_Museum_of_Art,_Architecture_and_Design.png");
	GLuint paintingTex3_5 = readTexture("assets/paintings/patterns/Washington_Crossing_the_Delaware_by_Emanuel_Leutze,_MMA-NYC,_1851.png");
	GLuint paintingTex4_1 = readTexture("assets/paintings/patterns/art1.png");
	GLuint paintingTex4_2 = readTexture("assets/paintings/patterns/art2.png");
	GLuint paintingTex4_3 = readTexture("assets/paintings/patterns/art3.png");
	GLuint paintingTex4_4 = readTexture("assets/paintings/patterns/art4.png");
	GLuint paintingTex4_5 = readTexture("assets/paintings/patterns/art5.png");

	GLuint floorTex = readTexture("assets/materials/floor.png");
	GLuint refTex = readTexture("assets/materials/sky.png");
	GLuint frameTex = readTexture("assets/paintings/patterns/goldframe.png");
	GLuint bronzeTex = readTexture("assets/materials/bronze_homemade.png");
	GLuint fancyframeTex = readTexture("assets/materials/fancyframeDiffuse.png");
	GLuint postumentTex = readTexture("assets/materials/pedestal.png");
	GLuint doorTex = readTexture("assets/materials/door.png");
	GLuint signTex = readTexture("assets/materials/sign.png");
	GLuint monkeTex = readTexture("assets/materials/monkey.png");
	GLuint tyranosaurusTex = readTexture("assets/materials/dino.png"); // :)
	GLuint easterTex = readTexture("assets/paintings/patterns/easter.png");
	GLuint eggTex = readTexture("assets/paintings/patterns/egg.png");
	GLuint coliseumTex = readTexture("assets/materials/koloseum.png");
	GLuint statueTex = readTexture("assets/materials/statue.png");
	GLuint thinkerTex = readTexture("assets/materials/TheThinker_Diffuse.png");
	GLuint vaseTex = readTexture("assets/materials/vase1.png");
	GLuint concreteTex = readTexture("assets/materials/concrete.png");

	GLuint visitorTex1 = readTexture("assets/materials/skins/skin2.png");
	GLuint visitorTex2 = readTexture("assets/materials/skins/skin5.png");
	GLuint visitorTex3 = readTexture("assets/materials/skins/skin4.png");


	bear.texture = bronzeTex;
	dog.texture = bronzeTex;
	cer.texture = steelTex;
	//cer.texture_refl = refTex;
	room.texture = wallTex;
	sky.texture = skyTex;
	//Room1
	painting.texture = paintingTex1;
	frame.texture = frameTex;
	painting2.texture = paintingTex2;
	painting3.texture = paintingTex3;
	painting4.texture = paintingTex4;
	painting5.texture = paintingTex5;
	//Room2
	painting2_1.texture = paintingTex2_1;
	frameB.texture = fancyframeTex;
	painting2_2.texture = paintingTex2_2;
	//Room3
	painting3_1.texture = paintingTex3_1;
	painting3_2.texture = paintingTex3_2;
	painting3_3.texture = paintingTex3_3;
	painting3_4.texture = paintingTex3_4;
	painting3_5.texture = paintingTex3_5;
	//Room4
	genie.texture = frameTex;
	jug.texture = coliseumTex;
	vase1.texture = vaseTex;
	vase2.texture = steelTex;
	vase3.texture = bronzeTex;
	vase4.texture = concreteTex;
	painting4_1.texture = paintingTex4_1;
	painting4_2.texture = paintingTex4_2;
	painting4_3.texture = paintingTex4_3;
	painting4_4.texture = paintingTex4_4;
	painting4_5.texture = paintingTex4_5;
	frameAlt.texture = postumentTex;


	corridor.texture = wallTex;
	transition.texture = wallTex;
	parquetry.texture = floorTex;
	postument.texture = postumentTex;
	cube.texture = postumentTex;
	door.texture = doorTex;
	visitorTom.texture = visitorTex1;
	visitorAlice.texture = visitorTex2;
	visitorJack.texture = visitorTex3;
	lion.texture = postumentTex;
	lion.texture_refl = refTex;
	brain.texture = frameTex;
	brain.texture_refl = refTex;
	exitSign.texture = signTex;
	monke.texture = monkeTex;
	dino.texture = tyranosaurusTex;
	coliseum.texture = coliseumTex;
	easter.texture = easterTex;
	egg.texture = eggTex;
	statue1.texture = statueTex;
	thinker.texture = thinkerTex;
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
	delete sp;
	delete sp_l;
	delete sp_main;
	delete sp_envmap;
	glDeleteTextures(1, &bear.texture);
	glDeleteTextures(1, &dog.texture);
	glDeleteTextures(1, &cer.texture);
	glDeleteTextures(1, &room.texture);
	glDeleteTextures(1, &sky.texture);
	glDeleteTextures(1, &painting.texture);
	glDeleteTextures(1, &painting2.texture);
	glDeleteTextures(1, &painting3.texture);
	glDeleteTextures(1, &painting4.texture);
	glDeleteTextures(1, &painting5.texture);
	glDeleteTextures(1, &painting2_1.texture);
	glDeleteTextures(1, &painting2_2.texture);
	glDeleteTextures(1, &painting3_1.texture);
	glDeleteTextures(1, &painting3_2.texture);
	glDeleteTextures(1, &painting3_3.texture);
	glDeleteTextures(1, &painting3_4.texture);
	glDeleteTextures(1, &painting3_5.texture);
	glDeleteTextures(1, &genie.texture);
	glDeleteTextures(1, &jug.texture);
	glDeleteTextures(1, &vase1.texture);
	glDeleteTextures(1, &vase2.texture);
	glDeleteTextures(1, &vase3.texture);
	glDeleteTextures(1, &vase4.texture);
	glDeleteTextures(1, &painting4_1.texture);
	glDeleteTextures(1, &painting4_2.texture);
	glDeleteTextures(1, &painting4_3.texture);
	glDeleteTextures(1, &painting4_4.texture);
	glDeleteTextures(1, &painting4_5.texture);
	glDeleteTextures(1, &frameAlt.texture);
	glDeleteTextures(1, &corridor.texture);
	glDeleteTextures(1, &transition.texture);
	glDeleteTextures(1, &parquetry.texture);
	glDeleteTextures(1, &postument.texture);
	glDeleteTextures(1, &cube.texture);
	glDeleteTextures(1, &door.texture);
	glDeleteTextures(1, &visitorTom.texture);
	glDeleteTextures(1, &visitorAlice.texture);
	glDeleteTextures(1, &visitorJack.texture);
	glDeleteTextures(1, &lion.texture);
	glDeleteTextures(1, &lion.texture_refl);
	glDeleteTextures(1, &brain.texture);
	glDeleteTextures(1, &brain.texture_refl);
	glDeleteTextures(1, &exitSign.texture);
	glDeleteTextures(1, &monke.texture);
	glDeleteTextures(1, &dino.texture);
	glDeleteTextures(1, &coliseum.texture);
	glDeleteTextures(1, &easter.texture);
	glDeleteTextures(1, &egg.texture);
	glDeleteTextures(1, &statue1.texture);
	glDeleteTextures(1, &thinker.texture);

}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float kat_x, float kat_y) {
	//Kat x - kat gora gol jak podnosimy glowe ,kat y - jak skrecimy glowe lewo prawo
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V = glm::lookAt(pos, pos + calcDir(kat_x, kat_y), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

	glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 300.0f); //Wylicz macierz rzutowania
	glm::mat4 M = glm::mat4(1.0f);
	
	allDrawInOnePlace(P, V, M);

	glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno
	srand(time(NULL));

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1000, 1000, "Art Museum", NULL, NULL);  //Utwórz okno  i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	float kat_x = 0;
	float kat_y = 0;
	glfwSetTime(0); //Zeruj timer
	//Główna pętla
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		
		kat_x += speed_x * glfwGetTime();
		kat_y += speed_y * glfwGetTime();
		pos += (float)(walk_speed * glfwGetTime()) * calcDir(0, kat_y);//wylaczenie latania: zamiast katu X dac 0
		glfwSetTime(0); //Zeruj timer
		drawScene(window, kat_x, kat_y);

		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
