/* FILE NAME: rndtex.c
 * PROGRAMMER: MH5
 * DATE: 23.06.2021
 * PURPOSE: 3D animation rendering shader handle functions module.
 */

#include <stdio.h>
#include <string.h>

#include "rndres.h"

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
  if (MH5_RndTextureSize >= MH5_MAX_TEXTURE)
    return -1;

  glGenTextures(1, &MH5_RndTextures[MH5_RndTextureSize].TexId);
  glBindTexture(GL_TEXTURE_2D, MH5_RndTextures[MH5_RndTextureSize].TexId);

  glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  strncpy(MH5_RndTextures[MH5_RndTextureSize].Name, Name, MH5_STR_MAX - 1);
  MH5_RndTextures[MH5_RndTextureSize].W = W;
  MH5_RndTextures[MH5_RndTextureSize].H = H;
  glBindTexture(GL_TEXTURE_2D, 0);

  return MH5_RndTextureSize++;
}

VOID MH5_RndTexAdd( CHAR *FileName )
{
  FILE *F;

  if ((F = fopen(FileName, "rb")) != NULL)
  {
    INT w = 0, h = 0;
    BYTE *mem;

    fread(&w, 2, 1, F);
    fread(&h, 2, 1, F);
    if ((mem = malloc(w * h * 3)) != NULL)
    {
      INT n;

      fread(mem, 3, w * h, F);

      glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, mem);

      n = log(w > h ? w : h) / log(2);
      n = n < 1 ? 1 : n;

      glTexStorage2D(GL_TEXTURE_2D, n, GL_RGB8, w, h);
      glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_BGR, GL_UNSIGNED_BYTE, mem);

      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      free(mem);
    }
    fclose(F);
  }
}


/* END OF 'rndtex.c' FILE */