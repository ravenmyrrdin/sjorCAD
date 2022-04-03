#include "dllspec.h"
#include <QtTest/QtTest>

class DLL_EXPORT TestLine3D : public QObject
{
  Q_OBJECT
  private slots:
    void testShortestSegmentLine();
    void testShortestSegmentLine_data();
    //void testShortestSegmentPoint();
    //void testShortestSegmentPoint_data();

};
