
#include "testline3d.h"
#include "line3d.h"

void TestLine3D::testShortestSegmentLine()
{
  QFETCH( Line3D, a );
  QFETCH( Line3D, b );
  QFETCH( Line3D, segment );
  
  QVERIFY( segment == a.shortestSegment( b ) );
}

void TestLine3D::testShortestSegmentLine_data()
{
  QTest::addColumn< Line3D >( "a" );
  QTest::addColumn< Line3D >( "b" );
  QTest::addColumn< Line3D >( "segment" );

  QTest::newRow( "" ) 
    << Line3D( Vector3D(0,0,0,Vector3D::POINT3DCART), Vector3D(1,0,0,Vector3D::POINT3DCART) ) 
    << Line3D( Vector3D(0,1,1,Vector3D::POINT3DCART), Vector3D(0,-1,1,Vector3D::POINT3DCART) ) 
    << Line3D( Vector3D(0,0,0,Vector3D::POINT3DCART), Vector3D(0,0,1,Vector3D::POINT3DCART) );
  QTest::newRow( "" ) 
    << Line3D( Vector3D(4,8,2,Vector3D::POINT3DCART), Vector3D(8,4,2,Vector3D::POINT3DCART) ) 
    << Line3D( Vector3D(4,4,4,Vector3D::POINT3DCART), Vector3D(8,8,4,Vector3D::POINT3DCART) ) 
    << Line3D( Vector3D(6,6,2,Vector3D::POINT3DCART), Vector3D(6,6,4,Vector3D::POINT3DCART) );
  QTest::newRow( "" ) 
    << Line3D( Vector3D(0,0,0,Vector3D::POINT3DCART), Vector3D(0,0,6,Vector3D::POINT3DCART) ) 
    << Line3D( Vector3D(-2,4,-7,Vector3D::POINT3DCART), Vector3D(4,-2,13,Vector3D::POINT3DCART) ) 
    << Line3D( Vector3D(0,0,3,Vector3D::POINT3DCART), Vector3D(1,1,3,Vector3D::POINT3DCART) );

  /*eye:  "(-2.812,1.242,10.562,1 )" 
grabPoint:  "(-14.895,-8.949,-87.664,1 )" 
absGrabPoint_:  "(0.263,0.160,20.830,1 )" 
relGrabPoint_:  "(-0.077,0.092,15.830,0 )" */
}

/*void TestLine3D::testShortestSegmentPoint()
{
}

void TestLine3D::testShortestSegmentPoint_data()
{
}*/
