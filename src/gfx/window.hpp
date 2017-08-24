#ifndef WINDOW_H
#define WINDOW_H

#include <EGL/egl.h>
#include <GLES2/gl2.h>

class GameWindow {
    EGLNativeWindowType native_window;
    EGLDisplay egl_display;
    EGLContext egl_context;
    EGLSurface egl_surface;

   public:
    GLint width;
    GLint height;

    auto create(EGLint attrs[], int w, int h) -> int {
        width = w;
        height = h;

        EGLint num_cfg, major_ver, minor_ver;
        EGLConfig config;
        EGLint ctx_attrs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE};

        egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (egl_display == EGL_NO_DISPLAY) return 1; /* error */

        if (!eglInitialize(egl_display, &major_ver, &minor_ver))
            return 1; /* error */

        if (!eglGetConfigs(egl_display, NULL, 0, &num_cfg))
            return 1; /* error */

        if (!eglChooseConfig(egl_display, attrs, &config, 1, &num_cfg))
            return 1; /* error */

        egl_surface = eglCreateWindowSurface(egl_display, config, NULL, NULL);
        if (egl_surface == EGL_NO_SURFACE) return 1; /* error */

        egl_context = eglCreateContext(egl_display, config, EGL_NO_CONTEXT,
                                       ctx_attrs);
        if (egl_context == EGL_NO_CONTEXT) return 1; /* error */

        if (!eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context))
            return 1; /* error */

        return 0;
    }

    auto update() -> int {
        eglSwapBuffers(&egl_display, &egl_surface);

        return 0;
    }

    virtual ~GameWindow() { // TODO: destroy context
    }
};
#endif  // WINDOW_H
