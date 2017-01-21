#include "OsgViewerFromWindow.h"

#include "OsgIncluder.h"
#include "MainEventHandler.h"
#include <osg/Depth>
#include <QtCore/QString>

void OsgViewerFromWindow::createHudCamera(int w, int h)
{
    m_hudCamera = new osg::Camera;
    m_hudCamera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    m_hudCamera->setProjectionMatrixAsOrtho2D(0, w, 0, h);
    m_hudCamera->setProjectionResizePolicy(osg::Camera::FIXED);
    m_hudCamera->setViewMatrix(osg::Matrix::identity());
    m_hudCamera->setRenderOrder(osg::Camera::POST_RENDER);
    m_hudCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
}

osg::Geode* createHudImage(const osg::Vec2& position, const float& width, const float& height, const std::string& imagePath) {

    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(osg::Vec3(position.x(), position.y(), 0));
    vertices->push_back(osg::Vec3(position.x(), position.y() + height, 0));
    vertices->push_back(osg::Vec3(position.x() + width, position.y() + height, 0));
    vertices->push_back(osg::Vec3(position.x() + width, position.y(), 0));

    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
    osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
    texcoords->push_back(osg::Vec2(0.0f, 0.0f));
    texcoords->push_back(osg::Vec2(0.0f, 1.0f));
    texcoords->push_back(osg::Vec2(1.0f, 1.0f));
    texcoords->push_back(osg::Vec2(1.0f, 0.0f));

    osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
    quad->setVertexArray(vertices.get());
    quad->setNormalArray(normals.get());
    quad->setNormalBinding(osg::Geometry::BIND_OVERALL);
    quad->setTexCoordArray(0, texcoords.get());
    quad->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(imagePath);
    osg::ref_ptr<osg::Geode> imageGeode = new osg::Geode;
    if (image != nullptr) {
        texture->setImage(image.get());
        imageGeode->addDrawable(quad.get());
        osg::StateSet *stateset = imageGeode->getOrCreateStateSet();
        stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
        stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
        stateset->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);
        stateset->setMode(GL_BLEND, osg::StateAttribute::ON);

        stateset->setTextureAttributeAndModes(0, texture.get());
    } else {
        std::cerr << "Couldn't load image " << imagePath << std::endl;
    }
    return imageGeode.release();
}

osg::Geode *createHudRectangle(const float &width, const float &height, const osg::Vec3 &leftBottomPoint, const osg::Vec4 &color) {
    std::string name = "Rectangle";

    osg::Geometry *geometry = new osg::Geometry;

    osg::Vec3Array* vertices = new osg::Vec3Array(4);
    (*vertices)[0] = leftBottomPoint;
    (*vertices)[1] = leftBottomPoint + osg::Vec3(width, 0.0, 0.0);
    (*vertices)[2] = leftBottomPoint + osg::Vec3(width, height, 0.0);
    (*vertices)[3] = leftBottomPoint + osg::Vec3(0.0, height, 0.0);

    geometry->setVertexArray(vertices);
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

    osg::Vec4Array *colors = new osg::Vec4Array;
    colors->push_back(color);

    geometry->setColorArray(colors, osg::Array::BIND_OVERALL);

    osg::Geode *geode = new osg::Geode();
    geode->addDrawable(geometry);

    geode->setName(name);
    osg::StateSet *stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
    stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
    stateset->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);


    return geode;
}

void changeHubText(osg::Geode* geode, const char* text) {
    osgText::Text *pDrawable = dynamic_cast<osgText::Text *>(geode->getDrawable(0));
    if (pDrawable) {
        pDrawable->setText(text,osgText::String::ENCODING_UTF8);
    }
}

osg::Geode* createHudText(const char* text, const osg::Vec3& position, const float& fontSize, const osg::Vec4& color)
{
    osgText::Font *font = osgText::readFontFile(
            "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
    osg::Geode* geode = new osg::Geode();
    osg::StateSet* stateset = geode->getOrCreateStateSet();
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
    geode->setName("simple");
    osgText::Text* textObject = new  osgText::Text;
    geode->addDrawable(textObject);
    textObject->setFont(font);
    textObject->setColor(color);
    textObject->setCharacterSize(fontSize);
    textObject->setText(text, osgText::String::ENCODING_UTF8);
    auto centerPosition = position - textObject->getBound().center();
    //osg::Vec3(200,0.0,0.0 );// + osg::Vec3(x.yMax()-x.yMin(),x.xMax()-x.xMin(),0.0 );//textObject->getBound().center();
    textObject->setPosition(centerPosition);
    return geode;
}


void setOptions(osg::MatrixTransform* m)
{
    m->setMatrix(osg::Matrix());
    m->preMult(osg::Matrix::translate(4.5, 118.25, 30.0));
    m->preMult(osg::Matrix::rotate(osg::inDegrees(90.0), 1.0, 0.0, 0.0));
    m->preMult(osg::Matrix::scale(5.0, 5.0, 5.0));
    m->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON); // Rescale normals.
}

OsgViewerFromWindow::OsgViewerFromWindow()
{

    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile("OSG/simpleObj.3ds");
    if (!loadedModel)
    {
        return;
    }

    m_model = new osg::MatrixTransform();
    m_model->addChild(loadedModel.get());
    osgText::Font *font = osgText::readFontFile("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
    osgText::Text *text = new osgText::Text();
    text->setFont(font);
    text->setCharacterSize(20);
    text->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));

    text->setText("Индикатор многофункциональный \nжидкокристаллический МФИ-ЖК-2", osgText::String::ENCODING_UTF8);
    text->setAxisAlignment(osgText::Text::XY_PLANE);
    text->setAutoRotateToScreen(true);
    auto g = new osg::Geode;
    g->setName("modelNameText");
    g->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);
    g->addDrawable(text);
// osg::AutoTransform
    m_root = new osg::MatrixTransform();
    m_root->setMatrix(osg::Matrix());
    m_root->addChild(m_model.get());
    m_root->addChild(g);

    m_superroot = new osg::MatrixTransform();
    m_superroot->addChild(m_root);
    setOptions( m_model);
}

OsgViewerFromWindow::~OsgViewerFromWindow()
{

}

void OsgViewerFromWindow::resize(int w, int h)
{
    if (windowGL)
    {
        windowGL->resized(0, 0, w, h);
        viewer->getCamera()->setViewport(new osg::Viewport(0, 0, w, h));
        windowGL->getEventQueue()->windowResize(0, 0, w, h );
    }
    else
    {
        viewer = new osgViewer::Viewer;
        // setUpAsEmbeddedInWindow instantiates an osg::GraphicsContext and an osgGA::GUIActionAdapter
        // in an osgViewer::GraphicsWindow, and then adds no-op methods for the window-management related functions.
        windowGL = viewer->setUpViewerAsEmbeddedInWindow(0, 0, w, h); // Sets viewport, and Projection to perspective with 30 degree FOV.

        viewer->getCamera()->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
        viewer->getCamera()->setProjectionResizePolicy(osg::Camera::FIXED);
        viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR); // Don't let OSG choose the clipping values.
        setTransparentBackground(true);
        viewer->getCamera()->setViewport(new osg::Viewport(0, 0, w, h)); // To allow negative values for left / bottom, need to manually set the viewport.
        viewer->getCamera()->setProjectionMatrix(osg::Matrix::identity());
        viewer->getCamera()->setViewMatrix(osg::Matrix::identity());

        // Set lighting model.

#if defined(OSG_GL_FIXED_FUNCTION_AVAILABLE)
        osg::ref_ptr<osg::LightModel> lm = new osg::LightModel();
        lm->setAmbientIntensity(osg::Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
        viewer->getCamera()->getOrCreateStateSet()->setAttributeAndModes(lm, osg::StateAttribute::ON);
#endif
        // Make sure glFrontFace is set when the viewer is created.
        osg::ref_ptr<osg::FrontFace> ff = new osg::FrontFace(/*arOsg->frontFaceWinding ? osg::FrontFace::CLOCKWISE :*/ osg::FrontFace::COUNTER_CLOCKWISE);
        viewer->getCamera()->getOrCreateStateSet()->setAttributeAndModes(ff, osg::StateAttribute::ON);

        //auto eh =new MainEventHandler(viewer->getCamera());

        MainEventHandler::getPtr()->addListenerOfEventIntersector([=]() {
            setMode(MODE_OBJECT_VIEW);
        });
        viewer->addEventHandler(MainEventHandler::getPtr());
        viewer->setSceneData(m_superroot.get());
        viewer->realize(); // Is a no-op when using embeded viewer.
        viewer->getCamera()->getViewMatrixAsLookAt(eye, center, up);
        createHudCamera(w, h);
        m_superroot->addChild(m_hudCamera);

        const int transparentRecHeight = 70;
        const int cameraPicW = 32;
        const int cameraPicH = 26;
        const int cameraPositionX = w/2- (cameraPicW*2)-5;
        const int cameraPositionY = h - transparentRecHeight/2 - (cameraPicH/2);

        const int leftArrowPicW = 11;
        const int leftArrowPicH = 16;

        cameraIconImage = createHudImage(osg::Vec2(cameraPositionX, cameraPositionY), cameraPicW, cameraPicH, "icons/camera_white.png");
        cameraIconImage->setName("cameraTitleImage");

        leftArrowImage = createHudImage(osg::Vec2(25, h - transparentRecHeight / 2 - leftArrowPicH / 2), leftArrowPicW, leftArrowPicH, "icons/arrow_left.png");
        leftArrowImage->setName("leftArrowImage");

        auto rect = createHudRectangle(w, 100, osg::Vec3(0.0, h - transparentRecHeight, 0.0), osg::Vec4(0.0, 0.0, 0.0, 0.35));
        osg::StateSet *rectStateset = rect->getOrCreateStateSet();
        rectStateset->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);
        rect->setName("transparentTitleRectagle");

        textOverTransparentRec = createHudText("Камера", osg::Vec3(w / 2, h - transparentRecHeight / 2, 0.0), 20,osg::Vec4(1.0f,1.0f,1.0f,1.0f));
        textOverTransparentRec->setName("cameraTitleText");

        backButtonRectangle = createHudRectangle(125, 50, osg::Vec3(10.0, h - transparentRecHeight + 10, 0.0), osg::Vec4(1.0, 1.0, 1.0, 0.75));
        backButtonRectangle->setName("backButtonRectangle");
        osg::StateSet *backButtonRectangleStateset = rect->getOrCreateStateSet();
        backButtonRectangleStateset->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);

        backButtonText = createHudText("Назад", osg::Vec3(75.0, h - transparentRecHeight / 2, 0.0), 20, osg::Vec4(0.0f,0.0f,0.0f,1.0f));
        backButtonText->setName("backButtonText");
        osg::StateSet *backButtonTextStateset = rect->getOrCreateStateSet();
        backButtonTextStateset->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);


        m_hudCamera->addChild(textOverTransparentRec);
        m_hudCamera->addChild(backButtonText);
        m_hudCamera->addChild(backButtonRectangle);
        m_hudCamera->addChild(leftArrowImage);
        m_hudCamera->addChild(cameraIconImage);
        m_hudCamera->addChild(rect);

        backButtonRectangle->setNodeMask(0);
        backButtonText->setNodeMask(0);
        leftArrowImage->setNodeMask(0);
    }

    viewer->getCamera()->setProjectionMatrix(osg::Matrixd(m_projection));
}

void OsgViewerFromWindow::update()
{
    if (viewer)
        viewer->frame();
}

void OsgViewerFromWindow::mousePressEvent(QMouseEvent *event)
{
  windowGL->getEventQueue()->mouseButtonPress( event->pos().x(), event->pos().y(), 1 );
}

void OsgViewerFromWindow::mouseReleaseEvent(QMouseEvent *event)
{
    windowGL->getEventQueue()->mouseButtonRelease(event->pos().x(), event->pos().y(), 1);
}

void OsgViewerFromWindow::mouseMoveEvent(QMouseEvent *event)
{

    windowGL->getEventQueue()->mouseMotion( event->pos().x(), event->pos().y() );
}

void OsgViewerFromWindow::setPos(double mx[16])
{    
    if (MODE_OBJECT_DETECTED==m_mode)
        m_root->setMatrix(osg::Matrix(mx));
}

void OsgViewerFromWindow::setProjectionMatrix(double p[16])
{
    //viewer->getCamera()->setProjectionMatrix(Matrixd(p));
    for(int i = 0; i<16; ++i)
    {
        m_projection[i] = p[i];
    }
}

void OsgViewerFromWindow::setVisibleModel(bool visible)
{
    if (MODE_OBJECT_VIEW == m_mode)
        return;
    if (visible) {
        m_root->setNodeMask(1);
    }
    else {
        m_root->setNodeMask(0);
    }
}

void OsgViewerFromWindow::setMode(const OsgViewerFromWindow::MODE_VIEWER& mode)
{
    m_mode = mode;
    for (const auto &f : func)
        f(mode);

    updateMode();
}

const OsgViewerFromWindow::MODE_VIEWER OsgViewerFromWindow::getMode() const
{
return m_mode;
}

void OsgViewerFromWindow::updateMode()
{
    if (MODE_OBJECT_DETECTED ==m_mode)
    {
       /**/
        osg::ref_ptr<osg::LightModel> lm = new osg::LightModel();
        lm->setAmbientIntensity(osg::Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
        viewer->getCamera()->getOrCreateStateSet()->setAttributeAndModes(lm, osg::StateAttribute::ON);
        //viewer->getCamera()->setViewport(new osg::Viewport(0, 0, 831, 601));
        // Make sure glFrontFace is set when the viewer is created.
        osg::ref_ptr<osg::FrontFace> ff = new osg::FrontFace(osg::FrontFace::COUNTER_CLOCKWISE);
        viewer->getCamera()->getOrCreateStateSet()->setAttributeAndModes(ff, osg::StateAttribute::ON);
        //viewer->getCamera()->setViewport(new osg::Viewport(0, 0, w, h));
        viewer->getCamera()->setProjectionMatrix(osg::Matrixd(m_projection));

        viewer->home();
        if (nullptr != viewer->getCameraManipulator())
            viewer->setCameraManipulator(/*new osgGA::CameraManipulator()*/nullptr);


        changeHubText(textOverTransparentRec, "Камера");
        cameraIconImage->setNodeMask(1);

        backButtonRectangle->setNodeMask(0);
        backButtonText->setNodeMask(0);
        leftArrowImage->setNodeMask(0);

        viewer->getCamera()->setViewMatrixAsLookAt(eye, center, up);

        viewer->getCamera()->setClearMask(0L);
/**/

    }
    else
    {
        viewer->setCameraManipulator(new osgGA::TrackballManipulator,true);
        setTransparentBackground(false);
        changeHubText(textOverTransparentRec, "3D");
        cameraIconImage->setNodeMask(0);

        backButtonRectangle->setNodeMask(1);
        backButtonText->setNodeMask(1);
        leftArrowImage->setNodeMask(1);
        //viewer->getCamera()->setClearColor(osg::Vec4(116.0/255.0, 138.0/255.0, 120.0/255.0, 1.0));
        //osg::Vec4(116.0, 138.0, 120.0, 0.35))
    }
}

void OsgViewerFromWindow::setTransparentBackground(bool isTransparent)
{
    if (isTransparent)
        viewer->getCamera()->setClearMask(0L);
    else
    {
        viewer->getCamera()->setClearMask(16640); // 16640 is default value of viewer->getCamera()->getClearMask()
    }
}

void OsgViewerFromWindow::addListenerOfEventIntersector(const ON_CHANGING_FUNC& f)
{
    func.push_back(f);
}