//////////////////////////////////////////////////////////////////////
// (c) Janusz Ganczarski
// http://www.januszg.hg.pl
// JanuszG@enter.net.pl
//////////////////////////////////////////////////////////////////////
#define GLEW_STATIC 
#include "textures.h"
#include <FreeImage.h>
#include <iostream>
#include "wszystko.h"
bool LoadTexture(const char *fileName, GLenum target, GLint level)
{

#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fileName, 0);

	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(fileName);

	FIBITMAP *dib;
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, fileName);

	if (!dib)
	{
		return false;
	}

	FreeImage_FlipVertical(dib);

	GLenum format;
	GLenum internalformat;
	switch (FreeImage_GetColorType(dib))
	{
	case FIC_MINISWHITE:
	case FIC_MINISBLACK:
		format = GL_RED;
		internalformat = GL_RED;
		break;

	case FIC_PALETTE:
	{
		FIBITMAP *dibTmp = FreeImage_ConvertTo24Bits(dib);
		FreeImage_Unload(dib);
		dib = dibTmp;
	}

	case FIC_RGB:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
		format = GL_RGB;
#else
		format = GL_BGR;
#endif
		internalformat = GL_RGB;
		break;

	case FIC_RGBALPHA:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
		format = GL_RGBA;
#else
		format = GL_BGRA;
#endif
		internalformat = GL_RGBA;
		break;

	default:
		return false;
	}

	GLenum type;
	if (FreeImage_GetImageType(dib) == FIT_BITMAP)
		switch (FreeImage_GetBPP(dib))
		{
		case 24:
			if (internalformat == GL_RGB)
				type = GL_UNSIGNED_BYTE;
			break;
		case 32:
			if (internalformat == GL_RGBA)
				type = GL_UNSIGNED_BYTE;
			else
				if (internalformat == GL_RGB)
				{
					type = GL_UNSIGNED_BYTE;
					internalformat = GL_RGBA;
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
					format = GL_RGBA;
#else
					format = GL_BGRA;
#endif
				}
			break;
		case 8:
			if (internalformat == GL_RED)
				type = GL_UNSIGNED_BYTE;
			break;

		default:
			return false;
		}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glTexImage2D(target, level, internalformat, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib),
		0, format, type, FreeImage_GetBits(dib));

	FreeImage_Unload(dib);

#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
#endif

	return true;
}

bool LoadTextures(const char **fileNames, GLenum target, GLint layers)
{
	if (layers <= 0)
		return false;

#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif

	for (int i = 0; i < layers; i++)
	{
		ERR
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(fileNames[i], 0);
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(fileNames[i]);
		if (fif == FIF_UNKNOWN)
			return false;
		FIBITMAP *dib;
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, fileNames[i]);
		if (!dib)
		{
			return false;
		}
		FreeImage_FlipVertical(dib);
		GLenum format;
		GLenum internalformat;
		switch (FreeImage_GetColorType(dib))
		{
		case FIC_MINISWHITE:
		case FIC_MINISBLACK:
			format = GL_RED;
			internalformat = GL_RED;
			break;
		case FIC_PALETTE:
		{
			FIBITMAP *dibTmp = FreeImage_ConvertTo24Bits(dib);
			FreeImage_Unload(dib);
			dib = dibTmp;
		}

		case FIC_RGB:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
			format = GL_RGB;
#else
			format = GL_BGR;
#endif
			internalformat = GL_RGB;
			break;

		case FIC_RGBALPHA:
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_RGB
			format = GL_RGBA;
#else
			format = GL_BGRA;
#endif
			internalformat = GL_RGBA;
			break;

		default:
			return false;
		}

		GLenum type;
		if (FreeImage_GetImageType(dib) == FIT_BITMAP)
			switch (FreeImage_GetBPP(dib))
			{
			case 24:
				if (internalformat == GL_RGB)
					type = GL_UNSIGNED_BYTE;
				break;
			case 32:
				if (internalformat == GL_RGBA)
					type = GL_UNSIGNED_BYTE;
				break;
			case 8:
				if (internalformat == GL_RED)
					type = GL_UNSIGNED_BYTE;
				break;

			default:
				return false;
			}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		if (i == 0)
			glTexImage3D(target, 0, internalformat,
				FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), layers,
				0, format, type, NULL);

		glTexSubImage3D(target, 0, 0, 0, i,
			FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 1,
			format, type, FreeImage_GetBits(dib));

		FreeImage_Unload(dib);
	}

#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
#endif

	return true;
}
/*
bool SaveCubeTexture(const char **fileNames, GLint level)
{
	// inicjalizacja biblioteki FreeImage w wersji statycznej
#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif

	// tryb upakowania bajt�w danych tekstury
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	// pobranie rozmiar�w tekstury
	GLint width, height;
	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, GL_TEXTURE_HEIGHT, &height);

	// utworzenie bitmapy
	FIBITMAP *dib = FreeImage_Allocate(width, height, 24);
	if (dib)
	{
		// bufor na dane poszczeg�lnych stron tekstury sze�ciennej
		GLubyte *buf = new GLubyte[width * height * 3];

		// zapis kolejnych tekstur
		for (int i = 0; i < 6; i++)
		{
			// pobranie danych tektury
			glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, GL_UNSIGNED_BYTE, buf);

			// kopiowanie danych pikseli
			GLubyte *pbuf = buf;
			unsigned int pitch = FreeImage_GetPitch(dib);
			BYTE *bits = (BYTE*)FreeImage_GetBits(dib);
			for (int y = 0; y < height; y++)
			{
				BYTE *pixel = (BYTE*)bits;
				for (int x = 0; x < width; x++)
				{
					pixel[FI_RGBA_RED] = *pbuf++;
					pixel[FI_RGBA_GREEN] = *pbuf++;
					pixel[FI_RGBA_BLUE] = *pbuf++;
					pixel += 3;
				}

				// nast�pny wiersz
				bits += pitch;
			}

			// obr�cenie obrazu
			FreeImage_FlipVertical(dib);

			// zapis pliku
			if (FreeImage_Save(FIF_PNG, dib, fileNames[i], 0) == false)
			{
				// porz�dki
				delete[] buf;
				FreeImage_Unload(dib);
				return false;
			}
		}

		// porz�dki
		delete[] buf;
		FreeImage_Unload(dib);
	}
	else
		return false;

	// deinicjalizacja biblioteki FreeImage w wersji statycznej
#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
#endif

	// sukces
	return true;
}
*/