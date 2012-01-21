/*
 Creation d'un masque
 Pour chaque image
 Chargement de l'image
 Reset du masque
 Pour chaque pixels de l'image
 Si pixel.valeur >= seuil
 Sauvegarde du pixels sur le masque
 /Si
 /Pour
 Traitement du masque
 Ajout du masque au master
 Ajout du master à l'image
 Sauvegarde de l'image/Users/tomgay/Documents/IMAC/IMAC1/Semestre2/Algorithmie/lemmings/ImageRGB.h
 /Pour
 */


#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

void loadImage(const char *path, unsigned char *image, unsigned int width, unsigned int height);
void resetMask(unsigned char *mask, unsigned int width, unsigned int height);
void writeMask(unsigned char *image, unsigned char *mask, unsigned int seuil, unsigned int width, unsigned int height);
void writeMaster(unsigned char *master, unsigned char *mask, unsigned int width, unsigned int height);
void writeImage(unsigned char *image, unsigned char *master, unsigned int width, unsigned int height);
void saveImage(const char *path, unsigned char *image, unsigned int width, unsigned int height);

#endif
