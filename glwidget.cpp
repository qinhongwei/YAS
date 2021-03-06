#include "glwidget.h"

const double TRACKBALL_RADIUS = 0.6;

using namespace cv;
using namespace std;

QGLViewerWidget::QGLViewerWidget(QWidget* _parent)
  : QGLWidget(_parent)
{

  setAttribute(Qt::WA_NoSystemBackground,true);
  setFocusPolicy(Qt::StrongFocus);
  setAcceptDrops(true);
  setCursor(Qt::PointingHandCursor);

  m_center = cv::Vec3f(0.0,0.0,1.0);
  m_radius = 1.0f;

  this->setWindowTitle("3D View");
}

QGLViewerWidget::~QGLViewerWidget() {}

void QGLViewerWidget::initializeGL() {

    glClearColor(0.5, 0.5, 0.5, 0.0);
    //glDisable(GL_DITHER);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_COLOR_MATERIAL);

    // set lights
    GLfloat pos1[] = { 0.1,  0.1, -0.02, 0.0};
    GLfloat pos2[] = {-0.1,  0.1, -0.02, 0.0};
    GLfloat pos3[] = { 0.0,  0.0,  0.1,  0.0};
    GLfloat col1[] = { 0.7,  0.7,  0.8,  1.0};
    GLfloat col2[] = { 0.8,  0.7,  0.7,  1.0};
    GLfloat col3[] = { 1.0,  1.0,  1.0,  1.0};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_POSITION, pos1);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,  col1);
    glLightfv(GL_LIGHT0,GL_SPECULAR, col1);

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1,GL_POSITION, pos2);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,  col2);
    glLightfv(GL_LIGHT1,GL_SPECULAR, col2);

    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2,GL_POSITION, pos3);
    glLightfv(GL_LIGHT2,GL_DIFFUSE,  col3);
    glLightfv(GL_LIGHT2,GL_SPECULAR, col3);

  //  glEnable(GL_LIGHTING);

  // scene pos and size
  //  glMatrixMode(GL_MODELVIEW);
  //  glLoadIdentity();
  //  glGetDoublev(GL_MODELVIEW_MATRIX, m_modelview_matrix);
  //  set_scene_pos(Vec3f(0.0, 0.0, 0.5),1.0);

  //  TomGine::vec4 vp;
  //  glGetFloatv(GL_VIEWPORT, vp);

  //  TomGine::mat4 intrinsic = TomGine::tgCamera::Perspective(1.0f/tan(0.5f * 60.0f * M_PI / 180.0f), vp[2]/vp[3], 10.0f, 0.1f);
  //  TomGine::mat4 extrinsic;

  //  m_cam_perspective.Set(intrinsic, extrinsic, vp[2], vp[3]);
  //  m_cam_perspective.Translate(TomGine::vec3(0,0,1));
  //  m_cam_perspective.ApplyTransform();
  //  m_cam_perspective.Activate();
}

void QGLViewerWidget::resizeGL(int w, int h)
{

  m_cam_perspective.SetViewport(w, h);
  m_cam_perspective.Activate();

  updateGL();

}

void QGLViewerWidget::drawCoordinates(float length)
{
  glDisable(GL_LIGHTING);
  glLineWidth(2.0);
  glBegin(GL_LINES);
  glColor3ub(255,0,0); glVertex3f(0.0f,0.0f,0.0f); glVertex3f(length,0.0f,0.0f);
  glColor3ub(0,255,0); glVertex3f(0.0f,0.0f,0.0f); glVertex3f(0.0f,length,0.0f);
  glColor3ub(0,0,255); glVertex3f(0.0f,0.0f,0.0f); glVertex3f(0.0f,0.0f,length);
  glEnd();
}

void QGLViewerWidget::drawPoints(float size)
{
  glDisable(GL_LIGHTING);
  glPointSize(size);
  glBegin(GL_POINTS);
  for(size_t i=0; i<m_points.size(); i++)
  {
    glColor3ubv(&m_colors[i][0]);
    glVertex3fv(&m_points[i].x);
  }
  glEnd();
}

void QGLViewerWidget::drawMesh()
{

  glEnable(GL_LIGHTING);

  m_mesh.DrawFaces();

}


void QGLViewerWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_cam_perspective.Activate();

  drawPoints();

  drawMesh();

  drawCoordinates();

}

void QGLViewerWidget::set_pcl(const std::vector<cv::Point3f>& points, const std::vector<cv::Vec3b>& colors) {

  m_points = points;
  m_colors = colors;

  updateGL();

}

void QGLViewerWidget::set_mesh(const PoissonRec::CoredVectorMeshData<PoissonRec::PlyVertex<float> > &mesh)
{

  m_mesh.Clear();
  mesh.resetIterator();

  for(size_t i=0; i<mesh.inCorePoints.size(); i++)
  {
    PoissonRec::PlyVertex<float> pv = mesh.inCorePoints[i];

    TomGine::tgVertex v;
    v.pos = TomGine::vec3(pv.point[0], pv.point[1], pv.point[2]);

    m_mesh.m_vertices.push_back(v);
  }

  for(size_t i=0; i<mesh.outOfCorePointCount(); i++)
  {
    PoissonRec::PlyVertex<float> pv;
    mesh.nextOutOfCorePoint(pv);

    TomGine::tgVertex v;
    v.pos = TomGine::vec3(pv.point[0], pv.point[1], pv.point[2]);

    m_mesh.m_vertices.push_back(v);
  }


  for(size_t i=0; i<mesh.polygonCount(); i++)
  {
    std::vector< PoissonRec::CoredVertexIndex > vertices;
    mesh.nextPolygon(vertices);

    TomGine::tgFace f;
    for(size_t j=0; j<vertices.size(); j++)
    {
      if( vertices[j].inCore )
        f.v.push_back(unsigned(vertices[j].idx));
      else
        f.v.push_back(unsigned(vertices[j].idx) + unsigned(mesh.inCorePoints.size()));
    }

    m_mesh.m_faces.push_back(f);
  }

  m_mesh.ComputeNormals();

  std::cout << "QGLViewerWidget::set_mesh (Vertices: " << m_mesh.m_vertices.size() <<
               ", Faces: " << m_mesh.m_faces.size() << ")" << std::endl;

  updateGL();

}

void QGLViewerWidget::configure_cam(const CCam& rgb, const CDepthCam& depth)
{

  //cv::Mat extrinsic = depth.GetExtrinsics();

  size_t size[2];
  float f[2];
  float c[2];
  float alpha;
  float k[5];
  float range[2];
  depth.GetIntrinsics(size, f, c, alpha, k);
  depth.GetRange(range);

  //  printf("%d %d\n", size[0], size[1]);
  //  printf("%f %f\n", f[0], f[1]);
  //  printf("%f %f\n", c[0], c[1]);
  //  printf("%f\n", alpha);
  //  printf("%f %f %f %f %f\n", k[0], k[1], k[2], k[3], k[4]);
  //  printf("%f %f\n", range[0], range[1]);

  TomGine::tgCamera::Parameter params;
  params.width = size[0];
  params.height = size[1];
  params.fx = f[0];
  params.fy = f[1];
  params.cx = c[0];
  params.cy = c[1];
  params.zNear = 0.5 * range[0];
  params.zFar = 2.0 * range[1];

  m_cam_origin.Set(params);

  m_cam_perspective = m_cam_origin;

  //  printf("[configure_cam] camera:\n");
  //  m_cam_perspective.Print();

}


void QGLViewerWidget::mousePressEvent(QMouseEvent* event)
{

  m_last_point_2d = event->pos();


}

void QGLViewerWidget::mouseMoveEvent(QMouseEvent* event) {

  QPoint newPoint2D = event->pos();

  float dx = newPoint2D.x() - m_last_point_2d.x();
  float dy = newPoint2D.y() - m_last_point_2d.y();

  float far = m_cam_perspective.GetZFar();
  float near = m_cam_perspective.GetZNear();

  // enable GL context
  makeCurrent();

  // move in z direction
  if ( (event->buttons() == (Qt::LeftButton+Qt::MidButton)) ||
       (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::ControlModifier))
  {

    m_cam_perspective.TranslateF(0.001f * (far - near) * dx);
    m_cam_perspective.TranslateF(0.001f * (far - near) * dy);


  }  // move in x,y direction
  else if ( (event->buttons() == Qt::MidButton) ||
            (event->buttons() == Qt::LeftButton && event->modifiers() == Qt::AltModifier) )
  {

    m_cam_perspective.TranslateS(-0.0005f * (far - near) * dx);
    m_cam_perspective.TranslateU(0.0005f * (far - near) * dy);

  } // rotate
  else if (event->buttons() == Qt::LeftButton)
  {

    TomGine::vec3 cor(m_center[0], m_center[1], m_center[2]);
    m_cam_perspective.Orbit(cor, m_cam_perspective.GetU(), -0.05f * dx);
    m_cam_perspective.Orbit(cor, m_cam_perspective.GetS(), -0.05f * dy);

  }

  m_cam_perspective.ApplyTransform();

  // remember this point
  m_last_point_2d = newPoint2D;


  // trigger redraw
  updateGL();
}

void QGLViewerWidget::wheelEvent(QWheelEvent* _event)
{

  float d = -(float)_event->delta() / 120.0 * 0.2 * m_radius;

  float far = m_cam_perspective.GetZFar();
  float near = m_cam_perspective.GetZNear();
  m_cam_perspective.TranslateF(0.01f * (far - near) * d);
  m_cam_perspective.TranslateF(0.01f * (far - near) * d);
  m_cam_perspective.ApplyTransform();

  //printf("wheel %f %f\n", d, m_radius);

  //  translate(Vec3f(0.0, 0.0, d));
  updateGL();
  _event->accept();

}

void QGLViewerWidget::keyPressEvent(QKeyEvent *event) {

  if(event->key() == Qt::Key_Z) {

    m_cam_perspective = m_cam_origin;
    m_cam_perspective.Activate();

  }

  updateGL();

}

