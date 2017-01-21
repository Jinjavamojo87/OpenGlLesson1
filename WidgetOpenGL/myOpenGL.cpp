#include "myOpenGL.h"

// ============================================================================
//	Includes
// ============================================================================

#ifdef _WIN32
#  include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>					// malloc(), free()
#include <string.h>

#ifdef _WIN32
#  define snprintf _snprintf
#endif
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <sys/time.h>

#include <AR/ar.h>
#include <AR/arMulti.h>
#include <AR/video.h>
#include <AR/gsub_lite.h>
#include <AR2/tracking.h>

#include "Artoolkit/ARMarkerNFT.h"
#include "Artoolkit/trackingSub.h"
#include "Artoolkit/VirtualEnvironment.h"
#include "Viewer/OsgViewerFromWindow.h"
#include "MainEventHandler.h"

// ============================================================================
//	Constants
// ============================================================================

#define PAGES_MAX               10          // Maximum number of pages expected. You can change this down (to save memory) or up (to accomodate more pages.)

#define VIEW_SCALEFACTOR		1.0			// Units received from ARToolKit tracking will be multiplied by this factor before being used in OpenGL drawing.
#define VIEW_DISTANCE_MIN		10.0		// Objects closer to the camera than this will not be displayed. OpenGL units.
#define VIEW_DISTANCE_MAX		10000.0		// Objects further away from the camera than this will not be displayed. OpenGL units.

#define FONT_SIZE 10.0f
#define FONT_LINE_SPACING 2.0f

// ============================================================================
//	Global variables
// ============================================================================

// Preferences.
static int prefWindowed = TRUE;           // Use windowed (TRUE) or fullscreen mode (FALSE) on launch.
static int prefWidth = 1024;               // Preferred initial window width.
static int prefHeight = 768;              // Preferred initial window height.
static int prefDepth = 32;                // Fullscreen mode bit depth. Set to 0 to use default depth.
static int prefRefresh = 0;				  // Fullscreen mode refresh rate. Set to 0 to use default rate.

// Image acquisition.
static ARUint8		*gARTImage = NULL;
static long			gCallCountMarkerDetect = 0;

// Markers.
static ARMarkerNFT *markersNFT = NULL;
static int markersNFTCount = 0;

// NFT.
static THREAD_HANDLE_T     *threadHandle = NULL;
static AR2HandleT          *ar2Handle = NULL;
static KpmHandle           *kpmHandle = NULL;
static int                  surfaceSetCount = 0;
static AR2SurfaceSetT      *surfaceSet[PAGES_MAX];

// NFT results.
static int detectedPage = -2; // -2 Tracking not inited, -1 tracking inited OK, >= 0 tracking online on page.
static float trackingTrans[3][4];

// Drawing.
static int gWindowW;
static int gWindowH;
static ARParamLT *gCparamLT = NULL;
static ARGL_CONTEXT_SETTINGS_REF gArglSettings = NULL;
static double gFPS;
static ARdouble cameraLens[16];
static ARdouble cameraPose[16];
static int cameraPoseValid;


GLfloat	cnt1;
GLfloat	cnt2;

static int marginTop = 10;
static int marginLeft = 10;
static char displayedObjectInfo[1024];
static bool isDisplayObjectInfo = false;
//static unsigned long prevDisplayInfoMillis = 0;
struct timeval prevDisplayInfoMillis;
struct timeval currentDisplayInfoMillis;


// ============================================================================
//	Function prototypes
// ============================================================================

static void usage(char *com);
static int setupCamera(const char *cparam_name, char *vconf, ARParamLT **cparamLT_p);
static int initNFT(ARParamLT *cparamLT, AR_PIXEL_FORMAT pixFormat);
static int loadNFTData(void);
static void cleanup(void);
static void Keyboard(unsigned char key, int x, int y);
static void Visibility(int visible);
static void Reshape(int w, int h);
//static void Display(void);
static void drawBackground(const float width, const float height, const float x, const float y);
static void printText(const char *text, const float x, const float y, int calculateXFromRightEdge, int calculateYFromTopEdge);
//static void mainLoop(void);

myOpenGL::myOpenGL()
{
   // setMouseTracking(true);
}
#include <iostream>
#include <QDir>
#include <stdio.h>
void myOpenGL::initializeGL()
{
    //Задаем цвет фона в OpenGL окне
     qglClearColor(Qt::black);
    MainEventHandler::create();
     //Задаем режим обработки полигонов - переднюю и заднюю часть, полигоны полностью закрашенные
     glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

     putenv("ARTOOLKIT5_VCONF=-device=GStreamer ! v4l2src device=/dev/video0 use-fixed-fps=false ! ffmpegcolorspace ! capsfilter caps=video/x-raw-rgb,bpp=24,width=640,height=480 ! identity name=artoolkit sync=true ! fakesink");
     char    glutGamemode[32] = "";
     char   *vconf = NULL;
     char    cparaDefault[] = "Data2/camera_para.dat";

     char   *cpara = NULL;
     int     i;
     int     gotTwoPartOption;
     const char markerConfigDataFilename[] = "Data2/markers.dat";
     const char objectDataFilename[] = "Data2/objects.dat";

 //#ifdef DEBUG
     arLogLevel = AR_LOG_LEVEL_DEBUG;
 //#endif

      //
     // Video setup.
     //
/*
    MainEventHandler::get().addListenerOfEventIntersector([=]
                                                          {
                                                              arVideoClose();
                                                          });
*/
    OsgViewerFromWindow::create();
    OsgViewerFromWindow::get().addListenerOfEventIntersector([=](const OsgViewerFromWindow::MODE_VIEWER& mode)
                                                             {
                                                                 if(OsgViewerFromWindow::MODE_OBJECT_VIEW == mode)
                                                                 {
                                                                     arVideoCapStop();
                                                                 }
                                                                 else
                                                                 {
                                                                    // setupCamera((cpara ? cpara : cparaDefault), vconf, &gCparamLT);
                                                                     arVideoCapStart();
                                                                 }
                                                             });
     if (!setupCamera((cpara ? cpara : cparaDefault), vconf, &gCparamLT)) {
         ARLOGe("main(): Unable to set up AR camera.\n");
         exit(-1);
     }

     //
     // AR init.
     //

     if (!initNFT(gCparamLT, arVideoGetPixelFormat())) {
         ARLOGe("main(): Unable to init NFT.\n");
         exit(-1);
     }

     //
     // Markers setup.
     //

     // Load marker(s).
     newMarkers(markerConfigDataFilename, &markersNFT, &markersNFTCount);
     if (!markersNFTCount) {
         ARLOGe("Error loading markers from config. file '%s'.\n", markerConfigDataFilename);
         cleanup();
         exit(-1);
     }
     ARLOGi("Marker count = %d\n", markersNFTCount);

     // Marker data has been loaded, so now load NFT data.
     if (!loadNFTData()) {
         ARLOGe("Error loading NFT data.\n");
         cleanup();
         exit(-1);
     }

     //
     // Graphics setup.
     //

     // Set up GL context(s) for OpenGL to draw into.
     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

     // Create the OpenGL projection from the calibrated camera parameters.
     arglCameraFrustumRH(&(gCparamLT->param), VIEW_DISTANCE_MIN, VIEW_DISTANCE_MAX, cameraLens);
     AR_PIXEL_FORMAT pf = arVideoGetPixelFormat();
     cameraPoseValid = FALSE;
     gArglSettings = arglSetupForCurrentContext(&(gCparamLT->param), arVideoGetPixelFormat());

     // Setup ARgsub_lite library for current OpenGL context.
     if ((gArglSettings = arglSetupForCurrentContext(&(gCparamLT->param), arVideoGetPixelFormat())) == NULL) {
         ARLOGe("main(): arglSetupForCurrentContext() returned error.\n");
         cleanup();
         exit(-1);
     }

     // Load objects (i.e. OSG models).
     VirtualEnvironmentInit(objectDataFilename);
     VirtualEnvironmentHandleARViewUpdatedCameraLens(cameraLens);

     //
     // Setup complete. Start tracking.
     //

     // Start the video.
     if (arVideoCapStart() != 0) {
         ARLOGe("setupCamera(): Unable to begin camera data capture.\n");
         return;
     }

     //auto viewer = std::unique_ptr<OsgViewerFromWindow>(new OsgViewerFromWindow);
     //viewer->setProjectionMatrix(cameraLens);
    OsgViewerFromWindow::create();
    OsgViewerFromWindow::get().setProjectionMatrix(cameraLens);

    startTimer(16);
}

// glutReshapeFunc
void myOpenGL::resizeGL(int w, int h)
{
    //viewer->resize(w,h);
    OsgViewerFromWindow::get().resize(w,h);

    glViewport(0, 0, w, h);

    GLint viewport[4];

    gWindowW = w;
    gWindowH = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    // Call through to anyone else who needs to know about window sizing here.
    viewport[0] = 0;
    viewport[1] = 0;
    viewport[2] = w;
    viewport[3] = h;
    VirtualEnvironmentHandleARViewUpdatedViewport(viewport);
}

//glutDisplayFunc
void myOpenGL::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

     //Задаем режим матрицы
     glMatrixMode(GL_PROJECTION);
     //Загружаем матрицу
     glLoadIdentity();
     //Здесь рисуем - для удобства в отдельной функции
     scene();
     //выводим на экран
     swapBuffers();
}

void myOpenGL::glDraw()
{
    //Задаем цвет фона в OpenGL окне
     qglClearColor(Qt::black);
}


void myOpenGL::scene()
{
    //mainLoop();
    mainLoop();

    Display();
    //viewer->update();
}

void myOpenGL::timerEvent(QTimerEvent *)
{
    paintGL();
}

void myOpenGL::mousePressEvent(QMouseEvent *event)
{
    //viewer->mousePressEvent(event);
    OsgViewerFromWindow::get().mousePressEvent(event);
}

void myOpenGL::mouseMoveEvent(QMouseEvent *event)
{
    //std::cout << "1231131313" << std::endl;
    //viewer->mouseMoveEvent(event);
    OsgViewerFromWindow::get().mouseMoveEvent(event);
}


//---------------------------------------------------------------------------------------------------
static void usage(char *com)
{
    ARLOG("Usage: %s [options]\n", com);
    ARLOG("Options:\n");
    ARLOG("  --vconf <video parameter for the camera>\n");
    ARLOG("  --cpara <camera parameter file for the camera>\n");
    ARLOG("  -cpara=<camera parameter file for the camera>\n");
    ARLOG("  --width w     Use display/window width of w pixels.\n");
    ARLOG("  --height h    Use display/window height of h pixels.\n");
    ARLOG("  --refresh f   Use display refresh rate of f Hz.\n");
    ARLOG("  --windowed    Display in window, rather than fullscreen.\n");
    ARLOG("  --fullscreen  Display fullscreen, rather than in window.\n");
    ARLOG("  -h -help --help: show this message\n");
    exit(0);
}

static int setupCamera(const char *cparam_name, char *vconf, ARParamLT **cparamLT_p)
{
    ARParam			cparam;
    int				xsize,ysize;
    AR_PIXEL_FORMAT pixFormat;


    // Open the video path.
    if (arVideoOpen(vconf) < 0) {
        ARLOGe("setupCamera(): Unable to open connection to camera.\n");
        return (FALSE);
    }

    // Find the size of the window.
    if (arVideoGetSize(&xsize, &ysize) < 0) {
        ARLOGe("setupCamera(): Unable to determine camera frame size.\n");
        arVideoClose();
        return (FALSE);
    }
    ARLOGi("Camera image size (x,y) = (%d,%d)\n", xsize, ysize);

    // Get the format in which the camera is returning pixels.
    pixFormat = arVideoGetPixelFormat();
    if (pixFormat == AR_PIXEL_FORMAT_INVALID) {
        ARLOGe("setupCamera(): Camera is using unsupported pixel format.\n");
        arVideoClose();
        return (FALSE);
    }

    // Load the camera parameters, resize for the window and init.
    if (arParamLoad(cparam_name, 1, &cparam) < 0) {
        ARLOGe("setupCamera(): Error loading parameter file %s for camera.\n", cparam_name);
        arVideoClose();
        return (FALSE);
    }
    if (cparam.xsize != xsize || cparam.ysize != ysize) {
        ARLOGw("*** Camera Parameter resized from %d, %d. ***\n", cparam.xsize, cparam.ysize);
        arParamChangeSize(&cparam, xsize, ysize, &cparam);
    }
#ifdef DEBUG
    ARLOG("*** Camera Parameter ***\n");
    arParamDisp(&cparam);
#endif
    if ((*cparamLT_p = arParamLTCreate(&cparam, AR_PARAM_LT_DEFAULT_OFFSET)) == NULL) {
        ARLOGe("setupCamera(): Error: arParamLTCreate.\n");
        arVideoClose();
        return (FALSE);
    }

    return (TRUE);
}

// Modifies globals: kpmHandle, ar2Handle.
static int initNFT(ARParamLT *cparamLT, AR_PIXEL_FORMAT pixFormat)
{
    ARLOGi("Initialising NFT.\n");
    //
    // NFT init.
    //

    // KPM init.
    kpmHandle = kpmCreateHandle(cparamLT, pixFormat);
    if (!kpmHandle) {
        ARLOGe("Error: kpmCreateHandle.\n");
        return (FALSE);
    }
    //kpmSetProcMode( kpmHandle, KpmProcHalfSize );

    // AR2 init.
    if( (ar2Handle = ar2CreateHandle(cparamLT, pixFormat, AR2_TRACKING_DEFAULT_THREAD_NUM)) == NULL ) {
        ARLOGe("Error: ar2CreateHandle.\n");
        kpmDeleteHandle(&kpmHandle);
        return (FALSE);
    }
    if (threadGetCPU() <= 1) {
        ARLOGi("Using NFT tracking settings for a single CPU.\n");
        ar2SetTrackingThresh(ar2Handle, 5.0);
        ar2SetSimThresh(ar2Handle, 0.50);
        ar2SetSearchFeatureNum(ar2Handle, 16);
        ar2SetSearchSize(ar2Handle, 6);
        ar2SetTemplateSize1(ar2Handle, 6);
        ar2SetTemplateSize2(ar2Handle, 6);
    } else {
        ARLOGi("Using NFT tracking settings for more than one CPU.\n");
        ar2SetTrackingThresh(ar2Handle, 5.0);
        ar2SetSimThresh(ar2Handle, 0.50);
        ar2SetSearchFeatureNum(ar2Handle, 16);
        ar2SetSearchSize(ar2Handle, 12);
        ar2SetTemplateSize1(ar2Handle, 6);
        ar2SetTemplateSize2(ar2Handle, 6);
    }
    // NFT dataset loading will happen later.
    return (TRUE);
}

// Modifies globals: threadHandle, surfaceSet[], surfaceSetCount
static int unloadNFTData(void)
{
    int i, j;

    if (threadHandle) {
        ARLOGi("Stopping NFT2 tracking thread.\n");
        trackingInitQuit(&threadHandle);
    }
    j = 0;
    for (i = 0; i < surfaceSetCount; i++) {
        if (j == 0) ARLOGi("Unloading NFT tracking surfaces.\n");
        ar2FreeSurfaceSet(&surfaceSet[i]); // Also sets surfaceSet[i] to NULL.
        j++;
    }
    if (j > 0) ARLOGi("Unloaded %d NFT tracking surfaces.\n", j);
    surfaceSetCount = 0;

    return 0;
}

// References globals: markersNFTCount
// Modifies globals: threadHandle, surfaceSet[], surfaceSetCount, markersNFT[]
static int loadNFTData(void)
{
    int i;
    KpmRefDataSet *refDataSet;

    // If data was already loaded, stop KPM tracking thread and unload previously loaded data.
    if (threadHandle) {
        ARLOGi("Reloading NFT data.\n");
        unloadNFTData();
    } else {
        ARLOGi("Loading NFT data.\n");
    }

    refDataSet = NULL;

    for (i = 0; i < markersNFTCount; i++) {
        // Load KPM data.
        KpmRefDataSet  *refDataSet2;
        ARLOGi("Reading %s.fset3\n", markersNFT[i].datasetPathname);
        if (kpmLoadRefDataSet(markersNFT[i].datasetPathname, "fset3", &refDataSet2) < 0 ) {
            ARLOGe("Error reading KPM data from %s.fset3\n", markersNFT[i].datasetPathname);
            markersNFT[i].pageNo = -1;
            continue;
        }
        markersNFT[i].pageNo = surfaceSetCount;
        ARLOGi("  Assigned page no. %d.\n", surfaceSetCount);
        if (kpmChangePageNoOfRefDataSet(refDataSet2, KpmChangePageNoAllPages, surfaceSetCount) < 0) {
            ARLOGe("Error: kpmChangePageNoOfRefDataSet\n");
            exit(-1);
        }
        if (kpmMergeRefDataSet(&refDataSet, &refDataSet2) < 0) {
            ARLOGe("Error: kpmMergeRefDataSet\n");
            exit(-1);
        }
        ARLOGi("  Done.\n");

        // Load AR2 data.
        ARLOGi("Reading %s.fset\n", markersNFT[i].datasetPathname);

        if ((surfaceSet[surfaceSetCount] = ar2ReadSurfaceSet(markersNFT[i].datasetPathname, "fset", NULL)) == NULL ) {
            ARLOGe("Error reading data from %s.fset\n", markersNFT[i].datasetPathname);
        }
        ARLOGi("  Done.\n");

        surfaceSetCount++;
        if (surfaceSetCount == PAGES_MAX) break;
    }
    if (kpmSetRefDataSet(kpmHandle, refDataSet) < 0) {
        ARLOGe("Error: kpmSetRefDataSet\n");
        exit(-1);
    }
    kpmDeleteRefDataSet(&refDataSet);

    // Start the KPM tracking thread.
    threadHandle = trackingInitInit(kpmHandle);
    if (!threadHandle) exit(-1);

    ARLOGi("Loading of NFT data complete.\n");
    return (TRUE);
}

static void cleanup(void)
{
    VirtualEnvironmentFinal();

    if (markersNFT) deleteMarkers(&markersNFT, &markersNFTCount);

    // NFT cleanup.
    unloadNFTData();
    ARLOGi("Cleaning up ARToolKit NFT handles.\n");
    ar2DeleteHandle(&ar2Handle);
    kpmDeleteHandle(&kpmHandle);
    arParamLTFree(&gCparamLT);

    // OpenGL cleanup.
    arglCleanup(gArglSettings);
    gArglSettings = NULL;

    // Camera cleanup.
    arVideoCapStop();
    arVideoClose();
}

static void Keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 0x1B:						// Quit.
        case 'Q':
        case 'q':
            cleanup();
            exit(0);
            break;
        case '?':
        case '/':
            ARLOG("Keys:\n");
            ARLOG(" q or [esc]    Quit demo.\n");
            ARLOG(" ? or /        Show this help.\n");
            ARLOG("\nAdditionally, the ARVideo library supplied the following help text:\n");
            arVideoDispOption();
            break;
        default:
            break;
    }
}

//#include <osg/Image>
//#include <osgDB/WriteFile>
//#include <osgDB/ReadFile>

void myOpenGL::mainLoop(void)
{
    static int ms_prev;
    int ms;
    float s_elapsed;
    ARUint8 *image;


    int             i, j, k;

    // Calculate time delta.
    ms = glutGet(GLUT_ELAPSED_TIME);
    s_elapsed = (float)(ms - ms_prev) * 0.001f;
    ms_prev = ms;

    // Grab a video frame.
    if ((image = arVideoGetImage()) != NULL) {
        gARTImage = image;	// Save the fetched image.
        // Calculate FPS every 30 frames.
        if (gCallCountMarkerDetect % 30 == 0) {
            gFPS = 30.0/arUtilTimer();
            arUtilTimerReset();
            gCallCountMarkerDetect = 0;
        }
        gCallCountMarkerDetect++; // Increment ARToolKit FPS counter.


        // Run marker detection on frame
        if (threadHandle) {
            // Perform NFT tracking.
            float            err;
            int              ret;
            int              pageNo;

            if( detectedPage == -2 ) {
                trackingInitStart( threadHandle, gARTImage );
                detectedPage = -1;
            }
            if( detectedPage == -1 ) {
                ret = trackingInitGetResult( threadHandle, trackingTrans, &pageNo);
                if( ret == 1 ) {
                    if (pageNo >= 0 && pageNo < surfaceSetCount) {
                        gettimeofday(&prevDisplayInfoMillis, NULL);

                        //prevDisplayInfoMillis = time(0);
                        //prevDisplayInfoMillis.tv_sec;
                        //changeDisplayInfoVisible(true);
                        ARLOGi("Detected page %d.\n", pageNo);
                        detectedPage = pageNo;
                        ar2SetInitTrans(surfaceSet[detectedPage], trackingTrans);
                    } else {
                        ARLOGe("Detected bad page %d.\n", pageNo);
                        detectedPage = -2;

                        //if (time(0) -prevDisplayInfoMillis > 1) {
                        //    changeDisplayInfoVisible(false);
                        //}

                    }
                } else if( ret < 0 ) {
                    ARLOGi("No page detected.\n");
                    detectedPage = -2;
                    gettimeofday(&currentDisplayInfoMillis, NULL);
                    if (currentDisplayInfoMillis.tv_sec - prevDisplayInfoMillis.tv_sec > 1) {
                        //changeDisplayInfoVisible(false);
                    }
                    //if (time(0) -prevDisplayInfoMillis > 1) {
                    //
                    //}
                }
            }
            if( detectedPage >= 0 && detectedPage < surfaceSetCount) {
                if( ar2Tracking(ar2Handle, surfaceSet[detectedPage], gARTImage, trackingTrans, &err) < 0 ) {
                    ARLOGi("Tracking lost.\n");
                    detectedPage = -2;
                } else {
                    ARLOGi("Tracked page %d (max %d).\n", detectedPage, surfaceSetCount - 1);
                }
            }
        } else {
            ARLOGe("Error: threadHandle\n");
            detectedPage = -2;
        }

        // Update markers.
        for (i = 0; i < markersNFTCount; i++) {
            markersNFT[i].validPrev = markersNFT[i].valid;
            if (markersNFT[i].pageNo >= 0 && markersNFT[i].pageNo == detectedPage) {
                markersNFT[i].valid = TRUE;
                for (j = 0; j < 3; j++) for (k = 0; k < 4; k++) markersNFT[i].trans[j][k] = trackingTrans[j][k];
            }
            else markersNFT[i].valid = FALSE;
            if (markersNFT[i].valid) {

                // Filter the pose estimate.
                if (markersNFT[i].ftmi) {
                    if (arFilterTransMat(markersNFT[i].ftmi, markersNFT[i].trans, !markersNFT[i].validPrev) < 0) {
                        ARLOGe("arFilterTransMat error with marker %d.\n", i);
                    }
                }

                if (!markersNFT[i].validPrev) {
                    // Marker has become visible, tell any dependent objects.
                    VirtualEnvironmentHandleARMarkerAppeared(i);
                }

                // We have a new pose, so set that.
                arglCameraViewRH((const ARdouble (*)[4])markersNFT[i].trans, markersNFT[i].pose.T, VIEW_SCALEFACTOR);
                // Tell any dependent objects about the update.
                //VirtualEnvironmentHandleARMarkerWasUpdated(i, markersNFT[i].pose);
                //viewer->setProjectionMatrix(markersNFT[i].pose.T);

                //viewer->setVisibleModel(true);
                //viewer->setPos((markersNFT[i].pose.T));

                OsgViewerFromWindow::get().setVisibleModel(true);
                OsgViewerFromWindow::get().setPos((markersNFT[i].pose.T));

            } else {

                if (markersNFT[i].validPrev) {
                    // Marker has ceased to be visible, tell any dependent objects.
                    VirtualEnvironmentHandleARMarkerDisappeared(i);
                    //viewer->setVisibleModel(false);
                    OsgViewerFromWindow::get().setVisibleModel(false);
                    // false
                }
            }
        }

        // Tell GLUT the display has changed.
     //   glutPostRedisplay();
    } else {
        arUtilSleep(2);
    }

}

//
//	This function is called on events when the visibility of the
//	GLUT window changes (including when it first becomes visible).
//
static void Visibility(int visible)
{
    if (visible == GLUT_VISIBLE) {
        //glutIdleFunc(mainLoop);
    } else {
        glutIdleFunc(NULL);
    }
}

//
//	This function is called when the
//	GLUT window is resized.
//
static void Reshape(int w, int h)
{
    GLint viewport[4];

    gWindowW = prefWidth;
    gWindowH = prefHeight;

    // Call through to anyone else who needs to know about window sizing here.
    viewport[0] = 0;
    viewport[1] = 0;
    viewport[2] = prefWidth;
    viewport[3] = prefHeight;
    glutReshapeWindow( prefWidth, prefHeight);
    VirtualEnvironmentHandleARViewUpdatedViewport(viewport);
}

static void print(const char *text, const float x, const float y)
{
    int i;
    size_t len;

    if (!text) return;
    len = strlen(text);
    glRasterPos2f(x, y);
    for (i = 0; i < len; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
}

//
// This function is called when the window needs redrawing.
//
//static void Display(void)
void myOpenGL::Display(void)
{
    char text[256];

    // Select correct buffer for this context.
    glDrawBuffer(GL_BACK);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the buffers for new frame.

    arglPixelBufferDataUpload(gArglSettings, gARTImage);
    arglDispImage(gArglSettings);
    gARTImage = NULL; // Invalidate image data.
    // Set up 3D mode.
    glMatrixMode(GL_PROJECTION);
#ifdef ARDOUBLE_IS_FLOAT
    glLoadMatrixf(cameraLens);
#else
    glLoadMatrixd(cameraLens);
#endif
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_DEPTH_TEST);

    // Set any initial per-frame GL state you require here.
    // --->

    // Lighting and geometry that moves with the camera should be added here.
    // (I.e. should be specified before camera pose transform.)
    // --->
    //return;

    //viewer->update();
    OsgViewerFromWindow::get().update();


//glViewport(0, 0, width(), height());
   VirtualEnvironmentHandleARViewDrawPreCamera();
   return;
    if (cameraPoseValid) {

#ifdef ARDOUBLE_IS_FLOAT
        glMultMatrixf(cameraPose);
#else
        glMultMatrixd(cameraPose);
#endif

        // All lighting and geometry to be drawn in world coordinates goes here.
        // --->
        VirtualEnvironmentHandleARViewDrawPostCamera();
    }

    // Set up 2D mode.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, (GLdouble)gWindowW, 0, (GLdouble)gWindowH, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Add your own 2D overlays here.
    // --->

    VirtualEnvironmentHandleARViewDrawOverlay();

    // Show some real-time info.
    glColor3ub(255, 255, 255);
    // FPS.
    snprintf(text, sizeof(text), "FPS %.1f", gFPS);
    print(text, 2.0f, (float)gWindowH - 12.0f);


    if (isDisplayObjectInfo) {
        int i;
        GLfloat  w, bw, bh;
        const char *helpText[] = {


            "become visible         ",
            "It's appear when marker         ",
            "This is test text        "
        };
        #define helpTextLineCount (sizeof(helpText)/sizeof(char *))

        bw = 5.0f;
        for (i = 0; i < helpTextLineCount; i++) {
            w = (float)glutBitmapLength(GLUT_BITMAP_HELVETICA_10, (unsigned char *)helpText[i]);
            if (w > bw) bw = w + 10;
        }
        bh = helpTextLineCount * 15.0f /* character height */+ (helpTextLineCount - 1) * 2.0f /* line spacing */;
        drawBackground(bw, prefHeight, prefWidth-bw-marginLeft, marginTop);
        for (i = 0; i < helpTextLineCount; i++)
            printText(helpText[i], prefWidth-bw-marginLeft + 5, prefHeight - (helpTextLineCount - 1 - i)*18.0f - (marginTop+12), 0, 0);;

    }
    //glutSwapBuffers();
}

static void drawBackground(const float width, const float height, const float x, const float y)
{
    GLfloat vertices[4][2];

    vertices[0][0] = x; vertices[0][1] = y;
    vertices[1][0] = width + x; vertices[1][1] = y;
    vertices[2][0] = width + x; vertices[2][1] = height + y;
    vertices[3][0] = x; vertices[3][1] = height + y;
    glLoadIdentity();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);	// 50% transparent black.
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f); // Opaque white.
    //glLineWidth(1.0f);
    //glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisable(GL_BLEND);
}

static int drawGLScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
    glLoadIdentity();					// Reset The Current Modelview Matrix
    glTranslatef(0.0f,0.0f,-1.0f);				// Move One Unit Into The Screen

    // Blue Text
    glColor3ub(0,0,0xff);

    // Position The WGL Text On The Screen
    glRasterPos2f(-0.40f, 0.35f);

    // Here We Print Some Text Using Our FreeType Font
    // The only really important command is the actual print() call,
    // but for the sake of making the results a bit more interesting
    // I have put in some code to rotate and scale the text.

    // Red text
    glColor3ub(0xff,0,0);

    glPushMatrix();
    glLoadIdentity();
    glRotatef(cnt1,0,0,1);
    glScalef(1,.8+.3*cos(cnt1/5),1);
    glTranslatef(-180,0,0);
    glPopMatrix();

    //Uncomment this to test out print's ability to handle newlines.
    //freetype::print(our_font, 320, 200, "Here\nthere\nbe\n\nnewlines\n.", cnt1);

    cnt1+=0.051f;	// Increase The First Counter
    cnt2+=0.005f;	// Increase The First Counter

    //SDL_GL_SwapBuffers();
    return( TRUE );
}

