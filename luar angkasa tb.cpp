#include <GL/glew.h>
#include <GL/glut.h>
#include <FreeImage.h>
#include <stdio.h>

// Untuk menyimpan ID texture yang sudah dibuat
GLuint texture_Sun_ID;
GLuint texture_Earth_ID;
GLuint texture_Moon_ID;
GLuint texture_Star_ID;

float scaleFactor = 1.0f; // Mengatur zoom tampilan awal pada tata surya
float tick = 0.0f; // Mengatur rotasi tata surya
int viewMode = 0; // Menetukan mode tampilan awal (0: Semua, 1: Matahari, 2: Bumi, 3: Bulan)
float cameraX = 0.0f, cameraY = 10.0f, cameraZ = 30.0f; // Posisi tampilan kamera
float lookAtX = 0.0f, lookAtY = 0.0f, lookAtZ = 0.0f;    // Arah pandang kamera
bool hidden = false; // Menyembunyikan/memunculkan tampilan awal garis kartesius

// Fungsi menggambar sumbu kartesius
void drawCartecius(){
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	
	glVertex3f(-200.0, 0.0, 0.0);
	glVertex3f(200.0, 0.0, 0.0);
	
	glVertex3f(0.0, -200.0, 0.0);
	glVertex3f(0.0, 200.0, 0.0);
	
	glVertex3f(0.0, 0.0, -200.0);
	glVertex3f(0.0, 0.0, 200.0);
	glEnd();
}

void hiddenCarte(){
	if(hidden){
		drawCartecius();
	}
}

// Fungsi untuk membuat objek bola dengan texture
void createObject(float radius) {
    GLUquadric* object = gluNewQuadric();
    gluQuadricTexture(object, GL_TRUE); // Mengaktifkan tekstur
    gluSphere(object, radius, 90, 100); // Membuat bola
}

// Fungsi untuk membuat latar belakang berbentuk persegi
void drawBackground() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glBindTexture(GL_TEXTURE_2D, texture_Star_ID);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, 1.0f);
    glEnd();

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// Fungsi untuk menampilkan teks pada layar
void renderText(float x, float y, void* font, const char* string, float r, float g, float b) {
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*string) {
        glutBitmapCharacter(font, *string++);
    }
}

void setupLighting() {
    glDisable(GL_LIGHTING); //aktifin cahaya
    glEnable(GL_LIGHT0); //aktifin sumber cahaya pertama

    // Mengatur properti pencahayaan (sumber cahaya)
    GLfloat light_position[] = {0.0, 0.0, 0.0, 1.0}; // Cahaya berasal dari matahari
    GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
    GLfloat light_diffuse[] = {1.0, 1.0, 0.8, 1.0};
    GLfloat light_specular[] = {1.0, 1.0, 0.8, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0f / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, // Posisi tampilan kamera
              lookAtX, lookAtY, lookAtZ, // Titik pandang kamera
              0.0f, 1.0f, 0.0f);         // Up vector (menentukan arah 'atas')
}

void update(int value) {
    tick += 0.1f; // Kecepatan rotasi tata surya
    glutTimerFunc(10, update, 0);
    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    hiddenCarte();
    glPopMatrix();
    
    drawBackground();
    
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, lookAtX, lookAtY, lookAtZ, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glScalef(scaleFactor, scaleFactor, scaleFactor);

    // Menampilkan Matahari
    if (viewMode == 0 || viewMode == 1) {
        glPushMatrix();
        glRotatef(-90, 1.0, 0.0, 0.0);
        glRotatef(tick += 0.01, 0.0, 0.0, 1.0);

        GLfloat sun_emission[] = {1.0, 0.5, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, sun_emission);

        glBindTexture(GL_TEXTURE_2D, texture_Sun_ID);
        createObject(4.0);

        GLfloat no_emission[] = {0.0, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);

        glPopMatrix();
    }

   // Menampilkan Bumi
    if (viewMode == 0 || viewMode == 2) {
        glPushMatrix();
        glRotatef(tick * 2, 0.0, 1.0, 0.0);
        glTranslatef(10.0, 0.0, 0.0);

        glPushMatrix(); // misahin bumi ke matahari sm bumi berporos sendiri
        glRotatef(tick * 4, 0.0, 1.0, 0.0); // rotasi bumi
        glBindTexture(GL_TEXTURE_2D, texture_Earth_ID);
        createObject(2.0);

        glPopMatrix();
    }

    // Menampilkan Bulan
    if (viewMode == 0 || viewMode == 3) {
        glPushMatrix();
        glRotatef(tick * 5, 0.0, 1.0, 0.0);
        glTranslatef(4.0, 0.0, 0.0);

        glPushMatrix();
        glRotatef(tick * 4, 0.0, 1.0, 0.0);
        glBindTexture(GL_TEXTURE_2D, texture_Moon_ID);
        createObject(1.0);
        glPopMatrix();

        glPopMatrix();
    }

    glPopMatrix();

    // Menampilkan GUI Label
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 100, 0, 100);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Menampilkan label mode
    if (viewMode == 0)
        renderText(5, 95, GLUT_BITMAP_HELVETICA_18, "Mode: Semua", 1.0, 1.0, 1.0);
    else if (viewMode == 1)
        renderText(5, 95, GLUT_BITMAP_HELVETICA_18, "Mode: Matahari", 1.0, 1.0, 0.0);
    else if (viewMode == 2)
        renderText(5, 95, GLUT_BITMAP_HELVETICA_18, "Mode: Bumi", 0.0, 1.0, 1.0);
    else if (viewMode == 3)
        renderText(5, 95, GLUT_BITMAP_HELVETICA_18, "Mode: Bulan", 0.5, 0.5, 1.0);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------//
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case '1': viewMode = 1; break;
        case '2': viewMode = 2; break;
        case '3': viewMode = 3; break;
        case '0': viewMode = 0; break;
        case '+': // Zoom in
            scaleFactor += 0.1f;
            if (scaleFactor > 3.0f) scaleFactor = 3.0f; // Batas maksimum zoom in
            break;
        case '-': // Zoom out
            scaleFactor -= 0.1f;
            if (scaleFactor < 0.1f) scaleFactor = 0.1f; // Batas minimum zoom out
            break;
		case 'c':
			hidden = !hidden;
			break;
        case 27: exit(0); break;  // ESC key
    }
    glutPostRedisplay();
}

GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    FREE_IMAGE_FORMAT format = FreeImage_GetFIFFromFilename(path);
    if (format == FIF_UNKNOWN) {
        printf("Unknown file type for texture image file %s\n", path);
        return 0;
    }
    
    FIBITMAP* bitmap = FreeImage_Load(format, path, 0);
    if (!bitmap) {
        printf("Failed to load image %s\n", path);
        return 0;
    }
    
    FIBITMAP* bitmap2 = FreeImage_ConvertTo24Bits(bitmap);
    FreeImage_Unload(bitmap);
    
    unsigned char* imgData = FreeImage_GetBits(bitmap2);
    int width = FreeImage_GetWidth(bitmap2);
    int height = FreeImage_GetHeight(bitmap2);
    
    if (imgData) {
        printf("Texture image loaded from file %s, size %dx%d\n", path, width, height);
        int format = (FI_RGBA_RED == 0) ? GL_RGB : GL_BGR;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, imgData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        FreeImage_Unload(bitmap2);
        return textureID;
    }
    
    printf("Failed to get texture data from %s\n", path);
    FreeImage_Unload(bitmap2);
    return 0;
}

void initProjection() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);

    // Load textures
    texture_Sun_ID = loadTexture("textures/sun.jpg");
    texture_Earth_ID = loadTexture("textures/bumi.png");
    texture_Moon_ID = loadTexture("textures/moon.jpg");
    texture_Star_ID = loadTexture("textures/star2.jpg");

    setupLighting();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("TB KELOMPOK 2");

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "GLEW initialization error: %s\n", glewGetErrorString(err));
        return 1;
    }

    initProjection();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
