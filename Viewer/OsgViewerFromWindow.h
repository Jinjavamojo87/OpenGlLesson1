#ifndef OSGVIEWERFROMWINDOW_H
#define OSGVIEWERFROMWINDOW_H

#include "OsgIncluder.h"
#include <QMouseEvent>
#include <Singleton.h>
#include <osgGA/CameraManipulator>

class OsgViewerFromWindow : public Singleton<OsgViewerFromWindow>
{
public:
    enum MODE_VIEWER{MODE_OBJECT_DETECTED, MODE_OBJECT_VIEW};

    using ON_CHANGING_FUNC = std::function<void(const MODE_VIEWER&)>;

    OsgViewerFromWindow();
    ~OsgViewerFromWindow();

    void resize(int w, int h);
    void update();

    OsgViewerFromWindow(const OsgViewerFromWindow& ) = delete;

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void setPos(double mx[16]);

    void setProjectionMatrix(double p[16]);

    void setVisibleModel(bool visible);

    const MODE_VIEWER getMode() const;

    void setMode(const MODE_VIEWER& mode);

    void addListenerOfEventIntersector(const ON_CHANGING_FUNC& f);

private:
    MODE_VIEWER m_mode{MODE_VIEWER::MODE_OBJECT_DETECTED};

    osg::ref_ptr<osgViewer::Viewer> viewer;
    osg::ref_ptr<osgViewer::GraphicsWindow> windowGL{nullptr};

    osg::ref_ptr<osg::MatrixTransform> m_model{nullptr};
    osg::ref_ptr<osg::MatrixTransform> m_pos{nullptr};
    osg::ref_ptr<osg::MatrixTransform> m_root{nullptr};
    osg::ref_ptr<osg::MatrixTransform> m_superroot{nullptr};

    osg::ref_ptr<osg::Camera> m_hudCamera{nullptr};

    osg::Geode *backButtonRectangle{nullptr};
    osg::Geode *backButtonText{nullptr};
    osg::Geode *leftArrowImage{nullptr};
    osg::Geode *cameraIconImage{nullptr};
    osg::Geode *textOverTransparentRec{nullptr};

    double m_projection[16];

    osg::Vec3d eye, up, center;

    std::vector<ON_CHANGING_FUNC> func;

    void setTransparentBackground(bool);
    void updateMode();
    void createHudCamera(int w, int h);


};

#endif // OSGVIEWERFROMWINDOW_H
