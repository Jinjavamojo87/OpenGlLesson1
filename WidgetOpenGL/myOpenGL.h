#ifndef MYOPENGL_H
#define MYOPENGL_H
#include<QGLWidget>
#include<GL/gl.h>
#include <memory>
class OsgViewerFromWindow;

class myOpenGL : public QGLWidget
{
public:
    myOpenGL();

protected:
    virtual void initializeGL() override;

    // glutReshapeFunc
    virtual void resizeGL(int w, int h) override;

    //glutDisplayFunc
    virtual void paintGL() override;

    virtual void glDraw() override;


    void scene();


    virtual void timerEvent(QTimerEvent *) override;


    virtual void mousePressEvent(QMouseEvent *event) override;
/*
    virtual void mouseReleaseEvent(QMouseEvent *event) override
    {

    }
*/
    virtual void mouseMoveEvent(QMouseEvent *event) override;
private:
    //std::unique_ptr<OsgViewerFromWindow> viewer;
    void Display(void);
    void mainLoop(void);
//    QTimer timer;
};

#endif // MYOPENGL_H
