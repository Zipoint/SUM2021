/* FILE NAME: rndtex.c
 * PROGRAMMER: MH5
 * DATE: 23.06.2021
 * PURPOSE: 3D animation rendering shader handle functions module.
 */

#include <stdio.h>
#include <string.h>

#include "rndres.h"

mh5TEXTURE MH5_RndTextures[MH5_MAX_TEXTURE];
INT MH5_RndTextureSize;

VOID MH5_RndTexInit( VOID )
{
  MH5_RndTextureSize = 0;
}

VOID MH5_RndTexClose( VOID )
{
  INT i;

  for (i = 0; i < MH5_RndTextureSize; i++)
    glDeleteTextures(1, &MH5_RndTextures[i].TexId);
  MH5_RndTextureSize = 0;
}

INT MH5_RndTexAddImg( CHAR *Name, INT W, INT H, INT C, VOID *Bits )
{
  INT mips;

  if (MH5_RndTextureSize >= MH5_MAX_TEXTURE)
    return -1;

  strncpy(MH5_RndTextures[MH5_RndTextureSize].Name, Name, MH5_STR_MAX - 1);
  MH5_RndTextures[MH5_RndTextureSize].W = W;
  MH5_RndTextures[MH5_RndTextureSize].H = H;
  glGenTextures(1, &MH5_RndTextures[MH5_RndTextureSize].TexId);

  glBindTexture(GL_TEXTURE_2D, MH5_RndTextures[MH5_RndTextureSize].TexId);

  mips = W > H ? H : W;
  mips = log(mips) / log(2);
  if (mips < 1)
    mips = 1;

  glTexStorage2D(GL_TEXTURE_2D, mips, C == 3 ? GL_RGB8 : C == 4 ? GL_RGBA8 : GL_R8, W, H);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, W, H, C == 3 ? GL_BGR : C == 4 ? GL_BGRA : GL_LUMINANCE, GL_UNSIGNED_BYTE, Bits);

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  return MH5_RndTextureSize++;
}

INT MH5_RndTexAddFromFile( CHAR *FileName )
{
  INT w, h, x, y, n = -1, bpp, flen;
  HBITMAP hBm;
  BITMAP bm;
  FILE *F;

  if ((hBm = LoadImage(NULL, FileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION)) != NULL)
  {
    GetObject(hBm, sizeof(bm), &bm);
    w = bm.bmWidth;
    h = bm.bmHeight;
    if (bm.bmBitsPixel == 24)
    {
      BYTE *Bits = bm.bmBits;
      BYTE *mem;

      if ((mem = malloc(w * h * 3)) != NULL)
      {
        for (y = 0; y < h; y++)
          for (x = 0; x < w; x++)
          {
            BYTE *ptr = &((BYTE *)bm.bmBits)[y * bm.bmWidthBytes];

            mem[(y * w + x) * 3 + 0] = *ptr++;
            mem[(y * w + x) * 3 + 1] = *ptr++;
            mem[(y * w + x) * 3 + 2] = *ptr++;
          }
          n = MH5_RndTexAddImg(FileName, w, h, 3, mem);
          free(mem);
      }
      DeleteObject(hBm);
    }
  }
  else if ((F = fopen(FileName, "rb")) != NULL)
  {
    DWORD *mem;

    w = 0;
    h = 0;

    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);
    fseek(F, 0, SEEK_END);
    flen = ftell(F);
    fseek(F, 4, SEEK_SET);
    if (flen == 4 + w * h * 4)
      bpp = 4;
    else
      bpp = 3;

    if ((mem = malloc(w * h * 4)) != NULL)
    {
      for (y = 0; y < h; y++)
        for (x = 0; x < w; x++)
        {
          DWORD Color = 0xFF000000;

          fread(&Color, bpp, 1, F);
          mem[(h - 1 - y) * w + x] = Color;
        }
        n = MH5_RndTexAddImg(FileName, w, h, 4, mem);
        free(mem);
    }
    fclose(F);
  }
  return n;
}


/* END OF 'rndtex.c' FILE */