// Filename: files.h
// Action: read, write data from files
//		   texture:*.png, *.jpg, map:*.txt, model:*.txt

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include "stb/stb_image.h"
#include "string2.h"

#define PI 3.1415926535897932384626433832795

float rotx = 0.0f;
float roty = 0.0f;
float rotz = 0.0f;
float scale = 1.0f;

// Model
struct Textures{
	char filename[100];
	unsigned int id;
};

struct Vec{
	float x, y, z;
};

struct VecDouble{
	double x, y, z;
};

struct Points{
	float x, y, z;
};
struct TexCoord{
	float x, y;
};
struct Faces{
	unsigned int a, b, c;
	struct TexCoord ta, tb, tc;
	unsigned int id;
};
struct Chars{
	float sx, sz;
	float ex, ez;
};
struct Characters{
	unsigned int id;
	unsigned int count;
	struct Chars chars[20];
};
struct Textures tex[10];
struct Points points[100];
struct Faces faces[100];
struct Characters characters[255];

unsigned int numTex = 0;
unsigned int numPoints = 0;
unsigned int numFaces = 0;
unsigned int charsCounter = 0;

// Global variables
char *g_title = "Model";
char *g_view = "3D";
char *g_state = "ROT";
const unsigned g_windowwidth = 1280;
const unsigned g_windowheight = 800;
const unsigned g_pointSize = 4;
const unsigned g_lineWidth = 2;

struct Vec rot;
struct Vec pos;
float g_scale = 1.0f;
bool g_perspective = true;
bool g_position = false;

//----- FUNCTIONS ------------------------------------------------------
void loadTexture(const char *filename, unsigned int i);
void textureBind(unsigned int ID);
void textureUnbind();
void textureShow();
void textureHide();
bool stringIsEmpty(const char *str);
long fileSize(const char *filename);
void loadChars(const char *filename);
void loadModel(const char *filedir);

//----------------------------------------------------------------------
void loadTexture(const char *filename, unsigned int i){
	unsigned int width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *pixels = stbi_load(filename, &width, &height,
		&channels, STBI_rgb_alpha);
	
	if (pixels == NULL){
		fprintf(stderr, "Cannot load file image %s\nSTB Reason: %s\n",
			filename, stbi_failure_reason());
		exit(EXIT_FAILURE);
	}
	
	glGenTextures(1, &tex[i].id);
	glBindTexture(GL_TEXTURE_2D, tex[i].id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, pixels);
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_image_free(pixels);
}
//----------------------------------------------------------------------

void textureBind(unsigned int ID){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ID);
	glEnable(GL_TEXTURE_2D);
}
//----------------------------------------------------------------------

void textureUnbind(){	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(numTex, 0);
}
//----------------------------------------------------------------------

void textureShow(){
	glEnable(GL_TEXTURE_2D);
}
//----------------------------------------------------------------------

void textureHide(){
	glDisable(GL_TEXTURE_2D);
}
//----------------------------------------------------------------------

long fileSize(const char *filename){
	FILE *fp = fopen(filename, "r");
	
	if (fp == NULL)
		return -1;
	
	if (fseek(fp, 0, SEEK_END) < 0) {
		fclose(fp);
		return -1;
	}
	long size = ftell(fp);
	return size;
}
//----------------------------------------------------------------------

void loadChars(const char *filename){
	FILE *fp;
	char line[255];
	fp = fopen(filename, "r");
	
	if (fp == NULL)
		exit(EXIT_FAILURE);
			
	// Read content of the file
	int a = 0;
	int i = 0;
	while (fgets(line, 255, fp)){
		if( strstr(line, ",") != NULL) {
			sscanf(line, "%f, %f %f, %f",
					&characters[a-1].chars[i].sx,
					&characters[a-1].chars[i].sz,
					&characters[a-1].chars[i].ex,
					&characters[a-1].chars[i].ez);
			i++;
			characters[a-1].count = i;
		} else {
			if( strstr(line, "//") == NULL) {
				sscanf(line, "%d", &characters[a].id);
				a++;
				i = 0;
			}			
		}
	}
	charsCounter = a;
	fclose(fp);
	
	// printing
	//~ for(int a=0; a<charsCounter; a++){
		//~ printf("%d count %d\n",
			//~ characters[a].id,
			//~ characters[a].count);
		//~ for(int i=0; i<characters[a].count; i++){
			//~ printf("%0.2f %0.2f %0.2f %0.2f\n",
				//~ characters[a].chars[i].sx,
				//~ characters[a].chars[i].sz,
				//~ characters[a].chars[i].ex,
				//~ characters[a].chars[i].ez);				
		//~ }
	//~ }
}
//----------------------------------------------------------------------

void loadModel(const char *filedir){
	char *filename = (char *)malloc(50);
	char *fileDir = (char *)malloc(50);	
	strcpy(filename, "data");
	strcpy(filename+strlen(filename), "/");
	strcpy(filename+strlen(filename), filedir);
	strcpy(filename+strlen(filename), "/");
	
	strcpy(fileDir, filename);
	strcpy(filename+strlen(filename), filedir);
	strcpy(filename+strlen(filename), ".txt");
	printf("Model: %s\n", filename);

	FILE *fp;
	char line[255];
	fp = fopen(filename, "r");
	
	if (fp == NULL)
		exit(EXIT_FAILURE);
	
	bool loadTextures = false;
	bool loadPoints = false;
	bool loadFaces = false;
	
	// Read content of the file
	while (fgets(line, 255, fp)){
		if( strstr(line, "// Textures") != NULL) {
			loadTextures = true;
			loadPoints = false;
			loadFaces = false;
		} else if( strstr(line, "// Points") != NULL) {
			loadTextures = false;
			loadPoints = true;
			loadFaces = false;
		} else if( strstr(line, "// Faces") != NULL) {
			loadTextures = false;
			loadPoints = false;
			loadFaces = true;
		} else if (stringIsEmpty(line)){
			loadTextures = false;
			loadPoints = false;
			loadFaces = false;
		} else {
			if(loadTextures){
				sscanf(line, "%s", tex[numTex].filename);
				//printf("tex %s\n", tex[numTex].filename);
				numTex++;
			} else if(loadPoints){
				sscanf(line, "%f %f %f",
					&points[numPoints].x,
  					&points[numPoints].z,
					&points[numPoints].y);
				//printf("point x = %f\n", points[numPoints].x);
				numPoints++;
			} else if(loadFaces){
				sscanf(line, "%d %d %d %f %f %f %f %f %f %d",
					&faces[numFaces].a,
					&faces[numFaces].b,
					&faces[numFaces].c,					
					&faces[numFaces].ta.x,
					&faces[numFaces].ta.y,					
					&faces[numFaces].tb.x,
					&faces[numFaces].tb.y,					
					&faces[numFaces].tc.x,
					&faces[numFaces].tc.y,					
					&faces[numFaces].id);
					numFaces++;
			}
		
		}
	}
	fclose(fp);
	
	printf(":Tex %d, Points %d, Faces %d, Size = %d B\n", 
		numTex,
		numPoints,
		numFaces,
		fileSize(filename));
	
	int lengthDir = strlen(fileDir)+1;
	char *fileDire = (char *)malloc(50);
	for(int i=0; i<numTex; i++){
		strncpy(fileDire, fileDir, lengthDir);
		strcpy(fileDire+strlen(fileDire), tex[i].filename);
		loadTexture(fileDire, i);
	}
}
