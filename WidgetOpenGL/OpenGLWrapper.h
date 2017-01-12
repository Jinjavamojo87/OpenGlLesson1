#ifndef MYOPENGL_H
#define MYOPENGL_H
#include<QGLWidget>
#include<GL/gl.h>

class OpenGLWrapper : public QGLWidget
{
public:
    OpenGLWrapper();

protected:
    virtual void initializeGL() override;

    // glutReshapeFunc
    virtual void resizeGL(int w, int h) override;

    //glutDisplayFunc
    virtual void paintGL() override;

    virtual void glDraw() override;

    void scene();

    virtual void timerEvent(QTimerEvent *) override;

private:
//    QTimer timer;
};

#endif
