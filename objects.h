// Filename: objects.h
// Action: draw, generate objects from loaded structures

//#include "files.h"
#include "objmtl.h"

const float axesSize = 3.0;
//----- FUNCTIONS ------------------------------------------------------

void line2D(float sx, float sy, float ex, float ey);
void drawLine(float sx, float sy, float sz, float ex, float ey,
	float ez);
void drawChar(int id);
void drawChar3D(int n);
void drawText(const char *text, unsigned textLineWidth, 
	float posx, float posy, float posz,
	float rotx, float roty, float rotz, float scale1);
void drawAxes(float axesSize, unsigned lineWidth);
void drawGrid2D(float gridLength, float gridCell);
void drawGrid(float gridLength, float gridCell);
void drawModel(unsigned show);

void calcGrass(float range, int points, int leafs, float xc);
void genGrass(float range, int points,  int leafs, float leaf_width, 
	float posx, float posy, float posz, float scale1);
//----------------------------------------------------------------------

void line2D(float sx, float sy, float ex, float ey){
	glVertex2f(sx, sy); glVertex2f(ex, ey);
}
//----------------------------------------------------------------------

void drawChar(int id){
	int a = 0;
	int i = 0;
	glBegin(GL_LINES);
	while(a < charsCounter){
		if(id == characters[a].id){
			while(i < characters[a].count){
				glVertex2f(characters[a].chars[i].sx,
					characters[a].chars[i].sz);
				glVertex2f(characters[a].chars[i].ex,
					characters[a].chars[i].ez);
				i++;
			}
			break;
		}
		a++;
	}
	glEnd();
}
//----------------------------------------------------------------------

void drawLine(float sx, float sy, float sz, float ex, float ey,
	float ez){
	glVertex3f(sx, sy, sz); glVertex3f(ex, ey, ez);
}
//----------------------------------------------------------------------

void drawText(const char *text, unsigned textLineWidth, 
	float posx, float posy, float posz,
	float rotx, float roty, float rotz, float scale1){
	
	glTranslatef(posx, posy, posz);
	glScalef(scale1, scale1, scale1);
	glRotatef(rotx, 0.0, 0.0, 1.0);
	glRotatef(roty, 1.0, 0.0, 0.0);
	glRotatef(rotz, 0.0, 1.0, 0.0);	
	
	textureHide();
	glLineWidth(textLineWidth);	

	float spaceChar = 0.6f;
	for(int i=0; i<strlen(text); i++) {
		glTranslatef(i*spaceChar, 0.0, 0.0);		
		drawChar((int)text[i]);
		glTranslatef(-(i*spaceChar), 0.0, 0.0);
	}	
	
	glRotatef(-rotx, 0.0, 0.0, 1.0);
	glRotatef(-roty, 1.0, 0.0, 0.0);
	glRotatef(-rotz, 0.0, 1.0, 0.0);
	glScalef(1.0/scale1, 1.0/scale1, 1.0/scale1);
	glTranslatef(-posx, -posy, -posz);	
}
//----------------------------------------------------------------------

void drawAxes(float axesSize, unsigned lineWidth){
	textureHide();
	glLineWidth(lineWidth);
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
			drawLine(-axesSize, 0.0, 0.0, axesSize, 0.0, 0.0);
		glColor3f(0.0, 1.0, 0.0);		
			drawLine(0.0, 0.0, -axesSize, 0.0, 0.0, axesSize);		
		glColor3f(0.0, 0.0, 1.0);
			drawLine(0.0, -axesSize, 0.0, 0.0, axesSize, 0.0);
	glEnd();
	glColor4f(1.0, 1.0, 1.0, 1.0);
}
//----------------------------------------------------------------------

void drawGrid2D(float gridLength, float gridCell){
	textureHide();
	glLineWidth(1);
	glColor4f(0.3, 0.3, 0.3, 0.3);
	glBegin(GL_LINES);	
	for(int k=0; k<(1/gridCell)*gridLength; k++) {		
		line2D(0, gridCell*k, gridLength, gridCell*k);
		line2D(gridCell*k, 0, gridCell*k, gridLength);
	}
	glEnd();
	glColor4f(1.0, 1.0, 1.0, 1.0);
}
//----------------------------------------------------------------------

void drawGrid(float gridLength, float gridCell){
	textureHide();
	glLineWidth(1);
	glColor4f(0.3, 0.3, 0.3, 0.3);
	glBegin(GL_LINES);	
	for(int k=0; k<gridLength*(1/gridCell); k++) {
		drawLine(-gridLength, 0.0, gridCell*(1+k), gridLength,
			0.0, gridCell*(1+k));
		drawLine(-gridLength, 0.0, -gridCell*(1+k), gridLength,
			0.0, -gridCell*(1+k));
		drawLine(gridCell*(1+k), 0.0, -gridLength, gridCell*(1+k),
			0.0,gridLength);
		drawLine(-gridCell*(1+k), 0.0, -gridLength, -gridCell*(1+k),
			0.0,gridLength);
	}
	glEnd();
	glColor4f(1.0, 1.0, 1.0, 1.0);
}
//----------------------------------------------------------------------

void drawModel(unsigned show){
	
	switch (show){
		case 0: 
			// Points
			glColor4f(1, 1, 1, 1);
			glPointSize(g_pointSize);
			glBegin(GL_POINTS);
			for(int k=0; k<numPoints; k++){
				glVertex3f(points[k].x,points[k].y,points[k].z );
			}
			glEnd();
			break;
		case 1:
			// Points
			glColor4f(1, 1, 1, 1);
			glPointSize(g_pointSize);
			glBegin(GL_POINTS);
			for(int k=0; k<numPoints; k++){
				glVertex3f(points[k].x,points[k].y,points[k].z );
			}
			glEnd();
			
			// Lines
			glColor4f(0, 1, 0, 1);	
			glBegin(GL_LINES);
			for(int k=1; k<numPoints; k++){
				glVertex3f(points[k-1].x,points[k-1].y,points[k-1].z );
				glVertex3f(points[k].x,points[k].y,points[k].z );
			}
			glEnd();
			
			// Faces
			glColor4f(1, 1, 1, 1);
			for(int l=0; l<numFaces; l++){					
				textureBind(tex[faces[l].id].id);
				glBegin(GL_TRIANGLES);
					glTexCoord2f(faces[l].ta.x, faces[l].ta.y);
					glVertex3f(points[faces[l].a].x,
						points[faces[l].a].y,
						points[faces[l].a].z );
					glTexCoord2f(faces[l].tb.x, faces[l].tb.y);
					glVertex3f(points[faces[l].b].x,
						points[faces[l].b].y,
						points[faces[l].b].z );
					glTexCoord2f(faces[l].tc.x, faces[l].tc.y);
					glVertex3f(points[faces[l].c].x,
						points[faces[l].c].y,
						points[faces[l].c].z );
				glEnd();
			}
			break;
		case 2:
			// Lines
			glColor4f(0, 1, 0, 1);	
			glBegin(GL_LINES);
			for(int k=1; k<numPoints; k++){
				glVertex3f(points[k-1].x,points[k-1].y,points[k-1].z );
				glVertex3f(points[k].x,points[k].y,points[k].z );
			}
			glEnd();
			
			// Faces
			glColor4f(1, 1, 1, 1);
			for(int l=0; l<numFaces; l++){					
				textureBind(tex[faces[l].id].id);
				glBegin(GL_TRIANGLES);
					glTexCoord2f(faces[l].ta.x, faces[l].ta.y);
					glVertex3f(points[faces[l].a].x,
						points[faces[l].a].y,
						points[faces[l].a].z );
					glTexCoord2f(faces[l].tb.x, faces[l].tb.y);
					glVertex3f(points[faces[l].b].x,
						points[faces[l].b].y,
						points[faces[l].b].z );
					glTexCoord2f(faces[l].tc.x, faces[l].tc.y);
					glVertex3f(points[faces[l].c].x,
						points[faces[l].c].y,
						points[faces[l].c].z );
				glEnd();
			}
			break;
		case 3:
			// Faces
			glColor4f(1, 1, 1, 1);
			for(int l=0; l<numFaces; l++){					
				textureBind(tex[faces[l].id].id);
				glBegin(GL_TRIANGLES);
					glTexCoord2f(faces[l].ta.x, faces[l].ta.y);
					glVertex3f(points[faces[l].a].x,
						points[faces[l].a].y,
						points[faces[l].a].z );
					glTexCoord2f(faces[l].tb.x, faces[l].tb.y);
					glVertex3f(points[faces[l].b].x,
						points[faces[l].b].y,
						points[faces[l].b].z );
					glTexCoord2f(faces[l].tc.x, faces[l].tc.y);
					glVertex3f(points[faces[l].c].x,
						points[faces[l].c].y,
						points[faces[l].c].z );
				glEnd();
			}
			break;
	}
	glFlush();	
}
//----------------------------------------------------------------------

double x[181], y[181], z[361], sc[101], r[101];
//----------------------------------------------------------------------

double rad(double degree) {
	return degree * (PI/180);
}
//----------------------------------------------------------------------

int genRandom(int min, int max) {	
	int result = rand() % (max - min + 1) + min;	
	return result;
}
//----------------------------------------------------------------------

void genRandomArray(int count, float min, float max, float *result) {
	result = malloc(count * sizeof(float));
	float scale;
	for (int i = 0; i < count; i++) {
		scale = rand() / (float) RAND_MAX;
		result[i] = min + scale * ( max - min );
	}
}
//----------------------------------------------------------------------

void calcGrass(float range, int points, int leafs, float xc){	
	float n = range/(float)points;
	float scale = rand() / (float) RAND_MAX;
	for(int i=0; i<points; i++) {
		x[i] = i*n/(range*xc)+ (0.12*scale);
		y[i] = sin(rad(i*n));
		//printf("i %d\tx[i] %0.2f\ty[i] %0.2f\n", i, x[i], y[i]);
	}
	
	float a = 360/(float)leafs;
	for(int j=0; j<leafs; j++) {
		z[j] = j*a*rand();
		r[j] = genRandom(40, 200)/101.0f;
		sc[j] = genRandom(70, 100)/101.0f;
		//printf("j %d, z[j] %0.2f\n", j, z[j]);
	}
}
//----------------------------------------------------------------------

void genGrass(float range, int points, int leafs, float leaf_width, 
	float posx, float posy, float posz, float scale1) {	
		
	float a, b;
	float n = 180/((float)points-1);	
	glTranslatef(posx, posy, posz);
	glScalef(scale1, scale1, scale1);
	
	textureHide();	
	glColor3f(1.0, 1.0, 1.0);
				
	glPointSize(10);
	//~ for(int j=0; j<leafs; j++) {
		//~ glRotatef(z[j], 0.0, 1.0, 0.0);
		//~ glBegin(GL_POINTS);	
		//~ for(int i=0; i<points; i++) {
			//~ glVertex3f(x[i]*sc[j], y[i], 0.0);
		//~ }		
		//~ glEnd();		
		//~ glRotatef(-z[j], 0.0, 1.0, 0.0);
	//~ }
	
	glColor3f(0.0, 1.0, 0.0);
	glLineWidth(3);
	//~ float *result;
	//~ genRandomArray(leaf, 0, leaf, result);
	//float scale = rand() / (float) RAND_MAX;
	//*(10*scale)
	for(int j=0; j<leafs; j++) {
		glRotatef(z[j], 0.0, 1.0, 0.0);
		glScalef(sc[j], sc[j], sc[j]);
		//~ glBegin(GL_LINES);	
		//~ for(int i=1; i<points; i++) {
			//~ drawLine(x[i-1], y[i-1], 0.0,		
					 //~ x[i], y[i], 0.0);
		//~ }	
		//~ glEnd();
		
		glColor3f(1.0, 1.0, 1.0);
		for(int i=1; i<points; i++) {
			a = leaf_width*sin(rad((i-1)*n));
			b = leaf_width*sin(rad(i*n));			
			//~ textureHide();
			//~ glColor3f(1.0, 1.0, 0.0);
			//~ glBegin(GL_POINTS);			
				//~ glVertex3f(x[i-1], y[i-1], a);			
			//~ glEnd();
					
			//~ glBegin(GL_LINES);
				//~ glColor3f(1, 0, 0);
				//~ drawLine(x[i-1], y[i-1], 0.0,
					//~ x[i], y[i], 0.0);
				//~ drawLine(x[i], y[i], 0.0,
					//~ x[i], y[i], b);
				//~ drawLine(x[i], y[i], b,
					//~ x[i-1], y[i-1], 0.0);
					
				//~ drawLine(x[i-1], y[i-1], 0.0,
					//~ x[i-1], y[i-1], a);
				//~ drawLine(x[i-1], y[i-1], a,
					//~ x[i], y[i], b);
				//~ drawLine(x[i], y[i], b,
					//~ x[i-1], y[i-1], 0.0);
			//~ glEnd();
			
			// Faces
			textureBind(tex[faces[0].id].id);
			glBegin(GL_TRIANGLES);
				glTexCoord2f(0, y[i-1]/i);
				glVertex3f(x[i-1]*r[j], y[i-1], 0.0);
				glTexCoord2f(0, y[i-1]/i);
				glVertex3f(x[i]*r[j], y[i], 0.0);
				glTexCoord2f(1, y[i-1]/i);
				glVertex3f(x[i]*r[j], y[i], b);
								
				glTexCoord2f(0, y[i-1]/i);
				glVertex3f(x[i-1]*r[j], y[i-1], 0.0);
				glTexCoord2f(1, y[i-1]/i);
				glVertex3f(x[i-1]*r[j], y[i-1], a);
				glTexCoord2f(1, y[i-1]/i);
				glVertex3f(x[i]*r[j], y[i], b);
				
				//~ glTexCoord2f(0.0, 0.0);
				//~ glVertex3f(x[i-1], y[i-1], 0.0);
				//~ glTexCoord2f(1.0, 0.0);
				//~ glVertex3f(x[i], y[i], 0.0);
				//~ glTexCoord2f(1.0, 1.0);
				//~ glVertex3f(x[i], y[i], b);
				
				//~ glTexCoord2f(0.0, 0.0);
				//~ glVertex3f(x[i-1], y[i-1], 0.0);
				//~ glTexCoord2f(0.0, 1.0);
				//~ glVertex3f(x[i-1], y[i-1], a);
				//~ glTexCoord2f(1.0, 1.0);
				//~ glVertex3f(x[i], y[i], b);
			glEnd();
		}			
		glScalef(1/sc[j], 1/sc[j], 1/sc[j]);
		glRotatef(-z[j], 0.0, 1.0, 0.0);
	}
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glFlush();
	
	glScalef(1.0/scale1, 1.0/scale1, 1.0/scale1);
	glTranslatef(-posx, -posy, -posz);
}
