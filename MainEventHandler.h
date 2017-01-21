//
// Created by anatoly on 1/17/17.
//

#ifndef ARFLY_MAINEVENTHANDLER_H
#define ARFLY_MAINEVENTHANDLER_H

#include <osg/Matrix>
#include <osg/Math>
#include <osg/FrontFace>
#include <osg/LightModel>
#include <osg/Node>
#include <osgGA/GUIEventHandler>
#include <functional>
#include <osgViewer/Viewer>
#include "Singleton.h"
#include <osg/Drawable>
#include <osgUtil/LineSegmentIntersector>
#include <osg/ref_ptr>

class MainEventHandler : public osgGA::GUIEventHandler, public Singleton<MainEventHandler>
{
public:
    using VOID_FUNC = std::function<void()>;
    MainEventHandler();

    void addListenerOfEventIntersector(const VOID_FUNC& f)
    {
        func.push_back(f);
    }

    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& alfa) override;

protected:
    ~MainEventHandler()
    {}

private:
    osg::Node* m_camera{nullptr};
    std::vector<VOID_FUNC> func;
};


#endif //ARFLY_MAINEVENTHANDLER_H
