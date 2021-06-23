/* FILE NAME   : rndbase.c
 * PROGRAMMER  : MH5
 * LAST UPDATE : 17.06.2021
 * PURPOSE     : 3D animation rendeing global module.
 */

#include "rnd.h"
#include "rnd.h"
#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

VOID MH5_RndInit( HWND hWnd )
{
  INT i, nums;
  PIXELFORMATDESCRIPTOR pfd = {0};
  HGLRC hRC;
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
                                  /* WGL_CONTEXT_CORE_PROFILE_BIT_ARB, */
    0
  };

  MH5_hRndWnd = hWnd;
  MH5_hRndDC = GetDC(hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(MH5_hRndDC, &pfd);
  DescribePixelFormat(MH5_hRndDC, i, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
  SetPixelFormat(MH5_hRndDC, i, &pfd);

  /* OpenGL init: setup rendering context */
  MH5_hRndGLRC = wglCreateContext(MH5_hRndDC);
  wglMakeCurrent(MH5_hRndDC, MH5_hRndGLRC);

  /* Initializing GLEW library */
  if (glewInit() != GLEW_OK)
  {
    MessageBox(MH5_hRndWnd, "Error extensions initializing", "Error",
      MB_ICONERROR | MB_OK);
    exit(0);
  }

  if (!(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    MessageBox(MH5_hRndWnd, "Error: no shaders support", "Error", MB_ICONERROR | MB_OK);
    exit(0);
  }

  /* Enable a new OpenGL profile support */
  wglChoosePixelFormatARB(MH5_hRndDC, PixelAttribs, NULL, 1, &i, &nums);
  hRC = wglCreateContextAttribsARB(MH5_hRndDC, NULL, ContextAttribs);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(MH5_hRndGLRC);

  MH5_hRndGLRC = hRC;
  wglMakeCurrent(MH5_hRndDC, MH5_hRndGLRC);
  /* Set default OpenGL parameters */
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.30, 0.47, 0.8, 1);
  MH5_RndShadersInit();
}

VOID MH5_RndClose( VOID )
{
  MH5_RndShadersClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(MH5_hRndGLRC);
  ReleaseDC(MH5_hRndWnd, MH5_hRndDC);
}

VOID MH5_RndProjSet( VOID )
{
  DBL ratio_x, ratio_y;

  ratio_x = ratio_y = MH5_RndProjSize / 2;
  if (MH5_RndFrameW > MH5_RndFrameH)
    ratio_x *= (DBL)MH5_RndFrameW / MH5_RndFrameH;
  else
    ratio_y *= (DBL)MH5_RndFrameH / MH5_RndFrameW;

  MH5_RndMatrProj =
    MatrFrustum(-ratio_x, ratio_x, -ratio_y, ratio_y, MH5_RndProjDist, MH5_RndProjFarClip);
  MH5_RndMatrVP = MatrMulMatr(MH5_RndMatrView, MH5_RndMatrProj);
}

VOID MH5_RndResize(INT W, INT H)
{
  glViewport(0, 0, W, H);
  MH5_RndFrameW = W;
  MH5_RndFrameH = H;
  MH5_RndProjSet();
}

VOID MH5_RndStart( VOID )
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

VOID MH5_RndEnd( VOID )
{
  glFinish();
}

VOID MH5_RndCopyFrame( VOID )
{
  wglSwapLayerBuffers(MH5_hRndDC, WGL_SWAP_MAIN_PLANE);
}

VOID MH5_RndCamSet( VEC Loc, VEC At, VEC Up1 )
{
  MH5_RndMatrView = MatrView(Loc, At, Up1);
  MH5_RndMatrVP = MatrMulMatr(MH5_RndMatrView, MH5_RndMatrProj);
}

/* END OF 'rndbase.c' FILE */
