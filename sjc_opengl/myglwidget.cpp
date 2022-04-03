#include "myglwidget.h"
#include "plane3d.h"
#include <QtTest/QtTest>

#define M_PI 3.1415

MyGLWidget::MyGLWidget( QWidget* parent ) 
: QGLWidget( parent ), rhoEye_( 5 ), phiEye_( 45 ), thetaEye_( 45 ), mode_( EYEROTATE )
{
  createObject( GLCylinderFactory( Vector3D( 0,0,0, Vector3D::POINT3DCART ), Vector3D( 0,0,10, Vector3D::POINT3DCART ), 1 ) );
  createObject( GLCylinderFactory( Vector3D( 1,0,0, Vector3D::POINT3DCART ), Vector3D( 1,0,10, Vector3D::POINT3DCART ), 1 ) );
  createObject( GLCylinderFactory( Vector3D( 5,5,5, Vector3D::POINT3DCART ), Vector3D( 8,8,8, Vector3D::POINT3DCART ), 1 ) );
}

/*void MyGLWidget::drawRectangle( const QRectF& rect )
{
  makeCurrent();
  GLuint list = glGenLists(1);
  glNewList(list, GL_COMPILE);

  qglColor( QColor( Qt::yellow ) );
  glRectf( rect.left(), rect.bottom(), rect.right(), rect.top() );

  glEndList();
  objects_ << list;
}*/

void MyGLWidget::initializeGL()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);

  rhoEye_ = 5;
  phiEye_ = 45;
  thetaEye_ = 45;

  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glMatrixMode(GL_MODELVIEW);
}

void MyGLWidget::resizeGL( int width, int height )
{
  glViewport( 0, 0, width, height );
  paintGL();
}

void MyGLWidget::paintGL()
{
  //qDebug() << "paintgl";
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  Vector3D newEye( rhoEye_, phiEye_ *M_PI/180.0, thetaEye_ *M_PI/180.0, Vector3D::POINT3DPOLE );
  newEye = newEye + lookat_;
  gluPerspective(45,(double)width()/(double)height(),0.1,100);
  gluLookAt( newEye.x(), newEye.y(), newEye.z(), 
             lookat_.x(), lookat_.y(), lookat_.z(), 
             0.0, 0.0, 1.0 );
  glMatrixMode(GL_MODELVIEW);
  

  for( QMap< GLuint, GLObject* >::const_iterator iter = objects_.constBegin(); iter != objects_.constEnd(); ++iter )
    glCallList( iter.key() );
}

void normalizeAngle(float *angle)
{
  while (*angle < 0)
    *angle += 360;
  while (*angle > 360)
    *angle -= 360;
}

void MyGLWidget::setPhi( float angle )
{
  normalizeAngle( &angle );
  if ( !qFuzzyCompare( angle, phiEye_ ) && angle > 5.0 && angle < 175.0 )
  {
    phiEye_ = angle;
    updateGL();
  }
}

void MyGLWidget::setTheta( float angle )
{
  normalizeAngle( &angle );
  if ( !qFuzzyCompare( angle, thetaEye_ ) )
  {
    thetaEye_ = angle;
    updateGL();
  }
}

void MyGLWidget::setRho( float rho )
{
  if( !qFuzzyCompare( rhoEye_, rho ) && rho > 1.0 )
  {
    rhoEye_ = rho;
    updateGL();
  }
}

Vector3D MyGLWidget::absEye() const 
{ 
  return Vector3D( rhoEye_, phiEye_ *M_PI/180.0, thetaEye_ *M_PI/180.0, Vector3D::POINT3DPOLE ) + lookat_;
}

Vector3D MyGLWidget::absLookat() const
{
  return lookat_;
}

Vector3D MyGLWidget::pointUnderPixel( const QPoint & p )
{
  makeCurrent();
  glMatrixMode( GL_MODELVIEW );
  float depth;
  glReadPixels( p.x(), p.y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth );

  GLdouble modelMatrix[16];
  glGetDoublev(GL_MODELVIEW_MATRIX,modelMatrix);
  GLdouble projMatrix[16];
  glGetDoublev(GL_PROJECTION_MATRIX,projMatrix);
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT,viewport);
  GLdouble position[3];
  gluUnProject(
     p.x(),
     p.y(),
     depth,
     modelMatrix,
     projMatrix,
     viewport,
     //the next 3 parameters are the pointers to the final object
     //coordinates. Notice that these MUST be double's
     &position[0], //-&gt; pointer to your own position (optional)
     &position[1], // id
     &position[2] // id
  );
  return Vector3D( position[0], position[1], position[2], Vector3D::POINT3DCART );
}

GLObject* MyGLWidget::objectUnderPixel( const QPoint& p )
{
  makeCurrent();

  // Prepare the selection mode
  glSelectBuffer( 4000, buffer_ );
  glRenderMode( GL_SELECT );
  glInitNames();

  // Loads the matrices
  glMatrixMode( GL_PROJECTION );
  glPushMatrix();
  glLoadIdentity();
  GLint viewport[4];
  glGetIntegerv( GL_VIEWPORT, viewport );
  Vector3D newEye( rhoEye_, phiEye_ *M_PI/180.0, thetaEye_ *M_PI/180.0, Vector3D::POINT3DPOLE );
  newEye = newEye + lookat_;
  gluPickMatrix( p.x(), p.y(), 3, 3, viewport );
  gluPerspective(45,(double)width()/(double)height(),0.1,100);
  gluLookAt( newEye.x(), newEye.y(), newEye.z(), 
             lookat_.x(), lookat_.y(), lookat_.z(), 
             0.0, 0.0, 1.0 );

  glMatrixMode(GL_MODELVIEW);

  for( QMap< GLuint, GLObject* >::const_iterator iter = objects_.constBegin(); iter != objects_.constEnd(); ++iter )
  {
    glPushName( (*iter)->begin() );
    glCallList( (*iter)->begin() );
    glPopName();
  }
  

  //glMatrixMode( GL_PROJECTION );
  glPopMatrix();
  //glMatrixMode( GL_MODELVIEW );

  // Flush GL buffers
  glFlush();
  

  // Get the number of hits that were seen through the pick matrix frustum. Reset GL_RENDER
  GLint nbHits = glRenderMode( GL_RENDER );

  /*// [0x5]
  GLuint hitnames[10];
  unsigned int hi = 0;
  GLuint* bufp = buffer_;
  GLuint name = -1, numnames, z1, z2;

  // [0x6]
  for(unsigned int j = 0; j < nbHits; j++){

    numnames = *bufp++;
    z1 = *bufp++;
    z2 = *bufp++;
    while(numnames--){
      name = *bufp++;
      hitnames[hi++] = name;
    }
  }
  return name;*/

  if( nbHits <= 0 )
    return 0;
  else
  {
    // Interpret results: each object created 4 values in the selectBuffer().
    // selectBuffer[4*i+1] is the object minimum depth value, while selectBuffer[4*i+3] is the id pushed on the stack.
    // Of all the objects that were projected in the pick region, we select the closest one (zMin comparison).
    // This code needs to be modified if you use several stack levels. See glSelectBuffer() man page.
    assert( buffer_[ 0 ] == 1 );
    GLuint zMin = buffer_[ 1 ];
    int selected = buffer_[ 3 ];
    for( int i = 1; i < nbHits; ++i )
    {
      assert( buffer_[ 4*i ] == 1 );
      if( buffer_[ 4*i+1 ] < zMin )
      {
        zMin = buffer_[ 4*i+1 ];
        selected = buffer_[ 4*i+3 ];
      }
    }
    assert( objects_.contains( selected ) );
    return objects_[ selected ];
  }
}

Vector3D MyGLWidget::planeIntersectionUnderPixel( const QPoint& p, float z )
{
  Vector3D neweye( rhoEye_, phiEye_ *M_PI/180.0, thetaEye_ *M_PI/180.0, Vector3D::POINT3DPOLE );
  neweye = neweye + lookat_;
  Vector3D pProjected = pointUnderPixel( p );

  float xa = neweye.x(),        xb = pProjected.x();
  float ya = neweye.y(),        yb = pProjected.y();
  float za = neweye.z(),        zb = pProjected.z();

  float x = (xb-xa)*(z - za)/(zb-za) + xa;
  float y = (yb-ya)*(z - za)/(zb-za) + ya;

  return Vector3D( x, y, z, Vector3D::POINT3DCART );
}

Vector3D MyGLWidget::verticalPlaneIntersectionUnderPixel( const QPoint& p, float x, float y )
{
  Vector3D neweye( rhoEye_, phiEye_ *M_PI/180.0, thetaEye_ *M_PI/180.0, Vector3D::POINT3DPOLE );
  neweye = neweye + lookat_;
  Vector3D pProjected = pointUnderPixel( p );

  Vector3D r = (pProjected-neweye).crossProduct( Vector3D(0,0,1, Vector3D::VECTOR3D ) );

  Line3D lookline( neweye, pProjected );
  Plane3D verticalPlane( 
    Vector3D(x,y,0,Vector3D::POINT3DCART), 
    Vector3D(x,y,0,Vector3D::POINT3DCART)+r, 
    Vector3D(x,y,1,Vector3D::POINT3DCART) );

  return Vector3D( x, y, verticalPlane.intersect( lookline ).z(), Vector3D::POINT3DCART );
}

void MyGLWidget::mousePressEvent( QMouseEvent *event )
{
  lastPos_ = event->pos();
  //qDebug() 
  //  << pointUnderPixel ( QPoint( lastPos_.x(), height() - lastPos_.y() ) ).toString() 
  //  << QPoint( lastPos_.x(), height() - lastPos_.y() ) 
  //  << objectUnderPixel( QPoint( lastPos_.x(), height() - lastPos_.y() ) );

  
  if( event->modifiers() & Qt::ShiftModifier )
  {
    mode_ = SELECT;
    emit objectSelected( objectUnderPixel( QPoint( lastPos_.x(), height() - lastPos_.y() ) ) );
  }
  else if( event->button() == Qt::LeftButton && (event->modifiers() & Qt::ControlModifier) )
  {
    emit objectAboutToMove( 
      objectUnderPixel( QPoint( lastPos_.x(), height() - lastPos_.y() ) ),
      pointUnderPixel(  QPoint( lastPos_.x(), height() - lastPos_.y() ) )
      );
    if( event->modifiers() & Qt::ShiftModifier )
      mode_ = RAISEPOLE;
    else
      mode_ = MOVEPOLE;
  }
  else if( event->button() == Qt::RightButton && (event->modifiers() & Qt::ControlModifier) )
  {
    emit objectAboutToRotate( 
      objectUnderPixel( QPoint( lastPos_.x(), height() - lastPos_.y() ) ),
      pointUnderPixel(  QPoint( lastPos_.x(), height() - lastPos_.y() ) )
      );
    mode_ = ROTATEPOLE;
  }
  else if( event->button() == Qt::LeftButton )
    mode_ = EYEROTATE;
  else if( event->button() == Qt::RightButton )
    mode_ = EYEMOVE;

  //qDebug() << planeIntersectionUnderPixel( QPoint( lastPos_.x(), height() - lastPos_.y() ), 0 ).toString();
}

void MyGLWidget::mouseReleaseEvent( QMouseEvent *event )
{
  if( mode_ == MOVEPOLE || mode_ == RAISEPOLE )
    emit objectMoved();
  else if( mode_ == ROTATEPOLE )
    emit objectRotated();

  mode_ = NONE;
  updateGL();
}

void MyGLWidget::mouseMoveEvent( QMouseEvent *event )
{
  if( mode_ == EYEROTATE )
  {
    int dx = event->x() - lastPos_.x();
    int dy = event->y() - lastPos_.y();

    setPhi  ( phiEye_   - dy );
    setTheta( thetaEye_ - dx );

    lastPos_ = event->pos();
  }
  else if( mode_ == MOVEPOLE || mode_ == RAISEPOLE )
  {
    if( event->modifiers() & Qt::ShiftModifier )
      mode_ = RAISEPOLE;
    else
      mode_ = MOVEPOLE;

    emit mouseMoved( QPoint( event->pos().x(), height() - event->pos().y() ), mode_ );
    updateGL();
  }
  else if( mode_ == ROTATEPOLE )
  {
    emit mouseMoved( QPoint( event->pos().x(), height() - event->pos().y() ), mode_ );
    updateGL();
  }
  else if( mode_ == EYEMOVE )
  {
    int dx = event->x() - lastPos_.x();
    int dy = event->y() - lastPos_.y();

    Vector3D lookatold = planeIntersectionUnderPixel( QPoint( lastPos_.x(), height() - lastPos_.y() ), lookat_.z() );
    Vector3D lookatnew = planeIntersectionUnderPixel( QPoint( event->pos().x(), height() - event->pos().y() ), lookat_.z() );
    lastPos_ = event->pos();
    lookat_ = lookat_ - ( lookatnew - lookatold );
    updateGL();
  }
}

void MyGLWidget::wheelEvent( QWheelEvent* event )
{
  setRho( rhoEye_ + event->delta() / 8 / 15 );
}


const GLObject* MyGLWidget::createObject( const GLFactory& fac )
{
  //qDebug() << "creating" << listBegin;
  makeCurrent();

  // Create a list of primitives, the start of the list will be the name of the object
  GLuint listBegin = glGenLists( fac.size() );
  assert( !objects_.contains( listBegin ) ); // OpenGL should make sure of this

  GLObject* globject = new GLObject( listBegin, fac.size(), this );
  overwriteObject( globject, fac );

  objects_[ listBegin ] = globject;
  return globject;
}

void MyGLWidget::removeObject( const GLObject* globject )
{
  //qDebug() << "removing" << globject->begin();
  makeCurrent();
  glDeleteLists( globject->begin(), globject->range() ); // delete from OpenGL
  GLObject* toDelete = objects_[ globject->begin() ]; // toDelete = wrapper in memory
  objects_.remove( globject->begin() ); // remove from widget
  delete toDelete;
}

void MyGLWidget::overwriteObject( const GLObject* globject, const GLFactory& fac )
{
  makeCurrent();
  qDebug() << fac.size() << globject->range();
  assert( fac.size() == globject->range() );
  glNewList( globject->begin(), GL_COMPILE );
  fac.create();
  glEndList();
}

