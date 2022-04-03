
#include "testline3d.h"
#include <QtTest/QtTest>

int main ( int argc, char **argv )
{
  TestLine3D * test1 = new TestLine3D();
  QTest::qExec( test1, argc, argv );
}