//
// Created by anatoly on 1/17/17.
//

#include <Viewer/OsgViewerFromWindow.h>
#include "MainEventHandler.h"
#include "Viewer/OsgViewerFromWindow.h"

MainEventHandler::MainEventHandler()
{

}


bool MainEventHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& alfa)
{
    m_camera = (dynamic_cast<osgViewer::Viewer*>(&alfa))->getCamera();
    switch (ea.getButton())
    {
        case(osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON) :
        {
            osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector = new osgUtil::LineSegmentIntersector(osgUtil::Intersector::PROJECTION, ea.getXnormalized(), ea.getYnormalized());
            osgUtil::IntersectionVisitor iv(intersector);

            m_camera->accept(iv);
            if (intersector->containsIntersections())
            {
                osgUtil::LineSegmentIntersector::Intersections &intersections = intersector->getIntersections();
                for (const osgUtil::LineSegmentIntersector::Intersection& alfa : intersections)
                {
                    const std::string& clickedItemName = alfa.drawable->getParent(0)->getName();
                    if (clickedItemName == "backButtonRectangle" || clickedItemName == "backButtonText" ||
                            clickedItemName == "leftArrowImage")
                    {
                        OsgViewerFromWindow::get().setMode(OsgViewerFromWindow::MODE_OBJECT_DETECTED);
                        break;
                    }

                    if (clickedItemName != "cameraTitleImage"
                        && clickedItemName != "transparentTitleRectagle"
                        && clickedItemName != "cameraTitleText"
                        && clickedItemName != "modelNameText")
                    {
                        for (const auto &f : func)
                            f();

                        break;
                    }
                }
            }
            break;
        }

        default:
            break;

    }
    return false;
}