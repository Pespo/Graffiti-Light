
 #include "headers.hpp"
 
void pause();
 
int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO); // Initialisation de la SDL
 
    SDL_SetVideoMode(800, 600, 32, SDL_HWSURFACE); // Ouverture de la fenêtre

	glEnable(GL_TEXTURE_RECTANGLE);
	int taille_max = 800;
	glGetIntegerv(GL_MAX_RECTANGLE_TEXTURE_SIZE,&taille_max);
	std::cout << " Taille max de texture supportée : " << taille_max << std::endl;
	GLuint rect_tex;
	glGenTextures(1,&rect_tex);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,rect_tex); // Exactement comme pour les tex2D
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGB8, 800, 600, 0, GL_RGB, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

	GLuint fbo;
	glGenFramebuffersEXT(1, &fbo);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,GL_TEXTURE_RECTANGLE_ARB, rect_tex, 0);

	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
	glEnable(GL_TEXTURE_2D);
	// Dessin

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0); // cela dessine dans le buffer normal, c'est à dire l'écran.
	// Un dessin...
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,rect_tex);
	// un autre dessin, texturé avec cette texture-ci.
	// rappellez vous que les bornes sont [0..w] et [0..h] et non [0..1] !

	glDeleteFramebuffersEXT(1, &fbo);
    
	//pause(); // Mise en pause du programme
 
    SDL_Quit(); // Arrêt de la SDL
 
    return EXIT_SUCCESS; // Fermeture du programme
}
 
void pause()
{
    int continuer = 1;
    SDL_Event event;
 
    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}