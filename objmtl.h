// Filename: objmtl.h
// Action: read, write data from files
//		   *.obj, *.mtl

#include "files.h"

struct Vert{
	float x, y, z;
};
struct Vtex{
	float u, v;
};
struct Face{
	unsigned int v, vn, vt;
};
struct FaceInt{
	struct Face f[50];
	char usemtlName[25];
	unsigned id; 		// face id
	unsigned tid;		// texture id
	unsigned count; 	// v/vn/vt triplets count
};
struct Name{
	char name[25];
};
struct Material{
	char newMtlName[25];
	float ns;
	struct Vert ka;
	struct Vert ks;
	struct Vert ke;
	float ni;
	float d;
	unsigned illum;
	char mapKdName[25];
};
struct ObjMtlModel{
	char fileName[25];
	long fileSize;
	
	unsigned id;
	unsigned show;
	
	// mtllib trafostanica.mtl
	unsigned numMtlLib;
	unsigned numObj;  // o Cube
	unsigned numVert; // v 1.000000 2.000844 -1.000000
	unsigned numVnorm; // vn -0.0000 1.0000 -0.0000
	unsigned numVtex; // vt -0.003906 -0.001953
	unsigned numSur; // s 0
	
	// usemtl Zadna  // <name from trafostanica.mtl>	
	unsigned numUsemtl;
	unsigned numFace;  // f 8/9/3 7/10/3 5/11/3 6/12/3 = <v / vt / vn>
	
	struct Name *mtllibName;
	struct Name *objectName;
	struct Vert *vert;
	struct Vert *vnorm;
	struct Vtex *vtex;
	unsigned *sur;
	struct Name *usemtlName;
	struct FaceInt *fi;	
	
	unsigned numMtlName;
	
	struct Vert pos;
	struct Vert rot;
	float scale;
	struct Name *entityname;
};

struct Material *mtl;
struct ObjMtlModel obj;
//----- FUNCTIONS ------------------------------------------------------

void fileLoadTextureObj(const char *filename, unsigned i);
void fileLoadObj(const char *file);
void file_Obj_Convert_To_Model(const char *fileObj);
void line(float sx, float sy, float sz,
		  float ex, float ey, float ez);
void objmtl(unsigned show, float posx, float posy, float posz,
		    float rotx, float roty, float rotz, float scale1);
//----------------------------------------------------------------------

void fileLoadTextureObj(const char *filename, unsigned i){
	unsigned width, height, channels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(filename, &width, &height,
		&channels, STBI_rgb_alpha);
	
	if(pixels == NULL) {
	   fprintf(stderr, "Cannot load file %s\nSTB Reason: %s\n",
			filename, stbi_failure_reason());
       exit(EXIT_FAILURE);
	}
	glGenTextures(1, &obj.fi[i].id);
	glBindTexture(GL_TEXTURE_2D, obj.fi[i].id);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, pixels);
	
	stbi_image_free(pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
//----------------------------------------------------------------------

void fileLoadObj(const char *file){
	
	sprintf(obj.fileName, "%s%s%s%s%s%s",
		"data",
		"/",
		file,
		"/",
		file,
		".obj");
		
	printf("Obj: %s\n", obj.fileName);
	
	FILE *fobj;
	char line[255];
	fobj = fopen(obj.fileName, "r");
	
	if (fobj == NULL)
		exit(EXIT_FAILURE);
	
	// Scan nums
	while (fgets(line, 255, fobj)) {
		if (substring(line, "mtllib")) {
			obj.numMtlLib++; 
		} else if(substring(line, "o")) {
			obj.numObj++; 
		} else if(substring(line, "v ")) {
			obj.numVert++; 
		} else if(substring(line, "vn")) {
			obj.numVnorm++;
		} else if(substring(line, "vt")) {
			obj.numVtex++;
		} else if(substring(line, "s")) {
			obj.numSur++;
		} else if(substring(line, "usemtl")) {
			obj.numUsemtl++;
		} else if(substring(line, "f")) {
			obj.numFace++;
		}
	}
	printf("Mtl %d, Obj %d, Vert %d, Vnorm %d, Vtex %d, Sur %d\n",
		obj.numMtlLib,
		obj.numObj,
		obj.numVert,
		obj.numVnorm,
		obj.numVtex,
		obj.numSur);
	
	printf("Usemtl %d, Face %d\n",
		obj.numUsemtl,
		obj.numFace);
	
	// Dynamic Data Allocation
	obj.mtllibName = malloc(obj.numMtlLib * sizeof(struct Name));
	obj.objectName = malloc(obj.numObj * sizeof(struct Name));
	obj.vert = malloc(obj.numVert * sizeof(struct Vert));
	obj.vnorm = malloc(obj.numVnorm * sizeof(struct Vert));
	obj.vtex = malloc(obj.numVtex * sizeof(struct Vtex));
	obj.sur = malloc(obj.numSur * sizeof(unsigned));
	obj.usemtlName = malloc(obj.numUsemtl * sizeof(struct Name));
	obj.fi = malloc(obj.numFace * sizeof(struct FaceInt));
	
	unsigned mt = 0;
	unsigned o = 0;
	unsigned i = 0;	
	unsigned j = 0;
	unsigned k = 0;
	unsigned l = 0;
	unsigned m = 0;
	unsigned n = 0;
	char lastobj[25];
	
	// Scan data
	fseek(fobj, 0L, SEEK_SET);
	while (fgets(line, 255, fobj)) {
		if (substring(line, "mtllib")) {
			sscanf(line, "mtllib %s", obj.mtllibName[mt].name);
			mt++;
		} else if (substring(line, "o")) {
			sscanf(line, "o %s", obj.objectName[o].name);
			o++;
		} else if (substring(line, "v ")) {
			sscanf(line, "v %f %f %f",
				&obj.vert[i].x, &obj.vert[i].z, &obj.vert[i].y);
				obj.vert[i].z = 2 - obj.vert[i].z;
			i++;
		} else if (substring(line, "vn ")) {
			sscanf(line, "vn %f %f %f",
				&obj.vnorm[j].x, &obj.vnorm[j].y, &obj.vnorm[j].z);
			j++;
		} else if (substring(line, "vt ")) {
			sscanf(line, "vt %f %f",
				&obj.vtex[k].u, &obj.vtex[k].v);
			k++;
		} else if (substring(line, "s ")) {
			sscanf(line, "s %d", &obj.sur[l]);
			l++;
		} else if (substring(line, "usemtl")) {
			sscanf(line, "usemtl %s", obj.usemtlName[m].name);
			strcpy(lastobj, obj.usemtlName[m].name);
			m++;
		} else if (substring(line, "f")) {
			// Detect face points
			int in = 0;
			int spaces = 0;
			char line2[20] = "";
			int index[50] = {0};
			int v, vn, vt;
									
			// Detect space indexes
			while (in < strlen(line)) {
				if (line[in] == ' ') {
					index[spaces] = in;
					spaces++;
				}
				in++;
			}
			index[spaces] = strlen(line);
			obj.fi[n].count = spaces;
			
			//printf("%s length %d n %d count points: %d\n", line, 
				//obj.fi[n].length, n, obj.fi[n].count);
			
			// Scan data
			for(int in=0; in<obj.fi[n].count; in++){
				strcpy(line2, "");
				getSubstring(line, line2, index[in], index[in+1]);
		
				if (obj.numVnorm == 0){
					if (obj.numVtex == 0){
						sscanf(line2, "%d", &obj.fi[n].f[in].v);
					} else {
						sscanf(line2, "%d/%d",
							&obj.fi[n].f[in].v,
							&obj.fi[n].f[in].vt);
					}
				} else if (obj.numVtex == 0){
					sscanf(line2, "%d//%d",
						&obj.fi[n].f[in].v,
						&obj.fi[n].f[in].vn);					
				} else {
					sscanf(line2, "%d/%d/%d",
						&obj.fi[n].f[in].v,
						&obj.fi[n].f[in].vt,
						&obj.fi[n].f[in].vn);
					//~ printf("%d/%d/%d ",
						//~ obj.fi[n].f[in].v,
						//~ obj.fi[n].f[in].vt,
						//~ obj.fi[n].f[in].vn);
					}
			}
			//~ printf("\n");
			sprintf(obj.fi[n].usemtlName, "%s", lastobj);
			n++;
		}
	} 
	fclose(fobj);
	
	//~ for(int n=0; n<obj.numFace; n++) {
		//~ printf("\n");
		//~ for(int in=0; in<obj.fi[n].count; in++) {
			//~ printf("%d/%d/%d\n",
					//~ obj.fi[n].f[in].v,
					//~ obj.fi[n].f[in].vt,
					//~ obj.fi[n].f[in].vn);
			
			//~ printf("%f %f %f\n", 
				//~ obj.vert[obj.fi[n].f[in].v].x,
				//~ obj.vert[obj.fi[n].f[in].v].y,
				//~ obj.vert[obj.fi[n].f[in].v].z);
		//~ }
	//~ }
	
	//~ printf("Mtl %d, Obj %d, Vert %d, Vnorm %d, Vtex %d, Sur %d,\
		Usemtl %d, Face %d\n",
		//~ mt, o, i, j, k, l, m, n);
	//~ printf("obj.mtllibName: %s\n", obj.mtllibName);	
	//~ printf("obj.objectName: %s\n", obj.objectName);
	
	//~ for(i=0; i<obj.numVert; i++) {
		//~ printf("v: %f %f %f\n", obj.vert[i].x, obj.vert[i].y,
			//~ obj.vert[i].z);
	//~ }
	
	//~ for(k=0; k<obj.numVtex; k++) {
		//~ printf("vt: %f %f\n", obj.vtex[k].u, obj.vtex[k].v);
	//~ }
	
	//~ for(n=0; n<obj.numFace; n++) {
		//~ printf("usemtl: %s\n", obj.fi[n].usemtlName);
	//~ }
	
	if (obj.numVtex == 0){
	
	} else {
		FILE *fmat;
		char mat[255];
		sprintf(mat, "%s%s%s%s%s", "data", "/", file, "/", 
			obj.mtllibName->name);
		//printf("mtl: %s\n", mat);
		fmat = fopen(mat, "r");
		
		if (fmat == NULL)
			exit(EXIT_FAILURE);
		
		// Scan nums
		while (fgets(line, 255, fmat)) {
			if (substring(line, "newmtl")) {
				obj.numMtlName++; 
			}
		}
		//printf("numMtlName: %d\n", obj.numMtlName);
		
		// Dynamic Data Allocation
		mtl = malloc(obj.numMtlName * sizeof(struct Material));
		int matl = -1;
		
		// Scan data
		fseek(fmat, 0L, SEEK_SET);
		while (fgets(line, 255, fmat)) {
			if (substring(line, "newmtl")) {
				matl++;
				sscanf(line, "newmtl %s", mtl[matl].newMtlName);
			} else if (substring(line, "Ns")) {
				sscanf(line, "Ns %f", &mtl[matl].ns);
			} else if (substring(line, "Ka")) {
				sscanf(line, "Ka %f %f %f",
					&mtl[matl].ka.x ,&mtl[matl].ka.y, &mtl[matl].ka.z);
			} else if (substring(line, "Ks")) {
				sscanf(line, "Ks %f %f %f",
					&mtl[matl].ks.x ,&mtl[matl].ks.y, &mtl[matl].ks.z);
			} else if (substring(line, "Ke")) {
				sscanf(line, "Ke %f %f %f",
					&mtl[matl].ke.x ,&mtl[matl].ke.y, &mtl[matl].ke.z);
			} else if (substring(line, "Ni")) {
				sscanf(line, "Ni %f", &mtl[matl].ni);
			} else if (substring(line, "d")) {
				sscanf(line, "d %f", &mtl[matl].d);
			} else if (substring(line, "illum")) {
				sscanf(line, "illum %u", &mtl[matl].illum);
			} else if (substring(line, "map_Kd")) {
				sscanf(line, "map_Kd %s", mtl[matl].mapKdName);
			}
		}
		fclose(fmat);
		
		// Load textures
		for(m=0; m<obj.numFace; m++) {
			//printf("%s\n", obj.fi[m].usemtlName);
			for(matl=0; matl<obj.numMtlName; matl++) {
				if (strcmp(obj.fi[m].usemtlName,
					mtl[matl].newMtlName) == 0) {
					obj.fi[m].tid = matl;
					sprintf(mat, "%s%s%s%s%s",
						"data",
						"/",
						file,
						"/",
						mtl[matl].mapKdName);
						
					fileLoadTextureObj(mat, m);
					//printf("%s\n", mat);
				}
			}
		}
	}
}
//----------------------------------------------------------------------

void file_Obj_Convert_To_Model(const char *fileObj){
	// Load *.obj to structures
	fileLoadObj(fileObj);
	
	// Convert data and Save to Model
	printf("SaveObjToModel:\n");
	FILE *fmodel;
	char line[255];
	char fileModelName[100];
	
	sprintf(fileModelName, "%s%s%s%s%s%s",
		"data",
		"/",
		fileObj,
		"/",
		fileObj,
		".txt");
		
	printf("%s\n", fileModelName);
	fmodel = fopen(fileModelName, "w");
	
	if (fmodel == NULL)
		exit(EXIT_FAILURE);
	
	fprintf(fmodel, "// Textures\n");
	int i = 0;
	while(i < obj.numMtlName){
		fprintf(fmodel, "%s\n", mtl[i].mapKdName);
		i++;
	}
	printf("tex = ok");
	fprintf(fmodel, "\n");
	fprintf(fmodel, "// Points\n");
	i = 0;
	while(i < obj.numVert){
		fprintf(fmodel, "%f %f %f\n",
			obj.vert[i].x,
			obj.vert[i].y,
			2.0-obj.vert[i].z);
		i++;
	}
	
	fprintf(fmodel, "\n");
	fprintf(fmodel, "// Faces\n");	
	int index_v[10];
	int index_vn[10];
	int index_vt[10];
	for(int n=0; n<obj.numFace; n++) {
		for(int i=0; i<obj.fi[n].count; i++) {
			index_v[i] = obj.fi[n].f[i].v-1;
			index_vn[i] = obj.fi[n].f[i].vn-1;
			index_vt[i] = obj.fi[n].f[i].vt-1;
		}
		
		if (obj.fi[n].count == 3) {				
			fprintf(fmodel, "%d %d %d ",
				index_v[0], index_v[1], index_v[2]);			
			fprintf(fmodel, "%f %f %f %f %f %f ",
				obj.vtex[index_vt[0]].u, obj.vtex[index_vt[0]].v,
				obj.vtex[index_vt[1]].u, obj.vtex[index_vt[1]].v,
				obj.vtex[index_vt[2]].u, obj.vtex[index_vt[2]].v);
				
			fprintf(fmodel, "%d\n", obj.fi[n].tid);
		} else if (obj.fi[n].count == 4) {			
			fprintf(fmodel, "%d %d %d ",
				index_v[0], index_v[1], index_v[2]);			
			fprintf(fmodel, "%f %f %f %f %f %f ",
				obj.vtex[index_vt[0]].u, obj.vtex[index_vt[0]].v,
				obj.vtex[index_vt[1]].u, obj.vtex[index_vt[1]].v,
				obj.vtex[index_vt[2]].u, obj.vtex[index_vt[2]].v);
				
			fprintf(fmodel, "%d\n", obj.fi[n].tid);
			
			fprintf(fmodel, "%d %d %d ",
				index_v[2], index_v[3], index_v[0]);			
			fprintf(fmodel, "%f %f %f %f %f %f ",
				obj.vtex[index_vt[2]].u, obj.vtex[index_vt[2]].v,
				obj.vtex[index_vt[3]].u, obj.vtex[index_vt[3]].v,
				obj.vtex[index_vt[0]].u, obj.vtex[index_vt[0]].v);
				
			fprintf(fmodel, "%d\n", obj.fi[n].tid);
		}
	}
	
	fclose(fmodel);
}
//----------------------------------------------------------------------

void line(float sx, float sy, float sz, 
		  float ex, float ey, float ez){
	glVertex3f(sx, sy, sz); glVertex3f(ex, ey, ez);
}
//----------------------------------------------------------------------

void objmtl(unsigned show, float posx, float posy, float posz,
		    float rotx, float roty, float rotz, float scale1){
	glTranslatef(posx, posy, posz);
	glScalef(scale1, scale1, scale1);
	glRotatef(rotx, 0.0, 0.0, 1.0);
	glRotatef(roty, 1.0, 0.0, 0.0);
	glRotatef(rotz, 0.0, 1.0, 0.0);
	
	//glRotatef(180, 0.0, 1.0, 0.0);
	switch(show){
		case 0:
			textureHide();
			glColor4f(0.9, 0.9, 1, 1);
			glPointSize(g_pointSize);
			glBegin(GL_POINTS);
			for(int i=0; i<obj.numVert; i++) {
				glVertex3f(obj.vert[i].x, obj.vert[i].y, obj.vert[i].z);
			}
			glEnd();
			break;
		case 1:
			glColor4f(0.5, 1, 0.5, 1);
			glDisable(GL_TEXTURE_2D);
			glLineWidth(g_lineWidth);
			glBegin(GL_LINES);
			for(int n=0; n<obj.numFace; n++) {
				for(int i=0; i<obj.fi[n].count-1; i++) {
					line(obj.vert[obj.fi[n].f[i].v-1].x,
						obj.vert[obj.fi[n].f[i].v-1].y,
						obj.vert[obj.fi[n].f[i].v-1].z,
						obj.vert[obj.fi[n].f[i+1].v-1].x,
						obj.vert[obj.fi[n].f[i+1].v-1].y,
						obj.vert[obj.fi[n].f[i+1].v-1].z);
				}
				line(obj.vert[obj.fi[n].f[obj.fi[n].count-1].v-1].x,
					obj.vert[obj.fi[n].f[obj.fi[n].count-1].v-1].y,
					obj.vert[obj.fi[n].f[obj.fi[n].count-1].v-1].z,
					obj.vert[obj.fi[n].f[0].v-1].x,
					obj.vert[obj.fi[n].f[0].v-1].y,
					obj.vert[obj.fi[n].f[0].v-1].z);
			}
			glEnd();
			break;
		case 2:
			glColor4f(1, 1, 1, 1);
			for(int n=0; n<obj.numFace; n++) {
				textureBind(obj.fi[n].id);
				glBegin(GL_POLYGON);
				for(int i=0; i<obj.fi[n].count; i++) {
					if (obj.numVtex != 0){
						glTexCoord2f(
							obj.vtex[obj.fi[n].f[i].vt-1].u,
							obj.vtex[obj.fi[n].f[i].vt-1].v);
					}
					glVertex3f(obj.vert[obj.fi[n].f[i].v-1].x,
						obj.vert[obj.fi[n].f[i].v-1].y,
						obj.vert[obj.fi[n].f[i].v-1].z);
				}
				glEnd();
			}
			
			glColor4f(0.5, 1, 0.5, 1);
			glDisable(GL_TEXTURE_2D);
			glLineWidth(g_lineWidth);
			glBegin(GL_LINES);
			for(int n=0; n<obj.numFace; n++) {
				for(int i=0; i<obj.fi[n].count-1; i++) {
					line(obj.vert[obj.fi[n].f[i].v-1].x,
						obj.vert[obj.fi[n].f[i].v-1].y,
						obj.vert[obj.fi[n].f[i].v-1].z,
						obj.vert[obj.fi[n].f[i+1].v-1].x,
						obj.vert[obj.fi[n].f[i+1].v-1].y,
						obj.vert[obj.fi[n].f[i+1].v-1].z);
				}
				line(obj.vert[obj.fi[n].f[obj.fi[n].count-1].v-1].x,
					obj.vert[obj.fi[n].f[obj.fi[n].count-1].v-1].y,
					obj.vert[obj.fi[n].f[obj.fi[n].count-1].v-1].z,
					obj.vert[obj.fi[n].f[0].v-1].x,
					obj.vert[obj.fi[n].f[0].v-1].y,
					obj.vert[obj.fi[n].f[0].v-1].z);
			}
			glEnd();
			break;
		case 3:
			glColor4f(1, 1, 1, 1);
			for(int n=0; n<obj.numFace; n++) {
				textureBind(obj.fi[n].id);
				glBegin(GL_POLYGON);
				for(int i=0; i<obj.fi[n].count; i++) {
					if (obj.numVtex != 0){
						glTexCoord2f(
							obj.vtex[obj.fi[n].f[i].vt-1].u,
							obj.vtex[obj.fi[n].f[i].vt-1].v);
					}
					glVertex3f(obj.vert[obj.fi[n].f[i].v-1].x,
						obj.vert[obj.fi[n].f[i].v-1].y,
						obj.vert[obj.fi[n].f[i].v-1].z);
				}
				glEnd();
			}
			break;
	}
	glFlush();
	
	glRotatef(-rotx, 0.0, 0.0, 1.0);
	glRotatef(-roty, 1.0, 0.0, 0.0);
	glRotatef(-rotz, 0.0, 1.0, 0.0);
	glScalef(1.0/scale1, 1.0/scale1, 1.0/scale1);
	glTranslatef(-posx, -posy, -posz);
}
